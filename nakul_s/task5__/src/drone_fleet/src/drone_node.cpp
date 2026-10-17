#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include "drone_fleet/MissionDrone.hpp"

#include <sstream>

class DroneNode : public rclcpp::Node
{
private:

    MissionDrone drone;

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr status_pub;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr telemetry_pub;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr alert_pub;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr mission_pub;

    rclcpp::TimerBase::SharedPtr status_timer;
    rclcpp::TimerBase::SharedPtr telemetry_timer;

public:

    DroneNode()
    : Node("drone_node"),
      drone("Alpha",100.0,"Survey")
    {
        drone.add_waypoint(0,0,10);
        drone.add_waypoint(10,0,10);
        drone.add_waypoint(10,10,10);
        drone.add_waypoint(0,10,10);
        drone.add_waypoint(0,0,0);

        drone.take_off(10);

        status_pub =
            create_publisher<std_msgs::msg::String>(
                "/drone/Alpha/status",10);

        telemetry_pub =
            create_publisher<std_msgs::msg::String>(
                "/drone/Alpha/telemetry",10);

        alert_pub =
            create_publisher<std_msgs::msg::String>(
                "/drone/Alpha/alert",10);

        mission_pub =
            create_publisher<std_msgs::msg::String>(
                "/drone/Alpha/mission_complete",10);

        status_timer =
            create_wall_timer(
                std::chrono::seconds(1),
                std::bind(
                    &DroneNode::publish_status,
                    this));

        telemetry_timer =
            create_wall_timer(
                std::chrono::seconds(2),
                std::bind(
                    &DroneNode::publish_telemetry,
                    this));
    }

private:

    void publish_status()
    {
        try
        {
            drone.drain_battery(0.5f);

            std_msgs::msg::String msg;

            std::stringstream ss;

            ss
            << "name:" << drone.get_name()
            << "|battery:" << drone.get_battery_level()
            << "|altitude:" << drone.get_altitude()
            << "|status:" << drone.get_status()
            << "|waypoint:"
            << drone.get_current_waypoint_index()
            << "/"
            << drone.get_total_waypoints();

            msg.data = ss.str();

            status_pub->publish(msg);

            if(drone.is_critical())
            {
                std_msgs::msg::String alert;

                alert.data =
                    drone.get_name()
                    + " critical battery";

                alert_pub->publish(alert);

                drone.land();
            }

            if(drone.mission_complete())
            {
                std_msgs::msg::String complete;

                complete.data =
                    drone.get_name()
                    + " mission complete";

                mission_pub->publish(complete);

                drone.reset_mission();
            }
        }
        catch(...)
        {
        }
    }

    void publish_telemetry()
    {
        std_msgs::msg::String msg;

        std::stringstream ss;

        ss
        << "{"
        << "\"name\":\""
        << drone.get_name()
        << "\","

        << "\"battery\":"
        << drone.get_battery_level()
        << ","

        << "\"altitude\":"
        << drone.get_altitude()
        << ","

        << "\"waypoint\":"
        << drone.get_current_waypoint_index()
        << ","

        << "\"status\":\""
        << drone.get_status()
        << "\""
        << "}";

        msg.data = ss.str();

        telemetry_pub->publish(msg);
    }
};

int main(int argc,char **argv)
{
    rclcpp::init(argc,argv);

    rclcpp::spin(
        std::make_shared<DroneNode>());

    rclcpp::shutdown();

    return 0;
}