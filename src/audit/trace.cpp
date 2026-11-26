#include "audit/trace.hpp"

#include <stdexcept>

namespace audit {

void Trace::append(const std::string& text) {
    TraceEvent event{text};
    events_.push_back(event);
}

std::vector<TraceEvent> Trace::events() const {
    return events_;
}

void Trace::clear() {
    events_.clear();
}

}  // namespace audit
