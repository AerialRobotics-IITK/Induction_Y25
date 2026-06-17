#include "drone_fleet/Utils.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

std::string get_current_timestamp()
{
    auto now = std::chrono::system_clock::now();

    std::time_t now_time =
        std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;

    ss << std::put_time(
        std::localtime(&now_time),
        "%Y-%m-%d %H:%M:%S"
    );

    return ss.str();
}

//isme doubt hai thoda