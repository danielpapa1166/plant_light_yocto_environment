SUMMARY = "GPIO Character Device Kernel Module"
DESCRIPTION = "A simple GPIO character device kernel module"
LICENSE = "CLOSED"

SRC_URI = "file://gpio_chardev.c \
           file://Makefile"

S = "${WORKDIR}"

inherit module

# Automatically load the module at boot
KERNEL_MODULE_AUTOLOAD += "gpio_chardev"
PACKAGES += "kernel-module-gpio_chardev"
DEPENDS += "virtual/kernel"
FILES_kernel-module-gpio_chardev = "/lib/modules/${KERNEL_VERSION}/extra/gpio_chardev.ko"


# do_install[noexec] = "1"


# Create directories expected by kernel-module-split.bbclass
do_install:append() {
    install -d ${D}/etc
    install -d ${D}/etc/modprobe.d
    install -d ${D}/etc/modules-load.d
}

# Optional: module dependencies (if any)
# RDEPENDS_kernel-module-gpio_chardev = ""

# Install step handled automatically by 'module' class
# But if you need a custom install, uncomment and adjust:
# do_install() {
#     install -d ${D}/lib/modules/${KERNEL_VERSION}/extra
#     install -m 0644 ${B}/gpio_chardev.ko ${D}/lib/modules/${KERNEL_VERSION}/extra/
# }
