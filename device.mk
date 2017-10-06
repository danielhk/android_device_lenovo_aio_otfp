#
# Copyright (C) 2017 The Lineage Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# 20171001 - Oero port - daniel_hk (danielhk@github.com)

# call the proprietary setup
$(call inherit-product, vendor/lenovo/aio_otfp/aio_otfp-vendor.mk)

# The gps config appropriate for this device
$(call inherit-product, device/common/gps/gps_as_supl.mk)

LOCAL_PATH := device/lenovo/aio_otfp

# Device uses high-density artwork where available
PRODUCT_AAPT_CONFIG := normal
PRODUCT_AAPT_PREF_CONFIG := xhdpi

PRODUCT_TAGS += dalvik.gc.type-precise

#### COPY_FILES ####

# Permissions
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.bluetooth_le.xml:system/etc/permissions/android.hardware.bluetooth_le.xml \
    frameworks/native/data/etc/android.hardware.bluetooth.xml:system/etc/permissions/android.hardware.bluetooth.xml \
    frameworks/native/data/etc/android.hardware.faketouch.xml:system/etc/permissions/android.hardware.faketouch.xml \
    frameworks/native/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/native/data/etc/android.hardware.sensor.accelerometer.xml:system/etc/permissions/android.hardware.sensor.accelerometer.xml \
    frameworks/native/data/etc/android.hardware.sensor.compass.xml:system/etc/permissions/android.hardware.sensor.compass.xml \
    frameworks/native/data/etc/android.hardware.sensor.gyroscope.xml:system/etc/permissions/android.hardware.sensor.gyroscope.xml \
    frameworks/native/data/etc/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
    frameworks/native/data/etc/android.hardware.sensor.proximity.xml:system/etc/permissions/android.hardware.sensor.proximity.xml \
    frameworks/native/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml \
    frameworks/native/data/etc/android.hardware.touchscreen.multitouch.distinct.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.distinct.xml \
    frameworks/native/data/etc/android.hardware.touchscreen.multitouch.jazzhand.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.jazzhand.xml \
    frameworks/native/data/etc/android.hardware.touchscreen.multitouch.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.xml \
    frameworks/native/data/etc/android.hardware.touchscreen.xml:system/etc/permissions/android.hardware.touchscreen.xml \
    frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml \
    frameworks/native/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml \
    frameworks/native/data/etc/android.hardware.wifi.direct.xml:system/etc/permissions/android.hardware.wifi.direct.xml \
    frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/native/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml

PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml \
    frameworks/native/data/etc/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml \
    frameworks/native/data/etc/android.hardware.camera.manual_sensor.xml:system/etc/permissions/android.hardware.camera.manual_sensor.xml

# Audio policy & codec
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/audio_policy.conf:system/etc/audio_policy.conf \
    $(LOCAL_PATH)/configs/media_codecs.xml:system/etc/media_codecs.xml \
    $(LOCAL_PATH)/configs/media_codecs_performance.xml:system/etc/media_codecs_performance.xml \
    $(LOCAL_PATH)/configs/media_profiles.xml:system/etc/media_profiles.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_audio.xml:system/etc/media_codecs_google_audio.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_telephony.xml:system/etc/media_codecs_google_telephony.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_video_le.xml:system/etc/media_codecs_google_video_le.xml

# RAMDISK
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/rootdir/enableswap.sh:root/enableswap.sh \
    $(LOCAL_PATH)/rootdir/fstab.mt6752:root/fstab.mt6752 \
    $(LOCAL_PATH)/rootdir/init.modem.rc:root/init.modem.rc \
    $(LOCAL_PATH)/rootdir/init.mt6752.usb.rc:root/init.mt6752.usb.rc \
    $(LOCAL_PATH)/rootdir/init.mt6752.rc:root/init.mt6752.rc \
    $(LOCAL_PATH)/rootdir/init.project.rc:root/init.project.rc \
    $(LOCAL_PATH)/rootdir/ueventd.mt6752.rc:root/ueventd.mt6752.rc

# SECCOMP
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/seccomp/mediacodec.policy:system/vendor/etc/seccomp_policy/mediacodec.policy

# SPN
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/spn-conf.xml:system/etc/spn-conf.xml

# Custom charger images
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/charger/res/values/charger/animation.txt:root/res/values/charger/animation.txt \
    $(LOCAL_PATH)/charger/res/images/my_battery_scale.png:root/res/images/my_battery_scale.png \
    $(LOCAL_PATH)/charger/res/images/font_log.png:root/res/images/font_log.png


