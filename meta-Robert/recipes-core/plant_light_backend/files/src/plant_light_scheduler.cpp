#include "plant_light_scheduler.hpp"
#include "plant_light_datetime.hpp"
#include "plant_light_limited_queue.hpp"
#include "plant_light_pwm_control.hpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <vector>

#define PLANT_LIGHT_SWITCHING_SCHEDULE_MAX_NUM      10

typedef struct light_switching {
    std::tm scheduled_time;     // date and time
    bool switch_type;           // on or off
} light_switching_st; 

static LimitedQueue<light_switching_st> 
        m_user_time_schedule_queue(
            PLANT_LIGHT_SWITCHING_SCHEDULE_MAX_NUM); 


static void display_light_switch_sample(
    const light_switching_st & f_light_sw_sample);


uint8_t add_element_to_light_switch_queue(
    const std::vector<uint8_t>& f_rx_command) {
    if(f_rx_command.size() < 11) {
        // 2 bytes header 
        // 1 byte command id
        // 7 bytes of date 
        // 1 byte on or off
        return 1; 
    }

    enum {
        header_byte_0_idx, 
        header_byte_1_idx, 
        command_type_idx, 
        payload_byte_year_0_idx, 
        payload_byte_year_1_idx, 
        payload_byte_month_idx, 
        payload_byte_day_idx, 
        payload_byte_hour_idx, 
        payload_byte_min_idx, 
        payload_byte_sec_idx, 
        payload_byte_on_or_off, 
    }; 

    light_switching_st l_act_sw_sample; 

    uint16_t l_year_u16 = (f_rx_command[payload_byte_year_0_idx] << 8)
                        | f_rx_command[payload_byte_year_1_idx]; 
    
    l_act_sw_sample.scheduled_time.tm_year = l_year_u16 - 1900; 
    l_act_sw_sample.scheduled_time.tm_mon = f_rx_command[payload_byte_month_idx] - 1; 
    l_act_sw_sample.scheduled_time.tm_mday = f_rx_command[payload_byte_day_idx];
    l_act_sw_sample.scheduled_time.tm_hour = f_rx_command[payload_byte_hour_idx]; 
    l_act_sw_sample.scheduled_time.tm_min = f_rx_command[payload_byte_min_idx]; 
    l_act_sw_sample.scheduled_time.tm_sec = f_rx_command[payload_byte_sec_idx]; 

    l_act_sw_sample.switch_type = (bool)f_rx_command[payload_byte_on_or_off]; 


    display_light_switch_sample(l_act_sw_sample);
    
    m_user_time_schedule_queue.push(l_act_sw_sample); 

    return 0; 
}


void check_light_switching_schedule(PwmControl & f_pwm_control) {
    std::tm l_act_time = get_act_local_time_tm(); 

    // temp limited queue with same size limit
    LimitedQueue<light_switching_st> tmp(
        m_user_time_schedule_queue.maxSize());   

    while (!m_user_time_schedule_queue.empty()) {
        light_switching_st switching_sample 
                = m_user_time_schedule_queue.front();
        m_user_time_schedule_queue.pop();

        // schedule time already elapsed: 
        bool time_elapsed 
                = (std::mktime(&switching_sample.scheduled_time) 
                < std::mktime(&l_act_time));

        if (!time_elapsed) {
            // keep only entries >= tm_limit
            tmp.push(switching_sample);
        }
        else {
            // execute the reuired switch function: 
            // sample will be removed
            printf("Sample will be removed, execute: "); 
            display_light_switch_sample(switching_sample); 

            if(switching_sample.switch_type) {
                // turn light on: 
                f_pwm_control.set_led_on(); 
            }
            else {
                // turn led off: 
                f_pwm_control.set_led_off(); 
            }
        }
    }

    m_user_time_schedule_queue = tmp;  // copy filtered results back
}



static void display_light_switch_sample(const light_switching_st & f_light_sw_sample) {
    char buffer[64]; 
    memset(buffer, 0, sizeof(buffer)); 
    std::strftime(
        buffer, 
        sizeof(buffer), 
        "%Y-%m-%d %H:%M:%S", 
        &f_light_sw_sample.scheduled_time); 

    if(f_light_sw_sample.switch_type) {
        printf("Switching light ON at:  "); 
    }
    else {
        printf("Switching light OFF at: "); 
    }
    for (uint8_t i = 0; i < 64; i ++) {
        printf("%c", buffer[i]); 
    }

    printf("\n"); 
}