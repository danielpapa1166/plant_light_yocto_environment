#include <chrono>
#include "plant_light_periodic_data_collector.hpp"

PeriodicDataCollector::PeriodicDataCollector() 
        : _running(false) {
} 

PeriodicDataCollector::~PeriodicDataCollector() {
    stop(); 
}

void PeriodicDataCollector::start() {
    _running = true;
    _worker = std::thread([this]() {
        while (_running) {
            auto start = std::chrono::steady_clock::now();

            task();  // work executed every ~100 ms

            std::this_thread::sleep_until(start + std::chrono::milliseconds(1000));
        }
    });
}

void PeriodicDataCollector::stop() {
    _running = false;
    if (_worker.joinable()) {
        _worker.join();
    }
}


void PeriodicDataCollector::get_aht10_sensor_data(
    float * const f_temp, float * const f_rel_hum) {

    std::lock_guard<std::mutex> lock(_mutex);
    * f_temp = _aht10_sensor._temperature_f; 
    * f_rel_hum = _aht10_sensor._rel_hum_f; 
}

