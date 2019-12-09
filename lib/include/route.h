/**
 * API для работы с библиотекой поиска маршрутов.
 * Конструктор маршрутизатора требует квадратичного относительно количества вершин объёма памяти!!!!
 */

#pragma once

#include <cstdint>
#include <memory>

namespace veeroute
{
    class Route
    {
    public:
        Route(const char *data_path);
        /**
         * Функция поиска кратчайшего маршрута.
         * В path_count д.б. записан размер буфера для остановок.
         * @param source_id Начало маршрута.
         * @param target_id Конец маршрута.
         * @param path Буфер для остановок по маршруту.
         * @param path_count Количество остановок в буфере.
         * @return true - машрут есть, false - маршрута нет.
         */
        bool FindRoute(uint64_t source_id, uint64_t target_id, uint64_t *path, int *path_count);

        void AddRoad(uint64_t begin_id, uint64_t end_id, double length, double velocity);

        /**
         * Функция удаляет участок дороги.
         * @param begin_id
         * @param end_id
         * @return true - успешно удалён, false - такого участка нет в базе.
         */
        bool DeleteRoad(uint64_t begin_id, uint64_t end_id);

        /**
         * Функция изменяет параметры участка дороги.
         * @param begin_id
         * @param end_id
         * @param length
         * @param velocity
         * @return true - успешно изменён, false - такого участка нет в базе.
         */
        bool EditRoad(uint64_t begin_id, uint64_t end_id, double length, double velocity);

        ~Route();
        Route(const Route &) = delete;
        Route(Route &&) noexcept;
        Route &operator=(const Route &) = delete;
        Route &operator=(Route &&) noexcept ;

    private:
        class RoadMap;
        std::unique_ptr<RoadMap> road_map_;
    };
}