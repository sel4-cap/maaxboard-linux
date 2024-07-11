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
all: ${OUT_PATH}/libvmm

${OUT_PATH}:
	mkdir ${OUT_PATH}

${OUT_PATH}/libvmm: | ${OUT_PATH}
	cd ${OUT_PATH} ; git clone "git@github.com:au-ts/libvmm.git" libvmm
	cd ${OUT_PATH} ; git -C libvmm submodule update --init
	cp ${SRC_PATH}/imx_sip.h ${OUT_PATH}/libvmm/include/libvmm/arch/aarch64/imx_sip.h
	cp ${SRC_PATH}/imx_sip.c ${OUT_PATH}/libvmm/src/arch/aarch64/imx_sip.c
	sed -i ${OUT_PATH}/libvmm/src/arch/aarch64/smc.c -e '/#include <libvmm/arch/aarch64/psci.h>/i#include <libvmm/arch/aarch64/imx_sip.h>'
	sed -i ${OUT_PATH}/libvmm/src/arch/aarch64/smc.c -e '/case SMC_CALL_STD_SERVICE:/icase SMC_CALL_SIP_SERVICE: return handle_imx_sip(vcpu_id, \&regs, fn_number, hsr); break;'

.PHONY: clean
clean:
	rm -rf ${TMP_PATH}

.PHONY: reset
reset: clean
	rm -rf ${OUT_PATH}

#===============================================================================
# End of file
#===============================================================================
