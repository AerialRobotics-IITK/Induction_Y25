#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include <deque>
#include <map>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <chrono>

using namespace std::chrono_literals;

struct DroneHealth {
    // std::deque used as a sliding window of the last 10 battery samples.
    // push_back adds to the back; pop_front removes the oldest from the front.
    std::deque<float>  battery_samples;
    std::deque<double> timestamps;
    float last_battery = -1.f;
};

class HealthMonitor : public rclcpp::Node {
public:
    HealthMonitor() : rclcpp::Node("health_monitor") {

        const std::vector<std::string> drones = {"Alpha", "Beta", "Gamma"};

        for (const auto& d : drones) {
            auto sub = create_subscription<std_msgs::msg::String>(
                "/drone/" + d + "/telemetry", 10,
                [this](const std_msgs::msg::String::SharedPtr msg) {
                    process_telemetry(msg->data);
                });
            subs_.push_back(sub);
            health_[d] = DroneHealth{};
        }

        warning_pub_ = create_publisher<std_msgs::msg::String>("/fleet/health_warning", 10);
        summary_pub_ = create_publisher<std_msgs::msg::String>("/fleet/health_summary",  10);

        diag_timer_ = create_wall_timer(10s, std::bind(&HealthMonitor::diagnostics_callback, this));

        RCLCPP_INFO(get_logger(), "HealthMonitor ready.");
    }

private:
    std::map<std::string, DroneHealth> health_;
    std::vector<rclcpp::Subscription<std_msgs::msg::String>::SharedPtr> subs_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr warning_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr summary_pub_;
    rclcpp::TimerBase::SharedPtr diag_timer_;

    void process_telemetry(const std::string& json) {
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

        std::string name    = extract("name");
        std::string bat_str = extract("battery");
        if (name.empty() || bat_str.empty() || !health_.count(name)) return;

        float  battery = std::stof(bat_str);
        // this->now().nanoseconds() gives ROS time as int64; divide by 1e9 for seconds
        double now_s   = static_cast<double>(this->now().nanoseconds()) / 1e9;

        auto& h = health_[name];
        h.battery_samples.push_back(battery);
        h.timestamps.push_back(now_s);

        // Keep only the last 10 samples by popping the oldest when we exceed the limit
        if (h.battery_samples.size() > 10) {
            h.battery_samples.pop_front();
            h.timestamps.pop_front();
        }

        if (h.battery_samples.size() >= 2) {
            float drain = compute_drain_rate(h);
            if (drain > 1.5f) {
                std::ostringstream warn;
                warn << std::fixed << std::setprecision(2);
                warn << "{\"drone\":\"" << name
                     << "\",\"drain_rate\":" << drain
                     << ",\"level\":\"high\"}";

                auto msg  = std_msgs::msg::String{};
                msg.data  = warn.str();
                warning_pub_->publish(msg);

                RCLCPP_WARN(get_logger(), "HIGH DRAIN on %s: %.2f%%/s (threshold 1.5%%/s)",
                            name.c_str(), drain);
            }
        }

        h.last_battery = battery;
    }

    float compute_drain_rate(const DroneHealth& h) {
        if (h.battery_samples.size() < 2) return 0.f;

        float  first_bat = h.battery_samples.front();
        float  last_bat  = h.battery_samples.back();
        double first_t   = h.timestamps.front();
        double last_t    = h.timestamps.back();

        double dt = last_t - first_t;
        if (dt < 0.001) return 0.f;

        // drain rate = battery lost divided by seconds elapsed
        float drain = (first_bat - last_bat) / static_cast<float>(dt);
        return std::max(0.f, drain);
    }

    void diagnostics_callback() {
        std::ostringstream table;
        table << "\nHEALTH DIAGNOSTICS\n";
        table << std::left
              << std::setw(8)  << "Drone"
              << std::setw(12) << "Drain/s"
              << std::setw(16) << "To Critical"
              << std::setw(16) << "To Depleted" << "\n";
        table << std::string(52, '-') << "\n";

        std::ostringstream json;
        json << std::fixed << std::setprecision(2);
        json << "{\"drones\":[";
        bool first = true;

        for (auto& [name, h] : health_) {
            if (h.battery_samples.empty()) continue;

            float drain   = compute_drain_rate(h);
            float battery = h.last_battery >= 0 ? h.last_battery : 0.f;

            // Time to critical (battery < 20%): remaining-above-20 divided by drain rate
            float ttc = (drain > 0.001f && battery > 20.f) ? (battery - 20.f) / drain : -1.f;
            // Time to depletion: all remaining battery divided by drain rate
            float ttd = (drain > 0.001f) ? battery / drain : -1.f;

            auto fmt_time = [](float t) -> std::string {
                if (t < 0) return "N/A";
                return std::to_string((int)(t / 60)) + "m " +
                       std::to_string((int)std::fmod(t, 60.f)) + "s";
            };

            table << std::left
                  << std::setw(8)  << name
                  << std::setw(12) << (std::to_string(drain).substr(0, 4) + "%/s")
                  << std::setw(16) << fmt_time(ttc)
                  << std::setw(16) << fmt_time(ttd) << "\n";

            if (!first) json << ",";
            json << "{\"name\":\"" << name
                 << "\",\"battery\":"          << battery
                 << ",\"drain_rate\":"         << drain
                 << ",\"time_to_critical\":"   << (ttc < 0 ? -1.f : ttc)
                 << ",\"time_to_depletion\":"  << (ttd < 0 ? -1.f : ttd)
                 << "}";
            first = false;
        }

        table << std::string(52, '-');
        json  << "]}";

        RCLCPP_INFO(get_logger(), "%s", table.str().c_str());

        auto msg = std_msgs::msg::String{};
        msg.data = json.str();
        summary_pub_->publish(msg);
    }
};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<HealthMonitor>());
    rclcpp::shutdown();
    return 0;
}
