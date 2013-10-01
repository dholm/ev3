#ifndef UART_DEVICE_H
#define UART_DEVICE_H

#include "port.h"

typedef struct uart_device_s uart_device_t;

uart_device_t* uart_device_create(void);
void           uart_device_destroy(uart_device_t* uart_device);

void	uart_device_get_info(uart_device_t* uart_device, in_port_t port);
uint8_t uart_device_get_value(uart_device_t* uart_device, in_port_t port);

#endif /* UART_DEVICE_H */
