#include "app/cli.hpp"

#include <iostream>
#include <string>
#include <stdexcept>
#include "app/cli.hpp"
#include <cctype>
namespace app {
namespace {
bool valid_text(const std::string& text){
    if(text.empty()){
        return false;
    }

    if (std::isspace(static_cast<unsigned char>(text.front()))){
        return false;
    }

    if (std::isspace(static_cast<unsigned char>(text.back()))){
        return false;
    }

    return true;
}
}  // namespace

std::string normalize_sku(const std::string& sku) {
    std::size_t first = 0;
    std::size_t end = sku.size();

    while (first < end &&
           std::isspace(static_cast<unsigned char>(sku[first]))) {
        ++first;
    }

    while (end > first &&
           std::isspace(static_cast<unsigned char>(sku[end - 1]))) {
        --end;
    }

    std::string result = sku.substr(first, end - first);

    for (char& ch : result) {
        ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    }

    return result;
}

bool valid_station_id(const std::string& station_id) {
    return valid_text(station_id);
}

bool valid_order_id(const std::string& order_id) {
    return valid_text(order_id);
}

bool valid_sku(const std::string& sku){
    return valid_text(sku);
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
