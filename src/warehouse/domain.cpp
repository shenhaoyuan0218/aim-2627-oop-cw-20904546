#include "warehouse/domain.hpp"

#include <stdexcept>

namespace warehouse {

void Inventory::add_station(const std::string& station_id, int stock) {
    (void)station_id;
    (void)stock;
    throw std::runtime_error("TODO: implement Inventory::add_station");
}

std::optional<PickStation> Inventory::station(
    const std::string& station_id) const {
    (void)station_id;
    throw std::runtime_error("TODO: implement Inventory::station");
}

std::vector<PickStation> Inventory::stations() const {
    throw std::runtime_error("TODO: implement Inventory::stations");
}

std::vector<PickStation> Inventory::available_for(const Order& order) const {
    (void)order;
    throw std::runtime_error("TODO: implement Inventory::available_for");
}

bool Inventory::reserve(const Order& order, const std::string& station_id) {
    (void)order;
    (void)station_id;
    throw std::runtime_error("TODO: implement Inventory::reserve");
}

bool Inventory::finish(const std::string& station_id,
                       const std::string& order_id) {
    (void)station_id;
    (void)order_id;
    throw std::runtime_error("TODO: implement Inventory::finish");
}

}  // namespace warehouse
