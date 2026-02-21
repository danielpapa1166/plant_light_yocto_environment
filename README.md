Raspberry Pi 4 Yocto project to build linux image. 
Creates web server to control plant light and collect and display sensor measurement data. 

Bitbake layers: 
layer                 path                                      priority
==========================================================================
meta                  /data/projects/yocto/build-rpi/../poky/meta  5
meta-poky             /data/projects/yocto/build-rpi/../poky/meta-poky  5
meta-yocto-bsp        /data/projects/yocto/build-rpi/../poky/meta-yocto-bsp  5
meta-raspberrypi      /data/projects/yocto/build-rpi/../meta-raspberrypi  9
meta-Robert           /data/projects/yocto/build-rpi/../meta-Robert  6
meta-Robert-firmware  /data/projects/yocto/build-rpi/../meta-Robert-firmware  6

meta-Robert: 
├── conf
│   └── layer.conf
├── recipes-connectivity
│   └── wpa-supplicant: WLAN0 wifi config 
├── recipes-core
│   ├── console-config: HUN keyboard, font set 
│   ├── images: extension of core-image-full-cmdline 
│   ├── plant_light_backend: C++ backend application to control low level interfaces 
│   └── plant_light_frontend: Bottle Python and HTML frontend
└── recipes-kernel
    ├── bcm2711-pwm: PWM controller 
    ├── gpio_chardev: GPIO character device 
    └── linux: I2C and Broadcom config files 

to be continued ... 
