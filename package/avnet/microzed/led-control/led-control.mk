################################################################################
#
# led-control
#
################################################################################

LED_CONTROL_SOURCE = led-control-$(LED_CONTROL_VERSION).tar.gz
LED_CONTROL_SITE = $(TOPDIR)/package/avnet/microzed/led-control/src
LED_CONTROL_SITE_METHOD = local
LED_CONTROL_REDISTRIBUTE = YES

# defaults to YES
LED_CONTROL_INSTALL_TARGET = YES
#LED_CONTROL_CONFIG_SCRIPTS =
LED_CONTROL_DEPENDENCIES =

define LED_CONTROL_BUILD_CMDS
    $(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" AR="$(TARGET_AR)" -C $(@D) all
endef

define LED_CONTROL_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/led-control $(TARGET_DIR)/usr/bin
endef

$(eval $(generic-package))
