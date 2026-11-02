#include <cstring>

#include "retail.hh"
#include "game/app.hh"
#include "game/audio/sound_response.hh"
#include "game/conversation.hh"
#include "game/frontend/conversation_menu_system.hh"
#include "game/frontend/dialog_box.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_scrapbook.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/game.hh"
#include "game/localized_string_table.hh"
#include "game/quest_manager.hh"
#include "game/ui_character_info_list.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/references.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mm/mm.hh"

namespace treyarch {
    using choice_event_method     = void (__thiscall*)(conversation_menu_system* self, mash::string choice); // raises the choice's event on the hero, owns the string
    using end_conversation_method = void (__thiscall*)(conversation_menu_system* self, bool completed);      // CONVERSATION_END or _CANCEL on the hero, then the end
} // treyarch

using namespace treyarch;

// sub_6F90E0
conversation_menu_system::~conversation_menu_system() {
    if (name_text) {
        name_text->destroy();
        name_text = nullptr;
    }

    if (text) {
        text->destroy();
        text = nullptr;
    }

    if (replies_text) {
        replies_text->destroy();
        replies_text = nullptr;
    }

    if (updown_button) {
        delete updown_button;
        updown_button = nullptr;
    }

    if (confirm_button) {
        delete confirm_button;
        confirm_button = nullptr;
    }

    if (cancel_button) {
        delete cancel_button;
        cancel_button = nullptr;
    }

    release_entity();

    for (i32 index = 7; index >= 0; --index)
        ((mash::string*)lines[index])->finalize(mash::ALLOCATED);
}

// slot 2
void conversation_menu_system::unk_002(i32 unk) {
    ((void (__thiscall*)(conversation_menu_system*, i32))retail::sub_68C230)(this, unk);
}

// slot 3
void conversation_menu_system::unk_003(i32 unk) {
    ((void (__thiscall*)(conversation_menu_system*, i32))retail::sub_6A0AC0)(this, unk);
}

// slot 4
void conversation_menu_system::unk_004(i32 unk) {
    ((void (__thiscall*)(conversation_menu_system*, i32))retail::sub_6A0AE0)(this, unk);
}

// slot 5
void conversation_menu_system::unk_005(i32 unk) {
    ((void (__thiscall*)(conversation_menu_system*, i32))retail::sub_689620)(this, unk);
}

// slot 6
void conversation_menu_system::unk_006(i32 unk) {
    ((void (__thiscall*)(conversation_menu_system*, i32))retail::sub_ADF440)(this, unk);
}

// sub_717520
void conversation_menu_system::unk_007(i32) {
    if (current_conversation->states.array[current_state()]->action == 9)
        return;

    i32 count = retail::sub_6A0950((u32*)current_conversation, current_state()); // the state's reply count
    i32 reply = count > 0 ? retail::sub_6A09C0((u32*)current_conversation, current_state(), selected_reply) : -1;

    conversation_state* picked = references::conversation_menu_type.read() >= 3 ?
        current_conversation->states.array[current_state()] :
        current_conversation->states.array[reply];

    switch (picked->action) {
        case 1:
            if (retail::sub_7F7CC0((i32*)quest_manager::inst(), (i32)&picked->quest, 1) == 2) // the quest's status
                retail::sub_7FF3A0((i32*)quest_manager::inst(), (i32)&picked->quest);

            show_first_state();
            break;

        case 2:
            if (retail::sub_7F7CC0((i32*)quest_manager::inst(), (i32)&picked->quest, 0) == 8) // the quest's status
                retail::sub_800620((i32*)quest_manager::inst(), (i32)&picked->quest);

            show_first_state();
            return;

        case 4:
            if (return_state == -1) {
                leave_state();
                leave_state();
            } else
                while (current_state() != return_state)
                    leave_state();

            return;

        case 5:
            ((choice_event_method)retail::sub_6A0C10)(this, picked->quest);
            ((end_conversation_method)retail::sub_6F92C0)(this, true);

            return;

        case 6: {
            i32 sibling = retail::sub_6A0A50((u32*)current_conversation, current_state()); // the next state on its level

            if (sibling <= -1) {
                show_first_state();
                return;
            }

            state_stack[state_depth - 1] = sibling;
            show_current_state();

            return;
        }

        case 7:
            ((choice_event_method)retail::sub_6A0C10)(this, picked->quest);
            ((end_conversation_method)retail::sub_6F92C0)(this, false);

            return;

        case 10:
            ((choice_event_method)retail::sub_6A0C10)(this, picked->quest);
            break;

        case 11:
            ((choice_event_method)retail::sub_6A0C10)(this, mash::string("one"));
            break;

        case 12:
            ((choice_event_method)retail::sub_6A0C10)(this, mash::string("two"));
            break;

        case 13:
            ((choice_event_method)retail::sub_6A0C10)(this, mash::string("three"));
            break;
    }

    if (reply < 0) {
        show_first_state();
        return;
    }

    if (count > 1 &&
        reply != -1 &&
        current_conversation->states.array[reply]->action != 4)

        return_state = current_state();

    enter_state(reply);
}

