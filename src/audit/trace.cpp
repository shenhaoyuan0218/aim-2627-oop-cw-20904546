#include "audit/trace.hpp"

#include <stdexcept>

namespace audit {

void Trace::append(const std::string& text) {
    (void)text;
    throw std::runtime_error("TODO: implement Trace::append");
}

std::vector<TraceEvent> Trace::events() const {
    throw std::runtime_error("TODO: implement Trace::events");
}

void Trace::clear() {
    throw std::runtime_error("TODO: implement Trace::clear");
}

}  // namespace audit
