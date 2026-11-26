#include "audit/trace.hpp"
#include "dispatch/policy.hpp"
#include "dispatch/scheduler.hpp"
#include "warehouse/domain.hpp"

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace warehouse_dispatch {
namespace {

warehouse::Inventory make_inventory() {
    warehouse::Inventory inventory;
    inventory.add_station("S1", 3);
    inventory.add_station("S2", 2);
    return inventory;
}

}

class DispatcherNode final : public rclcpp::Node {
public:
    explicit DispatcherNode(
        std::unique_ptr<dispatch::DispatchPolicy> dispatch_policy)
        : Node("warehouse_dispatcher"),
          inventory_(make_inventory()),
          scheduler_(inventory_, std::move(dispatch_policy), trace_) {
        result_publisher_ = create_publisher<std_msgs::msg::String>(
            "dispatch_results", 10);
        subscription_ = create_subscription<std_msgs::msg::String>(
            "warehouse_events", 10,
            [this](const std_msgs::msg::String::SharedPtr message) {
                handle_event(message->data);
            });
    }

private:
    void handle_event(const std::string& text) {
        std::istringstream input(text);
        std::vector<std::string> fields;
        std::string field;
        while (input >> field) {
            fields.push_back(field);
        }
        if (fields.size() == 2 && fields[0] == "COMPLETE") {
            std::string failure;
            const auto station = accepted_stations_.find(fields[1]);
            if (station == accepted_stations_.end()) {
                publish("BAD_COMPLETE", fields[1] + " unknown order");
                return;
            }
            const bool accepted = scheduler_.complete(
                station->second, fields[1], failure);
            publish(accepted ? "COMPLETE" : "BAD_COMPLETE",
                    fields[1] + (accepted ? "" : " " + failure));
            return;
        }
        if (fields.size() == 2 && fields[0] == "WAIT") {
            publish("WAITED", fields[1]);
            rclcpp::shutdown();
            return;
        }
        if (fields.size() != 4) {
            publish("BAD", text);
            return;
        }
        if (fields[0] == "ORDER") {
            int quantity = 0;
            try {
                quantity = std::stoi(fields[3]);
            } catch (const std::exception&) {
                publish("BAD", text);
                return;
            }
            const warehouse::Order order{fields[1], fields[2],
                                         quantity};
            const auto decision = scheduler_.submit(order);
            if (decision.accepted) {
                accepted_stations_.emplace(decision.order_id,
                                           decision.station_id);
            }
            publish(decision.accepted ? "ACCEPT" : "REJECT",
                    decision.accepted
                        ? decision.order_id + " " + decision.station_id
                        : decision.order_id);
        } else if (fields.size() == 4 && fields[0] == "COMPLETE") {
            std::string failure;
            const bool accepted = scheduler_.complete(
                fields[1], fields[2], failure);
            publish(accepted ? "COMPLETE" : "BAD_COMPLETE",
                    fields[2] + (accepted ? "" : " " + failure));
        } else {
            publish("BAD", text);
        }
    }

    void publish(const std::string& status, const std::string& detail) {
        std_msgs::msg::String message;
        message.data = status + " " + detail;
        RCLCPP_INFO(get_logger(), "%s", message.data.c_str());
        result_publisher_->publish(message);
    }

    warehouse::Inventory inventory_;
    audit::Trace trace_;
    dispatch::Scheduler scheduler_;
    std::unordered_map<std::string, std::string> accepted_stations_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr result_publisher_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

}  // namespace warehouse_dispatch

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_unique<warehouse_dispatch::DispatcherNode>(
        dispatch::make_policy("priority")));
    rclcpp::shutdown();
}
