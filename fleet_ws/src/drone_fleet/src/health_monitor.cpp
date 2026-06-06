#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

#include <sstream>
#include <iomanip>
#include <map>
#include <deque>
#include <string>
#include <chrono>
#include <ctime>
#include <mutex>

struct BatterySample {
    float battery;
    double timestamp;
};

static std::string current_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto tt = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&tt), "%H:%M:%S");
    return oss.str();
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

class HealthMonitorNode : public rclcpp::Node {
public:
    HealthMonitorNode() : Node("health_monitor") {
        drone_names_ = {"Alpha", "Beta", "Gamma"};

        health_warning_pub_ = this->create_publisher<std_msgs::msg::String>(
            "/fleet/health_warning", 10);
        health_summary_pub_ = this->create_publisher<std_msgs::msg::String>(
            "/fleet/health_summary", 10);

        for (const auto& name : drone_names_) {
            auto sub = this->create_subscription<std_msgs::msg::String>(
                "/drone/" + name + "/telemetry", 10,
                [this, name](const std_msgs::msg::String::SharedPtr msg) {
                    this->telemetry_callback(name, msg);
                });
            telemetry_subs_.push_back(sub);
        }

        diagnostics_timer_ = this->create_wall_timer(
            std::chrono::seconds(10),
            std::bind(&HealthMonitorNode::diagnostics_callback, this));

        RCLCPP_INFO(this->get_logger(), "Health Monitor started");
    }

private:
    void telemetry_callback(const std::string& name,
                             const std_msgs::msg::String::SharedPtr msg) {
        std::lock_guard<std::mutex> lock(mutex_);

        float battery = parse_json_float(msg->data, "battery");
        double now = this->get_clock()->now().seconds();

        auto& buffer = battery_history_[name];
        buffer.push_back({battery, now});
        if (buffer.size() > 10) {
            buffer.pop_front();
        }

        float drain_rate = compute_drain_rate(name);
        if (drain_rate > 1.5f) {
            auto warn_msg = std_msgs::msg::String();
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(2);
            oss << "[" << current_timestamp() << "] WARNING: "
                << name << " drain rate " << drain_rate
                << "/s exceeds threshold 1.5/s";
            warn_msg.data = oss.str();
            health_warning_pub_->publish(warn_msg);
            RCLCPP_WARN(this->get_logger(), "%s", warn_msg.data.c_str());
        }

        latest_battery_[name] = battery;
        latest_status_[name] = parse_json_string(msg->data, "status");
    }

    float compute_drain_rate(const std::string& name) {
        auto it = battery_history_.find(name);
        if (it == battery_history_.end() || it->second.size() < 2) {
            return 0.0f;
        }
        const auto& buf = it->second;
        float bat_diff = buf.front().battery - buf.back().battery;
        double time_diff = buf.back().timestamp - buf.front().timestamp;
        if (time_diff <= 0.0) return 0.0f;
        return static_cast<float>(bat_diff / time_diff);
    }

    void diagnostics_callback() {
        std::lock_guard<std::mutex> lock(mutex_);

        std::ostringstream table;
        table << "\n╔═══════════════════════════════════════════════════════════════════════╗\n";
        table << "║                    HEALTH DIAGNOSTICS                                 ║\n";
        table << "║  Time: " << std::setw(60) << std::left
              << current_timestamp() << " ║\n";
        table << "╠══════════╦════════════╦═════════════╦════════════════╦════════════════╣\n";
        table << "║  Drone   ║ Battery    ║ Drain Rate  ║ Time to Crit.  ║ Time to Depl.  ║\n";
        table << "╠══════════╬════════════╬═════════════╬════════════════╬════════════════╣\n";

        std::ostringstream json;
        json << "{\"timestamp\":\"" << current_timestamp() << "\",\"drones\":[";
        bool first = true;

        for (const auto& name : drone_names_) {
            float dr = compute_drain_rate(name);
            float bat = 0.0f;
            auto bat_it = latest_battery_.find(name);
            if (bat_it != latest_battery_.end()) bat = bat_it->second;

            float ttc = -1.0f;
            float ttd = -1.0f;
            if (dr > 0.0f) {
                float to_critical = bat - 20.0f;
                if (to_critical > 0.0f) ttc = to_critical / dr;
                if (bat > 0.0f) ttd = bat / dr;
            }

            std::ostringstream dr_str, ttc_str, ttd_str, bat_str;
            dr_str << std::fixed << std::setprecision(2) << dr << "/s";
            bat_str << std::fixed << std::setprecision(1) << bat << "%";

            if (ttc >= 0.0f)
                ttc_str << std::fixed << std::setprecision(0) << ttc << "s";
            else
                ttc_str << "N/A";

            if (ttd >= 0.0f)
                ttd_str << std::fixed << std::setprecision(0) << ttd << "s";
            else
                ttd_str << "N/A";

            table << "║ " << std::setw(9) << std::left << name
                  << "║ " << std::setw(11) << std::left << bat_str.str()
                  << "║ " << std::setw(12) << std::left << dr_str.str()
                  << "║ " << std::setw(15) << std::left << ttc_str.str()
                  << "║ " << std::setw(15) << std::left << ttd_str.str()
                  << "║\n";

            if (!first) json << ",";
            first = false;
            json << std::fixed << std::setprecision(2);
            json << "{\"name\":\"" << name << "\""
                 << ",\"battery\":" << bat
                 << ",\"drain_rate\":" << dr
                 << ",\"time_to_critical\":" << ttc
                 << ",\"time_to_depletion\":" << ttd << "}";
        }

        table << "╚══════════╩════════════╩═════════════╩════════════════╩════════════════╝\n";
        json << "]}";

        RCLCPP_INFO(this->get_logger(), "%s", table.str().c_str());

        auto summary_msg = std_msgs::msg::String();
        summary_msg.data = json.str();
        health_summary_pub_->publish(summary_msg);
    }

    std::vector<std::string> drone_names_;
    std::map<std::string, std::deque<BatterySample>> battery_history_;
    std::map<std::string, float> latest_battery_;
    std::map<std::string, std::string> latest_status_;
    std::mutex mutex_;

    std::vector<rclcpp::Subscription<std_msgs::msg::String>::SharedPtr> telemetry_subs_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr health_warning_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr health_summary_pub_;
    rclcpp::TimerBase::SharedPtr diagnostics_timer_;
};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<HealthMonitorNode>());
    rclcpp::shutdown();
    return 0;
}
