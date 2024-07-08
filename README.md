# Build Root

It is appropriate to use Build Root, to establish a specific configuration,
that can subsequently be used to create the desired builds. This is achieved
in two steps:
* **Configure**: Initial, one time only, configuration. Prepare a set of
  configuration files which may be subsequently used to orchestrate and adjust
the desire build. Configuration files have been stored in the **src** folder.
If these files are used skip straight to the [Build section](/maaxboard-linux/README.md#build)
* **Build**: Use previously established configuration files to achieve a
  desired build.

## Configure

### Initialise

#### Acquire Build Root

```
curl "https://buildroot.org/downloads/buildroot-2024.02.1.tar.gz" --output buildroot-2024.02.1.tar.gz
tar -xf buildroot-2024.02.1.tar.gz
rm -f buildroot-2024.02.1.tar.gz
```

#### Add usbip module to buildroot

Create a file named **linux-tool-usbip.mk.in** with the following text in this folder **buildroot-2024.02.1/package/linux-tools**:
```
################################################################################
#
# usbip
#
################################################################################

LINUX_TOOLS += usbip

USBIP_DEPENDENCIES = udev
USBIP_CONF_OPTS = --with-tcp-wrappers=no

define USBIP_BUILD_CMDS
 $(Q)if test ! -f $(LINUX_DIR)/tools/usb/usbip/Makefile.am ; then \
 echo "Your kernel version is too old and does not have the usbip tool." ; \
 echo "At least kernel 3.17 must be used." ; \
 exit 1 ; \
 fi

 cd $(LINUX_DIR)/tools/usb/usbip && PATH=$(BR_PATH) ./autogen.sh

 cd $(LINUX_DIR)/tools/usb/usbip && rm -rf config.cache && \
 $(TARGET_CONFIGURE_OPTS) \
 $(TARGET_CONFIGURE_ARGS) \
 $(USBIP_CONF_ENV) \
 CONFIG_SITE=/dev/null \
 ./configure \
 --target=$(GNU_TARGET_NAME) \
 --host=$(GNU_TARGET_NAME) \
 --build=$(GNU_HOST_NAME) \
 --prefix=/usr \
 --exec-prefix=/usr \
 --sysconfdir=/etc \
 --localstatedir=/var \
 --program-prefix="" \
 $(QUIET) $(USBIP_CONF_OPTS)

 $(TARGET_MAKE_ENV) $(MAKE) $(USBIP_MAKE_OPTS) \
 -C $(LINUX_DIR)/tools/usb/usbip/
endef

# for libusbip
define USBIP_INSTALL_STAGING_CMDS
 $(TARGET_MAKE_ENV) $(MAKE) -C $(LINUX_DIR)/tools/usb/usbip \
 $(USBIP_MAKE_OPTS) \
 DESTDIR=$(STAGING_DIR) \
 install
endef

define USBIP_INSTALL_TARGET_CMDS
 $(TARGET_MAKE_ENV) $(MAKE) -C $(LINUX_DIR)/tools/usb/usbip \
 $(USBIP_MAKE_OPTS) \
 DESTDIR=$(TARGET_DIR) \
 install
endef
```
Add this text to Config.in:
```
config BR2_PACKAGE_LINUX_TOOLS_USBIP
 bool "usbip"
 depends on BR2_PACKAGE_HAS_UDEV
 depends on BR2_TOOLCHAIN_HEADERS_AT_LEAST_3_17 # moved out of staging/ dir in kernel 3.17
 select BR2_PACKAGE_LINUX_TOOLS
 help
 USB/IP protocol allows to pass USB device from server to client over
 the network.
 You need to activate support for it in your kernel configuration.

 This (usbip) is the set of userspace tools used to handle connection
 and management.

 You can optionally add hwdata package to your BR config to have
 better runtime experience.

 https://github.com/torvalds/linux/blob/master/tools/usb/usbip/README

comment "usbip needs udev /dev management and a toolchain w/ headers >= 3.17"
 depends on !BR2_PACKAGE_HAS_UDEV || !BR2_TOOLCHAIN_HEADERS_AT_LEAST_3_17
 ```


#### Configure build root

```
mkdir config
make -C ./buildroot-2024.02.1 O="${PWD}/config" menuconfig
```

Note that Build Root creates content in "config", including a Makefile. Be
aware that, somewhat annoyingly, this Makefile is hard coded to this specific
Build Root instance. A sensible starting point is to select the most minimum
default configuration:
* Target Options::Target Architecture (AArch64 (little endian))
* Toolchain::Toolchain type (External toolchain)
* System configuration::/dev management (Dynamic using devtmpfs + eudev)::Dynamic using devtmpfs + eudev
* Kernel::Linux Kernel
* Kernel::Linux Kernel::Kernel configuration (Use the architecture default configuration)
* Kernel::Linux Kernel Tools::usbip
* Filesystem images::cpio the root filesystem (for use as an initial RAM filesystem)
* Filesystem images::cpio the root filesystem::Compression method (gzip)

Save the ".config" at the default location, which shall be within "config".

### Build buildroot

At this point, a full build is possible. This is helpful, as it will pull in
all needed items, such as the toolchain, and the Linux Kernel. While there is
a Makefile within "config" we are choosing to ignore this, as it is hard
coded, and we seek a portable (relative) configuration:
```
cd tmp
make -C ./buildroot-2024.02.1 O="${PWD}/config"
```
### Configure Linux

While the default build may be sufficient, it can be valuable to configure,
control, and change this.

Create complete default configuration for chosen architecture:
```
cd tmp
make -C ./config/build/linux-6.6.22 ARCH=arm64 defconfig
```

This shall create:
```
./tmp/config/build/linux-6.6.22/.config`
```

Edit ".config" as appropriate. The following changes have been made:
* Replace every "=m" with "=n", to avoid kernel modules.

Add the following lines to compile usbip modules
```
CONFIG_USBIP_CORE=y
CONFIG_USBIP_VHCI_HCD=y
CONFIG_USBIP_HOST=y
CONFIG_USBIP_DEBUG=y
```

Save as compact default configuration (defconfig):
```
cd tmp
make -C ./config/build/linux-6.6.22 ARCH=arm64 savedefconfig
```

This shall create:
```
./tmp/config/build/linux-6.6.22/defconfig`
```

Retain this, as our linux configuration:
```
cp ./tmp/config/build/linux-6.6.22/defconfig ./src/linux.defconfig
```

Configure build root to use this configuration:
```
cd tmp
make -C "${PWD}/buildroot-2024.02.1" O="${PWD}/config" menuconfig
```

Adjust buildroot configuration as follows:
* Kernel::Linux Kernel::Kernel configuration (Using a custom (def)config file)
* Kernel::Linux Kernel::Configuration file path (../config/linux.defconfig)

Save the ".config" at the default location, which shall be within "config".

Note that for Build Root, everything is relative to the location of its
outermost Makefile. So, knowing this, we pick a suitable relative path
("../config/linux.defconfig"). Keeping the entire configuration relative is
deliberate and desirable to permit use in locations.

### Configure Buildroot

The Build Root configuration is huge, and we nearly entirely want to use the
sensible normal defaults. So, frame in terms of this delta.

Save compact default configuration (defconfig):
```
cd tmp
make -C "${PWD}/buildroot-2024.02.1" O="${PWD}/config" savedefconfig
```

This will create:
```
./tmp/config/defconfig
```

Retain this, as our buildroot configuration:
```
cp ./tmp/config/defconfig ./src/buildroot.defconfig
```

### Clean

It may be good to clean up:
```
make -C "${PWD}/buildroot-2024.02.1" O="${PWD}/config" clean
```
## Build

Once we have configured, we may now build as follows:
```
mkdir tmp
mkdir tmp/config
cp ./src/buildroot.defconfig ./tmp/config/.config
cp ./src/linux.defconfig ./tmp/config/linux.defconfig 
cd tmp
curl "https://buildroot.org/downloads/buildroot-2024.02.1.tar.gz" --output buildroot-2024.02.1.tar.gz
tar -xf buildroot-2024.02.1.tar.gz
make -C "buildroot-2024.02.1" O="../config" olddefconfig
make -C "buildroot-2024.02.1" O="../config"
```

Retain the result:
```
cp ./tmp/config/images/Image ./src/Image
cp ./tmp/config/images/rootfs.cpio.gz ./src/rootfs.cpio.gz
```

Convert filesystem to uImage
```
mkimage -A arm -O linux -T ramdisk -a 0x44000000 -C gzip -n "Build Root File System" -d rootfs.cpio.gz initramfs.uImage
```

## Rebuild

For investigation purposes, it can be valuable to locally modify the Linux
kernel, and build from this locally modified version. This can be directly and
efficiently orchestrated via the following:
```
make -C "buildroot-2024.02.1" O="../config" linux-build
make -C "buildroot-2024.02.1" O="../config" linux-rebuild
make -C "buildroot-2024.02.1" O="../config" linux-clean
```

# Run on maaxboard
```
tftpboot 0x40480000 daniel_linux.img
tftpboot 0x44000000 bjemaaxboard.dtb
tftpboot 0x46000000 daniel_initramfs.uImage
booti 0x40480000 0x46000000 0x44000000
```

# Usbip setup

## Set up server (maaxboard)
Start the deamon in backgroun
```
usbipd -D
```
List the devices that can be shared
```
usbip list -l
```
Bind the device
```
usbip bind -b <bus ID>
```
## Set up client (lithium)
Load modules
```
sudo modprobe usbip-core
sudo modprobe vhci-hcd
```
Query the server
```
sudo usbip list -r <server ip>
```
Attach device 
```
sudo usbip attach -r <server> -b <bus ID>
```


