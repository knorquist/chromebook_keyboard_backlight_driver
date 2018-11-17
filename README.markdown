# Keyboard Backlight Driver for Chromebooks Running Linux Kernels 3.x / 4.1.x

This fork makes a few minor additions to make it easier to adjust the keyboard backlight via keyboard shortcut.

## Use NOPASSWD

In order to adjust the backlight levels on the fly, without dealing with nagging password prompts, we'll need to add the directory holding the project to your `sudoers.d` folder.

First, `cd` to `/etc/sudoers.d` and create a new file - you can name it whatever you like, but it should probably be something that will let you know what it's there for. In this case, I'll name it `keyboard_backlight`:

`user@host:/etc/sudoers.d$ sudo touch keyboard_backlight`

Second, modify it with the `visudo` command. The recommended method for editing `sudoers` and files in `sudoers.d` is with `visudo`, since it apparently has lots of built-in protections so you don't accidentally break something with sudo.

`user@host:/etc/sudoers.d$ sudo visudo -f keyboard_backlight`

Third, add the project directory so that you don't need to get prompted for your password every time you try to run a script in the project directory.

`user ALL=(root) NOPASSWD: /path/to/project/`

Obviously, this means trusting that whatever scripts run in this directory aren't malicious. I trust you can read through the code to satisfy any worries. Alternatively, you can selectively choose what scripts will run without prompting for your password.

## My Helper Scripts

I wrote these (very) small helper scripts mostly so I could easily adjust the backlight using keyboard shortcuts. The original helper script can be called by doing `./keyboard_brightness [up/down/on/off]`, which is great but doesn't really play well with keyboard shortcuts. So I wrote four wrapper scripts that call `keyboard_brightness` and pass the appropriate parameter.

Part of the way this driver works is writing to a file called `brightness` which requires `sudo`, hence using `NOPASSWD`.

## Configure Your Keyboard Shortcuts

After following the instructions below to make and install the drivers, you can simply assign my helper scripts to whatever key combo you want. For elementary OS, just navigate to `System Settings > Keyboard > Shortcuts > Custom`, and then add the path to the helper scripts and your key combo. The process should be similar for most Ubuntu-based distros.

If you want to be able to access the scripts without writing out the full path, you can add the project to your path variable. `cd` to your home directory and edit your `.bashrc` file by appending `export PATH=$PATH:/path/to/project/` to the end.


## Instructions

Below I've left the (mostly intact, minor spelling/grammar edits) original README for your perusal. The final step to make the modules load automatically at boot appeared to be unnecessary on my system - the backlight just works once the modules are loaded. But maybe I just haven't run into a situation where I'd need this yet. YMMV

---

# Original README

Since the newest Linux Kernel (3.12) ships with Chromebook Pixel support, both touchscreen and touchpad all work fine, but the kernel doesn't include the keyboard backlight driver. So I picked the driver from [https://github.com/brocktice/pixel_linux](https://github.com/brocktice/pixel_linux) and made it standalone and easy to use.

Thanks to [brocktice](http://blog.brocktice.com/2013/03/09/running-debian-wheezy-7-0-on-the-chromebook-pixel/)
[DaveM](http://vger.kernel.org/~davem/chromebook_pixel_linux.txt)

## Requirements
* Google Chromebook Pixel, HP Chromebook 13 (G1/Chell), Toshiba Chromebook 2 (2015) (Gandof) or Dell Chromebook 13 (Lulu) with Linux installed.
* Build toolchains.  

## Known Issue

If we register the keyboard backlight device as `backlight` to system, Gnome confuses the screen and keyboard backlight. In the system setting panel, display brightness adjust will operate on keyboard backlight.
So instead, we register it as `leds` devices to system to make Gnome happy.

## How To Install

First, clone the code.

If you want the keyboard backlight to work like `backlight` then edit `pixel_kb_backlight.c` 

	#ifdef KB_BACKLIGHT
	#define DEVICE_NAME "chromeos-keyboard-backlight"
	#else
	#define DEVICE_NAME "chromeos-keyboard-leds"
	#endif

Make these changes and the driver will act as `backlight` or `leds`.

Build with:

	make

You will get 3 drivers:
	chromeos_keyboard_bl.ko  	#backlight class driver
	chromeos_keyboard_leds.ko   #leds class driver
	chromebook_kb_backlight.ko		#keyboard backlight device driver
	
Now, load the drivers:

if you chose `backlight`

	sudo insmod chromeos_keyboard_bl.ko
	sudo insmod chromebook_kb_backlight.ko

if you chose `leds`

	sudo insmod chromeos_keyboard_leds.ko
	sudo insmod chromebook_kb_backlight.ko

Now, check whether the drivers loaded successfully:

if you chose `backlight`

	ls /sys/class/backlight
	
if you chose `leds`

	ls /sys/class/leds
	

If you can see `keyboard_backlight` or `chromeos::kbd_backlight` from what `ls` outputs, it worked!

### How to use

I have made a helper shell script to adjust the backlight level. **You need to edit the script if you chose `backlight`, to use the correct path**.

	# turn on the backlight
	sudo ./keyboard_brightness.sh on
	
	#turn off the backlight
	sudo ./keyboard_brightness.sh off
	
	#turn up 
	sudo ./keyboard_brightness.sh up
	
	#turn down
	sudo ./keyboard_brightness.sh down
	


## For `leds` as sample:

### Make it load automatically on boot

I installed Ubuntu 13.10 with Linux kernel 3.12.0-031200-generic, so I do the following:

	#copy our drivers to the kernel's modules dir
	sudo cp chromeos_keyboard_leds.ko /lib/modules/3.12.0-031200-generic/
	sudo cp chromebook_kb_backlight.ko /lib/modules/3.12.0-031200-generic/
	
	#tell system we have new modules
	sudo depmod -a
	sudo modprobe module
	
then edit `/etc/modules` and add two lines:

	chromeos_keyboard_leds
	chromebook_kb_backlight

Just reboot and the keyboard backlight driver will load automatically.

### Enjoy!
