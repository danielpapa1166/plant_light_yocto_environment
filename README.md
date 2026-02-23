# Raspberry Pi 4 Yocto project to build linux image. 

Custom Yocto-based Linux distribution for Raspberry Pi 4, designed to control a plant lighting system with a C++ backend and Python web frontend.

## Project structure: 

|Name                         | Description                     |
|-----------------------------|---------------------------------|
| build-rpi/                  | Yocto build directory           |
| meta-Robert/                | Custom Yocto layer              |
| meta-Robert-firmware/       | Firmware & application layer    |
| export_image.sh             | Image export script             |
| README.md                   |                                 |

## *build-rpi* Layer

Contains the Yocto build configuration:

* conf/bblayers.conf
* conf/local.conf


## *meta-Robert* Layer 

Custom Yocto layer contains low level kernel config and web service and backend applications. 

* conf
    * layer.conf        
* recipes-connectivity
    * wpa-supplicant — WLAN0 WiFi configuration
* recipes-core
    * console-config — HUN keyboard layout, font set
    * images — Extension of core-image-full-cmdline
    * plant_light_backend — C++ backend for low-level interface control
    * plant_light_frontend — Bottle Python + HTML frontend
* recipes-kernel
    * bcm2711-pwm — PWM controller
    * gpio_chardev — GPIO character device
    * linux — I2C and Broadcom configuration files

## *meta-Robert-firmware* layer
Recipe to be moved to the default layer 