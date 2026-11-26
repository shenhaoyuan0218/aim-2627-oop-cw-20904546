#ifndef AIM_OOP_WAREHOUSE_DOMAIN_HPP
#define AIM_OOP_WAREHOUSE_DOMAIN_HPP

#include <map>
#include <optional>
#include <string>
#include <vector>

namespace warehouse {

enum class RobotState { idle, busy };

struct Order {
    std::string id;
    std::string sku;
    int quantity;
};

struct PickStation {
    std::string id;
    int stock;
    RobotState robot_state;
    std::string current_order_id;
};

class Inventory {
public:
    void add_station(const std::string& station_id, int stock);
    std::optional<PickStation> station(const std::string& station_id) const;
    std::vector<PickStation> stations() const;
    std::vector<PickStation> available_for(const Order& order) const;
    bool reserve(const Order& order, const std::string& station_id);
    bool finish(const std::string& station_id, const std::string& order_id);

private:
    std::map<std::string, PickStation> stations_;
};

}  // namespace warehouse

#endif
