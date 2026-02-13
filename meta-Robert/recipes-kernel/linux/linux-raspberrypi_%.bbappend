
FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://brcmfmac.cfg"
SRC_URI += "file://i2c-config.cfg"
SRC_URI += "file://bcm2711-pwm-overlay.dts"