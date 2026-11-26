#include "dispatch/scheduler.hpp"
#include "app/cli.hpp"
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
    if (!warehouse::valid_order(order)) {
        return {false, order.id, "", "invalid order"};
    }

    warehouse::Order clean = order;
    clean.sku = app::normalize_sku(order.sku);

    auto candidates = inventory_.available_for(clean);
    if (candidates.empty()) {
        return {false, order.id, "", "no available station"};
    }

    auto selected = policy_->select(clean, candidates);
    if (!selected) {
        return {false, order.id, "", "no station selected"};
    }

    if (!inventory_.reserve(clean, *selected)) {
        return {false, order.id, "", "reservation failed"};
    }

    trace_.append("DISPATCH " + order.id + " " + *selected);
    return {true, order.id, *selected, "accepted"};
}

std::vector<DispatchDecision> Scheduler::submit_all(
    const std::vector<warehouse::Order>& orders) {
    std::vector<DispatchDecision> results;

    for (const auto& order : orders) {
        DispatchDecision decision = submit(order);
        results.push_back(decision);
    }

    return results;
}

bool Scheduler::complete(const std::string& station_id,
                         const std::string& order_id,
                         std::string& failure_reason) {
    bool finished = inventory_.finish(station_id, order_id);

    if (!finished) {
        failure_reason = "station is missing, idle, or holds another order";
        return false;
    }

    failure_reason.clear();
    trace_.append("COMPLETE " + order_id + " " + station_id);
    return true;
}
const warehouse::Inventory& Scheduler::inventory() const {
    return inventory_;
}

const audit::Trace& Scheduler::trace() const {
    return trace_;
}
}  // namespace dispatch
