#pragma once

#include "plant_light_pwm_control.hpp"
#include <cstdint>
#include <vector>

uint8_t add_element_to_light_switch_queue(const std::vector<uint8_t>& f_rx_command);
void check_light_switching_schedule(PwmControl & f_pwm_control); 