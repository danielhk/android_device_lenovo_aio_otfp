#
# Copyright (C) 2016 The CarbonROM Project
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
# Screen Resolution for the Bootanimation
TARGET_SCREEN_HEIGHT := 1920
TARGET_SCREEN_WIDTH := 1080

# Inherit from AOSP 64-bit support
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)

# Inherit from AOSP product configuration
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/telephony.mk)

# Inherit carbon's custom product configuration
$(call inherit-product, vendor/carbon/config/common.mk)

# Pull in GSM-specific stuff such as APNs
$(call inherit-product, vendor/carbon/config/gsm.mk)

# Call device specific makefile
$(call inherit-product, device/lenovo/aio_otfp/device.mk)

# Discard inherited values and use our own instead.
PRODUCT_NAME := carbon_aio_otfp
PRODUCT_DEVICE := aio_otfp
PRODUCT_BRAND := Lemon
PRODUCT_MODEL := K3Note
PRODUCT_MANUFACTURER := Lenovo
PRODUCT_RESTRICT_VENDOR_FILES := false
