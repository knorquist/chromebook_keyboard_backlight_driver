#include <stdlib.h>
#include <stdio.h>

#define BRIGHTNESS "/sys/class/leds/chromeos::kbd_backlight/brightness"
#define INCREMENT 10
#define MAX 100
#define MIN 0

int main(int argc, char **argv) {
	FILE *f;
	int new;
	char buf[32];

	if(argc < 2) {
		exit(-1);
	}


	if(!(f=fopen(BRIGHTNESS,"rt"))) {
		fprintf(stderr,"Unable to open brightness file (read): %s\n", BRIGHTNESS);
		exit(1);
	}
	fgets(buf,30,f);
	fclose(f);
	new = atoi(buf);

	if(strcmp(argv[1],"up") == 0) {
		new+=INCREMENT;
	}

	if(strcmp(argv[1],"down") == 0) {
		new-=INCREMENT;
	}

	if(strcmp(argv[1],"on") == 0) {
		new=100;
	}

	if(strcmp(argv[1],"off") == 0) {
		new=0;
	}

	if(new < 0) {
		new = 0;
	}

	if(new > 100) {
		new = 100;
	}

	if(!(f=fopen(BRIGHTNESS,"wt"))) {
		fprintf(stderr,"Unable to open brightness file (write): %s\n", BRIGHTNESS);
		exit(1);
	}
	fprintf(f,"%d\n",new);
	fclose(f);

	exit(0);
}
