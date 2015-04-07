/*
 * led-control.c
 *
 * Copyright (C) Savoir-faire Linux SA 2015
 * Author: Emeric Vigier <emeric.vigier@savoirfairelinux.com> for Savoir-faire Linux.
 * License terms:  GNU General Public License (GPL), version 2
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define GPIO_DIR     "/sys/class/gpio/"
#define GPIO_EXPORT  "/sys/class/gpio/export"
#define GPIO_REDLED 47

int exportGpio(const char *gpio) {
    int fd, n, ret = -1;
    size_t count;

    printf("opening %s...\n", GPIO_EXPORT);
    fd = open(GPIO_EXPORT, O_WRONLY);
    if (fd < 0) {
        perror("open export");
        goto end;
    }

    printf("writing %s to %s...\n", gpio, GPIO_EXPORT);
    count = strlen(gpio);
    n = write(fd, gpio, count);
    if (n == -1) {
        perror("write to export");
        goto close;
    } else if (n != count) {
        fprintf(stderr, "error: partial write to export\n");
        goto close;
    }

    ret = 0;
close:
    if(close(fd)) {
        perror("close");
    }
end:
    return ret;
}

int setGpio(int gpio, int value) {
    int ret = -1, fd_val, fd_dir, n;
    size_t count;
    struct stat sb;
    char gpiopath[100], gpiovalue[100], gpiodirection[100];
    char buf[16];
    char gpiostr[10];

    sprintf(gpiostr, "%u", gpio);

    printf("is gpio already exported? ");
    memset(gpiopath, 0, sizeof(gpiopath));
    strcat(gpiopath, GPIO_DIR);
    strcat(gpiopath, "gpio");
    strcat(gpiopath, gpiostr);
    ret = stat(gpiopath, &sb);
    if (ret == -1) {
        printf("no\n");
        ret = exportGpio(gpiostr);
        if (ret) {
            fprintf(stderr, "error: cannot export gpio%s\n", gpiostr);
            goto end;
        }
        ret = stat(gpiopath, &sb);
        if (ret == -1) {
            perror("export failed");
            goto end;
        }
    } else {
        printf("yes\n");
    }

    printf("is gpio direction set to output? ");
    memset(gpiodirection, 0, sizeof(gpiodirection));
    memcpy(gpiodirection, gpiopath, strlen(gpiopath));
    strcat(gpiodirection, "/direction");

    fd_dir = open(gpiodirection, O_RDWR);
    if (fd_dir < 0) {
        perror(gpiodirection);
        ret = -1;
        goto end;
    }

    ret = read(fd_dir, buf, sizeof(buf));
    if (n == -1) {
        perror("read");
        ret = -1;
        goto close_dir;
    }

    if (!strstr(buf, "out") && !strstr(buf, "high") && !strstr(buf, "low")) {
        printf("no\n");
        printf("writing 'out' to %s...\n", gpiodirection);
        count = 3;
        n = write(fd_dir, "out", count);
        if (n == -1) {
            perror("write");
            goto close_dir;
        } else if (n != count) {
            fprintf(stderr, "error: partial write\n");
            goto close_dir;
        }
    } else {
        printf("yes\n");
    }

    printf("setting gpio to %d...\n", value);
    memcpy(gpiovalue, gpiopath, sizeof(gpiopath));
    strcat(gpiovalue, "/value");
    fd_val = open(gpiovalue, O_WRONLY);
    if (fd_val < 0) {
        perror(gpiovalue);
        ret = -1;
        goto close_dir;
    }

    sprintf(buf, "%d", value);
    count = strlen(buf);
    n = write(fd_val, buf, count);
    if (n == -1) {
        perror("write");
        ret = -1;
        goto close_value;
    } else if (n != count) {
        fprintf(stderr, "error: partial write\n");
        ret = -1;
        goto close_value;
    }

    ret = 0;
close_value:
    close(fd_val);
close_dir:
    close(fd_dir);
end:
    return ret;
}

inline int enableGpio(unsigned int gpio) {
    return setGpio(gpio, 1);
}

inline int disableGpio(unsigned int gpio) {
    return setGpio(gpio, 0);
}

int main(int argc, char **argv) {
    int ret = 0;

	printf("led-control\n");

	while (!ret) {
	    ret = enableGpio(GPIO_REDLED);
	    sleep(1);
	    ret |= disableGpio(GPIO_REDLED);
	    sleep(1);
	}

end:
	return ret;
}
