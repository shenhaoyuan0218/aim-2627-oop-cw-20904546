#ifndef AIM_OOP_DISPATCH_POLICY_HPP
#define AIM_OOP_DISPATCH_POLICY_HPP

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "warehouse/domain.hpp"

namespace dispatch {

class DispatchPolicy {
public:
    virtual ~DispatchPolicy() = default;
    virtual std::string name() const = 0;
    virtual std::optional<std::string> select(
        const warehouse::Order& order,
        const std::vector<warehouse::PickStation>& candidates) const = 0;
};

std::unique_ptr<DispatchPolicy> make_policy(const std::string& name);

}  // namespace dispatch

#endif
