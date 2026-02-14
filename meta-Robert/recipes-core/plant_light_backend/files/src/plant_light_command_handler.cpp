#include "plant_light_command_handler.hpp"
#include "plant_light_pwm_control.hpp"
#include "plant_light_scheduler.hpp"
#include "plant_light_pwm_timer.hpp"


#include <cstdio>
#include <cstring>
#include <array>

std::array<uint8_t, 4> float_to_u8_array_le(float value);
float u8_array_to_float_le(const std::array<uint8_t, 4>& bytes);

void execute_udp_command(
        const std::vector<uint8_t>& f_rx_command, 
        std::vector<uint8_t>& f_tx_buffer, 
        PwmControl & f_pwm_control, 
        PeriodicDataCollector & f_pdc) {

    // copy RX to TX buffer as a default response: 
    f_tx_buffer = f_rx_command; 

    if(f_rx_command[0] != 0x52 || f_rx_command[1] != 0x50) {
        printf("Protocol error \n"); 
    }

    if(f_rx_command[2] == 0x4C) {
        // basic turn light on and off: 
        if(f_rx_command[3] == 0) {
            f_pwm_control.set_led_off(); 
        }
        else if(f_rx_command[3] == 1) {
            f_pwm_control.set_led_on(); 
        }
    }
    else if(f_rx_command[2] == 0x4D) {
        uint8_t pwm_setval = f_rx_command[3]; 
        
        f_pwm_control.set_duty((float)pwm_setval); 

    }
    else if (f_rx_command[2] == 0x53) {
        // schedule light switch: 
        (void) add_element_to_light_switch_queue(f_rx_command);   
    }
    else if(f_rx_command[2] == 0x61) {
        float l_temperature, l_rel_hum; 
        f_pdc.get_aht10_sensor_data(
            &l_temperature, 
            &l_rel_hum); 

        auto l_temperature_arr = float_to_u8_array_le(l_temperature); 
        auto l_rel_hum_arr = float_to_u8_array_le(l_rel_hum); 

        // assign the measurement values to the end of the buffer: 
        f_tx_buffer.insert(
            f_tx_buffer.end(), 
            l_temperature_arr.begin(), 
            l_temperature_arr.end()); 

        
        f_tx_buffer.insert(
            f_tx_buffer.end(), 
            l_rel_hum_arr.begin(), 
            l_rel_hum_arr.end()); 


    }
    else {
        printf("Command ID error \n"); 
    }
}



std::array<uint8_t, 4> float_to_u8_array_le(float value)
{
    std::array<uint8_t, 4> bytes{};
    std::memcpy(bytes.data(), &value, sizeof(float));
    return bytes;
}


float u8_array_to_float_le(const std::array<uint8_t, 4>& bytes)
{
    float value;
    std::memcpy(&value, bytes.data(), sizeof(float));
    return value;
}
