#ifndef AIM_OOP_AUDIT_TRACE_HPP
#define AIM_OOP_AUDIT_TRACE_HPP

#include <string>
#include <vector>

namespace audit {

struct TraceEvent {
    std::string text;
};

class Trace {
public:
    void append(const std::string& text);
    std::vector<TraceEvent> events() const;
    void clear();

private:
    std::vector<TraceEvent> events_;
};

}  // namespace audit

#endif
