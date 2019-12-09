#include "road_router.h"
#include "road.h"
#include "lib/include/route.h"

#include <fstream>
#include <iostream>
#include <unordered_set>

namespace viiroute
{
    using namespace std;

    class Route::RoadMap
    {
    public:
        RoadMap(const std::string &data_path);
        std::optional<std::vector<uint64_t>> FindRoute(uint64_t source_id, uint64_t target_id) const;
        void AddRoad(uint64_t begin_id, uint64_t end_id, double length, double velocity);
        bool DeleteRoad(uint64_t begin_id, uint64_t end_id);
        bool EditRoad(uint64_t begin_id, uint64_t end_id, double length, double velocity);
    private:
        static constexpr int ROAD_ITEM_SIZE = 8 * 4; // 4 поля по 8 байт
        Roads roads_;
        std::unique_ptr<RoadRouter> router_;
    };

    Route::Route(const char *data_path) : road_map_{make_unique<RoadMap>(data_path)}
    {
    }

    Route::~Route() = default;
    Route::Route(Route &&) noexcept = default;
    Route &Route::operator=(Route &&) noexcept = default;

    bool operator==(const Road &lhs, const Road &rhs)
    {
        return lhs.begin_ == rhs.begin_ && lhs.end_ == rhs.end_;
    }

    Route::RoadMap::RoadMap(const std::string &data_path)
    {
        ifstream input_data(data_path, ios::binary);

        if (input_data.is_open())
        {
            uint8_t buffer[ROAD_ITEM_SIZE] = {};

            while (input_data.read((char *) buffer, sizeof(buffer)))
            {
                auto *begin = reinterpret_cast<uint64_t *>(buffer);
                auto *end = reinterpret_cast<uint64_t *>(buffer + sizeof(*begin));
                auto *length = reinterpret_cast<double *>(buffer + sizeof(*begin) + sizeof(*end));
                auto *velocity = reinterpret_cast<double *>(buffer + sizeof(*begin) + sizeof(*end) + sizeof(*length));

                roads_.road_set_.insert({*begin, *end, *length, *velocity});
            }
        }
        else
        {
            cerr << "Failed to open data file: " << data_path << endl;
        }

        router_ = make_unique<RoadRouter>(roads_);
    }

    std::optional<std::vector<uint64_t>> Route::RoadMap::FindRoute(uint64_t source_id, uint64_t target_id) const
    {
        return router_->FindRoute(source_id, target_id);
    }

    bool Route::FindRoute(uint64_t source_id, uint64_t target_id, uint64_t *path, int *path_count)
    {
        auto route_info = road_map_->FindRoute(source_id, target_id);
        if (route_info != nullopt)
        {
            if (path && path_count)
            {
                *path_count = std::min((size_t) *path_count, route_info.value().size());
                copy(route_info.value().begin(), route_info.value().begin() + *path_count, path);
            }

            return true;
        }

        return false;
    }

    void Route::AddRoad(uint64_t begin_id, uint64_t end_id, double length, double velocity)
    {
        road_map_->AddRoad(begin_id, end_id, length, velocity);
    }

    bool Route::DeleteRoad(uint64_t begin_id, uint64_t end_id)
    {
        return road_map_->DeleteRoad(begin_id, end_id);
    }

    bool Route::EditRoad(uint64_t begin_id, uint64_t end_id, double length, double velocity)
    {
        return road_map_->EditRoad(begin_id, end_id, length, velocity);
    }

    void Route::RoadMap::AddRoad(uint64_t begin_id, uint64_t end_id, double length, double velocity)
    {
        roads_.road_set_.insert({begin_id, end_id, length, velocity});
        router_ = make_unique<RoadRouter>(roads_);
    }

    bool Route::RoadMap::DeleteRoad(uint64_t begin_id, uint64_t end_id)
    {
        if (roads_.road_set_.erase({begin_id, end_id, 0, 0}))
        {
            router_ = make_unique<RoadRouter>(roads_);
            return true;
        }

        return false;
    }

    bool Route::RoadMap::EditRoad(uint64_t begin_id, uint64_t end_id, double length, double velocity)
    {
        if (roads_.road_set_.erase({begin_id, end_id, length, velocity}))
        {
            AddRoad(begin_id, end_id, length, velocity);
            return true;
        }

        return false;
    }
}