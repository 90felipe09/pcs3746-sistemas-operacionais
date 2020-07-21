CC=$(CROSS_COMPILE)gcc -Wall -Wextra -static -g
USR_BIN_FILES:=mount_fs invader

USR_BIN:=$(addprefix build/initramfs_root/usr/bin/, $(USR_BIN_FILES))

$(shell mkdir -p build/initramfs_root/usr/bin)

.PHONY: all clean busybox
all: build/rootfs.gz

clean:
		rm -rf build
		find -name '*.o' -delete

build/rootfs.gz: build/initramfs_root/init $(USR_BIN) busybox
		cp -ruav rootfs/. build/initramfs_root/
		cd build/initramfs_root ; find -print0 | cpio -0 -ov -H newc | gzip -9 > ../rootfs.gz

build/initramfs_root/init: init.o
		$(CC) -o $@ $^

$(USR_BIN): build/initramfs_root/usr/bin/%: %.o
	$(CC) -o $@ $^

busybox: 
	./busybox/configure.sh
	$(MAKE) -C busybox/busybox
	cp -uav busybox/busybox/busybox build/initramfs_root/usr/bin/busybox