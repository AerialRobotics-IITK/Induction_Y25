#pragma once
#include "MissionDrone.h"
#include <cmath>
#include <set>
//ai mode for switching, obstacle, automatic path replanning

class AutonomousDrone:public MissionDrone{

private:
    std::string ai_mode;
    std::tuple<float,float,float> home_position;
    std::vector<std::string> obstacle_log;  // only AutonomousDrone sees raw entries

    // static member function doesn't need called as dist3d(a, b)
    static float dist3d(std::tuple<float,float,float> a, std::tuple<float,float,float> b) {
        float dx = std::get<0>(a) - std::get<0>(b);
        float dy = std::get<1>(a) - std::get<1>(b);
        float dz = std::get<2>(a) - std::get<2>(b);
        return std::sqrt(dx*dx + dy*dy + dz*dz);
    }


    public:
    AutonomousDrone(const std::string&name, float battery, const std::string & mission,
    const std::vector<std::tuple<float,float, float>>&wps, std::tuple<float, float, float> home,
    float max_alt=100.0f):
    MissionDrone(name, battery, mission, wps, max_alt), ai_mode("manual"), home_position(home)
    {
        add_log_entry("Autonomous drone ready. Home=("+std::to_string(std::get<0>(home))+","+std::to_string(std::get<1>(home))+","
        +std::to_string(std::get<2>(home))+")");

    }
    virtual ~AutonomousDrone()=default;

    void set_ai_mode(const std::string &mode){
        static const std::set<std::string> valid={"manual", "auto", "returm_home" };
        if(!valid.count(mode))
        throw InvalidStateError("Unknown AI mode: '"+mode+"'");
        ai_mode=mode;
        add_log_entry("AI mode to"+mode);

        if(mode=="return_home"){
            waypoints.insert(waypoints.begin()+current_waypoint_index, home_position);
            add_log_entry("Home position inserted as next waypoint");

        }
    }

    void detect_obstacle(std::tuple<float, float, float>pos, const std::string &severity){
        std::ostringstream entry;
        entry<<"Obstacle [" <<severity<<"] at ("<<std::get<0>(pos)<<","<<std::get<1>(pos)<<","<<std::get<2>(pos)
        <<") at "<<get_timestamp();

        obstacle_log.push_back(entry.str()); //privaate log
        add_log_entry(entry.str()); //main log

        if (severity=="high"){
            add_log_entry("HIGH Severity so emergency stop");
            emergency_stop();
        }
    }

    std::vector<std::tuple<float, float, float>> auto_replan (const std::vector <std::tuple<float, float, float>>& obstacles)
    {
        std::vector<std::tuple<float, float, float>> safe_plan;
        for(size_t i=static_cast<size_t>(current_waypoint_index); i<waypoints.size(); ++i){
            bool blocked=false;
            for(const auto &obs:obstacles){
                if (dist3d(waypoints[i], obs)<5.0f){
                    blocked=true;
                    add_log_entry("Waypoint excluded, too close to obstacle");
                    break;
                }
            }
            if(!blocked) safe_plan.push_back(waypoints[i]);

        }
        add_log_entry("Replanned: "+std::to_string(safe_plan.size())+ " safe waypoints");
        return safe_plan;
    }

        std::string get_info() const override{
        std::ostringstream oss;
        oss<<"AI Mode: "<<get_info()<<"\n" 
        << "  Home    : (" << std::get<0>(home_position) << ","<< std::get<1>(home_position) << ","
        << std::get<2>(home_position) << ")\n"<< "  Obstacles: " << obstacle_log.size() << " logged\n";
        return oss.str();
    }


};