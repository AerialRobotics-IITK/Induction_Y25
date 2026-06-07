#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_srvs/srv/trigger.hpp>

#include <map>
#include <vector>
#include <sstream>
#include <chrono>

using namespace std::chrono_literals;

class FleetManager : public rclcpp::Node {
public:
FleetManager() : Node("fleet_manager") {
drones_ = {"Alpha", "Beta", "Gamma"};


    for (auto &d : drones_) {
        // STATUS
        status_subs_.push_back(
            create_subscription<std_msgs::msg::String>(
                "/drone/" + d + "/status", 10,
                [this, d](std_msgs::msg::String::SharedPtr msg) {
                    status_map_[d] = msg->data;
                }));

        // TELEMETRY
        telemetry_subs_.push_back(
            create_subscription<std_msgs::msg::String>(
                "/drone/" + d + "/telemetry", 10,
                [this, d](std_msgs::msg::String::SharedPtr msg) {
                    parse_telemetry(d, msg->data);
                }));

        // ALERT
        alert_subs_.push_back(
            create_subscription<std_msgs::msg::String>(
                "/drone/" + d + "/alert", 10,
                [this](std_msgs::msg::String::SharedPtr msg) {
                    RCLCPP_WARN(this->get_logger(), "⚠ ALERT: %s", msg->data.c_str());
                }));

        // MISSION COMPLETE
        mission_subs_.push_back(
            create_subscription<std_msgs::msg::String>(
                "/drone/" + d + "/mission_complete", 10,
                [this](std_msgs::msg::String::SharedPtr msg) {
                    RCLCPP_INFO(this->get_logger(), "MISSION: %s", msg->data.c_str());
                }));
    }

    // Timer every 5 sec
    timer_ = create_wall_timer(5s, std::bind(&FleetManager::print_report, this));

    // Service
    service_ = create_service<std_srvs::srv::Trigger>(
        "/fleet/status_report",
        std::bind(&FleetManager::handle_service, this,
                  std::placeholders::_1, std::placeholders::_2));

    RCLCPP_INFO(this->get_logger(), "Fleet Manager Started");
}


private:
void parse_telemetry(const std::string &drone, const std::string &json) {
// VERY SIMPLE manual parsing
auto get_value = [&](const std::string &key) -> std::string {
size_t start = json.find(""" + key + "":");
if (start == std::string::npos) return "N/A";


        start += key.length() + 3;
        size_t end = json.find_first_of(",}", start);

        return json.substr(start, end - start);
    };

    telemetry_map_[drone]["battery"] = get_value("battery");
    telemetry_map_[drone]["altitude"] = get_value("altitude");
    telemetry_map_[drone]["waypoint"] = get_value("waypoint");
    telemetry_map_[drone]["status"] = get_value("status");
}

void print_report() {
    RCLCPP_INFO(this->get_logger(), "\n===== FLEET REPORT =====");

    for (auto &d : drones_) {
        auto &t = telemetry_map_[d];

        RCLCPP_INFO(this->get_logger(),
            "%s | Battery:%s | Alt:%s | WP:%s | Status:%s",
            d.c_str(),
            t["battery"].c_str(),
            t["altitude"].c_str(),
            t["waypoint"].c_str(),
            t["status"].c_str());
    }
}

void handle_service(
    const std::shared_ptr<std_srvs::srv::Trigger::Request>,
    std::shared_ptr<std_srvs::srv::Trigger::Response> res) {

    print_report();
    res->success = true;
    res->message = "Report generated";
}

std::vector<std::string> drones_;

std::map<std::string, std::string> status_map_;
std::map<std::string, std::map<std::string, std::string>> telemetry_map_;

std::vector<rclcpp::Subscription<std_msgs::msg::String>::SharedPtr> status_subs_;
std::vector<rclcpp::Subscription<std_msgs::msg::String>::SharedPtr> telemetry_subs_;
std::vector<rclcpp::Subscription<std_msgs::msg::String>::SharedPtr> alert_subs_;
std::vector<rclcpp::Subscription<std_msgs::msg::String>::SharedPtr> mission_subs_;

rclcpp::TimerBase::SharedPtr timer_;
rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr service_;


};

int main(int argc, char** argv) {
rclcpp::init(argc, argv);
rclcpp::spin(std::make_shared<FleetManager>());
rclcpp::shutdown();
return 0;
}
