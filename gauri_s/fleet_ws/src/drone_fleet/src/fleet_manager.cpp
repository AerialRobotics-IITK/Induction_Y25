#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_srvs/srv/trigger.hpp"

#include <map>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <string>
#include <vector>

using namespace std::chrono_literals;

struct DroneState {
    std::string name     = "?";
    float       battery  = 0.f;
    float       altitude = 0.f;
    std::string waypoint = "0/5";
    std::string status   = "unknown";
    bool        updated  = false;
};

class FleetManager : public rclcpp::Node {
public:
    FleetManager() : rclcpp::Node("fleet_manager") {

        const std::vector<std::string> drones = {"Alpha", "Beta", "Gamma"};

        for (const auto& d : drones) {
            // Lambda callbacks capture [this] so they can call member functions.
            // Subscriptions are stored in vectors to keep them alive; a subscription
            // that goes out of scope is automatically unregistered.

            auto sub_s = create_subscription<std_msgs::msg::String>(
                "/drone/" + d + "/status", 10,
                [this](const std_msgs::msg::String::SharedPtr msg) {
                    parse_status(msg->data);
                });
            status_subs_.push_back(sub_s);

            auto sub_a = create_subscription<std_msgs::msg::String>(
                "/drone/" + d + "/alert", 10,
                [this](const std_msgs::msg::String::SharedPtr msg) {
                    // this->now() returns current ROS time; .nanoseconds() gives int64
                    RCLCPP_WARN(get_logger(), "[%ld] ALERT: %s",
                                this->now().nanoseconds(), msg->data.c_str());
                });
            alert_subs_.push_back(sub_a);

            auto sub_m = create_subscription<std_msgs::msg::String>(
                "/drone/" + d + "/mission_complete", 10,
                [this](const std_msgs::msg::String::SharedPtr msg) {
                    RCLCPP_INFO(get_logger(), "MISSION COMPLETE: %s", msg->data.c_str());
                });
            mission_subs_.push_back(sub_m);

            auto sub_t = create_subscription<std_msgs::msg::String>(
                "/drone/" + d + "/telemetry", 10,
                [this](const std_msgs::msg::String::SharedPtr msg) {
                    parse_telemetry(msg->data);
                });
            telemetry_subs_.push_back(sub_t);

            fleet_[d] = DroneState{};
            fleet_[d].name = d;
        }

        report_timer_ = create_wall_timer(5s, std::bind(&FleetManager::print_fleet_report, this));

        // create_service registers a ROS 2 service endpoint.
        // When a client calls /fleet/status_report, the lambda fires.
        // We fill in res->success and res->message, which are sent back to the caller.
        service_ = create_service<std_srvs::srv::Trigger>(
            "/fleet/status_report",
            [this](const std_srvs::srv::Trigger::Request::SharedPtr,
                         std_srvs::srv::Trigger::Response::SharedPtr res) {
                print_fleet_report();
                res->success = true;
                res->message = "Fleet report triggered";
            });

        RCLCPP_INFO(get_logger(), "FleetManager ready. Report every 5s.");
    }

private:
    std::map<std::string, DroneState> fleet_;

    std::vector<rclcpp::Subscription<std_msgs::msg::String>::SharedPtr> status_subs_;
    std::vector<rclcpp::Subscription<std_msgs::msg::String>::SharedPtr> alert_subs_;
    std::vector<rclcpp::Subscription<std_msgs::msg::String>::SharedPtr> mission_subs_;
    std::vector<rclcpp::Subscription<std_msgs::msg::String>::SharedPtr> telemetry_subs_;

    rclcpp::TimerBase::SharedPtr report_timer_;
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr service_;

    void parse_status(const std::string& data) {
        // Split by '|', then each token by ':', build a key-value map
        std::map<std::string, std::string> kv;
        std::string token;
        std::istringstream ss(data);
        while (std::getline(ss, token, '|')) {
            auto colon = token.find(':');
            if (colon != std::string::npos) {
                kv[token.substr(0, colon)] = token.substr(colon + 1);
            }
        }

        std::string name = kv.count("name") ? kv["name"] : "?";
        auto& s = fleet_[name];
        s.name     = name;
        s.battery  = kv.count("battery")  ? std::stof(kv["battery"])  : 0.f;
        s.altitude = kv.count("altitude") ? std::stof(kv["altitude"]) : 0.f;
        s.status   = kv.count("status")   ? kv["status"]              : "?";
        s.waypoint = kv.count("waypoint") ? kv["waypoint"]            : "?";
        s.updated  = true;
    }

    void parse_telemetry(const std::string& json) {
        // Manual JSON parsing: find "key": then read value until next , or }
        auto extract = [&](const std::string& key) -> std::string {
            std::string search = "\"" + key + "\":";
            auto pos = json.find(search);
            if (pos == std::string::npos) return "";
            pos += search.size();
            if (json[pos] == '"') {
                ++pos;
                auto end = json.find('"', pos);
                return json.substr(pos, end - pos);
            }
            auto end = json.find_first_of(",}", pos);
            return json.substr(pos, end - pos);
        };

        std::string name = extract("name");
        if (name.empty() || !fleet_.count(name)) return;

        auto& s = fleet_[name];
        std::string bat = extract("battery");
        if (!bat.empty()) s.battery = std::stof(bat);
        std::string alt = extract("altitude");
        if (!alt.empty()) s.altitude = std::stof(alt);
        s.status  = extract("status");
        s.updated = true;
    }

    void print_fleet_report() {
        // std::setw(N) sets column width; std::left left-aligns within that width
        std::ostringstream oss;
        oss << "\nFLEET STATUS REPORT\n";
        oss << std::left
            << std::setw(8)  << "Drone"
            << std::setw(10) << "Battery"
            << std::setw(10) << "Altitude"
            << std::setw(10) << "Waypoint"
            << std::setw(12) << "Status" << "\n";
        oss << std::string(50, '-') << "\n";

        for (auto& [name, s] : fleet_) {
            if (!s.updated) continue;
            oss << std::left
                << std::setw(8)  << s.name
                << std::setw(10) << (std::to_string((int)s.battery)  + "%")
                << std::setw(10) << (std::to_string((int)s.altitude) + "m")
                << std::setw(10) << s.waypoint
                << std::setw(12) << s.status << "\n";
        }

        RCLCPP_INFO(get_logger(), "%s", oss.str().c_str());
    }
};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<FleetManager>());
    rclcpp::shutdown();
    return 0;
}
