SUMMARY = "BCM2711 PWM Device Tree Overlay"
LICENSE = "CLOSED"

DEPENDS = "dtc-native"

SRC_URI = "file://bcm2711-pwm-overlay.dts"

S = "${WORKDIR}"

do_compile() {
    dtc -@ -I dts -O dtb -o bcm2711-pwm-overlay.dtbo \
        bcm2711-pwm-overlay.dts
}

do_install() {
    mkdir -p ${D}/boot/overlays
    install -m 0644 bcm2711-pwm-overlay.dtbo \
        ${D}/boot/overlays/
}

FILES:${PN} += "/boot/overlays/bcm2711-pwm-overlay.dtbo"
