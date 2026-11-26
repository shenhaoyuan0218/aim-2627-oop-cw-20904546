#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

#include <chrono>
#include <thread>
#include <string>

namespace warehouse_dispatch {

class SimulatorNode final : public rclcpp::Node {
public:
    SimulatorNode() : Node("warehouse_simulator") {
        event_publisher_ = create_publisher<std_msgs::msg::String>(
            "warehouse_events", 10);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        timer_ = create_wall_timer(
            std::chrono::milliseconds(300),
            [this]() { publish_next_event(); });
    }

private:
    void publish_next_event() {
        static const std::string events[] = {
            "ORDER O1 SKU-A 1",
            "ORDER O2 SKU-A 1",
            "ORDER O3 SKU-A 2",
            "COMPLETE O1",
            "ORDER O4 SKU-A 3",
            "WAIT 1",
        };
        if (next_event_ == 6) {
            rclcpp::shutdown();
            return;
        }
        std_msgs::msg::String message;
        message.data = events[next_event_++];
        event_publisher_->publish(message);
    }

    unsigned int next_event_ = 0;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr event_publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

}  // namespace warehouse_dispatch

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<warehouse_dispatch::SimulatorNode>());
    rclcpp::shutdown();
}
