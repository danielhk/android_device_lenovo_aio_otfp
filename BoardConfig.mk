#
# Copyright (C) 2016 The OmniROM Project
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

# inherit from the proprietary version
-include vendor/lenovo/aio_otfp/BoardConfigVendor.mk

LOCAL_PATH := device/lenovo/aio_otfp
 
MTK_K64_SUPPORT := yes

# Platform
TARGET_BOARD_PLATFORM := mt6752
MTK_BOARD_PLATFORMS := mt6752
TARGET_NO_BOOTLOADER := true
TARGET_NO_FACTORYIMAGE := true
TARGET_BOOTLOADER_BOARD_NAME := mt6752

# make_ext4fs requires numbers in dec format
BOARD_BOOTIMAGE_PARTITION_SIZE := 16777216
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 16777216
BOARD_PROTECT1IMAGE_PARTITION_SIZE:=10485760
BOARD_PROTECT2IMAGE_PARTITION_SIZE:=10485760
BOARD_SYSTEMIMAGE_PARTITION_SIZE:=2684354560
BOARD_CACHEIMAGE_PARTITION_SIZE:=444596224
BOARD_USERDATAIMAGE_PARTITION_SIZE:=1237319680

BOARD_HAS_LARGE_FILESYSTEM := true
TARGET_USERIMAGES_USE_EXT4 := true

ifneq ($(MTK_K64_SUPPORT),yes)
ARCH := arm
TARGET_ARCH := arm
TARGET_CPU_VARIANT := cortex-a7
TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_ARCH_VARIANT := armv7-a-neon
TARGET_2ND_CPU_VARIANT := cortex-a53
BOARD_KERNEL_CMDLINE := bootopt=64S3,32S1,32S1 androidboot.selinux=permissive
BOARD_KERNEL_OFFSET := 0x00008000
BOARD_KERNEL_IMAGE_NAME := zImage
else
ARCH := arm64
TARGET_ARCH := arm64
TARGET_ARCH_VARIANT := armv8-a
TARGET_CPU_ABI := arm64-v8a
TARGET_CPU_ABI2 :=
TARGET_CPU_VARIANT := cortex-a53
TARGET_2ND_ARCH := arm
TARGET_2ND_ARCH_VARIANT := armv7-a-neon
TARGET_2ND_CPU_ABI := armeabi-v7a
TARGET_2ND_CPU_ABI2 := armeabi
TARGET_2ND_CPU_VARIANT := cortex-a53
TARGET_KERNEL_CROSS_COMPILE_PREFIX := aarch64-linux-android-
BOARD_KERNEL_CMDLINE := bootopt=64S3,32N2,64N2 androidboot.selinux=permissive
TARGET_USES_64_BIT_BINDER := true
TARGET_IS_64_BIT :=true
TARGET_BOARD_SUFFIX := _64
TARGET_CPU_ABI_LIST := arm64-v8a,armeabi-v7a,armeabi
TARGET_CPU_ABI_LIST_32_BIT := armeabi-v7a,armeabi
TARGET_CPU_ABI_LIST_64_BIT := arm64-v8a
TARGET_SUPPORTS_32_BIT_APPS := true
TARGET_SUPPORTS_64_BIT_APPS := true
BOARD_KERNEL_OFFSET := 0x00080000
BOARD_KERNEL_IMAGE_NAME := Image.gz
endif
TARGET_CPU_CORTEX_A53 := true
ARCH_ARM_HAVE_TLS_REGISTER := true
TARGET_CPU_SMP := true

BOARD_RAMDISK_OFFSET := 0x04000000
BOARD_TAGS_OFFSET := 0xE000000
BOARD_KERNEL_BASE := 0x40000000
BOARD_KERNEL_PAGESIZE := 2048
BOARD_MKBOOTIMG_ARGS := --kernel_offset $(BOARD_KERNEL_OFFSET) --ramdisk_offset $(BOARD_RAMDISK_OFFSET) --tags_offset $(BOARD_TAGS_OFFSET)
BOARD_FLASH_BLOCK_SIZE := 131072
TARGET_KMODULES := true

# Kernel
BOARD_CUSTOM_BOOTIMG_MK := $(LOCAL_PATH)/bootimg/bootimg.mk
TARGET_KERNEL_CONFIG := aio_otfp_n_defconfig
TARGET_KERNEL_SOURCE := kernel/lenovo/aio_otfp
TARGET_MTK_KERNEL := true

# Recovery allowed devices
TARGET_OTA_ASSERT_DEVICE := K50-t5,aio_otfp_m,aio_otfp,K3Note

# recovery
TARGET_USE_CUSTOM_LUN_FILE_PATH := "/sys/class/android_usb/android0/f_mass_storage/lun/file"
BOARD_HAS_NO_SELECT_BUTTON := true
BOARD_RECOVERY_SWIPE := true
BOARD_SUPPRESS_EMMC_WIPE := true
BOARD_USE_CUSTOM_RECOVERY_FONT := \"roboto_15x24.h\"
TARGET_RECOVERY_PIXEL_FORMAT := "RGBA_8888"
BOARD_USES_FULL_RECOVERY_IMAGE := true

