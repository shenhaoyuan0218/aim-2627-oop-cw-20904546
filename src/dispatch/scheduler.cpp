#include "dispatch/scheduler.hpp"

#include <stdexcept>

namespace dispatch {

Scheduler::Scheduler(warehouse::Inventory& inventory,
                     std::unique_ptr<DispatchPolicy> policy,
                     audit::Trace& trace)
    : inventory_(inventory), policy_(std::move(policy)), trace_(trace) {
    if (!policy_) {
        throw std::invalid_argument("dispatch policy cannot be null");
    }
}

DispatchDecision Scheduler::submit(const warehouse::Order& order) {
    (void)order;
    throw std::runtime_error("TODO: implement Scheduler::submit");
}

std::vector<DispatchDecision> Scheduler::submit_all(
    const std::vector<warehouse::Order>& orders) {
    (void)orders;
    throw std::runtime_error("TODO: implement Scheduler::submit_all");
}

bool Scheduler::complete(const std::string& station_id,
                         const std::string& order_id,
                         std::string& failure_reason) {
    (void)station_id;
    (void)order_id;
    (void)failure_reason;
    throw std::runtime_error("TODO: implement Scheduler::complete");
}

}  // namespace dispatch
