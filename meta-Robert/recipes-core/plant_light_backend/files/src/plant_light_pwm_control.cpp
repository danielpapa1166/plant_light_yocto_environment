#include "plant_light_pwm_timer.hpp"
#include "plant_light_pwm_control.hpp"
#include "plant_light_gpio_control.hpp"
#include <cstdio>
#include <sys/select.h>
#include <csignal>


static PwmTimer * m_pwm_timer_insatnce_ptr = nullptr; 

static void timer_interrupt_handler(int signum) {
    if(signum != SIGALRM) {
        return; 
    }

    
    if(m_pwm_timer_insatnce_ptr) {
        m_pwm_timer_insatnce_ptr->tmr_intr_handler(); 
    }

}

PwmControl::PwmControl(void) {
    GpioControl * gpio_control = new GpioControl(); 
    m_pwm_timer_insatnce_ptr = new PwmTimer(
        * gpio_control, 
        timer_interrupt_handler);
}

void PwmControl::set_duty(float f_duty_val) {
    m_pwm_timer_insatnce_ptr->pwm_timer_set_duty(f_duty_val); 
}

void PwmControl::set_led_on(void) {
    m_pwm_timer_insatnce_ptr->pwm_timer_set_duty(
        PwmTimer::PWM_DUTY_MAX); 
}

void PwmControl::set_led_off(void) {
    m_pwm_timer_insatnce_ptr->pwm_timer_set_duty(
        PwmTimer::PWM_DUTY_MIN); 
}