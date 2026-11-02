#include <new>

#include "game/app.hh"
#include "game/frontend/message_text.hh"
#include "game/game.hh"
#include "game/input/input_device.hh"
#include "game/input/input_mgr.hh"
#include "game/localized_string_table.hh"

using namespace treyarch;

// sub_68B310
message_text::message_text() {
    new (&text) mash::string();

    end_time       = 0.0f;
    time           = 0.0f;
    unk_18         = 0;
    fade_in_end    = 0.0f;
    fade_out_start = 0.0f;

    text = "";

    unk_19 = 0;
    unk_28 = -1;
    alpha  = 0.0f;
    unk_2c = 1;
}

// sub_68B3A0
// inlined @ sub_6B9F80
message_text::~message_text() {
    text.finalize(mash::ALLOCATED);
}

// sub_69B7D0
void message_text::update(f32 time_inc) {
    if (!unk_19 || !unk_18)
        return;

    time = (f32)((f64)time_inc + (f64)time);

    if ((f64)fade_in_end > (f64)time) {
        f32 rising = (f32)((f64)time * 2.0);

        alpha = 0.3 > (f64)rising ? 0.0f : rising;

        return;
    }

    if ((f64)time >= (f64)fade_in_end &&
        (0.0f > fade_out_start || (f64)fade_out_start >= (f64)time)) {

        alpha = 1.0f;

        return;
    }

    if (!(0.0f > fade_out_start) &&
        (f64)time > (f64)fade_out_start &&
        (f64)end_time > (f64)time) {

        alpha = (f32)(1.0 - ((f64)time - (f64)fade_out_start) / ((f64)end_time - (f64)fade_out_start));

        return;
    }

    unk_18 = 0;
    alpha  = 0.0f;
}

// sub_68B3C0
void message_text::set_text(mash::string value) {
    text = value;
}

// sub_90AB90
mash::string* message_text::get_text(mash::string* result) {
    return new (result) mash::string(text);
}

// sub_68B440
// inlined @ sub_6B9F80
void message_text::clear() {
    if (text.m_size > 0)
        text.clear();
}

// sub_6B2760
void message_text::replace_pad_tokens() { // swaps each "~token" for its button's name in brackets
    input_mgr*    inputs = input_mgr::inst();
    input_device* device = inputs->devices[joystick_2_device];

    if (!device) {
        device = inputs->devices[joystick_1_device];

        if (!device)
            return;
    }

    if (!device->unk_034() || !text.data()[0])
        return;

    i32 cursor = 0;

    do {
        if (text.data()[cursor] == '~') {
            alignas(mash::string) u8 name_storage  [sizeof(mash::string)];
            alignas(mash::string) u8 before_storage[sizeof(mash::string)];
            alignas(mash::string) u8 after_storage [sizeof(mash::string)];
            alignas(mash::string) u8 glyph_storage [sizeof(mash::string)];

            auto* name   = new (name_storage) mash::string("[");
            auto* before = get_text((mash::string*)before_storage);
            auto* after  = get_text((mash::string*)after_storage);

            before->data()[cursor] = '\0';

            after->remove_leading(before->c_str());

            char token[32] = {};

            const char* source = text.c_str();
            char*       target = token;

            for (char value = source[cursor]; value != ' ' && value && value != '.' && value != ',' && value != ')'; value = source[cursor]) {
                ++cursor;

                *target++ = value;
            }

            i32 code    = device->unk_038(mash::string(token));
            i32 text_id = 0;

            if (code == 0x3039)
                text_id = 0xDC2;
            else if (code == 0xB91C57)
                text_id = 0xDC1;
            else if (code == 0xE03F)
                text_id = 0xDC4;

            mash::string* glyph;

            if (text_id) {
                glyph = new (glyph_storage) mash::string();

                *glyph = references::game.read()->string_localizer->lookup_localized_string(text_id);
            } else
                glyph = new (glyph_storage) mash::string(device->get_name(code));

            *name += *glyph;

            glyph->finalize(mash::ALLOCATED);

            *name += "]";

            after->remove_leading(token);

            *before += *name;
            *before += *after;

            text = *before;

            cursor -= 0x21;

            if (cursor < 0)
                cursor = 0;

            after ->finalize(mash::ALLOCATED);
            before->finalize(mash::ALLOCATED);
            name  ->finalize(mash::ALLOCATED);
        }

        ++cursor;
    } while (cursor <= text.m_size && text.data()[cursor]);
}

// inlined @ sub_6B9F80
// inlined @ sub_85FA70
void message_text::substitute_pad_tokens(mash::string* value) {
    message_text message;

    message.set_text(*value);
    message.replace_pad_tokens();

    alignas(mash::string) u8 storage[sizeof(mash::string)];

    mash::string* rewritten = message.get_text((mash::string*)storage);

    bool changed = value->compare(rewritten->c_str()) != 0;

    rewritten->finalize(mash::ALLOCATED);

    if (changed) {
        rewritten = message.get_text((mash::string*)storage);

        *value = *rewritten;

        rewritten->finalize(mash::ALLOCATED);
    }

    message.clear();
}
