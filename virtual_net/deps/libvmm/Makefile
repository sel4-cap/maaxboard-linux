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

all: ${BLT_PATH}/libvmm

${BLT_PATH}:
	mkdir ${BLT_PATH}

${BLT_PATH}/libvmm:| ${BLT_PATH}
	cd ${BLT_PATH} ; git clone "git@github.com:au-ts/libvmm.git" libvmm
	cd ${BLT_PATH} ; git -C libvmm reset --hard "dcfa24a2e5dbee7e90ab5a0e3a1432813d0e2a5a"
	cd ${BLT_PATH} ; git submodule update --init
	cp ${SRC_PATH}/imx_sip.h ${BLT_PATH}/libvmm/src/arch/aarch64/imx_sip.h
	cp ${SRC_PATH}/imx_sip.c ${BLT_PATH}/libvmm/src/arch/aarch64/imx_sip.c
	sed -i ${BLT_PATH}/libvmm/src/arch/aarch64/smc.c -e '/#include "psci.h"/i#include "imx_sip.h"'
	sed -i ${BLT_PATH}/libvmm/src/arch/aarch64/smc.c -e '/case SMC_CALL_STD_SERVICE:/icase SMC_CALL_SIP_SERVICE: return handle_imx_sip(vcpu_id, \&regs, fn_number, hsr); break;'

.PHONY: clean
clean:
	rm -rf ${TMP_PATH}

.PHONY: reset
reset: clean
	rm -rf ${BLT_PATH}

#===============================================================================
# End of file
#===============================================================================
