#ifndef PWM_DEVICE_H
#define PWM_DEVICE_H

#include <stddef.h>
#include <sys/types.h>
#include <inttypes.h>

typedef struct pwm_device_s pwm_device_t;

pwm_device_t* pwm_device_create(void);
void          pwm_device_destroy(pwm_device_t* pwm_device);

int pwm_device_send(pwm_device_t* pwm_device, const uint8_t* command, size_t len);

void* pwm_device_mmap(pwm_device_t* pwm_device, size_t len, off_t offset);
void pwm_device_munmap(pwm_device_t* pwm_device, void* ptr, size_t len);

#endif /* PWM_DEVICE_H */
