ifneq ($(KERNELRELEASE),)
obj-m := chromebook_kb_backlight.o chromeos_keyboard_bl.o chromeos_keyboard_leds.o
else
KDIR ?= /lib/modules/`uname -r`/build

all: chromebook_kb_backlight chromeos_keyboard_bl chromeos_keyboard_leds

chromebook_kb_backlight: chromebook_kb_backlight.c
	$(MAKE) -C $(KDIR) M=$$PWD

chromeos_keyboard_bl: chromeos_keyboard_bl.c
	$(MAKE) -C $(KDIR) M=$$PWD

chromeos_keyboard_leds: chromeos_keyboard_leds.c
	$(MAKE) -C $(KDIR) M=$$PWD

clean:
	rm -f *.o *.ko modules.order Module.symvers *.mod.c
endif
