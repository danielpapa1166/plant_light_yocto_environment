inherit systemd

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " \
    file://wpa_supplicant.conf \
    file://wpa_supplicant-wlan0.conf \
    file://10-wlan0.network \
"

do_install:append() {
    # install wpa_supplicant.conf
    install -d ${D}${sysconfdir}/wpa_supplicant
    install -m 0600 ${WORKDIR}/wpa_supplicant.conf \
        ${D}${sysconfdir}/wpa_supplicant/

    install -m 0600 ${WORKDIR}/wpa_supplicant-wlan0.conf \
        ${D}${sysconfdir}/wpa_supplicant/

    # install network file for systemd-networkd
    install -d ${D}${sysconfdir}/systemd/network
    install -m 0644 ${WORKDIR}/10-wlan0.network \
        ${D}${sysconfdir}/systemd/network/10-wlan0.network
}

# SYSTEMD_SERVICE:${PN} = "wpa_supplicant.service"
SYSTEMD_SERVICE:${PN} += "wpa_supplicant@wlan0.service"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"
