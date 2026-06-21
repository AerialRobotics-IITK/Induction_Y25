#pragma once
#include "Drone.h"
#include <vector>
#include <tuple> // 3d coordinates
#include <utility> 
#include <stdexcept>

class MissionDrone: public Drone{
protected:
std::string mission_name;
std::vector<std::tuple<float,float,float>> waypoints;
int current_waypoint_index;

private:
std::vector<std::pair<std::tuple<float, float, float>, std::string>> visited_waypoints;

    public:
        MissionDrone(const std::string &name, float battery, const std::string &mission,
        const std::vector<std::tuple<float, float, float>> &wps,
        float max_alt=100.0f):
        Drone(name, battery, max_alt), mission_name(mission), waypoints(wps), 
        current_waypoint_index(0)
        {
            add_log_entry("Mission '"+mission + "' loaded with "+ std::to_string(wps.size())+" waypoints");

        }
    virtual ~MissionDrone()=default;

    std::tuple<float, float, float> next_waypoint(){
        if(mission_complete())
        throw std::out_of_range("no more waypoints, mission completed");
        auto wp=waypoints[static_cast<size_t> (current_waypoint_index)];
        drain_battery(1.5f);

        std::ostringstream record;
        record <<"Visited ("<<std::get<0> (wp)<<", "<<std::get<1> (wp)<<", "<<std::get<2> (wp)<<")" 
        <<"at "<< get_timestamp();
        //std::get<0> means x coordinate

        ++current_waypoint_index;
        add_log_entry(record.str());
        return wp;
    }

    void skip_waypoint(const std::string &reason){
        if (mission_complete()) return;
        auto wp=waypoints[static_cast<size_t> (current_waypoint_index)];
        std::ostringstream record;
        record<<"Skipped ("<<std::get<0>( wp)<<", "<<std::get<1>( wp)<<", "<<std::get<1>( wp)
        <<") because of "<<reason<<" at "<<get_timestamp();

        visited_waypoints.push_back(std::make_pair(wp, record.str()));
        ++current_waypoint_index;
        add_log_entry(record.str());

    }

    bool mission_complete() const{
        return static_cast<size_t> (current_waypoint_index)>=waypoints.size();
    }

    std::string mission_summary() const{
        std::ostringstream oss;
        oss<<"Mission Summary: "<<mission_name<<": \n"
        <<"Status: "<<(mission_complete()?"Complete":"In progress")<<" \n"
        <<"Progress: "  <<current_waypoint_index<<"/"<<waypoints.size()<<"\n"<<
        "Battery : "<<get_battery()<< "% \n Waypoint log: \n";

        for(const auto &entry : visited_waypoints) oss<<" "<<entry.second<<"\n";
        return oss.str();

    }

    std::string get_info () const override{
        std::ostringstream oss;
        oss<<Drone::get_info()<<" Mission: "<<mission_name<<"\n" <<" Progress: "<< current_waypoint_index<< "/" 
        <<waypoints.size()<<"\n" <<" Complete: "<<(mission_complete()? "Yes": "No")<<"\n";
        return oss.str();
    }

    
};
