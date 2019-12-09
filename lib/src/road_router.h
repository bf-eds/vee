#pragma once

#include "road.h"
#include "graph.h"
#include "router.h"
#include <unordered_set>
#include <memory>

namespace viiroute
{
    class RoadRouter
    {
    public:
        RoadRouter(const Roads &roads);
        std::optional<std::vector<uint64_t>> FindRoute(uint64_t source_id, uint64_t target_id) const;

    private:
        Graph::DirectedWeightedGraph<double> graph_;
        std::unique_ptr<Graph::Router<double>> router_;
        std::unordered_map<uint64_t, Graph::VertexId> id_to_vertex_;
        std::vector<uint64_t> vertex_to_id_;
    };
}
