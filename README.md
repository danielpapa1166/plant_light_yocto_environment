README WORK IN PROGRESS

Raspberry Pi 4 Yocto project to build linux image. 
Creates web server to control plant light and collect and display sensor measurement data. 

meta-Robert Layer Structure
    - conf
        - layer.conf        
    - recipes-connectivity
        - wpa-supplicant — WLAN0 WiFi configuration
    - recipes-core
        - console-config — HUN keyboard layout, font set
        - images — Extension of core-image-full-cmdline
        - plant_light_backend — C++ backend for low-level interface control
        - plant_light_frontend — Bottle Python + HTML frontend
    - recipes-kernel
        - bcm2711-pwm — PWM controller
        - gpio_chardev — GPIO character device
        - linux — I2C and Broadcom configuration files

to be continued ... 
