#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_srvs/srv/trigger.hpp>

#include <sstream>
#include <iomanip>
#include <map>
#include <string>
#include <chrono>
#include <ctime>
#include <mutex>

struct DroneStatus {
    std::string name;
    float battery = 0.0f;
    float altitude = 0.0f;
    std::string status = "unknown";
    int waypoint_current = 0;
    int waypoint_total = 0;
    float speed = 0.0f;
    std::string mission;
    bool critical = false;
};

static std::string current_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto tt = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&tt), "%H:%M:%S");
    return oss.str();
}

static std::string parse_json_string(const std::string& json,
                                      const std::string& key) {
    std::string search = "\"" + key + "\":\"";
    auto pos = json.find(search);
    if (pos == std::string::npos) return "";
    pos += search.size();
    auto end = json.find("\"", pos);
    if (end == std::string::npos) return "";
    return json.substr(pos, end - pos);
}

static float parse_json_float(const std::string& json,
                               const std::string& key) {
    std::string search = "\"" + key + "\":";
    auto pos = json.find(search);
    if (pos == std::string::npos) return 0.0f;
    pos += search.size();
    while (pos < json.size() && json[pos] == ' ') pos++;
    auto end = json.find_first_of(",}", pos);
    return std::stof(json.substr(pos, end - pos));
}

static int parse_json_int(const std::string& json, const std::string& key) {
    std::string search = "\"" + key + "\":";
    auto pos = json.find(search);
    if (pos == std::string::npos) return 0;
    pos += search.size();
    while (pos < json.size() && json[pos] == ' ') pos++;
    auto end = json.find_first_of(",}", pos);
    return std::stoi(json.substr(pos, end - pos));
}

static bool parse_json_bool(const std::string& json, const std::string& key) {
    std::string search = "\"" + key + "\":";
    auto pos = json.find(search);
    if (pos == std::string::npos) return false;
    pos += search.size();
    while (pos < json.size() && json[pos] == ' ') pos++;
    return json.substr(pos, 4) == "true";
}

class FleetManagerNode : public rclcpp::Node {
public:
    FleetManagerNode() : Node("fleet_manager") {
        drone_names_ = {"Alpha", "Beta", "Gamma"};

        for (const auto& name : drone_names_) {
            fleet_[name] = DroneStatus{name};

            auto status_sub = this->create_subscription<std_msgs::msg::String>(
                "/drone/" + name + "/status", 10,
                [this, name](const std_msgs::msg::String::SharedPtr msg) {
                    this->status_callback(name, msg);
                });
            status_subs_.push_back(status_sub);

            auto alert_sub = this->create_subscription<std_msgs::msg::String>(
                "/drone/" + name + "/alert", 10,
                [this, name](const std_msgs::msg::String::SharedPtr msg) {
                    this->alert_callback(name, msg);
                });
            alert_subs_.push_back(alert_sub);

            auto mc_sub = this->create_subscription<std_msgs::msg::String>(
                "/drone/" + name + "/mission_complete", 10,
                [this, name](const std_msgs::msg::String::SharedPtr msg) {
                    this->mission_complete_callback(name, msg);
                });
            mc_subs_.push_back(mc_sub);

            auto telem_sub = this->create_subscription<std_msgs::msg::String>(
                "/drone/" + name + "/telemetry", 10,
                [this, name](const std_msgs::msg::String::SharedPtr msg) {
                    this->telemetry_callback(name, msg);
                });
            telemetry_subs_.push_back(telem_sub);
        }

        report_timer_ = this->create_wall_timer(
            std::chrono::seconds(5),
            std::bind(&FleetManagerNode::print_report, this));

        service_ = this->create_service<std_srvs::srv::Trigger>(
            "/fleet/status_report",
            std::bind(&FleetManagerNode::service_callback, this,
                      std::placeholders::_1, std::placeholders::_2));

        RCLCPP_INFO(this->get_logger(), "Fleet Manager started");
    }

private:
    void status_callback(const std::string& /*name*/,
                         const std_msgs::msg::String::SharedPtr /*msg*/) {
    }

