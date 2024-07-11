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
# Layout
#-------------------------------

DEP_PATH := dep
SRC_PATH := src
TMP_PATH := tmp
OUT_PATH := out

#-------------------------------
# Usage
#-------------------------------

.PHONY: usage
usage: 
	@echo "usage: make <target>"
	@echo ""
	@echo "<target> is one off:"
	@echo "all   (create tmp and out content)"
	@echo "clean (delete tmp content)"
	@echo "reset (delete tmp and out content)"

#-------------------------------
# Define
#-------------------------------

#-------------------------------
# Target
#-------------------------------

.PHONY: all
all: ${OUT_PATH}/Image ${OUT_PATH}/rootfs.cpio.gz

${TMP_PATH}:
	mkdir ${TMP_PATH}

${OUT_PATH}:
	mkdir ${OUT_PATH}

${TMP_PATH}/config: | ${TMP_PATH}
	mkdir ${TMP_PATH}/config

${TMP_PATH}/config/.config: ${SRC_PATH}/buildroot.defconfig | ${TMP_PATH}/config
	cp ${SRC_PATH}/buildroot.defconfig ${TMP_PATH}/config/.config

${TMP_PATH}/config/linux.defconfig: ${SRC_PATH}/linux.defconfig | ${TMP_PATH}/config
	cp ${SRC_PATH}/linux.defconfig ${TMP_PATH}/config/linux.defconfig

${OUT_PATH}/Image ${OUT_PATH}/rootfs.cpio.gz &: ${TMP_PATH}/config/.config ${TMP_PATH}/config/linux.defconfig | ${TMP_PATH} ${OUT_PATH}
	cd ${TMP_PATH} ; curl "https://buildroot.org/downloads/buildroot-2024.02.1.tar.gz" --output buildroot-2024.02.1.tar.gz
	cd ${TMP_PATH} ; tar -xf buildroot-2024.02.1.tar.gz
	cd ${TMP_PATH} ; make -C "buildroot-2024.02.1" O="../config" olddefconfig
	cd ${TMP_PATH} ; make -C "buildroot-2024.02.1" O="../config"
	cp ${TMP_PATH}/config/images/Image ${OUT_PATH}/Image
	cp ${TMP_PATH}/config/images/rootfs.cpio.gz ${OUT_PATH}/rootfs.cpio.gz

.PHONY: clean
clean:
	rm -rf ${TMP_PATH}

.PHONY: reset
reset: clean
	rm -rf ${OUT_PATH}

#===============================================================================
# End of file
#===============================================================================
