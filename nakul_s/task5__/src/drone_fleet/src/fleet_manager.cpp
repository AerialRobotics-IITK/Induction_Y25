#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_srvs/srv/trigger.hpp"

#include <map>
#include <string>
#include <sstream>

class FleetManager : public rclcpp::Node
{
private:

    struct DroneInfo
    {
        double battery = 0.0;
        std::string status = "unknown";
    };

    std::map<std::string, DroneInfo> fleet_;

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr alpha_status_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr beta_status_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr gamma_status_;

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr alpha_alert_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr beta_alert_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr gamma_alert_;

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr alpha_telemetry_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr beta_telemetry_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr gamma_telemetry_;

    rclcpp::TimerBase::SharedPtr report_timer_;

    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr service_;

public:

    FleetManager()
    : Node("fleet_manager")
    {
        alpha_status_ =
            create_subscription<std_msgs::msg::String>(
                "/drone/Alpha/status",
                10,
                std::bind(
                    &FleetManager::status_callback,
                    this,
                    std::placeholders::_1));

        beta_status_ =
            create_subscription<std_msgs::msg::String>(
                "/drone/Beta/status",
                10,
                std::bind(
                    &FleetManager::status_callback,
                    this,
                    std::placeholders::_1));

        gamma_status_ =
            create_subscription<std_msgs::msg::String>(
                "/drone/Gamma/status",
                10,
                std::bind(
                    &FleetManager::status_callback,
                    this,
                    std::placeholders::_1));

        alpha_alert_ =
            create_subscription<std_msgs::msg::String>(
                "/drone/Alpha/alert",
                10,
                std::bind(
                    &FleetManager::alert_callback,
                    this,
                    std::placeholders::_1));

        beta_alert_ =
            create_subscription<std_msgs::msg::String>(
                "/drone/Beta/alert",
                10,
                std::bind(
                    &FleetManager::alert_callback,
                    this,
                    std::placeholders::_1));

        gamma_alert_ =
            create_subscription<std_msgs::msg::String>(
                "/drone/Gamma/alert",
                10,
                std::bind(
                    &FleetManager::alert_callback,
                    this,
                    std::placeholders::_1));

        alpha_telemetry_ =
            create_subscription<std_msgs::msg::String>(
                "/drone/Alpha/telemetry",
                10,
                std::bind(
                    &FleetManager::telemetry_callback,
                    this,
                    std::placeholders::_1));

        beta_telemetry_ =
            create_subscription<std_msgs::msg::String>(
                "/drone/Beta/telemetry",
                10,
                std::bind(
                    &FleetManager::telemetry_callback,
                    this,
                    std::placeholders::_1));

        gamma_telemetry_ =
            create_subscription<std_msgs::msg::String>(
                "/drone/Gamma/telemetry",
                10,
                std::bind(
                    &FleetManager::telemetry_callback,
                    this,
                    std::placeholders::_1));

        report_timer_ =
            create_wall_timer(
                std::chrono::seconds(5),
                std::bind(
                    &FleetManager::print_report,
                    this));

        service_ =
            create_service<std_srvs::srv::Trigger>(
                "/fleet/status_report",
                std::bind(
                    &FleetManager::service_callback,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2));
    }

private:

    std::string extract_string(
        const std::string& json,
        const std::string& key)
    {
        auto start =
            json.find("\"" + key + "\":\"");

        if(start == std::string::npos)
            return "";

        start += key.size() + 4;

        auto end =
            json.find("\"", start);

        return json.substr(
            start,
            end - start);
    }

    double extract_number(
        const std::string& json,
        const std::string& key)
    {
        auto start =
            json.find("\"" + key + "\":");

        if(start == std::string::npos)
            return 0;

        start += key.size() + 3;

        auto end =
            json.find(",", start);

        if(end == std::string::npos)
            end = json.find("}", start);

        return std::stod(
            json.substr(start,
            end - start));
    }

    void telemetry_callback(
        const std_msgs::msg::String::SharedPtr msg)
    {
        std::string name =
            extract_string(
                msg->data,
                "name");

        fleet_[name].battery =
            extract_number(
                msg->data,
                "battery");

        fleet_[name].status =
            extract_string(
                msg->data,
                "status");
    }

    void status_callback(
        const std_msgs::msg::String::SharedPtr)
    {
    }

    void alert_callback(
        const std_msgs::msg::String::SharedPtr msg)
    {
        RCLCPP_WARN(
            get_logger(),
            "%s",
            msg->data.c_str());
    }

    void print_report()
    {
        std::cout
            << "\n==============================\n";

        std::cout
            << "Drone\tBattery\tStatus\n";

        std::cout
            << "------------------------------\n";

        for(const auto& drone : fleet_)
        {
            std::cout
                << drone.first << "\t"
                << drone.second.battery << "\t"
                << drone.second.status
                << "\n";
        }

        std::cout
            << "==============================\n";
    }

    void service_callback(
        const std::shared_ptr<
            std_srvs::srv::Trigger::Request>,
        std::shared_ptr<
            std_srvs::srv::Trigger::Response>
        response)
    {
        print_report();

        response->success = true;
        response->message =
            "Fleet report generated";
    }
};

int main(int argc,char **argv)
{
    rclcpp::init(argc,argv);

    rclcpp::spin(
        std::make_shared<FleetManager>());

    rclcpp::shutdown();

    return 0;
}