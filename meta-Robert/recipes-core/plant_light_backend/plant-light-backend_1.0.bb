SUMMARY = "Plant light backend C++ application"
LICENSE = "CLOSED"

SRC_URI = "file://src/"

S = "${WORKDIR}/src"

do_compile() {
    oe_runmake
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 plant_light_backend ${D}${bindir}/plant_light_backend
}
