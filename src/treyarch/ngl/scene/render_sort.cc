#include "treyarch/ngl/list/render_sort.hh"
#include "treyarch/ngl/scene/render_sort.hh"

using namespace treyarch;

void __cdecl ngl::render_sort(scene* value) {
    for (scene* child = value->first_child; child; child = child->next_sibling)
        render_sort(child);

    list::sort_render_list_by_hash(value->opaque_render_list,
                                   value->opaque_render_list_count);
    list::sort_render_list_by_hash(value->specialized_render_list_0,
                                   value->specialized_render_list_count_0);
    list::sort_render_list_by_hash(value->specialized_render_list_3,
                                   value->specialized_render_list_count_3);
    list::sort_render_list_by_distance(value->specialized_render_list_2,
                                       value->specialized_render_list_count_2);
    list::sort_render_list_by_distance(value->translucent_render_list,
                                       value->translucent_render_list_count);
    list::sort_render_list_by_distance(value->specialized_render_list_1,
                                       value->specialized_render_list_count_1);
}
