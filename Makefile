#
# Copyright (C) 2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=magent
PKG_RELEASE:=1.0

PKG_MAINTAINER:=nick <nicklgw@aliyun.com>
PKG_LICENSE:=LGPL-2.1

include $(INCLUDE_DIR)/package-seccomp.mk
include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/magent
	SECTION:=leedarson
	CATEGORY:=Leedarson
	TITLE:=Drive mosquitto with uloop event
	DEPENDS:=+libjson-c +libubox +libubus +libblobmsg-json
endef

define Package/magent/description
	Taking advantage of uloop events to handle mosquitto, 
	allows the entire business to complete with just one single thread.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Package/magent/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mosquitto-uloop $(1)/usr/bin/
endef

$(eval $(call BuildPackage,magent))

