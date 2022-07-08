CC ?= clang
ARCH ?= x64
ENABLE_TPM ?= 0
PREFIX ?= /usr/local

ifeq ($(ARCH),ia32)
	target_triple := i686-microsoft-windows
	mde_inc_dir := Ia32
	bootloader_file := systemd-bootia32.efi
	linux_stub_file := linuxia32.efi.stub
	stub_objects_platform := src/boot/efi/linux_x86.o
else ifeq ($(ARCH),x64)
	target_triple := x86_64-microsoft-windows
	mde_inc_dir := X64
	bootloader_file := systemd-bootx64.efi
	linux_stub_file := linuxx64.efi.stub
	stub_objects_platform := src/boot/efi/linux_x86.o
else ifeq ($(ARCH),arm)
	target_triple := arm-microsoft-windows
	mde_inc_dir := Arm
	bootloader_file := systemd-bootarm.efi
	linux_stub_file := linuxarm.efi.stub
	stub_objects_platform := src/boot/efi/linux.o
else ifeq ($(ARCH),aarch64)
	target_triple := aarch64-microsoft-windows
	mde_inc_dir := AArch64
	bootloader_file := systemd-bootaa64.efi
	linux_stub_file := linuxaa64.efi.stub
	stub_objects_platform := src/boot/efi/linux.o
else
	$(error Unsupported platform)
endif

internal_cflags := \
	-target $(target_triple) \
	-DSD_BOOT \
	-fno-builtin \
	-ffreestanding \
	-fshort-wchar \
	-fvisibility=hidden \
	-Iinclude \
	-Iedk2/MdePkg/Include \
	-Iedk2/MdePkg/Include/$(mde_inc_dir) \
	-Isrc/boot/efi \
	-Isrc/fundamental \
	-include src/boot/efi/efi_config.h \
	-include src/boot/efi/version.h \
	-std=gnu11 \
	-Wall \
	-Wextra \
	-Wno-macro-redefined \
	-Wno-microsoft-static-assert \
	-Wno-visibility \
	-Wno-pointer-sign \
	-Wno-unused-but-set-variable \
	-Wno-implicit-const-int-float-conversion \
	-Wno-sign-compare \
	-fno-stack-protector \
	-fno-strict-aliasing \
	-Werror
internal_cflags_ia32 := -mno-sse -mno-mmx
internal_cflags_x64 := -mno-red-zone -mno-sse -mno-mmx
internal_cflags_arm := -mgeneral-regs-only -mfpu=none

internal_ldflags := \
	-target $(target_triple) \
	-nostdlib \
	-fuse-ld=lld \
	-Wl,-entry:efi_main \
	-Wl,-subsystem:efi_application \
	-Wl,-errorlimit:0

ifeq ($(ARCH),ia32)
	internal_cflags += $(internal_cflags_ia32)
else ifeq ($(ARCH),x64)
	internal_cflags += $(internal_cflags_x64)
else ifeq ($(ARCH),arm)
	internal_cflags += $(internal_cflags_arm)
endif

general_objs := \
	src/data.o \
	src/fundamental/efivars-fundamental.o \
	src/fundamental/string-util-fundamental.o \
	src/boot/efi/assert.o \
	src/boot/efi/devicetree.o \
	src/boot/efi/disk.o \
	src/boot/efi/efi-string.o \
	src/boot/efi/graphics.o \
	src/boot/efi/initrd.o \
	src/boot/efi/measure.o \
	src/boot/efi/pe.o \
	src/boot/efi/secure-boot.o \
	src/boot/efi/ticks.o \
	src/boot/efi/util.o

stub_objects := \
	src/boot/efi/cpio.o \
	src/boot/efi/splash.o \
	src/boot/efi/stub.o \
	 $(stub_objects_platform)

boot_objects := \
	src/fundamental/bootspec-fundamental.o \
	src/fundamental/sha256.o \
	src/boot/efi/bcd.o \
	src/boot/efi/boot.o \
	src/boot/efi/console.o \
	src/boot/efi/drivers.o \
	src/boot/efi/random-seed.o \
	src/boot/efi/shim.o \
	src/boot/efi/xbootldr.o

generated_hdrs := \
	src/boot/efi/efi_config.h \
	src/boot/efi/version.h \
	src/boot/efi/sbat.h

all: $(generated_hdrs) $(linux_stub_file) $(bootloader_file)

$(linux_stub_file): $(general_objs) $(stub_objects)
	$(CC) $(internal_ldflags) -o $@ $?

$(bootloader_file): $(general_objs) $(boot_objects)
	$(CC) $(internal_ldflags) -o $@ $?

clean: $(generated_hdrs) $(general_objs) $(stub_objects) $(linux_stub_file) $(boot_objects)
	rm $?

src/boot/efi/efi_config.h:
	echo "#pragma once" >> $@
	echo "#define COLOR_EDIT EFI_TEXT_ATTR(EFI_BLACK, EFI_LIGHTGRAY)" >> $@
	echo "#define COLOR_ENTRY EFI_TEXT_ATTR(EFI_LIGHTGRAY, EFI_BLACK)" >> $@
	echo "#define COLOR_HIGHLIGHT EFI_TEXT_ATTR(EFI_BLACK, EFI_LIGHTGRAY)" >> $@
	echo "#define COLOR_NORMAL EFI_TEXT_ATTR(EFI_LIGHTGRAY, EFI_BLACK)" >> $@
	echo "#define ENABLE_TPM $(ENABLE_TPM)" >> $@
	echo '#define EFI_MACHINE_TYPE_NAME "'$(ARCH)'"' >> $@

src/boot/efi/version.h:
	echo '#pragma once' >> $@
	echo '#define GIT_VERSION "0.0.0"' >> $@

src/boot/efi/sbat.h:
	touch $@

.c.o:
	$(CC) $(internal_cflags) -c $< -o $@

.SUFFIXES: .c .o