// slot 10
void conversation_menu_system::unk_010(i32 unk) {
    ((void (__thiscall*)(conversation_menu_system*, i32))retail::sub_6F9340)(this, unk);
}

// sub_7177F0
void conversation_menu_system::update(f32 time_inc) {
    if (release_delay > 0.0f) {
        release_delay = (f32)((f64)release_delay - (f64)time_inc);

        if (release_delay <= 0.0f) {
            retail::sub_97BE60((u32*)references::game.read(), true); // the hero's controls
            references::g_world_ptr.read()->get_hero_ptr()->ext_flags &= ~entity_ext_flag_invulnerable;
            release_delay = 0.0f;
        }
    }

    if (references::frontend.get().igo->scrapbook->is_active() || !active)
        return;

    if (input_delay > 0.0f)
        input_delay = (f32)((f64)input_delay - (f64)time_inc);
    else
        retail::sub_6B1B30((u32*)(igo_generic_input_listener*)this); // polls the inputs and dispatches a new press

    if (references::frontend.get().igo->scrapbook->is_active() || !active)
        return;

    if (text) {
        i32 hidden_lines = (i32)text->line_count - text->get_visible_line_count();

        if (hidden_lines > 0 && text->get_scroll_line() < hidden_lines) {
            text_scroll_time = (f32)((f64)time_inc + (f64)text_scroll_time);

            if (text_scroll_time > 2.0f) {
                text->scroll(1, false);
                text_scroll_time = 0.0f;
            }
        }
    }

    if (current_conversation) {
        i32 action = current_conversation->states.array[current_state()]->action;

        if (action == 8) {
            ((choice_event_method)retail::sub_6A0C10)(this, current_conversation->states.array[current_state()]->quest);
            ((end_conversation_method)retail::sub_6F92C0)(this, true);
        } else if (action == 9) {
            dialog_box* box = references::frontend.get().igo->dialog;

            if (box->confirmed) {
                i32 choice = box->choice ? 1 : 0;

                box->hide();
                enter_state(retail::sub_6A09C0((u32*)current_conversation, current_state(), choice));
            }
        }
    }

    // a lone reply on the hero's turn is taken by itself
    if (references::conversation_menu_type.read() < 2 &&
        state_depth % 2 == 0 &&
        current_conversation &&
        retail::sub_6A0950((u32*)current_conversation, current_state()) == 1)

        enter_state(retail::sub_6A09C0((u32*)current_conversation, current_state(), 0));

    if (line_state == 2 || line_state == 1) {
        if (line_response && !retail::sub_8FE8D0((u8*)line_response)) // still playing
            line_state = 0;
    } else if (current_line < line_count && !line_state) {
        if (!line_response) {
            entity* hero       = references::g_world_ptr.read()->get_hero_ptr();
            void*   allocation = mm::allocate(sizeof(sound_response));

            if (allocation) {
                string_hash line_event;
                retail::sub_48E150(&line_event, (i32)lines[current_line]); // from the line's string

                line_response = ((sound_response* (__thiscall*)(void*, string_hash, entity*))retail::sub_8FFA40)(allocation, line_event, hero);
            } else
                line_response = nullptr;
        } else {
            string_hash line_event;
            retail::sub_48E150(&line_event, (i32)lines[current_line]); // from the line's string

            ((void (__thiscall*)(sound_response*, string_hash))retail::sub_8FE8C0)(line_response, line_event); // sets the event
        }

        if (skip_line) {
            ++current_line;
            skip_line  = false;
            line_state = 0;
        } else {
            retail::sub_901790((u32*)line_response); // posts the event
            ++current_line;
            line_state = 1;
        }
    }

    if (replies_text)
        replies_text->update(time_inc);
}

// +0x10 slot 3
void conversation_menu_system::render() {
    ((void (__thiscall*)(igo_3d_widget*))retail::sub_715B30)((igo_3d_widget*)this);
}

