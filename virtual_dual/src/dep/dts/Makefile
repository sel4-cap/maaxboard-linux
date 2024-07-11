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
all: ${OUT_PATH}/maaxboard.dts

${TMP_PATH}:
	mkdir ${TMP_PATH}

${OUT_PATH}:
	mkdir ${OUT_PATH}

${OUT_PATH}/maaxboard.dts: | ${TMP_PATH}
${OUT_PATH}/maaxboard.dts: | ${OUT_PATH}
	cd ${TMP_PATH} ; git clone "git@github.com:Avnet/linux-imx.git"
	cd ${TMP_PATH} ; cpp -nostdinc -I linux-imx/include/ -undef -x assembler-with-cpp linux-imx/arch/arm64/boot/dts/freescale/maaxboard.dts > maaxboard.dts
	cp ${TMP_PATH}/maaxboard.dts ${OUT_PATH}/maaxboard.dts

.PHONY: clean
clean:
	rm -rf ${TMP_PATH}

.PHONY: reset
reset: clean
	rm -rf ${OUT_PATH}

#===============================================================================
# End of file
#===============================================================================
