#include "Vehicle.h"
class Drone:public Vehicle{

    protected:
        float max_altitude, altitude; 
        //other derived classes can read these
    private:
        float speed; //only Drone class ke own method can manage speed

    public:
    Drone(const std::string &name, float battery, float max_alt=100.0f, float spd=10.0f):
        Vehicle(name, battery), max_altitude(max_alt), altitude(0.0f), speed(spd)
        {
        add_log_entry("Drone ready, max altitude is "+std::to_string(max_alt)+ "metres, speed= "+ std::to_string(spd)+ "m/s");

        }

    virtual ~Drone()=default;

    //take_off
    void take_off(float target_altitude){
        if (target_altitude>max_altitude)
        throw AltitudeError("Target "+std::to_string(target_altitude)+" metres > Maximum "+std::to_string(max_altitude)+ "metres for '"+name+"'");

        set_status("flying");
        altitude=target_altitude;
        add_log_entry("Took off to " +std::to_string(altitude)+ "metres");
    }

    void land(){
        altitude=0.0f; set_status("landed"); add_log_entry("Landed");
    }
    
    void emergency_stop(){
        set_status ("emergency");
        add_log_entry("Emergency STOP");
        try{
            drain_battery(30.0f);

        }catch (const BatteryDepletedError &){
            add_log_entry("Battery already 0 during emergency stop");
        }
    }
    std::string get_info() const override{
        std::ostringstream oss;
        oss<<"Drone: "<<get_name()<<"\n" 
        << "Battery: "<<get_battery()<<"%" <<(is_critical()? "[Critical]": "")<<"\n"
        <<"Statur: "<<get_status()<<"\n"<<"Altitude: "<<altitude <<"metres \n"
        <<"Max Altitude: "<<max_altitude<<"m \n" <<"Speed: "<<speed<<"m/s\n";
        return oss.str();

    }
};