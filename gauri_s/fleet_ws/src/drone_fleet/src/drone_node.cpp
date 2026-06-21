#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "drone_fleet/MissionDrone.h"

#include <sstream>
#include <iomanip>
#include <chrono>

using namespace std::chrono_literals;

class DroneNode : public rclcpp::Node {
public:
    DroneNode() : rclcpp::Node("drone_node") {

        // Declare ROS 2 parameters with default values.
        // These can be overridden from the launch file or command line with -p name:=value.
        declare_parameter("drone_name",      std::string("Alpha"));
        declare_parameter("initial_battery", 100.0);
        declare_parameter("mission_name",    std::string("Default Mission"));

        std::string name    = get_parameter("drone_name").as_string();
        double      battery = get_parameter("initial_battery").as_double();
        std::string mission = get_parameter("mission_name").as_string();

        std::vector<std::tuple<float,float,float>> waypoints = {
            {10.f,  0.f, 15.f},
            {20.f, 10.f, 20.f},
            {30.f,  5.f, 25.f},
            {25.f, -5.f, 18.f},
            {10.f,-10.f, 10.f}
        };

        drone_ = std::make_unique<MissionDrone>(
            name, static_cast<float>(battery), mission, waypoints, 120.0f
        );
        drone_->take_off(15.0f);

        // create_publisher<MsgType>(topic, queue_size)
        // queue_size controls how many unsent messages to buffer
        status_pub_    = create_publisher<std_msgs::msg::String>("/drone/" + name + "/status",           10);
        alert_pub_     = create_publisher<std_msgs::msg::String>("/drone/" + name + "/alert",            10);
        complete_pub_  = create_publisher<std_msgs::msg::String>("/drone/" + name + "/mission_complete", 10);
        telemetry_pub_ = create_publisher<std_msgs::msg::String>("/drone/" + name + "/telemetry",        10);

        // create_wall_timer(interval, callback) fires the callback repeatedly
        // std::bind wraps the member function so the timer knows which object to call
        status_timer_    = create_wall_timer(1s, std::bind(&DroneNode::status_callback,    this));
        telemetry_timer_ = create_wall_timer(2s, std::bind(&DroneNode::telemetry_callback, this));

        RCLCPP_INFO(get_logger(), "DroneNode '%s' started, battery=%.1f%%", name.c_str(), battery);
    }

private:
    std::unique_ptr<MissionDrone> drone_;
    int publish_count_ = 0;

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr status_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr alert_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr complete_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr telemetry_pub_;

    rclcpp::TimerBase::SharedPtr status_timer_;
    rclcpp::TimerBase::SharedPtr telemetry_timer_;

    void status_callback() {
        ++publish_count_;

        try {
            drone_->drain_battery(0.1f);
        } catch (const BatteryDepletedError& e) {
            RCLCPP_WARN(get_logger(), "Battery depleted: %s", e.what());
        }

        // Advance to next waypoint every 3 publishes (every 3 seconds)
        if (publish_count_ % 3 == 0 && !drone_->mission_complete()) {
            try {
                drone_->next_waypoint();
            } catch (const std::exception& e) {
                RCLCPP_WARN(get_logger(), "Waypoint error: %s", e.what());
            }
        }

        // When all waypoints are done, publish completion and restart the mission
        if (drone_->mission_complete()) {
            auto msg = std_msgs::msg::String{};
            msg.data = drone_->get_name() + " mission complete, restarting";
            complete_pub_->publish(msg);

            std::string n   = drone_->get_name();
            float        bat = drone_->get_battery();
            std::vector<std::tuple<float,float,float>> wps = {
                {10.f,0.f,15.f},{20.f,10.f,20.f},{30.f,5.f,25.f},
                {25.f,-5.f,18.f},{10.f,-10.f,10.f}
            };
            drone_ = std::make_unique<MissionDrone>(n, bat, "Restarted Mission", wps, 120.f);
            drone_->take_off(15.0f);
            publish_count_ = 0;
            RCLCPP_INFO(get_logger(), "Mission restarted for %s", n.c_str());
        }

        // Battery critical: publish alert and land
        if (drone_->is_critical()) {
            auto alert = std_msgs::msg::String{};
            alert.data = "CRITICAL: " + drone_->get_name() +
                         " battery at " + std::to_string(drone_->get_battery()) + "%";
            alert_pub_->publish(alert);

            if (drone_->get_status() != "landed") {
                drone_->land();
                RCLCPP_WARN(get_logger(), "Critical battery, landing %s", drone_->get_name().c_str());
            }
        }

        // Build pipe-delimited status string and publish it
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1);
        oss << "name:"     << drone_->get_name()
            << "|battery:" << drone_->get_battery()
            << "|altitude:15.2"
            << "|status:"  << drone_->get_status()
            << "|waypoint:0/5"
            << "|speed:3.2";

        auto msg = std_msgs::msg::String{};
        msg.data = oss.str();
        status_pub_->publish(msg);
    }

    void telemetry_callback() {
        // Publish full drone state as a hand-crafted JSON string
        std::ostringstream json;
        json << std::fixed << std::setprecision(2);
        json << "{"
             << "\"name\":\""     << drone_->get_name()   << "\","
             << "\"battery\":"    << drone_->get_battery() << ","
             << "\"status\":\""   << drone_->get_status()  << "\","
             << "\"altitude\":15.20,"
             << "\"speed\":3.20,"
             << "\"is_critical\":" << (drone_->is_critical()      ? "true" : "false") << ","
             << "\"mission_complete\":" << (drone_->mission_complete() ? "true" : "false")
             << "}";

        auto msg = std_msgs::msg::String{};
        msg.data = json.str();
        telemetry_pub_->publish(msg);
    }
};

int main(int argc, char* argv[]) {
    // rclcpp::init sets up ROS 2 internals and parses ROS-specific command line args
    rclcpp::init(argc, argv);
    // rclcpp::spin blocks here, dispatching timer and subscription callbacks until Ctrl+C
    rclcpp::spin(std::make_shared<DroneNode>());
    rclcpp::shutdown();
    return 0;
}
