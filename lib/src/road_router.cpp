#include "road_router.h"

#include <iostream>
#include <cstdint>
#include <unordered_set>

namespace veeroute
{
    RoadRouter::RoadRouter(const Roads &roads)
    {
        std::unordered_set<uint64_t> ids;

        for (const auto &road : roads.road_set_)
        {
            ids.insert(road.begin_);
            ids.insert(road.end_);
        }

        const size_t vertex_count = ids.size();
        graph_ = Graph::DirectedWeightedGraph<double>(vertex_count);
        vertex_to_id_.resize(vertex_count);
        id_to_vertex_.reserve(vertex_count);
        Graph::VertexId vertex_id = 0;

        for (const auto &road : roads.road_set_)
        {
            if (!id_to_vertex_.count(road.begin_))
            {
                vertex_to_id_[vertex_id] = road.begin_;
                id_to_vertex_[road.begin_] = vertex_id++;
            }

            if (!id_to_vertex_.count(road.end_))
            {
                vertex_to_id_[vertex_id] = road.end_;
                id_to_vertex_[road.end_] = vertex_id++;
            }

            graph_.AddEdge({id_to_vertex_[road.begin_], id_to_vertex_[road.end_], road.length_});
        }

        assert(vertex_id == graph_.GetVertexCount());

        router_ = std::make_unique<Graph::Router<double>>(graph_);
    }

    std::optional<std::vector<uint64_t>> RoadRouter::FindRoute(uint64_t source_id, uint64_t target_id) const
    {
        if (!(id_to_vertex_.count(source_id) && id_to_vertex_.count(target_id)))
        {
            return std::nullopt;
        }

        const Graph::VertexId vertex_from = id_to_vertex_.at(source_id);
        const Graph::VertexId vertex_to = id_to_vertex_.at(target_id);
        const auto route = router_->BuildRoute(vertex_from, vertex_to);

        if (!route)
        {
            return std::nullopt;
        }

        std::vector<uint64_t> route_info;
        route_info.reserve(route->edge_count);

        for (size_t edge_idx = 0; edge_idx < route->edge_count; ++edge_idx)
        {
            const Graph::EdgeId edge_id = router_->GetRouteEdge(route->id, edge_idx);
            const auto &edge = graph_.GetEdge(edge_id);
            route_info.push_back(vertex_to_id_[edge.to]);
        }

        router_->ReleaseRoute(route->id);

        return route_info;
    }
}