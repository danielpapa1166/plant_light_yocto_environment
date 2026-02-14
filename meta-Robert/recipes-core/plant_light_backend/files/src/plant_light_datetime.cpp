#include "plant_light_datetime.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>

static std::tm convert_to_tz(time_t t, const char* tz_name);


std::tm get_act_local_time_tm(void) {
    // get current time: 
    std::time_t now = std::time(nullptr); 

    // convert to local time: 
    std::tm local_time = convert_to_tz(now, "Europe/Budapest"); 

    return local_time; 
}

std::string get_act_local_time_str(void) {
    std::tm local_time = get_act_local_time_tm(); 

    char buffer[64]; 
    memset(buffer, 0, sizeof(buffer)); 
    std::strftime(
        buffer, 
        sizeof(buffer), 
        "%Y-%m-%d %H:%M:%S", 
        &local_time); 

    std::string s(buffer); 

    return s; 
}


static std::tm convert_to_tz(time_t t, const char* tz_name) {
    // Save old timezone
    char* old_tz = getenv("TZ");

    // Set new timezone
    setenv("TZ", tz_name, 1);
    tzset();

    std::tm result = *std::localtime(&t);

    // Restore old timezone
    if (old_tz)
        setenv("TZ", old_tz, 1);
    else
        unsetenv("TZ");
    tzset();

    return result;
}