    void alert_callback(const std::string& name,
                        const std_msgs::msg::String::SharedPtr msg) {
        RCLCPP_WARN(this->get_logger(), "[%s] ALERT from %s: %s",
                    current_timestamp().c_str(), name.c_str(),
                    msg->data.c_str());
    }

    void mission_complete_callback(const std::string& name,
                                    const std_msgs::msg::String::SharedPtr msg) {
        RCLCPP_INFO(this->get_logger(), "[%s] %s: %s",
                    current_timestamp().c_str(), name.c_str(),
                    msg->data.c_str());
    }

    void telemetry_callback(const std::string& name,
                             const std_msgs::msg::String::SharedPtr msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto& ds = fleet_[name];
        ds.name = parse_json_string(msg->data, "name");
        ds.battery = parse_json_float(msg->data, "battery");
        ds.altitude = parse_json_float(msg->data, "altitude");
        ds.status = parse_json_string(msg->data, "status");
        ds.waypoint_current = parse_json_int(msg->data, "waypoint_current");
        ds.waypoint_total = parse_json_int(msg->data, "waypoint_total");
        ds.speed = parse_json_float(msg->data, "speed");
        ds.mission = parse_json_string(msg->data, "mission");
        ds.critical = parse_json_bool(msg->data, "critical");
    }

    std::string generate_report() {
        std::lock_guard<std::mutex> lock(mutex_);
        std::ostringstream oss;
        oss << "\n╔══════════════════════════════════════════════════════════════════╗\n";
        oss << "║                    FLEET STATUS REPORT                          ║\n";
        oss << "║  Time: " << std::setw(54) << std::left
            << current_timestamp() << " ║\n";
        oss << "╠══════════╦══════════╦══════════╦══════════╦══════════════════════╣\n";
        oss << "║  Drone   ║ Battery  ║ Altitude ║ Waypoint ║      Status         ║\n";
        oss << "╠══════════╬══════════╬══════════╬══════════╬══════════════════════╣\n";

        for (const auto& name : drone_names_) {
            auto it = fleet_.find(name);
            if (it == fleet_.end()) continue;
            const auto& ds = it->second;
            std::ostringstream batt;
            batt << std::fixed << std::setprecision(1) << ds.battery << "%";
            std::string batt_str = batt.str();
            if (ds.critical) batt_str += " !";

            std::ostringstream alt;
            alt << std::fixed << std::setprecision(1) << ds.altitude << "m";

            std::ostringstream wp;
            wp << ds.waypoint_current << "/" << ds.waypoint_total;

            oss << "║ " << std::setw(9) << std::left << name
                << "║ " << std::setw(9) << std::left << batt_str
                << "║ " << std::setw(9) << std::left << alt.str()
                << "║ " << std::setw(9) << std::left << wp.str()
                << "║ " << std::setw(21) << std::left << ds.status
                << "║\n";
        }

        oss << "╚══════════╩══════════╩══════════╩══════════╩══════════════════════╝\n";
        return oss.str();
    }

    void print_report() {
        RCLCPP_INFO(this->get_logger(), "%s", generate_report().c_str());
    }

    void service_callback(
        const std::shared_ptr<std_srvs::srv::Trigger::Request> /*request*/,
        std::shared_ptr<std_srvs::srv::Trigger::Response> response) {
        response->success = true;
        response->message = generate_report();
        RCLCPP_INFO(this->get_logger(), "Status report requested via service");
    }

    std::vector<std::string> drone_names_;
    std::map<std::string, DroneStatus> fleet_;
    std::mutex mutex_;

    std::vector<rclcpp::Subscription<std_msgs::msg::String>::SharedPtr> status_subs_;
    std::vector<rclcpp::Subscription<std_msgs::msg::String>::SharedPtr> alert_subs_;
    std::vector<rclcpp::Subscription<std_msgs::msg::String>::SharedPtr> mc_subs_;
    std::vector<rclcpp::Subscription<std_msgs::msg::String>::SharedPtr> telemetry_subs_;

    rclcpp::TimerBase::SharedPtr report_timer_;
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr service_;
};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<FleetManagerNode>());
    rclcpp::shutdown();
    return 0;
}
