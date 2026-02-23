SUMMARY = "Control Application to test the PWM control Library natively "
LICENSE = "CLOSED"

SRC_URI = "file://pwm_control_app.c"

S = "${WORKDIR}"

# Declare dependency on pwm-control-lib
DEPENDS = "pwm-control-lib"
RDEPENDS:${PN} = "pwm-control-lib"

# Skip QA check for file-level runtime deps (dependency is properly declared)
INSANE_SKIP:${PN} = "file-rdeps"

do_compile() {
    echo "STAGING_LIBDIR: ${STAGING_LIBDIR}"
    echo "STAGING_INCDIR: ${STAGING_INCDIR}"
    echo "Contents of STAGING_LIBDIR:"
    ls -la ${STAGING_LIBDIR} || echo "Directory does not exist"
    echo "Contents of STAGING_INCDIR:"
    ls -la ${STAGING_INCDIR} || echo "Directory does not exist"
    
    ${CC} ${CFLAGS} -I${STAGING_INCDIR} -L${STAGING_LIBDIR} \
        -o pwm-control-native-app pwm_control_app.c \
        -lpwm_control ${LDFLAGS}
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 pwm-control-native-app ${D}${bindir}/
}

FILES:${PN} = "${bindir}/*"