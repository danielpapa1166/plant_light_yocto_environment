#pragma once 


struct GpioControl {
    GpioControl(); 
    ~GpioControl();
    void led_on(); 
    void led_off(); 

private: 
    int _fd = 0; 
}; 