# TWRP
RECOVERY_VARIANT := twrp
TARGET_RECOVERY_FSTAB := $(LOCAL_PATH)/rootdir/recovery.fstab
TARGET_PREBUILT_RECOVERY_KERNEL := $(LOCAL_PATH)/kernel
HAVE_SELINUX := true
TW_INCLUDE_CRYPTO := true
TW_NO_USB_STORAGE := false
RECOVERY_GRAPHICS_USE_LINELENGTH := true
TW_NO_REBOOT_BOOTLOADER := false
TW_USE_TOOLBOX := false
RECOVERY_SDCARD_ON_DATA := true
TW_THEME := portrait_hdpi
TW_DEFAULT_LANGUAGE := en
TW_DEFAULT_EXTERNAL_STORAGE := true
TW_BRIGHTNESS_PATH := /sys/class/leds/lcd-backlight/brightness
TW_MAX_BRIGHTNESS := 255
TW_CUSTOM_CPU_TEMP_PATH := /sys/devices/virtual/thermal/thermal_zone1/temp

# Disable memcpy opt (for audio libraries)
TARGET_CPU_MEMCPY_OPT_DISABLE := true

# MTK Hardware
BOARD_HAS_MTK_HARDWARE := true
BOARD_USES_LEGACY_MTK_AV_BLOB := true
BOARD_USES_MTK_AUDIO := true
USE_CAMERA_STUB := true
BOARD_PROVIDES_RILD := true
BOARD_RIL_CLASS := ../../../device/lenovo/aio_otfp/ril

TARGET_GLOBAL_CFLAGS   += -mfpu=neon -mfloat-abi=softfp
TARGET_GLOBAL_CPPFLAGS += -mfpu=neon -mfloat-abi=softfp
BOARD_DISABLE_HW_ID_MATCH_CHECK := true;

# Deodex
WITH_DEXPREOPT := false

# Enable Minikin text layout engine (will be the default soon)
USE_MINIKIN := true

# Charger
BOARD_CHARGER_SHOW_PERCENTAGE := true

# Fonts
EXTENDED_FONT_FOOTPRINT := true

# FM
MTK_FM_SUPPORT:=yes

# Bluetooth
BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_MTK := true
BOARD_BLUETOOTH_DOES_NOT_USE_RFKILL := true
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR := $(LOCAL_PATH)/bluetooth

# Graphic
USE_OPENGL_RENDERER := true
BOARD_EGL_CFG := $(LOCAL_PATH)/configs/egl.cfg
BOARD_EGL_WORKAROUND_BUG_10194508 := true
TARGET_RUNNING_WITHOUT_SYNC_FRAMEWORK := true

# GPS
BOARD_GPS_LIBRARIES := true
BOARD_CONNECTIVITY_MODULE := conn_soc 
BOARD_MEDIATEK_USES_GPS := true

# MTK_WLAN_SUPPORT
BOARD_CONNECTIVITY_VENDOR        := MediaTek
WPA_SUPPLICANT_VERSION           := VER_0_8_X
BOARD_HOSTAPD_DRIVER             := NL80211
BOARD_HOSTAPD_PRIVATE_LIB        := lib_driver_cmd_mt66xx
BOARD_WPA_SUPPLICANT_DRIVER      := NL80211
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_mt66xx
WIFI_DRIVER_FW_PATH_PARAM        := "/dev/wmtWifi"
WIFI_DRIVER_FW_PATH_STA          := STA
WIFI_DRIVER_FW_PATH_AP           := AP
WIFI_DRIVER_FW_PATH_P2P          := P2P

# RIL
MTK_CIP_SUPPORT := no
MTK_DT_SUPPORT := no
MTK_VT3G324M_SUPPORT := no
MTK_UMTS_TDD128_MODE := no
MTK_SHARE_MODEM_CURRENT := 2
MTK_SHARE_MODEM_SUPPORT := 2
MTK_MD_SHUT_DOWN_NT := yes
MTK_FD_SUPPORT := yes
MTK_IPV6_SUPPORT := yes
MTK_LTE_SUPPORT := yes
MTK_LTE_DC_SUPPORT := no
MTK_SVLTE_SUPPORT := no
MTK_EAP_SIM_AKA := yes
MTK_IRAT_SUPPORT := no
MTK_DTAG_DUAL_APN_SUPPORT := no
MTK_MD1_SUPPORT := 5
MTK_MD2_SUPPORT := 4
MTK_MD3_SUPPORT := 2
MTK_MD5_SUPPORT := 5
MTK_ENABLE_MD1 = yes
MTK_ENABLE_MD2 = no
MTK_ENABLE_MD3 = no
MTK_ENABLE_MD5 = no
MTK_TC1_FEATURE := no
MTK_SIM_RECOVERY := yes
MTK_SIM_HOT_SWAP_COMMON_SLOT := yes
MTK_WORLD_PHONE := no
MTK_VOLTE_SUPPORT := no
MTK_VILTE_SUPPORT := no
MTK_WIFI_CALLING_RIL_SUPPORT := no
MTK_WLAN_SUPPORT := yes

TARGET_SYSTEM_PROP := $(LOCAL_PATH)/system.prop
TARGET_SPECIFIC_HEADER_PATH := $(LOCAL_PATH)/include

# Hack for build
TARGET_BLOBS_BYPASS_SYMBOL_ERR := true

# Hardware SEPolicy
BOARD_SEPOLICY_DIRS += \
    device/lenovo/aio_otfp_m/sepolicy
