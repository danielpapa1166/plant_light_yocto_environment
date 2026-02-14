#pragma once 

struct PwmControl {
    PwmControl(void); 
    void set_duty(float f_duty_val);
    void set_led_on(void); 
    void set_led_off(void); 
}; 