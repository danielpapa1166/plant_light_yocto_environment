#include "plant_light_pwm_control.hpp"
#include "plant_light_periodic_data_collector.hpp"
#include <cstdint>
#include <vector>

void execute_udp_command(
        const std::vector<uint8_t>& f_rx_command, 
        std::vector<uint8_t>& f_tx_buffer, 
        PwmControl & f_pwm_control, 
        PeriodicDataCollector & f_pdc);