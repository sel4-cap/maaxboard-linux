#===============================================================================
# Makefile
#===============================================================================

#-------------------------------
# Check
#-------------------------------

MBE_PATH_FILE := /maaxboard-sel4-dev-env.version.txt

ifeq ($(wildcard ${MBE_PATH_FILE}),)
$(error MaaxBoard seL4 Developement Environment indicator absent: ${MBE_PATH_FILE})
endif

#-------------------------------
# Define
#-------------------------------

SRC_PATH := src
TMP_PATH := tmp
BLT_PATH := blt

#-------------------------------
# Target
#-------------------------------

.PHONY: usage
usage: 
	@echo "usage: make <target>"
	@echo ""
	@echo "<target> is one off:"
	@echo "all   (create tmp and blt content)"
	@echo "clean (delete tmp content)"
	@echo "reset (delete tmp and blt content)"

.PHONY: all
all: ${BLT_PATH}/Image ${BLT_PATH}/rootfs.cpio.gz

${TMP_PATH}:
	mkdir ${TMP_PATH}

${BLT_PATH}:
	mkdir ${BLT_PATH}

${TMP_PATH}/config: | ${TMP_PATH}
	mkdir ${TMP_PATH}/config

${TMP_PATH}/config/.config: ${SRC_PATH}/buildroot.defconfig | ${TMP_PATH}/config
	cp ${SRC_PATH}/buildroot.defconfig ${TMP_PATH}/config/.config

${TMP_PATH}/config/linux.defconfig: ${SRC_PATH}/linux.defconfig | ${TMP_PATH}/config
	cp ${SRC_PATH}/linux.defconfig ${TMP_PATH}/config/linux.defconfig

${BLT_PATH}/Image ${BLT_PATH}/rootfs.cpio.gz &: ${TMP_PATH}/config/.config ${TMP_PATH}/config/linux.defconfig | ${TMP_PATH} ${BLT_PATH}
	cd ${TMP_PATH} ; curl "https://buildroot.org/downloads/buildroot-2024.02.1.tar.gz" --output buildroot-2024.02.1.tar.gz
	cd ${TMP_PATH} ; tar -xf buildroot-2024.02.1.tar.gz
	cd ${TMP_PATH} ; make -C "buildroot-2024.02.1" O="../config" olddefconfig
	cd ${TMP_PATH} ; make -C "buildroot-2024.02.1" O="../config"
	cp ${TMP_PATH}/config/images/Image ${BLT_PATH}/Image
	cp ${TMP_PATH}/config/images/rootfs.cpio.gz ${BLT_PATH}/rootfs.cpio.gz

.PHONY: clean
clean:
	rm -rf ${TMP_PATH}

.PHONY: reset
reset: clean
	rm -rf ${BLT_PATH}

#===============================================================================
# End of file
#===============================================================================