// +0x10 slot 42
void conversation_menu_system::spawn_entity() {
    ((void (__thiscall*)(igo_3d_widget*))retail::sub_6FE1A0)((igo_3d_widget*)this);
}

// +0x10 slot 44
i32 conversation_menu_system::render_congomerate_mesh(i32 unk_0, i32 unk_1, void* unk_2, i32 unk_3, i32 unk_4, i32 unk_5) {
    return ((i32 (__thiscall*)(igo_3d_widget*, i32, i32, void*, i32, i32, i32))retail::sub_6B40F0)((igo_3d_widget*)this,
                                                                                                   unk_0,
                                                                                                   unk_1,
                                                                                                   unk_2,
                                                                                                   unk_3,
                                                                                                   unk_4,
                                                                                                   unk_5);
}

// +0x10 slot 47
i32 conversation_menu_system::render_child_widget(i32 unk_0, i32 unk_1, i32 unk_2, i32 unk_3, i32 unk_4) {
    return ((i32 (__thiscall*)(igo_3d_widget*, i32, i32, i32, i32, i32))retail::sub_68C200)((igo_3d_widget*)this,
                                                                                            unk_0,
                                                                                            unk_1,
                                                                                            unk_2,
                                                                                            unk_3,
                                                                                            unk_4);
}

// sub_68BF40
bool conversation_menu_system::is_conversation_active() {
    if (references::frontend.get().igo->scrapbook->is_active())
        return false;

    return active;
}

// sub_717CE0
void conversation_menu_system::begin_conversation(conversation* new_conversation, bool met_before, bool can_cancel) {
    if (!references::frontend.get().igo->scrapbook->is_active() && active) {
        if (new_conversation == current_conversation)
            return;

        retail::sub_6F9250((i32)this); // ends the conversation without an event
    }

    input_delay          = 0.5f;
    current_conversation = new_conversation;
    introduction         = !met_before;
    unk_17d              = false;
    skip_line            = false;
    release_delay        = 0.0f;

    retail::sub_97BE60((u32*)references::game.read(), false); // the hero's controls
    references::g_world_ptr.read()->get_hero_ptr()->ext_flags |= entity_ext_flag_invulnerable;

    active       = true;
    cancelable   = can_cancel;
    return_state = -1;

    show_first_state();
    set_shown(true);

    references::g_world_ptr.read()->get_hero_ptr()->raise_event(references::conversation_begin.read());

    current_line = 0;
}

// sub_717390
void conversation_menu_system::show_first_state() {
    std::memset(condition_results, 0, sizeof(condition_results));

    for (u32 index = 0; index < current_conversation->conditions.size; ++index)
        condition_results[index] = retail::sub_68BE20((u32*)current_conversation->conditions.array[index]); // evaluates the condition

    retail::sub_6A0B30((u32*)this); // stops the voice lines

    unk_2c4       = 0;
    speaker_index = 0;
    state_depth   = 0;

    i32 root = retail::sub_6A09C0((u32*)current_conversation, -1, 0); // the first root state that holds

    if (root >= 0)
        enter_state(root);
    else
        ((end_conversation_method)retail::sub_6F92C0)(this, false);
}

