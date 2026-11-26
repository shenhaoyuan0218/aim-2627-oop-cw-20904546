#include "dispatch/policy.hpp"

#include <stdexcept>

namespace dispatch {
namespace {

class PriorityPolicy final : public DispatchPolicy {
public:
    std::string name() const override;
    std::optional<std::string> select(
        const warehouse::Order& order,
        const std::vector<warehouse::PickStation>& candidates) const override;
};

class StockPolicy final : public DispatchPolicy {
public:
    std::string name() const override;
    std::optional<std::string> select(
        const warehouse::Order& order,
        const std::vector<warehouse::PickStation>& candidates) const override;
};

}  // namespace

std::unique_ptr<DispatchPolicy> make_policy(const std::string& name) {
    (void)name;
    throw std::runtime_error("TODO: implement make_policy");
}

std::string PriorityPolicy::name() const { return "priority"; }

std::optional<std::string> PriorityPolicy::select(
    const warehouse::Order& order,
    const std::vector<warehouse::PickStation>& candidates) const {
    (void)order;
    (void)candidates;
    throw std::runtime_error("TODO: implement PriorityPolicy");
}

std::string StockPolicy::name() const { return "stock"; }

std::optional<std::string> StockPolicy::select(
    const warehouse::Order& order,
    const std::vector<warehouse::PickStation>& candidates) const {
    (void)order;
    (void)candidates;
    throw std::runtime_error("TODO: implement StockPolicy");
}

}  // namespace dispatch
