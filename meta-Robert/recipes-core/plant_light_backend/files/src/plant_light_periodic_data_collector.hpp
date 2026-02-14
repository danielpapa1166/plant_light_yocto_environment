#pragma once

#include "plant_light_AHT10_sensor.hpp"
#include "plant_light_datetime.hpp"
#include "plant_light_file_handler.hpp"
#include <mutex>
#include <thread>
#include <atomic>

 
struct PeriodicDataCollector {

    PeriodicDataCollector(); 
    ~PeriodicDataCollector(); 
    void start(); 
    void stop(); 

    void get_aht10_sensor_data(float * const f_temp, float * const f_rel_hum);


private:
    void task()
    {
        _aht10_sensor.new_measurement(); 
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _aht10_sensor.fetch_data(); 
        }

        float l_temp, l_rel_hum; 
        this->get_aht10_sensor_data(&l_temp, &l_rel_hum); 


        // increment relative humidity lump sum: 
        _temperature_lump_sum += l_temp; 
        _rel_hum_lump_sum += l_rel_hum; 

        // increment cycle counter: 
        _cycle_counter ++; 

        if(_cycle_counter >= _max_num_of_cycles) {

            float l_temp_avg = _temperature_lump_sum / ((float)_cycle_counter); 
            float l_rel_hum_avg = _rel_hum_lump_sum / ((float)_cycle_counter); 

            // reset counter: 
            _cycle_counter = 0; 

            // reset lump sums: 
            _temperature_lump_sum = 0; 
            _rel_hum_lump_sum = 0; 

            std::string local_datetime = get_act_local_time_str(); 

            // save data to file: 
            plant_light_write_log(
                local_datetime, 
                l_temp_avg, 
                l_rel_hum_avg); 

        }
    }

    std::atomic<bool> _running;
    std::thread _worker;
    mutable std::mutex _mutex;
    AHT10_Sensor _aht10_sensor = AHT10_Sensor(); 

    const uint32_t _max_num_of_cycles = 60; 
    uint32_t _cycle_counter = 0; 
    float _temperature_lump_sum = 0.0f; 
    float _rel_hum_lump_sum = 0.0f; 
};
