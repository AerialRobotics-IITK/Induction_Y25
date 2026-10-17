#pragma once
# include <string>
#include <vector>
#include <sstream>
#include <set>
#include <chrono>
#include <iomanip>
#include "drone_exceptions.h"

static std::string get_timestamp(){
    std::time_t now=std::time(nullptr);
    std::tm t=*std::localtime(&now);
    char buf[20];
    std::strftime(buf, 20, "%d/%m/%Y %H:%M:%S", &t);
    return buf; 
}

//Vehicle-Abstract Base Class
class Vehicle{


    //Subclasses can read name directly and call add_log_entry.
    // But battery_level and status are private

    protected:
        std::string name;
        void add_log_entry(const std::string&entry){
        flight_log.push_back("["+ get_timestamp()+"]"+entry);
        }
    private:
        float battery_level;
        std::string status;
        std::vector<std::string> flight_log;
        
        

    public:
    //constructor-
    Vehicle(const std::string &n, float b) : name(n), battery_level(b), status("idle") 
    {
        add_log_entry("Vehicle '"+name+"'created, battery= "+std::to_string(b)+ "%");

    }
    //destructor-
    virtual ~Vehicle()=default;
    virtual std::string get_info() const=0;

    void drain_battery(float amount){
        if(battery_level<=0.0f)
        throw BatteryDepletedError("Battery depleted on '"+name+"'");
        battery_level=std::max(0.0f, battery_level-amount);
        add_log_entry("Drained "+std::to_string(amount)+ "% Remaining= "+ std::to_string(battery_level)+"%");

    }
    //charge battery
    void charge_battery(float amount, int duration_seconds){
        if(status!="charging")
        throw InvalidStateError("Cannot charge '"+name+"', status is not 'charging'");
        battery_level=std::min(100.0f, battery_level+amount);
        add_log_entry("Charged "+std::to_string(amount)+"% in "+std::to_string(duration_seconds)+" seconds. Level="+std::to_string(battery_level)+"%");

    }
    //is_critical returns true when battery below 20%
    bool is_critical() const{
        return battery_level<20.0f;
    } 
    //to get flight log as string-
    std::string get_flight_log() const{
        std::ostringstream oss;
        oss<<"Flight Log: "<<name<<"\n";
        for(size_t i=0;i<flight_log.size(); ++i)
            oss<<"["<<(i+1)<<"]"<<flight_log[i]<<"\n";
        return oss.str();
    }

    void set_status(const std::string &new_status){
        static const std::set<std::string> allowed={
            "idle", "flying", "charging","emergency", "mission" , "landed"
        };
    if (allowed.count(new_status)==0)
    throw InvalidStateError("Invalid status '"+new_status+"'");
    std::string old=status;
    status=new_status;
    add_log_entry("Status: "+old+" to " + new_status);
    }
   
    //public access, read-only to private members

    const std::string &get_name() const {return name;}
    float get_battery() const {return battery_level;}
    const std::string&get_status() const {return status;}


};