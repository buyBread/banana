#include "game/meshmap.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// sub_7DE5D0
void meshmap::construct_mashed_class() {
    self = nullptr;

    a.construct_mashed_class();
    b.construct_mashed_class();
    c.construct_mashed_class();
    d.construct_mashed_class();
    e.construct_mashed_class();
    f.construct_mashed_class();
    g.construct_mashed_class();
    h.construct_mashed_class();
    i.construct_mashed_class();
    j.construct_mashed_class();
    k.construct_mashed_class();
    l.construct_mashed_class();
    m.construct_mashed_class();
    n.construct_mashed_class();
    o.construct_mashed_class();
    p.construct_mashed_class();
}

// sub_7DC6C0
void meshmap::destruct_mashed_class() {
    a.destruct_mashed_class();
    b.destruct_mashed_class();
    c.destruct_mashed_class();
    d.destruct_mashed_class();
    e.destruct_mashed_class();
    f.destruct_mashed_class();
    g.destruct_mashed_class();
    h.destruct_mashed_class();
    i.destruct_mashed_class();
    j.destruct_mashed_class();
    k.destruct_mashed_class();
    l.destruct_mashed_class();
    m.destruct_mashed_class();
    n.destruct_mashed_class();
    o.destruct_mashed_class();
    p.destruct_mashed_class();
}

// sub_7DDB80
void meshmap::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    a.unmash(mash_info, this, buffer);
    b.unmash(mash_info, this, buffer);
    c.unmash(mash_info, this, buffer);
    d.unmash(mash_info, this, buffer);
    e.unmash(mash_info, this, buffer);
    f.unmash(mash_info, this, buffer);
    g.unmash(mash_info, this, buffer);
    h.unmash(mash_info, this, buffer);
    i.unmash(mash_info, this, buffer);
    j.unmash(mash_info, this, buffer);
    k.unmash(mash_info, this, buffer);
    l.unmash(mash_info, this, buffer);
    m.unmash(mash_info, this, buffer);
    n.unmash(mash_info, this, buffer);
    o.unmash(mash_info, this, buffer);
    p.unmash(mash_info, this, buffer);
}
