SUMMARY = "Library to control the PWM device "
LICENSE = "CLOSED"

SRC_URI = "file://pwm_control.c file://pwm_control.h"

S = "${WORKDIR}"

do_compile() {
    ${CC} ${CFLAGS} -fPIC -c pwm_control.c -o pwm_control.o
    ${CC} -shared -o libpwm_control.so pwm_control.o ${LDFLAGS}
}

do_install() {
    install -d ${D}${libdir}
    install -m 0755 libpwm_control.so ${D}${libdir}/

    install -d ${D}${includedir}
    install -m 0644 pwm_control.h ${D}${includedir}/
}

# Keep it simple - library in main package, headers in -dev
FILES:${PN} = "${libdir}/libpwm_control.so*"
FILES:${PN}-dev = "${includedir}/*"
FILES:${PN}-dbg = "${libdir}/.debug/*"

# Skip QA checks
INSANE_SKIP:${PN} = "dev-so"
INSANE_SKIP:${PN}-dev = "dev-elf"
INSANE_SKIP:${PN}-dev = "dev-elf"