#ifndef AIM_OOP_APP_CLI_HPP
#define AIM_OOP_APP_CLI_HPP

#include <string>

namespace app {

std::string normalize_sku(const std::string& sku);
bool valid_station_id(const std::string& station_id);
bool valid_order_id(const std::string& order_id);

}  // namespace app

#endif
