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

DEP_PATH := deps
DEP_DTS_PATH := ${DEP_PATH}/dts
DEP_GST_PATH := ${DEP_PATH}/guest
DEP_LVM_PATH := ${DEP_PATH}/libvmm

SRC_PATH := src
TMP_PATH := tmp
BLT_PATH := blt

MKT_BOARD := maaxboard
MKT_CONFIG := debug

MKT_SDK := /packages/microkit/auts_microkit_stable/release/microkit-sdk-1.2.6

MKT_PATH_FILE := ${MKT_SDK}/bin/microkit
MKT_RTM_PATH_FILE := ${MKT_SDK}/board/${MKT_BOARD}/${MKT_CONFIG}

CPU := cortex-a53
TCH := aarch64-linux-gnu
CC := ${TCH}-gcc
LD := ${TCH}-ld
AS := ${TCH}-as

CC_OPS := -mcpu=${CPU} \
          -ffreestanding \
          -nostdlib \
          -mstrict-align \
          -g3 \
          -O3 \
          -Wall -Wno-unused-function -Werror

LVM_CFG_BOARD := BOARD_imx8mm_evk_hyp

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
all: ${BLT_PATH}/vxhci.img

${DEP_DTS_PATH}/blt/maaxboard.dts:
	make -C ${DEP_DTS_PATH} all

${DEP_GST_PATH}/blt/Image ${DEP_GST_PATH}/blt/rootfs.cpio.gz &:
	make -C ${DEP_GST_PATH} all

${DEP_LVM_PATH}/blt/libvmm:
	make -C ${DEP_LVM_PATH} all

${TMP_PATH}:
	mkdir ${TMP_PATH}

${BLT_PATH}:
	mkdir ${BLT_PATH}

${BLT_PATH}/vxhci.img: ${SRC_PATH}/vxhci.system ${TMP_PATH}/vxhci.elf | ${TMP_PATH} ${BLT_PATH}
	${MKT_PATH_FILE} ${SRC_PATH}/vxhci.system --search-path ${TMP_PATH} --board ${MKT_BOARD} --config ${MKT_CONFIG} --output ${BLT_PATH}/vxhci.img --report ${BLT_PATH}/report.txt

${TMP_PATH}/vxhci.elf: ${TMP_PATH}/maaxboard_vmm.o ${TMP_PATH}/maaxboard_guest.o ${TMP_PATH}/guest.o ${TMP_PATH}/printf.o ${TMP_PATH}/util.o ${TMP_PATH}/fault.o ${TMP_PATH}/linux.o ${TMP_PATH}/imx_sip.o ${TMP_PATH}/psci.o ${TMP_PATH}/smc.o ${TMP_PATH}/tcb.o ${TMP_PATH}/vcpu.o ${TMP_PATH}/virq.o ${TMP_PATH}/vgic.o ${TMP_PATH}/vgic_v3.o | ${TMP_PATH}
	${LD} \
	-L $(MKT_RTM_PATH_FILE)/lib \
	$^ \
	-lmicrokit \
	-Tmicrokit.ld \
	-o $@

${TMP_PATH}/maaxboard_vmm.o: ${SRC_PATH}/maaxboard_vmm.c ${DEP_LVM_PATH}/blt/libvmm | ${TMP_PATH}
	${CC} \
	-c \
	${CC_OPS} \
	-D ${LVM_CFG_BOARD} \
	-I ${DEP_LVM_PATH}/blt/libvmm/src \
	-I ${MKT_RTM_PATH_FILE}/include \
	$^ \
	-o $@

${TMP_PATH}/maaxboard_guest.o: ${DEP_LVM_PATH}/blt/libvmm/tools/package_guest_images.S ${DEP_GST_PATH}/blt/Image ${DEP_GST_PATH}/blt/rootfs.cpio.gz ${TMP_PATH}/maaxboard.dtb ${DEP_LVM_PATH}/blt/libvmm | ${TMP_PATH}
	${CC} \
	-c \
	-g3 \
	-x assembler-with-cpp \
	-DGUEST_KERNEL_IMAGE_PATH=\"${DEP_GST_PATH}/blt/Image\" \
	-DGUEST_DTB_IMAGE_PATH=\"${TMP_PATH}/maaxboard.dtb\" \
	-DGUEST_INITRD_IMAGE_PATH=\"${DEP_GST_PATH}/blt/rootfs.cpio.gz\" \
	"${DEP_LVM_PATH}/blt/libvmm/tools/package_guest_images.S" \
	-o $@

${TMP_PATH}/maaxboard.dtb: ${DEP_DTS_PATH}/blt/maaxboard.dts ${SRC_PATH}/maaxboard-overlay.dts | ${TMP_PATH}
	cat $^ > ${TMP_PATH}/guest.dts
	dtc -q -I dts -O dtb ${TMP_PATH}/guest.dts -o $@

${TMP_PATH}/%.o: ${DEP_LVM_PATH}/blt/libvmm/src/%.c | ${TMP_PATH}
	${CC} \
	-c \
	${CC_OPS} \
	-I ${DEP_LVM_PATH}/blt/libvmm/src \
	-I ${MKT_RTM_PATH_FILE}/include \
	$< \
	-o $@

${TMP_PATH}/%.o: ${DEP_LVM_PATH}/blt/libvmm/src/util/%.c | ${TMP_PATH}
	${CC} \
	-c \
	${CC_OPS} \
	-I ${DEP_LVM_PATH}/blt/libvmm/src/util \
	-I ${MKT_RTM_PATH_FILE}/include \
	$< \
	-o $@

${TMP_PATH}/%.o: ${DEP_LVM_PATH}/blt/libvmm/src/arch/aarch64/%.c | ${TMP_PATH}
	${CC} \
	-c \
	${CC_OPS} \
	-D ${LVM_CFG_BOARD} \
	-I ${DEP_LVM_PATH}/blt/libvmm/src \
	-I ${DEP_LVM_PATH}/blt/libvmm/src/util \
	-I ${DEP_LVM_PATH}/blt/libvmm/src/arch/aarch64 \
	-I ${MKT_RTM_PATH_FILE}/include \
	$< \
	-o $@

${TMP_PATH}/%.o: ${DEP_LVM_PATH}/blt/libvmm/src/arch/aarch64/vgic/%.c | ${TMP_PATH}
	${CC} \
	-c \
	${CC_OPS} \
	-D ${LVM_CFG_BOARD} \
	-I ${DEP_LVM_PATH}/blt/libvmm/src/util \
	-I ${DEP_LVM_PATH}/blt/libvmm/src/arch/aarch64 \
	-I ${MKT_RTM_PATH_FILE}/include \
	$< \
	-o $@

.PHONY: clean
clean:
	make -C ${DEP_DTS_PATH} clean
	make -C ${DEP_GST_PATH} clean
	make -C ${DEP_LVM_PATH} clean
	rm -rf ${TMP_PATH}

.PHONY: reset
reset: clean
	make -C ${DEP_DTS_PATH} reset
	make -C ${DEP_GST_PATH} reset
	make -C ${DEP_LVM_PATH} reset
	rm -rf ${BLT_PATH}

#===============================================================================
# End of file
#===============================================================================
