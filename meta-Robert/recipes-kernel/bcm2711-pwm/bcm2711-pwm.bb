SUMMARY = "BCM2711 PWM Driver"
# note: reinventing the wheel for learning purposes 
LICENSE = "CLOSED"

inherit module

SRC_URI = "file://bcm2711_pwm.c \
           file://Makefile"

S = "${WORKDIR}"
