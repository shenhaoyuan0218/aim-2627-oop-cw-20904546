#include "app/cli.hpp"

#include <iostream>
#include <string>
#include <stdexcept>

namespace app {
namespace {

}  // namespace

std::string normalize_sku(const std::string& sku) {
    (void)sku;
    throw std::runtime_error("TODO: implement normalize_sku");
}

bool valid_station_id(const std::string& station_id) {
    (void)station_id;
    throw std::runtime_error("TODO: implement valid_station_id");
}

bool valid_order_id(const std::string& order_id) {
    (void)order_id;
    throw std::runtime_error("TODO: implement valid_order_id");
}

int run() {
    std::cout << "Use the ROS 2 demo package for the end-to-end runtime.\n";
    return 0;
}

int print_trace(int argc, char** argv) {
    if (argc > 1 && std::string(argv[1]) == "--trace") {
        std::cout << "TRACE command is reserved for grading\n";
        return 0;
    }
    return run();
}

}  // namespace app
