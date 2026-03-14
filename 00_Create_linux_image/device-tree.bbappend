FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
EXTRA_DT_INCLUDE_FILES:append:linux = " system-user.dtsi"
EXTRA_DT_INCLUDE_FILES:append:linux-gnueabi = " system-user.dtsi"

