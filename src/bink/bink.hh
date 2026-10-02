#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

struct BINK {
    u32 Width;
    u32 Height;
    u32 Frames;
    u32 FrameNum;
    // the rest of the SDK structure is not modelled
};

using HBINK = BINK*;

struct BINKPLANE {
    i32   Allocate;
    void* Buffer;
    u32   BufferPitch;
};

struct BINKFRAMEPLANESET {
    BINKPLANE YPlane;
    BINKPLANE cRPlane;
    BINKPLANE cBPlane;
    BINKPLANE APlane;
};

constexpr u32 BINKMAXFRAMEBUFFERS = 2;

struct BINKFRAMEBUFFERS {
    i32               TotalFrames;
    u32               YABufferWidth;
    u32               YABufferHeight;
    u32               cRcBBufferWidth;
    u32               cRcBBufferHeight;
    u32               FrameNum;
    BINKFRAMEPLANESET Frames[BINKMAXFRAMEBUFFERS];
};

ASSERT_SIZEOF  (BINKPLANE,                          0x0C);
ASSERT_SIZEOF  (BINKFRAMEPLANESET,                  0x30);
ASSERT_SIZEOF  (BINKFRAMEBUFFERS,                   0x78);
ASSERT_OFFSETOF(BINKFRAMEBUFFERS, FrameNum,         0x14);
ASSERT_OFFSETOF(BINKFRAMEBUFFERS, Frames,           0x18);
ASSERT_OFFSETOF(BINK,             FrameNum,         0x0C);

using BINKSNDOPEN    = void*;
using BINKSNDSYSOPEN = BINKSNDOPEN (__stdcall*)(u32 param);

namespace bink {
    namespace references {
        inline util::memory_reference<i32         (__stdcall*)(HBINK bink, i32 pause)>             BinkPause                { 0x00B7948C };
        inline util::memory_reference<i32         (__stdcall*)(HBINK bink)>                        BinkWait                 { 0x00B79490 };
        inline util::memory_reference<i32         (__stdcall*)(HBINK bink)>                        BinkDoFrame              { 0x00B79494 };
        inline util::memory_reference<HBINK       (__stdcall*)(const char* name, u32 flags)>       BinkOpen                 { 0x00B79498 };
        inline util::memory_reference<void        (__stdcall*)(HBINK bink, BINKFRAMEBUFFERS* set)> BinkGetFrameBuffersInfo  { 0x00B7949C };
        inline util::memory_reference<void        (__stdcall*)(HBINK bink, BINKFRAMEBUFFERS* set)> BinkRegisterFrameBuffers { 0x00B794A0 };
        inline util::memory_reference<void        (__stdcall*)(HBINK bink, u32 frame, i32 flags)>  BinkGoto                 { 0x00B794A4 };
        inline util::memory_reference<void        (__stdcall*)(HBINK bink)>                        BinkClose                { 0x00B794A8 };
        inline util::memory_reference<void        (__stdcall*)(HBINK bink, u32 track, i32 volume)> BinkSetVolume            { 0x00B794AC };
        inline util::memory_reference<void        (__stdcall*)(u32 total_tracks, u32* tracks)>     BinkSetSoundTrack        { 0x00B794B0 };
        inline util::memory_reference<i32         (__stdcall*)(BINKSNDSYSOPEN open, u32 param)>    BinkSetSoundSystem       { 0x00B794B4 };
        inline util::memory_reference<void        (__stdcall*)(HBINK bink)>                        BinkNextFrame            { 0x00B794B8 };
        inline util::memory_reference<BINKSNDOPEN (__stdcall*)(u32 param)>                         BinkOpenWaveOut          { 0x00B794BC };
        inline util::memory_reference<i32         (__stdcall*)(HBINK bink)>                        BinkShouldSkip           { 0x00B794C0 };
    } // references
} // bink

inline i32 BinkPause(HBINK bink, i32 pause) {
    return bink::references::BinkPause.read()(bink, pause);
}

inline i32 BinkWait(HBINK bink) {
    return bink::references::BinkWait.read()(bink);
}

inline i32 BinkDoFrame(HBINK bink) {
    return bink::references::BinkDoFrame.read()(bink);
}

inline HBINK BinkOpen(const char* name, u32 flags) {
    return bink::references::BinkOpen.read()(name, flags);
}

inline void BinkGetFrameBuffersInfo(HBINK bink, BINKFRAMEBUFFERS* set) {
    bink::references::BinkGetFrameBuffersInfo.read()(bink, set);
}

inline void BinkRegisterFrameBuffers(HBINK bink, BINKFRAMEBUFFERS* set) {
    bink::references::BinkRegisterFrameBuffers.read()(bink, set);
}

inline void BinkGoto(HBINK bink, u32 frame, i32 flags) {
    bink::references::BinkGoto.read()(bink, frame, flags);
}

inline void BinkClose(HBINK bink) {
    bink::references::BinkClose.read()(bink);
}

inline void BinkSetVolume(HBINK bink, u32 track, i32 volume) {
    bink::references::BinkSetVolume.read()(bink, track, volume);
}

inline void BinkSetSoundTrack(u32 total_tracks, u32* tracks) {
    bink::references::BinkSetSoundTrack.read()(total_tracks, tracks);
}

inline i32 BinkSetSoundSystem(BINKSNDSYSOPEN open, u32 param) {
    return bink::references::BinkSetSoundSystem.read()(open, param);
}

inline void BinkNextFrame(HBINK bink) {
    bink::references::BinkNextFrame.read()(bink);
}

inline BINKSNDOPEN BinkOpenWaveOut(u32 param) {
    return bink::references::BinkOpenWaveOut.read()(param);
}

inline i32 BinkShouldSkip(HBINK bink) {
    return bink::references::BinkShouldSkip.read()(bink);
}
