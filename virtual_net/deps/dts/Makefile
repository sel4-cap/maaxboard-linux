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
all: ${BLT_PATH}/maaxboard.dts

${TMP_PATH}:
	mkdir ${TMP_PATH}

${BLT_PATH}:
	mkdir ${BLT_PATH}

${BLT_PATH}/maaxboard.dts: | ${TMP_PATH} ${BLT_PATH}
	cd ${TMP_PATH} ; git clone "git@github.com:Avnet/linux-imx.git"
	cd ${TMP_PATH} ; cpp -nostdinc -I linux-imx/include/ -undef -x assembler-with-cpp linux-imx/arch/arm64/boot/dts/freescale/maaxboard.dts > maaxboard.dts
	cp ${TMP_PATH}/maaxboard.dts ${BLT_PATH}/maaxboard.dts

.PHONY: clean
clean:
	rm -rf ${TMP_PATH}

.PHONY: reset
reset: clean
	rm -rf ${BLT_PATH}

#===============================================================================
# End of file
#===============================================================================
