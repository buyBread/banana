#include <cstring>
#include <lzo/lzo1x.h>

#include "retail.hh"
#include "treyarch/jq/jq.hh"
#include "treyarch/nfl/nfl.hh"
#include "treyarch/nfl/system.hh"

using namespace treyarch;

// inlined @ sub_A16380
// inlined @ sub_A16D60
nfl::chunk_decompress_job* treyarch::nfl::get_chunk_decompress_job(i32 id) {
    const i32 index = tx::slot_index(&references::chunk_decompress_job_pool.get(), id);

    return index == -1 ? nullptr : &references::chunk_decompress_jobs.read()[index];
}

// sub_A16380
void treyarch::nfl::nfs_begin_segment_decompress(nfd_driver* driver, nfs_request* request) { // the read buffer stays locked until its jobs finish
    const nfd_io_command &io_command = driver->disk_read.io_command;

    request->read_buffer_slot = -1;

    const i32 chunk_count = ((chunk_header*)io_command.buffer)->num_chunks_in_segment;

    if (std::memcmp(((chunk_header*)io_command.buffer)->chunk_id, "NCH", 4))
        nfl::log.err("request {} \"{}\" -- invalid chunk header at 0x{:X}",
                     io_command.request,
                     nfs_file_path(request->file),
                     io_command.offset);
    else if (chunk_count <= 0)
        nfl::log.err("request {} \"{}\" -- {} chunks in the segment at 0x{:X}",
                     io_command.request,
                     nfs_file_path(request->file),
                     chunk_count,
                     io_command.offset);

    u32 offset = 0;

    for (i32 index = 0; index < chunk_count; ++index) {
        auto* record = (chunk_header*)((u8*)io_command.buffer + offset);

        const i32 job_id = tx::slot_new(&references::chunk_decompress_job_pool.get());

        if (job_id == -1)
            nfl::log.err("request {} \"{}\" -- out of chunk jobs ({}), {} chunks in the segment",
                         io_command.request,
                         nfs_file_path(request->file),
                         references::chunk_decompress_job_pool.get().count,
                         chunk_count);

        chunk_decompress_job* job = get_chunk_decompress_job(job_id);

        job->request = io_command.request;
        job->record  = record;
        job->state   = 0;
        job->output  = request->destination + record->prior_output;
        job->header  = *record;

        ++request->active_decompress_jobs;
        ++driver->chunk_decompress_jobs;

        request->bytes_scheduled += record->output_size;

        offset += record->span;
    }
}

// sub_A16D60
void treyarch::nfl::nfs_update_chunk_decompression_jobs(nfd_driver* driver) {
    tx::slot_pool* jobs_pool = &references::chunk_decompress_job_pool.get();

    for (i32 next_id = tx::slot_first(jobs_pool); next_id != -1;) {
        const i32             id      = next_id;
        chunk_decompress_job* job     = get_chunk_decompress_job(id);
        nfs_request*          request = nfs_get_request(job->request);

        next_id = tx::slot_next(jobs_pool, id);

        if (job->state == 0) {
            nfs_create_jq_decompress_job(job, request);

            job->state = 1;
        } else if (job->state != 1)
            continue;

        // the whole group has to finish, so all of a request's jobs complete in the same walk
        if (references::decompress_on_jq.read()) {
            const i32 batch_count = retail::sub_A14260(request->group_handle); // jq_get_batch_count

            if (batch_count + retail::sub_A14280(request->group_handle)) // jq_get_executing_batch_count
                continue;
        }

        --request->active_decompress_jobs;
        --driver->chunk_decompress_jobs;

        request->bytes_decompressed += job->header.output_size;

        if (!request->active_decompress_jobs) {
            for (i32 slot = 0; slot < 2; ++slot) {
                if (request->read_buffers_locked[slot] && slot != request->read_buffer_slot)
                    nfd_unlock_read_buffer(driver, job->request, slot);
            }

            if ((request->bytes_decompressed == request->output_size && request->bytes_read == request->disk_size) ||
                request->state == nfs_request_state_canceling) {

                if (request->state == nfs_request_state_waiting && request->bytes_read == request->disk_size)
                    request->state = nfs_request_state_workdone;
                else if (request->read_buffer_slot == -1)
                    request->state = nfs_request_state_canceled;
            }
        }

        tx::slot_free(jobs_pool, id);
    }
}

// sub_A16D00
void treyarch::nfl::nfs_create_jq_decompress_job(chunk_decompress_job* job, nfs_request* request) {
    jq::batch batch; // payload[2], tag and unk_24 stay uninitialized

    batch.payload[0] = job->record;
    batch.payload[1] = job->output;
    batch.count      = 0;
    batch.unk_10[0]  = 0;
    batch.unk_10[1]  = 0;
    batch.descriptor = &nfl_jq_decompress_descriptor;
    batch.group      = request->group_handle;

    if (references::decompress_on_jq.read())
        retail::sub_A143E0((u64*)&batch, 0); // jq_add_batch
    else
        nfl_jq_decompress_module(&batch);
}

// sub_A17B80
void treyarch::nfl::nfl_jq_decompress_module(jq::batch* batch) { // runs on the job queue workers with no lock held
    auto* record = (chunk_header*)batch->payload[0];

    const u8* payload = (u8*)record + record->span - record->stored_size;

    if (record->stored_size == record->output_size)
        std::memcpy(batch->payload[1], payload, record->stored_size);
    else {
        lzo_uint out_len = record->output_size;

        const i32 result = lzo1x_decompress(payload, record->stored_size, (u8*)batch->payload[1], &out_len, nullptr);

        if (result != LZO_E_OK)
            nfl::log.err("lzo decompression failed -- error {}, record {}, output {}, stored 0x{:X}, expected 0x{:X}, got 0x{:X}",
                         result,
                         (void*)record,
                         batch->payload[1],
                         record->stored_size,
                         record->output_size,
                         out_len);
        else if (out_len != record->output_size)
            nfl::log.err("lzo output size mismatch -- record {}, output {}, expected 0x{:X}, got 0x{:X}",
                         (void*)record,
                         batch->payload[1],
                         record->output_size,
                         out_len);
    }
}
