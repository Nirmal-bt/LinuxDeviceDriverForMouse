#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const char ____versions[]
__used __section("__versions") =
	"\x1c\x00\x00\x00\x37\x52\x5e\x53"
	"usb_register_driver\0"
	"\x18\x00\x00\x00\xf6\x30\xa4\x49"
	"usb_kill_urb\0\0\0\0"
	"\x20\x00\x00\x00\xb6\x2b\x2e\x34"
	"input_unregister_device\0"
	"\x18\x00\x00\x00\x9e\x77\xd9\xbd"
	"usb_free_urb\0\0\0\0"
	"\x1c\x00\x00\x00\x0b\xcb\xe2\xd0"
	"usb_free_coherent\0\0\0"
	"\x10\x00\x00\x00\xba\x0c\x7a\x03"
	"kfree\0\0\0"
	"\x14\x00\x00\x00\xc2\x02\xb5\x9f"
	"cdev_del\0\0\0\0"
	"\x24\x00\x00\x00\x33\xb3\x91\x60"
	"unregister_chrdev_region\0\0\0\0"
	"\x18\x00\x00\x00\xc2\x9c\xc4\x13"
	"_copy_from_user\0"
	"\x18\x00\x00\x00\xe1\xbe\x10\x6b"
	"_copy_to_user\0\0\0"
	"\x18\x00\x00\x00\x86\x2e\x6c\xe2"
	"usb_submit_urb\0\0"
	"\x14\x00\x00\x00\x3a\xd3\x39\x29"
	"_dev_err\0\0\0\0"
	"\x14\x00\x00\x00\x72\x17\xda\xd1"
	"input_event\0"
	"\x18\x00\x00\x00\xf8\x56\xc2\x0b"
	"usb_deregister\0\0"
	"\x18\x00\x00\x00\xbd\xcf\x6b\x9e"
	"kmalloc_caches\0\0"
	"\x18\x00\x00\x00\x77\x2b\xd6\xb9"
	"kmalloc_trace\0\0\0"
	"\x20\x00\x00\x00\x9b\x23\xb2\x8a"
	"input_allocate_device\0\0\0"
	"\x1c\x00\x00\x00\x8a\xab\xca\xe0"
	"usb_alloc_coherent\0\0"
	"\x18\x00\x00\x00\xbd\xfc\xe4\x78"
	"usb_alloc_urb\0\0\0"
	"\x10\x00\x00\x00\x9c\x53\x4d\x75"
	"strlen\0\0"
	"\x10\x00\x00\x00\x94\xb6\x16\xa9"
	"strnlen\0"
	"\x10\x00\x00\x00\x38\xdf\xac\x69"
	"memcpy\0\0"
	"\x14\x00\x00\x00\x6e\x4a\x6e\x65"
	"snprintf\0\0\0\0"
	"\x20\x00\x00\x00\x2f\x3d\x99\x2b"
	"input_register_device\0\0\0"
	"\x1c\x00\x00\x00\x2b\x2f\xec\xe3"
	"alloc_chrdev_region\0"
	"\x14\x00\x00\x00\xd4\x5f\x24\x3e"
	"cdev_alloc\0\0"
	"\x14\x00\x00\x00\x88\x09\xf8\xb3"
	"cdev_add\0\0\0\0"
	"\x1c\x00\x00\x00\x62\xda\x04\x83"
	"input_free_device\0\0\0"
	"\x18\x00\x00\x00\x8c\x89\xd4\xcb"
	"fortify_panic\0\0\0"
	"\x14\x00\x00\x00\xbb\x6d\xfb\xbd"
	"__fentry__\0\0"
	"\x1c\x00\x00\x00\x73\xe5\xd0\x6b"
	"down_interruptible\0\0"
	"\x10\x00\x00\x00\x7e\x3a\x2c\x12"
	"_printk\0"
	"\x1c\x00\x00\x00\xca\x39\x82\x5b"
	"__x86_return_thunk\0\0"
	"\x0c\x00\x00\x00\x66\x69\x2a\xcf"
	"up\0\0"
	"\x18\x00\x00\x00\x65\x3b\xd5\xea"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");

MODULE_ALIAS("usb:v*p*d*dc*dsc*dp*ic03isc01ip02in*");

MODULE_INFO(srcversion, "EEEDF6C8B292706A5C7B70A");
