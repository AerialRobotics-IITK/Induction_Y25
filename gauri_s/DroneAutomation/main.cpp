#include <iostream>
#include <vector>
#include <memory>

#include "AutonomousDrone.h"

int main(){
        auto drone = std::make_unique<Drone>("IROC Drone", 80.0f, 150.0f, 15.0f);

    auto mdrone = std::make_unique<MissionDrone>(
        "Alpha", 90.0f, "Test IROC Drone",
        std::vector<std::tuple<float,float,float>>{
            {10.f, 20.f, 30.f}, {40.f, 50.f, 60.f}, {70.f, 80.f, 90.f}
        }
    );

    auto adrone = std::make_unique<AutonomousDrone>(
        "Beta", 100.0f, "Deep Recon",
        std::vector<std::tuple<float,float,float>>{
            {5.f, 10.f, 15.f}, {20.f, 25.f, 30.f},
            {35.f, 40.f, 45.f}, {50.f, 55.f, 60.f}
        },
        std::tuple<float,float,float>{0.f, 0.f, 0.f}, 200.0f);

          std::vector<Vehicle*> fleet = { drone.get(), mdrone.get(), adrone.get() };

    std::cout << "Polymorphism\n";
    for (Vehicle* v : fleet) {
        // The compiler doesn't hard-code which get_info() to call; it looks it up at runtime.
        std::cout << v->get_info() << "\n";
    }

    //SHOW THAT PRIVATE MEMBERS CANNOT BE ACCESSED DIRECTLY
    std::cout << "Private Members \n";
    std::cout << "Battery (via getter): " << drone->get_battery() << "%\n";
    std::cout << "(battery_level is private, so inaccessible outside Vehicle)\n\n";
    std::cout << "Exception Handling-\n";

    try {
        Drone dead("DeadBird", 0.0f);
        dead.drain_battery(10.0f);         // throws BatteryDepletedError
    } catch (const BatteryDepletedError& e) {
        std::cout << "[BatteryDepletedError] " << e.what() << "\n";
    }

    try {
        Drone low("Lowlow", 90.0f, 50.0f);
        low.take_off(200.0f);              // throws AltitudeError (200 > 50)
    } catch (const AltitudeError& e) {
        std::cout << "[AltitudeError] " << e.what() << "\n";
    }

    try {
        Drone grounded("GrClose", 50.0f); // status = "idle" by default
        grounded.charge_battery(20.0f, 60);
    } catch (const InvalidStateError& e) {
        std::cout << "[InvalidStateError] " << e.what() << "\n";
    }

    try {
        Drone t("Tester", 0.0f);
        t.drain_battery(5.0f);             // throws BatteryDepletedError
    } catch (const DroneException& e) {
        // DroneException is the base of all three specific error, so it catches any drone-related error
        std::cout << "[DroneException base catch] " << e.what() << "\n";
    }

    // FULL AUTONOMOUS MISSION
    //    Take off, iterate waypoints, simulate high obstacle, summary
    std::cout << "\nFull Autonomous Mission\n";

    try {
        adrone->take_off(50.0f);
        std::cout << "Mysterio in air at 50m\n";

        adrone->set_ai_mode("auto");

        // Visit every waypoint until mission_complete() returns true
        while (!adrone->mission_complete()) {
            auto [x, y, z] = adrone->next_waypoint();
            std::cout << "  Waypoint (" << x << ", " << y << ", " << z << ")\n";
        }
        std::cout << "All waypoints done.\n";

    } catch (const BatteryDepletedError& e) {
        std::cout << "[Mission abort ? battery] " << e.what() << "\n";
    } catch (const DroneException& e) {
        std::cout << "[Mission abort] " << e.what() << "\n";
    }

    // High-severity then emergency_stop() fires inside detect_obstacle()
    std::cout << "\nSimulating high-severity obstacle...\n";
    adrone->detect_obstacle({25.f, 30.f, 35.f}, "high");
    std::cout << "Status after: " << adrone->get_status() << "\n";

    // Print the mission summary and full flight log
    std::cout << "\n" << adrone->mission_summary();
    std::cout << "\n" << adrone->get_flight_log();

  std::cout << "Fleet Battery Status \n";
    for (Vehicle* v : fleet)
        std::cout << v->get_name() << ": " << v->get_battery() << "%"
                  << (v->is_critical() ? " [CRITICAL]" : "") << "\n";

    // unique_ptrs destroyed here, Virtual destructor in Vehicle ensures all destructors run in the right order
    std::cout << "\n[Done ? all memory freed automatically]\n";
    return 0;
}