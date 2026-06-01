#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

#include "drone_fleet/MissionDrone.hpp"
#include "drone_fleet/DroneExceptions.hpp"

#include <sstream>
#include <iomanip>
#include <chrono>
#include <memory>

class DroneNode : public rclcpp::Node {
public:
    DroneNode() : Node("drone_node") {
        this->declare_parameter<std::string>("drone_name", "Alpha");
        this->declare_parameter<double>("initial_battery", 100.0);
        this->declare_parameter<std::string>("mission_name", "Patrol");

        drone_name_ = this->get_parameter("drone_name").as_string();
        double init_bat = this->get_parameter("initial_battery").as_double();
        std::string mission = this->get_parameter("mission_name").as_string();

        std::vector<std::tuple<float, float, float>> waypoints = {
            {10.0f, 20.0f, 15.0f},
            {30.0f, 40.0f, 25.0f},
            {50.0f, 60.0f, 20.0f},
            {70.0f, 80.0f, 30.0f},
            {90.0f, 10.0f, 10.0f}
        };

        drone_ = std::make_unique<MissionDrone>(
            drone_name_, mission, waypoints,
            static_cast<float>(init_bat), 120.0f, 3.2f);

        try {
            drone_->take_off(15.0f);
        } catch (const DroneException& e) {
            RCLCPP_ERROR(this->get_logger(), "Failed to take off: %s", e.what());
        }

        status_pub_ = this->create_publisher<std_msgs::msg::String>(
            "/drone/" + drone_name_ + "/status", 10);
        alert_pub_ = this->create_publisher<std_msgs::msg::String>(
            "/drone/" + drone_name_ + "/alert", 10);
        mission_complete_pub_ = this->create_publisher<std_msgs::msg::String>(
            "/drone/" + drone_name_ + "/mission_complete", 10);
        telemetry_pub_ = this->create_publisher<std_msgs::msg::String>(
            "/drone/" + drone_name_ + "/telemetry", 10);

        publish_count_ = 0;

        status_timer_ = this->create_wall_timer(
            std::chrono::seconds(1),
            std::bind(&DroneNode::status_callback, this));

        telemetry_timer_ = this->create_wall_timer(
            std::chrono::seconds(2),
            std::bind(&DroneNode::telemetry_callback, this));

        RCLCPP_INFO(this->get_logger(),
                    "Drone '%s' started with battery=%.1f mission='%s'",
                    drone_name_.c_str(), init_bat, mission.c_str());
    }

private:
    void status_callback() {
        try {
            drone_->drain_battery(0.5f);
        } catch (const BatteryDepletedError&) {
            // handled below
        }
        publish_count_++;

        if (publish_count_ % 3 == 0 && !drone_->mission_complete()) {
            try {
                drone_->next_waypoint();
            } catch (const DroneException& e) {
                RCLCPP_WARN(this->get_logger(), "Waypoint error: %s", e.what());
            }
        }

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1);
        oss << "name:" << drone_name_
            << "|battery:" << drone_->get_battery_level()
            << "|altitude:" << drone_->get_altitude()
            << "|status:" << drone_->get_status()
            << "|waypoint:" << drone_->get_current_waypoint_index()
            << "/" << drone_->get_total_waypoints()
            << "|speed:" << drone_->get_speed();

        auto msg = std_msgs::msg::String();
        msg.data = oss.str();
        status_pub_->publish(msg);

        if (drone_->is_critical()) {
            auto alert_msg = std_msgs::msg::String();
            alert_msg.data = drone_name_ + " battery CRITICAL: " +
                std::to_string(drone_->get_battery_level()) + "%";
            alert_pub_->publish(alert_msg);
            RCLCPP_WARN(this->get_logger(), "CRITICAL battery: %.1f%%",
                        drone_->get_battery_level());

            if (drone_->get_status() == "flying") {
                try {
                    drone_->land();
                    RCLCPP_INFO(this->get_logger(),
                                "Emergency landing due to critical battery");
                } catch (const DroneException& e) {
                    RCLCPP_ERROR(this->get_logger(), "Land failed: %s",
                                 e.what());
                }
            }
        }

        if (drone_->mission_complete()) {
            auto mc_msg = std_msgs::msg::String();
            mc_msg.data = drone_name_ + " mission complete. Restarting...";
            mission_complete_pub_->publish(mc_msg);
            RCLCPP_INFO(this->get_logger(), "Mission complete, restarting.");
            drone_->reset_mission();
            if (drone_->get_status() == "landed" &&
                !drone_->is_critical()) {
                try {
                    drone_->take_off(15.0f);
                } catch (const DroneException& e) {
                    RCLCPP_WARN(this->get_logger(),
                                "Could not re-take off: %s", e.what());
                }
            }
        }
    }

    void telemetry_callback() {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << "{";
        oss << "\"name\":\"" << drone_name_ << "\",";
        oss << "\"battery\":" << drone_->get_battery_level() << ",";
        oss << "\"altitude\":" << drone_->get_altitude() << ",";
        oss << "\"status\":\"" << drone_->get_status() << "\",";
        oss << "\"waypoint_current\":" << drone_->get_current_waypoint_index() << ",";
        oss << "\"waypoint_total\":" << drone_->get_total_waypoints() << ",";
        oss << "\"speed\":" << drone_->get_speed() << ",";
        oss << "\"mission\":\"" << drone_->mission_name << "\",";
        oss << "\"critical\":" << (drone_->is_critical() ? "true" : "false");
        oss << "}";

        auto msg = std_msgs::msg::String();
        msg.data = oss.str();
        telemetry_pub_->publish(msg);
    }

    std::string drone_name_;
    std::unique_ptr<MissionDrone> drone_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr status_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr alert_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr mission_complete_pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr telemetry_pub_;
    rclcpp::TimerBase::SharedPtr status_timer_;
    rclcpp::TimerBase::SharedPtr telemetry_timer_;
    int publish_count_;
};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DroneNode>());
    rclcpp::shutdown();
    return 0;
}
