#include "vehicle.hpp"
#include "drone.hpp"
#include "mission_drone.hpp"
#include "autonomous_drone.hpp"
#include "drone_exceptions.hpp"

#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>
#include <thread>

// -------------------------------
// SIMPLE ROS-STYLE LOG MACROS
// -------------------------------
#define LOG_INFO(msg)    std::cout << "[INFO] " << msg << std::endl
#define LOG_WARN(msg)    std::cout << "[WARN] " << msg << std::endl
#define LOG_ERROR(msg)   std::cerr << "[ERROR] " << msg << std::endl
#define LOG_SUCCESS(msg) std::cout << "[SUCCESS] " << msg << std::endl

int main() {

    LOG_INFO("Starting Drone Fleet Manager...");

    // -------------------------------
    // WAYPOINTS
    // -------------------------------
    std::vector<std::tuple<float,float,float>> wps = {
        {1,2,3}, {4,5,6}, {7,8,9}
    };

    // -------------------------------
    // OBJECT CREATION
    // -------------------------------
    LOG_INFO("Creating drone objects...");

    Drone d("Alpha", 100, 5, 2);
    MissionDrone md("Beta", 60, 4, 2, "Survey", wps);
    AutonomousDrone ad("Gamma", 35, 6, 2, "Recon", wps, {0,0,0});

    // -------------------------------
    // POLYMORPHISM TEST
    // -------------------------------
    LOG_INFO("Running polymorphism test...");

    std::vector<Vehicle*> fleet = {&d, &md, &ad};

    for (auto v : fleet) {
        LOG_INFO(v->get_info());
    }

    // -------------------------------
    // BASIC FUNCTION TESTS
    // -------------------------------
    LOG_INFO("Running basic function tests...");

    // ---- Drone ----
    try {
        LOG_INFO("[Alpha] Takeoff initiated");
        d.take_off(3);
        LOG_SUCCESS("[Alpha] Takeoff successful");
    } catch (const std::exception& e) {
        LOG_ERROR(std::string("[Alpha] ") + e.what());
    }

    try {
        LOG_INFO("[Alpha] Draining battery");
        d.drain_battery(20);
        LOG_SUCCESS("[Alpha] Battery updated");
    } catch (const std::exception& e) {
        LOG_ERROR(std::string("[Alpha] ") + e.what());
    }

    // ---- MissionDrone ----
    try {
        LOG_INFO("[Beta] Takeoff initiated");
        md.take_off(3);
        LOG_SUCCESS("[Beta] Takeoff successful");
    } catch (const std::exception& e) {
        LOG_ERROR(std::string("[Beta] ") + e.what());
    }

    try {
        LOG_INFO("[Beta] Moving to next waypoint");
        md.next_waypoint();
        LOG_SUCCESS("[Beta] Waypoint reached");
    } catch (const std::exception& e) {
        LOG_ERROR(std::string("[Beta] ") + e.what());
    }

    // ---- AutonomousDrone ----
    try {
        LOG_INFO("[Gamma] Takeoff initiated");
        ad.take_off(4);
        LOG_SUCCESS("[Gamma] Takeoff successful");
    } catch (const std::exception& e) {
        LOG_ERROR(std::string("[Gamma] ") + e.what());
    }

    try {
        LOG_WARN("[Gamma] Simulating obstacle (LOW)");
        ad.detect_obstacle({5,5,5}, "LOW");
    } catch (const std::exception& e) {
        LOG_ERROR(std::string("[Gamma] ") + e.what());
    }

    // -------------------------------
    // FULL AUTONOMOUS MISSION
    // -------------------------------
    LOG_INFO("Starting full autonomous mission...");

    while (!ad.mission_complete()) {
        try {
            auto wp = ad.next_waypoint();

            LOG_INFO("[Gamma] Visiting waypoint (" +
                std::to_string(std::get<0>(wp)) + "," +
                std::to_string(std::get<1>(wp)) + "," +
                std::to_string(std::get<2>(wp)) + ")");

            // ✅ Battery monitoring
            LOG_INFO("[Gamma] Remaining battery: " + std::to_string(ad.get_battery()));

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        catch (const BatteryDepletedError& e) {
            LOG_ERROR(std::string("[Gamma] BATTERY FAILURE: ") + e.what());
            LOG_WARN("[Gamma] Mission aborted due to low battery");
            break;
        }

        catch (const std::exception& e) {
            LOG_ERROR(std::string("[Gamma] ERROR: ") + e.what());
            break;
        }
    }

    // -------------------------------
    // POST-MISSION
    // -------------------------------
    if (ad.mission_complete()) {
        try {
            LOG_WARN("[Gamma] Simulating HIGH severity obstacle");
            ad.detect_obstacle({10,10,10}, "HIGH");
        } 
        catch (const std::exception& e) {
            LOG_ERROR(std::string("[Gamma] ") + e.what());
        }
    }

    // -------------------------------
    // FINAL OUTPUT
    // -------------------------------
    LOG_INFO("Mission completed. Printing summary...");

    std::cout << "\n--- MISSION SUMMARY ---\n";
    std::cout << ad.mission_summary() << std::endl;

    std::cout << "\n--- FLIGHT LOG ---\n";
    std::cout << ad.get_flight_log() << std::endl;

    LOG_INFO("Shutting down system...");

    return 0;
}