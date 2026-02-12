SUMMARY = "Console keyboard layout configuration"
LICENSE = "CLOSED"

SRC_URI = "file://vconsole.conf"

do_install() {
    install -d ${D}${sysconfdir}
    install -m 0644 ${WORKDIR}/vconsole.conf ${D}${sysconfdir}/vconsole.conf
}

FILES:${PN} = "${sysconfdir}/vconsole.conf"
