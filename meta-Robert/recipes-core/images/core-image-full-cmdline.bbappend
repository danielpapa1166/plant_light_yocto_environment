IMAGE_INSTALL:append = "    \
    openssh                 \
    openssh-sftp-server     \
    gpiochardev             \
    wpa-supplicant          \
    linux-firmware          \
    iw                      \
    kbd-keymaps             \
    console-config          \
    kernel-modules          \
    firmware-brcm           \
    plant-light-backend     \
    bcm2711-pwm             \
    bcm2711-pwm-overlay     \
    pwm-control-lib         \
    pwm-control-native-app  \
"

SYSTEMD_AUTO_ENABLE:pn-openssh = "enable"
SYSTEMD_AUTO_ENABLE:pn-systemd-networkd = "enable"

