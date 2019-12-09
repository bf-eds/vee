#pragma once

#include <cstdint>
#include <functional>
#include <unordered_set>

namespace viiroute
{
    struct Road
    {
        uint64_t begin_;
        uint64_t end_;
        double length_;
        double velocity_;
    };

    struct RoadHasher
    {
        size_t operator()(const Road &road) const
        {
            const std::hash<long long> long_hasher;
            return long_hasher(road.begin_) ^ long_hasher(road.end_);
        }
    };

    using RoadSet = std::unordered_set<Road, RoadHasher>;

    struct Roads
    {
        RoadSet road_set_;
        std::unordered_set<uint64_t> ids_;
    };
}