#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <lms2012.h>

#include "macros.h"
#include "panic.h"
#include "pwm_device.h"

struct pwm_device_s {
    int fd;
};

pwm_device_t* pwm_device_create(void)
{
    pwm_device_t* pwm_device = NULL;

    pwm_device = calloc(1, sizeof(pwm_device_t));
    if (unlikely(!pwm_device)) {
        panic("Memory allocation failed!");
    }

    pwm_device->fd = open(PWM_DEVICE_NAME, O_RDWR | O_SYNC);
    if (unlikely(pwm_device->fd == -1)) {
        panic("Unable to open %s!", PWM_DEVICE_NAME);
    }

    return pwm_device;
}

void pwm_device_destroy(pwm_device_t* pwm_device)
{
    if (pwm_device) {
        close(pwm_device->fd);
        memset(pwm_device, 0UL, sizeof(*pwm_device));
        free(pwm_device);
    }
}

int pwm_device_send(pwm_device_t* pwm_device, const uint8_t* command, size_t len)
{
    return write(pwm_device->fd, command, len);
}

void* pwm_device_mmap(pwm_device_t* pwm_device, size_t len, off_t offset)
{
    int   prot  = PROT_READ | PROT_WRITE;
    int   flags = MAP_FILE | MAP_SHARED;
    void* ptr;

    ptr = mmap(0, len, prot, flags, pwm_device->fd, offset);
    if (ptr == MAP_FAILED) {
        return NULL;
    }
    return ptr;
}

void pwm_device_munmap(UNUSED(pwm_device_t* pwm_device), void* ptr, size_t len)
{
    (void) munmap(ptr, len);
}
