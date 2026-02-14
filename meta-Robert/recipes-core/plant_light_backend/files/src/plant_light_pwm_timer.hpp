#include "plant_light_gpio_control.hpp"
#include <cstdint>
#include <cstdio>
#include <sys/select.h>
#include <sys/time.h>
#include <csignal>


struct PwmTimer {

    PwmTimer(GpioControl & f_gpio_ctrl, void (*intr_hdl_func)(int)) 
            :_gpio_ctrl(f_gpio_ctrl) {

        _gpio_ctrl.led_on(); 

        // init timer: 
        signal(SIGALRM, intr_hdl_func); 

        itimerval timer{}; 
        timer.it_value.tv_sec = 0; 
        timer.it_value.tv_usec = _timer_interval_nsec; 
        timer.it_interval.tv_sec = 0; 
        timer.it_interval.tv_usec = _timer_interval_nsec; 

        setitimer(ITIMER_REAL, &timer, nullptr); 
    } 


    ~ PwmTimer() {

    }

    void tmr_intr_handler(void) { 
        
        // keep the led turned off below a given limit: 
        if(_act_pwm_duty_u32 <= _led_full_off_max_duty) {
            _gpio_ctrl.led_off(); 
            return;
        }

        // keep the led turned on above a given limit: 
        if(_act_pwm_duty_u32 >= _led_full_on_min_duty) {
            _gpio_ctrl.led_on(); 
            return;
        }

        // compare actual counter value to the pwm duty:
        if(intr_cnt <= _act_pwm_duty_u32) {
            _gpio_ctrl.led_on(); 
        }
        else {
            _gpio_ctrl.led_off(); 
        }


        // increment interrupt counter and check overflow: 
        intr_cnt ++; 
        if (intr_cnt >= _timer_max_intr_count) {
            intr_cnt = 0; 
        }
    }

    void pwm_timer_set_duty(float f_duty_val) {

        // limit input: 
        if(f_duty_val > PWM_DUTY_MAX) {
            f_duty_val = PWM_DUTY_MAX; 
        }
        if (f_duty_val < PWM_DUTY_MIN) {
            f_duty_val = PWM_DUTY_MIN; 
        }

        // calculate off time: 
        _act_pwm_duty_u32 = (uint32_t)(((float)_timer_max_intr_count) \
                * f_duty_val / 100.0f); 

    }

    constexpr static const float PWM_DUTY_MIN = 0.0f; 
    constexpr static const float PWM_DUTY_MAX = 100.0f; 

private: 
    uint32_t intr_cnt; 
    uint32_t _act_pwm_duty_u32; 
    GpioControl _gpio_ctrl; 

    const int _timer_interval_nsec = 100; 
    const uint32_t _timer_max_intr_count = 100; 


    // above this limit the led will be turned on (90%): 
    const uint32_t _led_full_on_min_duty = _timer_max_intr_count / 10 * 9;
    
    // below this limit the led will be turned off (10%): 
    const uint32_t _led_full_off_max_duty = _timer_max_intr_count / 10;

}; 