#### PACKAGES ####

# Audio
PRODUCT_PACKAGES += \
    audio.a2dp.default \
    audio.r_submix.default \
    audio.usb.default \
    libaudio-resampler \
    libtinyalsa \
    libtinycompress \
    libtinymix \
    libtinyxml \
    SoundRecorder

# Audio HAL
PRODUCT_PACKAGES += \
    android.hardware.audio@2.0-impl \
    android.hardware.audio.effect@2.0-impl \
    android.hardware.audio@2.0-service
#    android.hardware.soundtrigger@2.0-impl \

# RIL
PRODUCT_PACKAGES += \
    libccci_util \
    librilmtk \
    mtkrild

PRODUCT_PACKAGES += \
    com.android.future.usb.accessory \
    libemoji \
    libnl_2

# MTK Helpers
PRODUCT_PACKAGES += \
    libion \
    libwvmsym \
    libiodev

# Bluetooth
PRODUCT_PACKAGES += \
    android.hardware.bluetooth@1.0-impl \
    android.hardware.bluetooth@1.0-service \
    libbt-vendor

# Camera HAL
PRODUCT_PACKAGES += \
    camera.device@1.0-impl \
    camera.device@3.2-impl \
    android.hardware.camera.provider@2.4-impl \
    android.hardware.camera.provider@2.4-service

# FM Radio
PRODUCT_PACKAGES += \
    android.hardware.broadcastradio@1.0-impl \
    FMRadio \
    libfmjni

# GPS
PRODUCT_PACKAGES += \
    android.hardware.gnss@1.0-impl \
    gps.mt6752 \
    libcurl \
    YGPS

# Graphic HAL
PRODUCT_PACKAGES += \
    android.hardware.graphics.allocator@2.0-impl \
    android.hardware.graphics.allocator@2.0-service \
    android.hardware.graphics.composer@2.1-impl \
    android.hardware.graphics.mapper@2.0-impl \
    libgralloc_extra \
    libgui_ext

# Health HAL
PRODUCT_PACKAGES += \
    android.hardware.health@1.0-impl \
    android.hardware.health@1.0-service

# Keymaster HAL
PRODUCT_PACKAGES += \
    android.hardware.keymaster@3.0-impl

# Memtrack HAL
PRODUCT_PACKAGES += \
    android.hardware.memtrack@1.0-impl

# Power HAL
PRODUCT_PACKAGES += \
    android.hardware.power@1.0-impl

# Sensors
PRODUCT_PACKAGES += \
    android.hardware.light@2.0-impl \
    android.hardware.light@2.0-service \
    android.hardware.sensors@1.0-impl \
    android.hardware.sensors@1.0-service \
    lights.mt6752

# WiFi
PRODUCT_PACKAGES += \
    dhcpcd.conf \
    hostapd \
    libwpa_client \
    wpa_supplicant \
    wpa_supplicant.conf

DEVICE_PACKAGE_OVERLAYS := $(LOCAL_PATH)/overlay

#### PROPERTIES ####

# default.prop
PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
    ro.adb.secure=0 \
    ro.secure=0 \
    camera.disable_zsl_mode=1 \
    dalvik.vm.dex2oat-Xms=64m \
    dalvik.vm.dex2oat-Xmx=512m \
    dalvik.vm.image-dex2oat-Xms=64m \
    dalvik.vm.image-dex2oat-Xmx=64m \
    persist.service.acm.enable=0 \
    persist.sys.usb.config=mtp,adb \
    ro.allow.mock.location=0 \
    ro.config.low_ram=false \
    ro.debuggable=1 \
    ro.dalvik.vm.native.bridge=0 \
    ro.mount.fs=EXT4

# build.prop
PRODUCT_PROPERTY_OVERRIDES += \
    media.stagefright.legacyencoder=true \
    media.stagefright.less-secure=true \
    persist.service.adb.enable=1 \
    persist.service.debuggable=1 \
    persist.sys.root_access=0 \
    ro.sys.fw.bg_apps_limits=5

# extra log controls prop
PRODUCT_PROPERTY_OVERRIDES += \
    persist.ril.log=0 \
    ro.disable.xlog=0

# Dalvik & hwui defaults
$(call inherit-product-if-exists, frameworks/native/build/phone-xxhdpi-2048-dalvik-heap.mk)
$(call inherit-product-if-exists, frameworks/native/build/phone-xxhdpi-2048-hwui-memory.mk)
