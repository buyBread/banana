#pragma once

#include "treyarch/nal/pose.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace nal {
    void blend(nal::pose* pose_r,
               f32        alpha,
               nal::pose* pose_a,
               nal::pose* pose_b);

    void blend_trajectory(nal::pose* pose_r,
                          f32        alpha,
                          nal::pose* pose_a,
                          nal::pose* pose_b);
}} // treyarch::nal
