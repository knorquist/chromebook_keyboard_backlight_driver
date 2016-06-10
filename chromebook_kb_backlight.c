/*
 *  chromebook_kb_backlight.c - Driver to Chromebook keyboard backlight devices.
 *
 *  Author : Benson Leung <bleung@chromium.org>
 * 
 *  Copyright (C) 2012 Google, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <linux/dmi.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>

#ifdef KB_BACKLIGHT
#define DEVICE_NAME "chromeos-keyboard-backlight"
#else
#define DEVICE_NAME "chromeos-keyboard-leds"
#endif

static struct platform_device *kb_backlight_device;

static int __init setup_keyboard_backlight(const struct dmi_system_id *id)
{
	kb_backlight_device =
		platform_device_register_simple(DEVICE_NAME,
						-1, NULL, 0);
	if (IS_ERR(kb_backlight_device)) {
		pr_warn("Error registering Chromebook keyboard LEDs.\n");
		kb_backlight_device = NULL;
	}
	return 0;
}

static struct dmi_system_id __initdata chromebook_kb_backlight_dmi_table[] = {
	{
		.ident = "Chromebook pixel - Keyboard backlight",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "GOOGLE"),
			DMI_MATCH(DMI_PRODUCT_NAME, "Link"),
		},
		.callback = setup_keyboard_backlight,
	},
	{
		.ident = "Toshiba Chromebook 2 - Keyboard backlight",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "GOOGLE"),
			DMI_MATCH(DMI_PRODUCT_NAME, "Gandof"),
		},
		.callback = setup_keyboard_backlight,
	},
	{
		.ident = "Toshiba Chromebook 2 - Keyboard backlight",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "GOOGLE"),
			DMI_MATCH(DMI_PRODUCT_NAME, "Gandof"),
		},
		.callback = setup_keyboard_backlight,
	},
	{
		.ident = "Dell Chromebook 13 - Keyboard backlight",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "GOOGLE"),
			DMI_MATCH(DMI_PRODUCT_NAME, "Lulu"),
		},
		.callback = setup_keyboard_backlight,
	}, 
	{
		.ident = "Dell Chromebook 13 - Keyboard backlight",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Google"),
			DMI_MATCH(DMI_PRODUCT_NAME, "Lulu"),
		},
		.callback = setup_keyboard_backlight,
	}, 
    { }
};
MODULE_DEVICE_TABLE(dmi, chromebook_kb_backlight_dmi_table);

static int __init chromebook_kb_backlight_init(void)
{
	if (!dmi_check_system(chromebook_kb_backlight_dmi_table)) {
		pr_debug("%s unsupported system.\n", __func__);
		return -ENODEV;
	}
	return 0;
}

static void __exit chromebook_kb_backlight_exit(void)
{
	if (kb_backlight_device)
		platform_device_unregister(kb_backlight_device);
}

module_init(chromebook_kb_backlight_init);
module_exit(chromebook_kb_backlight_exit);

MODULE_DESCRIPTION("Chromebook Keyboard backlight driver");
MODULE_AUTHOR("Benson Leung <bleung@chromium.org>");
MODULE_LICENSE("GPL");
