#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include <unordered_map>
#include <deque>
#include <sstream>
#include <string>
#include <chrono>
#include <cmath>

using namespace std::chrono;

class HealthMonitor : public rclcpp::Node
{
private:

    struct Sample
    {
        double battery;
        double time;
    };

    struct DroneData
    {
        std::deque<Sample> samples;
        double drain_rate = 0.0;
    };

    std::unordered_map<std::string, DroneData> drones_;

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr alpha_sub_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr beta_sub_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr gamma_sub_;

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr warning_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr summary_pub_;

    rclcpp::TimerBase::SharedPtr timer_;

public:

    HealthMonitor()
    : Node("health_monitor")
    {
        warning_pub_ =
            create_publisher<std_msgs::msg::String>(
                "/fleet/health_warning", 10);

        summary_pub_ =
            create_publisher<std_msgs::msg::String>(
                "/fleet/health_summary", 10);

        alpha_sub_ =
            create_subscription<std_msgs::msg::String>(
                "/drone/Alpha/telemetry",
                10,
                std::bind(&HealthMonitor::callback, this, std::placeholders::_1));

        beta_sub_ =
            create_subscription<std_msgs::msg::String>(
                "/drone/Beta/telemetry",
                10,
                std::bind(&HealthMonitor::callback, this, std::placeholders::_1));

        gamma_sub_ =
            create_subscription<std_msgs::msg::String>(
                "/drone/Gamma/telemetry",
                10,
                std::bind(&HealthMonitor::callback, this, std::placeholders::_1));

        timer_ =
            create_wall_timer(
                std::chrono::seconds(10),
                std::bind(&HealthMonitor::report, this));
    }

private:

    double now()
    {
        return duration_cast<seconds>(
            steady_clock::now().time_since_epoch()
        ).count();
    }

    std::string extract(const std::string &json, const std::string &key)
    {
        auto pos = json.find("\"" + key + "\":");

        if (pos == std::string::npos)
            return "";

        pos += key.size() + 3;

        auto end = json.find_first_of(",}", pos);

        return json.substr(pos, end - pos);
    }

    void callback(const std_msgs::msg::String::SharedPtr msg)
    {
        std::string data = msg->data;

        std::string name = extract(data, "name");
        std::string battery_str = extract(data, "battery");

        if (name.empty() || battery_str.empty())
            return;

        double battery = std::stod(battery_str);
        double t = now();

        auto &d = drones_[name];

        if (!d.samples.empty())
        {
            auto last = d.samples.back();

            double dt = t - last.time;
            double db = last.battery - battery;

            if (dt > 0)
                d.drain_rate = db / dt;
        }

        d.samples.push_back({battery, t});

        if (d.samples.size() > 10)
            d.samples.pop_front();

        if (d.drain_rate > 1.5)
        {
            std_msgs::msg::String warn;
            warn.data = name + " high drain rate detected!";
            warning_pub_->publish(warn);
        }
    }

    void report()
    {
        std::stringstream ss;

        ss << "\n===== HEALTH REPORT =====\n";
        ss << "Drone | DrainRate | ETA Critical | ETA Empty\n";

        for (auto &p : drones_)
        {
            std::string name = p.first;
            auto &d = p.second;

            double drain = d.drain_rate;
            double battery = d.samples.empty() ? 0 : d.samples.back().battery;

            double eta_critical = (battery - 20) / drain;
            double eta_empty = battery / drain;

            if (drain <= 0)
            {
                eta_critical = -1;
                eta_empty = -1;
            }

            ss << name << " | "
               << drain << " | "
               << eta_critical << "s | "
               << eta_empty << "s\n";
        }

        ss << "========================\n";

        std_msgs::msg::String msg;
        msg.data = ss.str();

        summary_pub_->publish(msg);

        std::cout << msg.data << std::endl;
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<HealthMonitor>());

    rclcpp::shutdown();

    return 0;
}