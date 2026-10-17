#include "vehicle.hpp"
#include "drone.hpp"
#include "mission_drone.hpp"
#include "autonomous_drone.hpp"
#include "drone_exceptions.hpp"

#include <iostream>
#include <vector>
#include <tuple>

int main() {
    std::cout << "========================================\n";
    std::cout << "   Virtual Drone Fleet - OOP Demo\n";
    std::cout << "========================================\n\n";

    Drone basic_drone("Scout-1", 80.0f, 100.0f, 3.0f);

    std::vector<std::tuple<float, float, float>> mission_wps = {
        {10.0f, 20.0f, 15.0f},
        {30.0f, 40.0f, 25.0f},
        {50.0f, 60.0f, 20.0f},
        {70.0f, 80.0f, 30.0f},
        {90.0f, 10.0f, 10.0f}
    };
    MissionDrone mission_drone("Mapper-1", "Survey-Alpha", mission_wps,
                               90.0f, 120.0f, 4.5f);

    std::vector<std::tuple<float, float, float>> auto_wps = {
        {5.0f, 10.0f, 12.0f},
        {15.0f, 25.0f, 18.0f},
        {35.0f, 45.0f, 22.0f},
        {55.0f, 65.0f, 28.0f},
        {75.0f, 85.0f, 15.0f}
    };
    AutonomousDrone auto_drone("Phoenix-1", "Recon-Delta", auto_wps,
                               {0.0f, 0.0f, 0.0f}, 100.0f, 150.0f, 6.0f);

    // Polymorphism demo: store all in a vector of Vehicle* and call get_info()
    std::cout << "--- Polymorphism: calling get_info() on Vehicle* ---\n";
    std::vector<Vehicle*> fleet = {&basic_drone, &mission_drone, &auto_drone};
    for (const auto* v : fleet) {
        std::cout << v->get_info() << "\n";
    }
    std::cout << "\n";

    // Private member access demo
    // basic_drone.battery_level_ = 999;  // ERROR: 'battery_level_' is private
    // basic_drone.status_ = "hacked";    // ERROR: 'status_' is private
    // Direct access to private members would produce a compile error.
    std::cout << "--- Private members are not directly accessible ---\n";
    std::cout << "Battery via getter: " << basic_drone.get_battery_level()
              << "%\n";
    std::cout << "Status via getter: " << basic_drone.get_status() << "\n\n";

    // Exception handling demos
    std::cout << "--- Exception Handling ---\n";

    try {
        Drone low_drone("LowBat", 5.0f);
        low_drone.drain_battery(5.0f);
        std::cout << "Drained LowBat to 0. Attempting another drain...\n";
        low_drone.drain_battery(1.0f);
    } catch (const BatteryDepletedError& e) {
        std::cout << "Caught BatteryDepletedError: " << e.what() << "\n";
    }

    try {
        std::cout << "\nAttempting take_off beyond max altitude...\n";
        basic_drone.take_off(999.0f);
    } catch (const AltitudeError& e) {
        std::cout << "Caught AltitudeError: " << e.what() << "\n";
    }

    try {
        std::cout << "\nAttempting charge without charging state...\n";
        basic_drone.charge_battery(10.0f, 5);
    } catch (const InvalidStateError& e) {
        std::cout << "Caught InvalidStateError: " << e.what() << "\n";
    }

    try {
        Drone depleted("Empty", 0.0f);
        depleted.drain_battery(1.0f);
    } catch (const DroneException& e) {
        std::cout << "\nCaught generic DroneException: " << e.what() << "\n";
    }
    std::cout << "\n";

    // Full autonomous mission
    std::cout << "========================================\n";
    std::cout << "   Full Autonomous Mission Demo\n";
    std::cout << "========================================\n\n";

    auto_drone.set_ai_mode("auto");
    auto_drone.take_off(25.0f);
    std::cout << "Phoenix-1 took off. " << auto_drone.get_info() << "\n\n";

    int wp_count = 0;
    while (!auto_drone.mission_complete()) {
        try {
            auto [x, y, z] = auto_drone.next_waypoint();
            wp_count++;
            std::cout << "Waypoint " << wp_count << " reached: ("
                      << x << ", " << y << ", " << z << ")\n";

            if (wp_count == 3) {
                std::cout << "\n*** High-severity obstacle detected! ***\n";
                auto_drone.detect_obstacle({36.0f, 46.0f, 22.0f}, "high");
                std::cout << "After emergency stop: "
                          << auto_drone.get_info() << "\n";
                auto_drone.take_off(20.0f);
                std::cout << "Recovered and airborne again.\n\n";
            }
        } catch (const BatteryDepletedError& e) {
            std::cout << "Battery depleted during mission: " << e.what()
                      << "\n";
            break;
        } catch (const DroneException& e) {
            std::cout << "Drone error during mission: " << e.what() << "\n";
            break;
        }
    }

    std::cout << "\n--- Auto Replan Demo ---\n";
    std::vector<std::tuple<float, float, float>> obstacles = {
        {12.0f, 22.0f, 14.0f},
        {52.0f, 62.0f, 27.0f}
    };
    auto replanned = auto_drone.auto_replan(obstacles);
    std::cout << "Replanned waypoints:\n";
    for (size_t i = 0; i < replanned.size(); ++i) {
        auto [x, y, z] = replanned[i];
        std::cout << "  [" << i << "] (" << x << ", " << y << ", " << z
                  << ")\n";
    }

    auto_drone.land();

    std::cout << "\n" << auto_drone.mission_summary() << "\n";
    std::cout << auto_drone.get_flight_log() << "\n";

    std::cout << "--- Final Fleet State ---\n";
    for (const auto* v : fleet) {
        std::cout << v->get_info() << "\n";
    }

    return 0;
}
