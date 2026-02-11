SUMMARY = "Minimal Raspberry Pi 4 Broadcom Wi-Fi firmware"
DESCRIPTION = "Installs only brcmfmac43455-sdio.raspberrypi,4-model-b.bin"
LICENSE = "CLOSED"

SRC_URI = "file://brcmfmac43455-sdio.raspberrypi,4-model-b.bin"

S = "${WORKDIR}"

inherit deploy

do_install() {
    # Create the firmware directory
    install -d ${D}${nonarch_base_libdir}/firmware/brcm
    # Copy the firmware
    install -m 0644 ${WORKDIR}/brcmfmac43455-sdio.raspberrypi,4-model-b.bin \
        ${D}${nonarch_base_libdir}/firmware/brcm/
}

FILES:${PN} += "${nonarch_base_libdir}/firmware/brcm/brcmfmac43455-sdio.raspberrypi,4-model-b.bin"

#FILES_${PN} += "${libdir}/*"
#FILES_${PN}-dev = "${libdir}/* ${includedir}"

#INSANE_SKIP:${PN} += "installed-vs-shipped"

# build everything directly into rootfs
#IMAGE_FEATURES_remove = "package-management"
#ALLOW_EMPTY_${PN} = "1" 