// sub_711840
void conversation_menu_system::show_current_state() {
    text_scroll_time = 0.0f;
    text->set_scroll_line(0);

    unk_2c4       = 0;
    speaker_index = 0;

    i32                 index = state_stack[state_depth - 1];
    conversation_state* state = current_conversation->states.array[index];

    if (state->action == 9) {
        void*       allocation = mm::allocate(sizeof(dialog_box));
        dialog_box* box        = nullptr;

        if (allocation) {
            box = (dialog_box*)retail::sub_70BE00((u32*)allocation);
            box->stamp_source_vtables();
        }

        references::frontend.get().igo->set_dialog_box(box, false);
    }

    alignas(mash::string) u8 line_storage[sizeof(mash::string)];

    ((mash::string* (__thiscall*)(conversation_state*, void*, i32*, bool))retail::sub_6B4070)(state, line_storage, condition_results, true); // the text with its markup resolved

    auto &line = *(mash::string*)line_storage;

    i32 count = retail::sub_6A0950((u32*)current_conversation, index); // the state's reply count

    replies_text->clear_items();

    for (i32 reply = 0; reply < count; ++reply)
        replies_text->add_item(current_conversation->states.array[retail::sub_6A09C0((u32*)current_conversation, index, reply)]->name);

    if (!count) {
        replies_text->clear_items();

        if (state->action == 1) {
            quest_manager* quests = quest_manager::inst();

            replies_text->add_item(retail::sub_7EACB0((u32*)quests, quests->get_quest_index(state->quest)) ? 410 : 935);
        } else
            replies_text->add_item(state->action == 4 ? 936 : 410);
    }

    retail::sub_68BF90((u32*)this, 0); // selects the first reply

    // under a reply labelled 935 or 936 the hero speaks
    bool hero_speaks = false;

    if (state_depth > 1) {
        i32 first_reply = retail::sub_6A09C0((u32*)current_conversation, state_stack[state_depth - 2], 0);

        if (first_reply != -1) {
            conversation_state*     reply   = current_conversation->states.array[first_reply];
            localized_string_table* strings = references::game.read()->string_localizer;

            hero_speaks = reply->name == mash::string(strings->lookup_localized_string(935)) ||
                          reply->name == mash::string(strings->lookup_localized_string(936));
        }
    }

    bool other_speaks = false;

    if (!hero_speaks) {
        if (state->unk_3c == 2 || state->unk_3c == 3 || state->unk_3c == 6)
            other_speaks = state_depth % 2 == 0;
        else
            other_speaks = state_depth % 2 != 0;
    }

    void* speaker_map = references::ui_character_info_map.read();

    if (other_speaks) {
        i32 speaker = speaker_index;

        if ((u32)speaker >= current_conversation->speakers.size)
            speaker = (i32)current_conversation->speakers.size - 1;

        if (speaker >= 0) {
            string_hash key;
            retail::sub_6EFF40((u32*)speaker_map, (u32*)&key, current_conversation->speakers.array[speaker]); // the key for the speaker

            alignas(mash::string) u8 name_storage[sizeof(mash::string)];
            ((mash::string* (__thiscall*)(void*, void*, string_hash))retail::sub_6E16A0)(speaker_map, name_storage, key); // the speaker's name

            auto &name = *(mash::string*)name_storage;

            ((void (__thiscall*)(conversation_menu_system*, string_hash, u32))retail::sub_6EBBB0)(this, key, 1); // sets the speaker
            name_text->set_text_no_localize(name);

            name.finalize(mash::ALLOCATED);
        }

        unk_2e4 = 1;
    } else {
        string_hash key;
        retail::sub_6EFF40((u32*)speaker_map, (u32*)&key, current_conversation->hero_speaker); // the key for the speaker

        alignas(mash::string) u8 name_storage[sizeof(mash::string)];
        ((mash::string* (__thiscall*)(void*, void*, string_hash))retail::sub_6E16A0)(speaker_map, name_storage, key); // the speaker's name

        auto &name = *(mash::string*)name_storage;

        ((void (__thiscall*)(conversation_menu_system*, string_hash, u32))retail::sub_6EBBB0)(this, key, 0); // sets the speaker
        name_text->set_text_no_localize(name);

        unk_2e4 = 0;

        name.finalize(mash::ALLOCATED);
    }

    // a root state shown again comes up without its line
    if (unk_17d &&
        state_depth == 1 &&
        state->condition.type != 5 &&
        state->condition.type != 3) {

        text->set_text_no_localize("");
        skip_line = true;
    } else
        text->set_text_no_localize(line);

    unk_17d = true;

    i32 items   = replies_text->get_item_count();
    i32 visible = replies_text->get_visible_item_count();
    i32 rows    = items < visible ? items : visible;

    replies_text->set_text_position_y((f32)(150.0 - (f64)(f32)(rows - 1) * 14.0));

    reply_count = count;

    bool selection_moves = false;

    if (replies_text && replies_text->get_selected_item() > 0)
        selection_moves = true;
    else if (replies_text) {
        i32 last = replies_text->get_item_count() - 1;

        selection_moves = replies_text->get_selected_item() < last;
    }

    updown_button->set_shown(selection_moves);

    line.finalize(mash::ALLOCATED);
}

// sub_715B90
void conversation_menu_system::enter_state(i32 state) {
    if (state_depth >= 32)
        return;

    if (current_conversation->states.array[state]->condition.type == 7)
        introduction = false;

    state_stack[state_depth] = state;
    ++state_depth;

    show_current_state();
}

// sub_715BD0
void conversation_menu_system::leave_state() {
    --state_depth;

    if (state_depth > 0)
        show_current_state();
    else
        ((end_conversation_method)retail::sub_6F92C0)(this, false);
}
