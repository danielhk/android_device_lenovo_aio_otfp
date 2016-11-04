LOCAL_PATH := device/lenovo/aio_otfp_m

MKIMAGE :=  $(LOCAL_PATH)/bootimg/mkimage

$(INSTALLED_RAMDISK_TARGET): $(MKIMAGE)
	$(call pretty,"Target ram disk: $@")
	$(hide) $(MKBOOTFS) $(TARGET_ROOT_OUT) | $(MINIGZIP) > $@
	$(hide) $(MKIMAGE) $@ ROOTFS > $(PRODUCT_OUT)/ramdisk_android.img
	$(hide) mv -f $(PRODUCT_OUT)/ramdisk_android.img $@

.PHONY: ramdisk-nodeps
ramdisk-nodeps: $(MKBOOTFS) | $(MINIGZIP)
	@echo "make $@: ignoring dependencies"
	$(hide) $(MKBOOTFS) $(TARGET_ROOT_OUT) | $(MINIGZIP) > $(INSTALLED_RAMDISK_TARGET)
	$(hide) $(MKIMAGE) $(INSTALLED_RAMDISK_TARGET) ROOTFS > $(PRODUCT_OUT)/ramdisk_android.img
	$(hide) mv -f $(PRODUCT_OUT)/ramdisk_android.img $(INSTALLED_RAMDISK_TARGET)


$(INSTALLED_BOOTIMAGE_TARGET): $(MKBOOTIMG) $(INTERNAL_BOOTIMAGE_FILES)
	$(call pretty,"Target boot image: $@")
	$(hide) $(MKIMAGE) $(TARGET_PREBUILT_INT_KERNEL)-dtb KERNEL > $(PRODUCT_OUT)/kernel
	$(hide) $(MKBOOTIMG) $(INTERNAL_BOOTIMAGE_ARGS) $(BOARD_MKBOOTIMG_ARGS) --output $@
	$(hide) $(call assert-max-image-size,$@,$(BOARD_BOOTIMAGE_PARTITION_SIZE),raw)
	@echo -e ${CL_CYN}"Made boot image: $@"${CL_RST}
			
.PHONY: bootimage-nodeps
bootimage-nodeps: $(MKBOOTIMG)
	@echo "make $@: ignoring dependencies"
	$(hide) $(MKIMAGE) $(TARGET_PREBUILT_INT_KERNEL)-dtb KERNEL > $(PRODUCT_OUT)/kernel	
	$(hide) $(MKBOOTIMG) $(INTERNAL_BOOTIMAGE_ARGS) $(BOARD_MKBOOTIMG_ARGS) --output $(INSTALLED_BOOTIMAGE_TARGET)
	$(hide) $(call assert-max-image-size,$(INSTALLED_BOOTIMAGE_TARGET),$(BOARD_BOOTIMAGE_PARTITION_SIZE),raw)
	@echo -e ${CL_INS}"Made boot image: $@"${CL_RST}

$(recovery_ramdisk): $(MINIGZIP) $(recovery_uncompressed_ramdisk)
	@echo -e ${CL_CYN}"----- Making compressed recovery ramdisk ------"${CL_RST}
	$(hide) $(MINIGZIP) < $(recovery_uncompressed_ramdisk) > $@
	$(hide) $(MKIMAGE) $@ RECOVERY > $(PRODUCT_OUT)/ramdisk_android.img
	$(hide) mv -f $(PRODUCT_OUT)/ramdisk_android.img $@

$(INSTALLED_RECOVERYIMAGE_TARGET): $(MKBOOTIMG) $(INTERNAL_RECOVERYIMAGE_FILES) $(recovery_ramdisk)
	$(call pretty,"Target recovery image: $@")
	$(hide) $(MKBOOTIMG) $(INTERNAL_RECOVERYIMAGE_ARGS) $(BOARD_MKBOOTIMG_ARGS) --output $@ --id > $(RECOVERYIMAGE_ID_FILE)
	$(hide) $(call assert-max-image-size,$@,$(BOARD_RECOVERYIMAGE_PARTITION_SIZE),raw)
	@echo -e ${CL_CYN}"Made recovery image: $@"${CL_RST}

