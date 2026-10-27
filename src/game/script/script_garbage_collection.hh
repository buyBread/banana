#pragma once

#include "treyarch/chuck/vm/script_instance.hh"

// the manager's defaults per garbage-collection type; each releases what a dying instance's natives left listed
namespace treyarch { namespace script_garbage_collection {
    void entity_trackers        (chuck::vm::script_instance* si, chuck::vm::stuff_to_delete_list_t &stuff_to_delete);
    void line_infos             (chuck::vm::script_instance* si, chuck::vm::stuff_to_delete_list_t &stuff_to_delete);
    void triggers               (chuck::vm::script_instance* si, chuck::vm::stuff_to_delete_list_t &stuff_to_delete);
    void entities               (chuck::vm::script_instance* si, chuck::vm::stuff_to_delete_list_t &stuff_to_delete);
    void sound_responses        (chuck::vm::script_instance* si, chuck::vm::stuff_to_delete_list_t &stuff_to_delete);
    void generic_event_callbacks(chuck::vm::script_instance* si, chuck::vm::stuff_to_delete_list_t &stuff_to_delete);
    void points_of_interest     (chuck::vm::script_instance* si, chuck::vm::stuff_to_delete_list_t &stuff_to_delete);
    void mutexes                (chuck::vm::script_instance* si, chuck::vm::stuff_to_delete_list_t &stuff_to_delete);
    void widgets_3d             (chuck::vm::script_instance* si, chuck::vm::stuff_to_delete_list_t &stuff_to_delete);
    void city_life_trackers     (chuck::vm::script_instance* si, chuck::vm::stuff_to_delete_list_t &stuff_to_delete);
    void fingers_of_god         (chuck::vm::script_instance* si, chuck::vm::stuff_to_delete_list_t &stuff_to_delete);
    void fight_groups           (chuck::vm::script_instance* si, chuck::vm::stuff_to_delete_list_t &stuff_to_delete);
    void obstacles              (chuck::vm::script_instance* si, chuck::vm::stuff_to_delete_list_t &stuff_to_delete);
}} // treyarch::script_garbage_collection
