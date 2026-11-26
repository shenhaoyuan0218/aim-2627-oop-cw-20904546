#include "app/application.hpp"

#include <string>

int main(int argc, char** argv) {
    if (argc > 1 && std::string(argv[1]) == "--trace") {
        return app::print_trace(argc, argv);
    }
    return app::run();
}
