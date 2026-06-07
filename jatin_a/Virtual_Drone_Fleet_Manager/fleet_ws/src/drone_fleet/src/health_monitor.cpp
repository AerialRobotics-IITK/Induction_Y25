#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

#include <deque>
#include <map>
#include <vector>
#include <chrono>
#include <sstream>

using namespace std::chrono_literals;

class HealthMonitor : public rclcpp::Node {
public:
HealthMonitor() : Node("health_monitor") {
drones_ = {"Alpha", "Beta", "Gamma"};

    warning_pub_ = create_publisher<std_msgs::msg::String>("/fleet/health_warning", 10);
    summary_pub_ = create_publisher<std_msgs::msg::String>("/fleet/health_summary", 10);

    for (auto &d : drones_) {
        subs_.push_back(
            create_subscription<std_msgs::msg::String>(
                "/drone/" + d + "/telemetry", 10,
                [this, d](std_msgs::msg::String::SharedPtr msg) {
                    double battery = extract_battery(msg->data);

                    history_[d].push_back(battery);
                    if (history_[d].size() > 10)
                        history_[d].pop_front();

                    check_drain(d);
                }));
    }

    timer_ = create_wall_timer(10s, std::bind(&HealthMonitor::report, this));

    RCLCPP_INFO(this->get_logger(), "Health Monitor Running");
}


private:
double extract_battery(const std::string &json) {
size_t pos = json.find("\"battery\":");
if (pos == std::string::npos) return 0.0;


    pos += 10;
    size_t end = json.find(",", pos);

    return std::stod(json.substr(pos, end - pos));
}

void check_drain(const std::string &d) {
    if (history_[d].size() < 2) return;

    double drain = history_[d].front() - history_[d].back();

    if (drain > 1.5) {
        std_msgs::msg::String msg;
        msg.data = d + " HIGH BATTERY DRAIN";
        warning_pub_->publish(msg);
    }
}

void report() {
    RCLCPP_INFO(this->get_logger(), "\n=== HEALTH REPORT ===");

    std::stringstream json;
    json << "{";

    for (auto &d : drones_) {
        if (history_[d].size() < 2) continue;

        double drain = history_[d].front() - history_[d].back();

        RCLCPP_INFO(this->get_logger(),
            "%s | Drain: %.2f",
            d.c_str(), drain);

        json << "\"" << d << "\":" << drain << ",";
    }

    json << "}";

    std_msgs::msg::String msg;
    msg.data = json.str();
    summary_pub_->publish(msg);
}

std::vector<std::string> drones_;
std::map<std::string, std::deque<double>> history_;

std::vector<rclcpp::Subscription<std_msgs::msg::String>::SharedPtr> subs_;

rclcpp::Publisher<std_msgs::msg::String>::SharedPtr warning_pub_;
rclcpp::Publisher<std_msgs::msg::String>::SharedPtr summary_pub_;

rclcpp::TimerBase::SharedPtr timer_;


};

int main(int argc, char** argv) {
rclcpp::init(argc, argv);
rclcpp::spin(std::make_shared<HealthMonitor>());
rclcpp::shutdown();
return 0;
}
