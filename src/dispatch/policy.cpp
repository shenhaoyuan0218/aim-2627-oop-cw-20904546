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
    if (name == "priority") {
        return std::make_unique<PriorityPolicy>();
    }

    if (name == "stock") {
        return std::make_unique<StockPolicy>();
    }

    throw std::invalid_argument("unknown dispatch policy");
}

std::string PriorityPolicy::name() const { return "priority"; }

std::optional<std::string> PriorityPolicy::select(
    const warehouse::Order& order,
    const std::vector<warehouse::PickStation>& candidates) const {
    (void)order;

    if (candidates.empty()) {
        return std::nullopt;
    }

    return candidates.front().id;
}

std::string StockPolicy::name() const { return "stock"; }

std::optional<std::string> StockPolicy::select(
    const warehouse::Order& order,
    const std::vector<warehouse::PickStation>& candidates) const {
    (void)order;

    if (candidates.empty()) {
        return std::nullopt;
    }

    std::size_t best = 0;

    for (std::size_t i = 1; i < candidates.size(); ++i) {
        if (candidates[i].stock > candidates[best].stock) {
            best = i;
        }
    }

    return candidates[best].id;
}

}  // namespace dispatch
