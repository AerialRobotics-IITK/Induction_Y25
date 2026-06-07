#include "rclcpp/rclcpp.hpp"

#include "vehicle.hpp"
#include "drone.hpp"
#include "mission_drone.hpp"
#include "autonomous_drone.hpp"

#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class DroneNode : public rclcpp::Node {
public:
DroneNode() : Node("drone_node"), publish_count_(0), current_waypoint_(0) {

    // Parameters
    this->declare_parameter("drone_name", "Alpha");
    this->declare_parameter("initial_battery", 100.0);
    this->declare_parameter("mission_name", "Survey");

    name_ = this->get_parameter("drone_name").as_string();
    battery_ = this->get_parameter("initial_battery").as_double();
    mission_name_ = this->get_parameter("mission_name").as_string();

    altitude_ = 10.0;
    speed_ = 3.2;
    total_waypoints_ = 5;

    // Publishers
    status_pub_ = this->create_publisher<std_msgs::msg::String>("/drone/" + name_ + "/status", 10);
    telemetry_pub_ = this->create_publisher<std_msgs::msg::String>("/drone/" + name_ + "/telemetry", 10);
    alert_pub_ = this->create_publisher<std_msgs::msg::String>("/drone/" + name_ + "/alert", 10);
    mission_pub_ = this->create_publisher<std_msgs::msg::String>("/drone/" + name_ + "/mission_complete", 10);

    // Timers
    status_timer_ = this->create_wall_timer(1s, std::bind(&DroneNode::publish_status, this));
    telemetry_timer_ = this->create_wall_timer(2s, std::bind(&DroneNode::publish_telemetry, this));

    RCLCPP_INFO(this->get_logger(), "Drone %s initialized", name_.c_str());
}

private:
void publish_status() {
publish_count_++;

    // Battery drain
    battery_ -= 0.5;

    // Waypoint advance every 3 publishes
    if (publish_count_ % 3 == 0) {
        current_waypoint_++;
    }

    // Alert condition
    if (battery_ <= 20.0 && !alert_sent_) {
        std_msgs::msg::String msg;
        msg.data = name_ + " LOW BATTERY";
        alert_pub_->publish(msg);

        RCLCPP_WARN(this->get_logger(), "%s battery critical!", name_.c_str());

        land();
        alert_sent_ = true;
    }

    // Mission complete
    if (current_waypoint_ >= total_waypoints_) {
        std_msgs::msg::String msg;
        msg.data = name_ + " mission complete";
        mission_pub_->publish(msg);

        restart_mission();
    }

    // Status message
    std_msgs::msg::String msg;
    std::stringstream ss;

    ss << "name:" << name_
       << "|battery:" << battery_
       << "|altitude:" << altitude_
       << "|status:" << (battery_ <= 20 ? "landing" : "flying")
       << "|waypoint:" << current_waypoint_ << "/" << total_waypoints_
       << "|speed:" << speed_;

    msg.data = ss.str();
    status_pub_->publish(msg);
}

void publish_telemetry() {
    std_msgs::msg::String msg;
    std::stringstream ss;

    ss << "{"
       << "\"name\":\"" << name_ << "\","
       << "\"battery\":" << battery_ << ","
       << "\"altitude\":" << altitude_ << ","
       << "\"waypoint\":" << current_waypoint_ << ","
       << "\"status\":\"" << (battery_ <= 20 ? "landing" : "flying") << "\""
       << "}";

    msg.data = ss.str();
    telemetry_pub_->publish(msg);
}

void land() {
    altitude_ = 0.0;
}

void restart_mission() {
    current_waypoint_ = 0;
    publish_count_ = 0;
    alert_sent_ = false;
}

// Variables
std::string name_;
std::string mission_name_;

double battery_;
double altitude_;
double speed_;

int publish_count_;
int current_waypoint_;
int total_waypoints_;
bool alert_sent_ = false;

// ROS interfaces
rclcpp::Publisher<std_msgs::msg::String>::SharedPtr status_pub_;
rclcpp::Publisher<std_msgs::msg::String>::SharedPtr telemetry_pub_;
rclcpp::Publisher<std_msgs::msg::String>::SharedPtr alert_pub_;
rclcpp::Publisher<std_msgs::msg::String>::SharedPtr mission_pub_;

rclcpp::TimerBase::SharedPtr status_timer_;
rclcpp::TimerBase::SharedPtr telemetry_timer_;

};

int main(int argc, char** argv) {
rclcpp::init(argc, argv);
rclcpp::spin(std::make_shared<DroneNode>());
rclcpp::shutdown();
return 0;
}