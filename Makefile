ifneq ($(KERNELRELEASE),)
obj-m := chromebook_kb_backlight.o chromeos_keyboard_bl.o chromeos_keyboard_leds.o
else
KDIR ?= /lib/modules/`uname -r`/build

all: chromebook_kb_backlight chromeos_keyboard_bl chromeos_keyboard_leds keyboard_brightness

chromebook_kb_backlight: chromebook_kb_backlight.c
	$(MAKE) -C $(KDIR) M=$$PWD

chromeos_keyboard_bl: chromeos_keyboard_bl.c
	$(MAKE) -C $(KDIR) M=$$PWD

chromeos_keyboard_leds: chromeos_keyboard_leds.c
	$(MAKE) -C $(KDIR) M=$$PWD

keyboard_brightness: keyboard_brightness.c
	gcc -o keyboard_brightness keyboard_brightness.c

clean:
	rm -f *.o *.ko modules.order Module.symvers *.mod.c keyboard_brightness
endif

install: chromebook_kb_backlight chromeos_keyboard_leds
	cp ./chromebook_kb_backlight.ko /lib/modules/`uname -r`/kernel/drivers/platform/chrome/
	cp ./chromeos_keyboard_leds.ko /lib/modules/`uname -r`/kernel/drivers/platform/chrome/
	depmod -a
	modprobe chromebook_kb_backlight
	modprobe chromeos_keyboard_leds
