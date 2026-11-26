#ifndef AIM_OOP_DISPATCH_SCHEDULER_HPP
#define AIM_OOP_DISPATCH_SCHEDULER_HPP

#include <memory>
#include <string>
#include <vector>

#include "audit/trace.hpp"
#include "dispatch/policy.hpp"
#include "warehouse/domain.hpp"

namespace dispatch {

struct DispatchDecision {
    bool accepted;
    std::string order_id;
    std::string station_id;
    std::string reason;
};

class Scheduler {
public:
    Scheduler(warehouse::Inventory& inventory,
              std::unique_ptr<DispatchPolicy> policy,
              audit::Trace& trace);

    DispatchDecision submit(const warehouse::Order& order);
    std::vector<DispatchDecision> submit_all(
        const std::vector<warehouse::Order>& orders);
    bool complete(const std::string& station_id,
                  const std::string& order_id,
                  std::string& failure_reason);
    const warehouse::Inventory& inventory() const;
    const audit::Trace& trace() const;

private:
    warehouse::Inventory& inventory_;
    std::unique_ptr<DispatchPolicy> policy_;
    audit::Trace& trace_;
};

}  // namespace dispatch

#endif
