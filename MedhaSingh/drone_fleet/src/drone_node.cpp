#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include "drone_fleet/MissionDrone.hpp"
#include "drone_fleet/DroneExceptions.hpp"

#include <sstream>
#include <chrono>
#include <functional>
#include <memory>

class DroneNode : public rclcpp::Node
{
public:
    
    DroneNode()
    : Node("drone_node")
    {
        declare_parameter("drone_name", "Alpha");
        declare_parameter("initial_battery", 100.0);
        declare_parameter("mission_name", "SurveyMission");

        std::string drone_name =
            get_parameter("drone_name").as_string();

        double initial_battery =
            get_parameter("initial_battery").as_double();

        std::string mission_name =
            get_parameter("mission_name").as_string();

        (void)initial_battery;

        status_pub_ =
            create_publisher<std_msgs::msg::String>(
                "/drone/" + drone_name + "/status",
                10
            );

        timer_ =
            create_wall_timer(
                std::chrono::seconds(1),
                std::bind(
                    &DroneNode::timer_callback,
                    this
                )
            );
        
        telemetry_pub_ =
        create_publisher<std_msgs::msg::String>(
            "/drone/" + drone_name + "/telemetry",
            10
        );

        alert_pub_ =
        create_publisher<std_msgs::msg::String>(
            "/drone/" + drone_name + "/alert",
            10
        );

        telemetry_timer_ =
        create_wall_timer(
            std::chrono::seconds(2),
            std::bind(
                &DroneNode::telemetry_callback,
                this
            )
        );

        std::vector<std::tuple<float,float,float>> waypoints =
        {
            {10.0f, 10.0f, 10.0f},
            {20.0f, 10.0f, 15.0f},
            {30.0f, 15.0f, 20.0f},
            {40.0f, 20.0f, 20.0f},
            {50.0f, 25.0f, 25.0f}
        };

        drone_ = std::make_unique<MissionDrone>(
            drone_name,
            static_cast<float>(initial_battery),
            100.0f,
            5.0f,
            mission_name,
            waypoints
        );


    }

private:

    int publish_count_ = 0;
    void timer_callback()
    {
        try
        {
            std_msgs::msg::String msg;

            drone_->drain_battery(0.5f);

            publish_count_++;

            if (publish_count_ % 3 == 0)
            {
                try
                {
                    drone_->next_waypoint();
                }
                catch (...)
                {
                }
            }

            if (
                drone_->is_critical() &&
                !critical_alert_sent_
            )
            {
                critical_alert_sent_ = true;

                RCLCPP_WARN(
                    get_logger(),
                    "Battery critical!"
                );

                std_msgs::msg::String alert_msg;

                alert_msg.data =
                    "CRITICAL BATTERY";

                alert_pub_->publish(alert_msg);

                drone_->land();
            }

            std::stringstream ss;

            ss
                << "name:" << drone_->get_name()
                << "|battery:" << drone_->get_battery_level()
                << "|status:" << drone_->get_status()
                << "|waypoint:"
                << drone_->get_current_waypoint_index()
                << "/"
                << drone_->get_total_waypoints();

            msg.data = ss.str();

            status_pub_->publish(msg);
        }
        catch(const BatteryDepletedError& e)
        {
            RCLCPP_ERROR(
                get_logger(),
                "%s",
                e.what()
            );

            return;
        }
        
    }

    void telemetry_callback()
    {
        std_msgs::msg::String msg;

        std::stringstream ss;

        ss
        << "{"
        << "\"name\":\"" << drone_->get_name() << "\","
        << "\"battery\":" << drone_->get_battery_level() << ","
        << "\"status\":\"" << drone_->get_status() << "\""
        << "}";

        msg.data = ss.str();

        telemetry_pub_->publish(msg);
    }

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr
        status_pub_;

    rclcpp::TimerBase::SharedPtr timer_;

    rclcpp::TimerBase::SharedPtr telemetry_timer_;

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr telemetry_pub_;

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr alert_pub_;

    std::unique_ptr<MissionDrone> drone_;

    bool critical_alert_sent_ = false;

};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(
        std::make_shared<DroneNode>()
    );

    rclcpp::shutdown();

    return 0;
}