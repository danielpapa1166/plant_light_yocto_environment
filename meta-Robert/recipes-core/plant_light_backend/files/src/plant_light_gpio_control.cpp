#include "plant_light_gpio_control.hpp"
#include <cstdio>
#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <string.h>
#include <unistd.h>

static const char * _led_on_msg = "21,1"; 
static const char * _led_off_msg = "21,0"; 

static std::filesystem::path GPIO_CHARDEV_PATH = "/proc"; 
static std::filesystem::path GPIO_CHARDEV_NAME = "gpio_chardev"; 

    
GpioControl::GpioControl() {
    _fd = open((GPIO_CHARDEV_PATH / GPIO_CHARDEV_NAME).c_str(), O_RDWR);
    if (_fd < 0) {
        std::cerr << "Failed to open device: " << strerror(errno) << std::endl;
        return;
    }
}

GpioControl::~GpioControl() {
    close(_fd); 
}

void GpioControl::led_on() {
    ssize_t n = write(_fd, _led_on_msg, strlen(_led_on_msg));
    if (n < 0) {
        std::cerr << "Write failed: " << strerror(errno) << std::endl;
    }
}

void GpioControl::led_off() {
    ssize_t n = write(_fd, _led_off_msg, strlen(_led_off_msg));
    if (n < 0) {
        std::cerr << "Write failed: " << strerror(errno) << std::endl;
    }
}