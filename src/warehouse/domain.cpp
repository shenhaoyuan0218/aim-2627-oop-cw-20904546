#include "warehouse/domain.hpp"
#include "app/cli.hpp"


#include <stdexcept>

namespace warehouse {

bool valid_order(const Order& order) {
    if (order.quantity <= 0) {
        return false;
    }

    if (!app::valid_order_id(order.id)) {
        return false;
    }

    if (!app::valid_sku(order.sku)) {
        return false;
    }

    return true;
}

void Inventory::add_station(const std::string& station_id, int stock) {
    if (!app::valid_station_id(station_id) || stock < 0){
        throw std::invalid_argument("invalid station id or stack");
    }

    if (stations_.count(station_id) != 0){
        throw std::invalid_argument("duplicate station");
    }

    PickStation station{station_id, stock, RobotState::idle, ""};
    stations_.emplace(station_id, station);
}

std::optional<PickStation> Inventory::station(const std::string& station_id) const {
    auto it = stations_.find(station_id);

    if (it == stations_.end()){
        return std::nullopt;
    }

    return it->second;
}

std::vector<PickStation> Inventory::stations() const {
    std::vector<PickStation> result;

    for (const auto& item : stations_) {
        result.push_back(item.second);
    }

    return result;
}

std::vector<PickStation> Inventory::available_for(const Order& order) const {
    std::vector<PickStation> result;

    if (!valid_order(order)) {
        return result;
    }

    for (const auto& item : stations_) {
        const PickStation& station = item.second;

        if (station.robot_state == RobotState::idle &&
            station.stock >= order.quantity) {
            result.push_back(station);
        }
    }

    return result;
}

bool Inventory::reserve(const Order& order, const std::string& station_id) {
    if (!valid_order(order)){
        return false;
    }

    auto it = stations_.find(station_id);
    if (it == stations_.end()){
        return false;
    }

    PickStation& station = it->second;

    if (station.robot_state != RobotState::idle){
        return false;
    }

    if (station.stock < order.quantity) {
        return false;
    }

    station.current_order_id = order.id;
    station.stock -= order.quantity;
    station.robot_state = RobotState::busy;

    return true;    
}

bool Inventory::finish(const std::string& station_id,
                       const std::string& order_id) {
    auto it = stations_.find(station_id);
    if (it == stations_.end()) {
        return false;
    }

    PickStation& station = it->second;

    if (station.robot_state != RobotState::busy) {
        return false;
    }

    if (station.current_order_id != order_id) {
        return false;
    }

    station.current_order_id.clear();
    station.robot_state = RobotState::idle;
    return true;
}

}// namespace warehouse
