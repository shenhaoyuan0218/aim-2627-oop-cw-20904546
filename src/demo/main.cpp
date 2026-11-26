#include "app/contracts.hpp"
#include "audit/trace.hpp"
#include "dispatch/policy.hpp"
#include "dispatch/scheduler.hpp"
#include "warehouse/domain.hpp"

#include <iostream>

int main() {
    std::cout << app::version << '\n';
    warehouse::Inventory inventory;
    audit::Trace trace;
    dispatch::Scheduler scheduler(
        inventory, dispatch::make_policy("priority"), trace);
    static_cast<void>(scheduler);
    return 0;
}
