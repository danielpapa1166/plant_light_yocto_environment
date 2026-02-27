SUMMARY = "Plant light frontend Python application"
LICENSE = "CLOSED"

inherit systemd 

# Runtime dependencies
RDEPENDS:${PN} = "python3"

# Installation directory on target
INSTALL_DIR = "/opt/plant-light-frontend"

# Install all Python files and resources
do_install() {
    install -d ${D}${INSTALL_DIR}
    
    # Install Python files with executable permissions
    install -m 755 ${THISDIR}/files/*.py ${D}${INSTALL_DIR}/
    
    # Install resource directories and files
    install -d ${D}${INSTALL_DIR}/resources
    install -m 644 ${THISDIR}/files/resources/* ${D}${INSTALL_DIR}/resources/
    
    install -d ${D}${INSTALL_DIR}/static
    install -m 644 ${THISDIR}/files/static/* ${D}${INSTALL_DIR}/static/
    
    install -d ${D}${INSTALL_DIR}/views
    install -m 644 ${THISDIR}/files/views/* ${D}${INSTALL_DIR}/views/
}

# Make the package provide files in the installation directory
FILES:${PN} = "${INSTALL_DIR}/*"


SYSTEMD_SERVICE:${PN} = "plant-light-frontend.service"

do_install:append() {
    install -d ${D}${systemd_unitdir}/system
    install -m 644 ${THISDIR}/files/plant-light-frontend.service ${D}${systemd_unitdir}/system/
}

FILES:${PN}:append = " ${systemd_unitdir}"