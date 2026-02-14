#include <cstdio>
#include <stdlib.h>
#include <ctime>
#include <vector>

#include "plant_light_udp.hpp"
#include "plant_light_command_handler.hpp"
#include "plant_light_scheduler.hpp"
#include "plant_light_pwm_control.hpp"
#include "plant_light_periodic_data_collector.hpp"

static UdpHandler udp_handler = UdpHandler();
static PwmControl pwm_control = PwmControl(); 

static std::vector<uint8_t> m_udp_tx_buffer; 

static PeriodicDataCollector pdc = PeriodicDataCollector(); 

int main() {

    pdc.start(); 

    printf("Application Starts. \n"); 
    while (1) {
        if(udp_handler.wait_for_message() > 0) {
            execute_udp_command(
                udp_handler.get_rx_data(), 
                m_udp_tx_buffer, 
                pwm_control, 
                pdc); 
            
            // send response: 
            udp_handler.send_data(m_udp_tx_buffer); 
        }

        check_light_switching_schedule(
            pwm_control);
    }
}


