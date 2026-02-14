#pragma once 

#include "plant_light_ICC_device_def.hpp"
#include <cstdint>

struct AHT10_Sensor {
    AHT10_Sensor(); 
    void new_measurement(); 
    void fetch_data();
    float _rel_hum_f; 
    float _temperature_f; 

    const uint8_t _dev_icc_addr = 0x38; 

    ICC_Bus & _icc_bus_instance = ICC_Bus::instance(); 
}; 