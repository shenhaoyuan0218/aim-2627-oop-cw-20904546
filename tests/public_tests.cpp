#include "app/cli.hpp"
#include "audit/trace.hpp"
#include "dispatch/policy.hpp"
#include "dispatch/scheduler.hpp"
#include "warehouse/domain.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

void require(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(1);
    }
}

void test_inventory_and_scheduler() {
    warehouse::Inventory inventory;
    inventory.add_station("S1", 2);
    inventory.add_station("S2", 1);
    audit::Trace trace;
    dispatch::Scheduler scheduler(
        inventory, dispatch::make_policy("priority"), trace);

    const auto first = scheduler.submit({"O1", "SKU-A", 1});
    require(first.accepted && first.station_id == "S1", "first order S1");
    require(inventory.station("S1")->stock == 1, "stock decremented");
    require(inventory.station("S1")->robot_state == warehouse::RobotState::busy,
            "robot becomes busy");

    const auto second = scheduler.submit({"O2", "SKU-A", 1});
    require(second.accepted && second.station_id == "S2",
            "busy station skipped");
    std::string failure;
    require(scheduler.complete("S1", "O1", failure), "completion succeeds");
    require(inventory.station("S1")->robot_state == warehouse::RobotState::idle,
            "completion restores idle");
}

void test_stock_policy_and_failed_submission() {
    warehouse::Inventory inventory;
    inventory.add_station("S1", 1);
    inventory.add_station("S2", 4);
    inventory.add_station("S3", 4);
    audit::Trace trace;
    dispatch::Scheduler scheduler(
        inventory, dispatch::make_policy("stock"), trace);

    const auto first = scheduler.submit({"O1", "SKU-A", 1});
    require(first.accepted && first.station_id == "S2",
            "stock policy tie uses station id");
    const auto rejected = scheduler.submit({"O2", "SKU-A", 10});
    require(!rejected.accepted, "insufficient stock rejected");
    require(inventory.station("S1")->stock == 1,
            "failed submission has no side effects");
    require(trace.events().size() == 1, "failed submission is not traced");
}

void test_trace_and_validation() {
    audit::Trace trace;
    trace.append("O1 accepted");
    trace.append("O1 completed");
    require(trace.events().size() == 2, "trace preserves order");
    trace.clear();
    require(trace.events().empty(), "trace clears");

    require(app::normalize_sku(" sku-a ") == "SKU-A", "SKU normalized");
    require(!app::valid_station_id("S1 "), "station id rejects whitespace");
    require(!app::valid_order_id(""), "order id rejects empty");
}

}  // namespace

int main() {
    try {
        test_inventory_and_scheduler();
        test_stock_policy_and_failed_submission();
        test_trace_and_validation();
        std::cout << "PUBLIC 2/2\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "ERROR: " << error.what() << '\n';
        return 1;
    }
}
