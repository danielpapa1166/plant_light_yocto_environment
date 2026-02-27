SUMMARY = "Plant light backend C++ application"
LICENSE = "CLOSED"

inherit systemd 

SRC_URI = "file://src/"

S = "${WORKDIR}/src"

do_compile() {
    oe_runmake
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 plant_light_backend ${D}${bindir}/plant_light_backend
}


SYSTEMD_SERVICE:${PN} = "plant-light-backend.service"

do_install:append() {
    install -d ${D}${systemd_unitdir}/system
    install -m 644 ${THISDIR}/files/plant-light-backend.service ${D}${systemd_unitdir}/system/
}

FILES:${PN}:append = " ${systemd_unitdir}"
