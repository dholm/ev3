#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#include <lms2012.h>

#include "panic.h"
#include "uart_device.h"

struct uart_device_s {
    int   fd;
    UART* uart;
};

uart_device_t* uart_device_create(void)
{
    uart_device_t* uart_device;
    int            prot        = PROT_READ | PROT_WRITE;
    int            flags       = MAP_FILE | MAP_SHARED;

    uart_device = calloc(1, sizeof(uart_device_t));
    if (unlikely(!uart_device)) {
        panic("Memory allocation failed!");
    }

    uart_device->fd = open(UART_DEVICE_NAME, O_RDWR | O_SYNC);
    if (unlikely(uart_device->fd < 0)) {
        panic("Unable to open %s!", UART_DEVICE_NAME);
    }

    uart_device->uart = (UART*) mmap(0, sizeof(UART), prot, flags, uart_device->fd, 0);
    if (unlikely(uart_device->uart == MAP_FAILED)) {
        panic("Unable to map UART device!");
    }

    return uart_device;
}

void uart_device_destroy(uart_device_t* uart_device)
{
    if (uart_device) {
        munmap(uart_device->uart, sizeof(UART));
        close(uart_device->fd);
        memset(uart_device, 0UL, sizeof(uart_device_t));
        free(uart_device);
    }
}

static void uart_device_refresh_port_info(uart_device_t* uart_device, in_port_t port)
{
    UARTCTL uart_ctl = {
        .Port = port,
        .Mode = 0,
    };

    (void) ioctl(uart_device->fd, UART_READ_MODE_INFO, &uart_ctl);
    if (!uart_ctl.TypeData.Name[0]) {
        return;
    }

    if (uart_ctl.TypeData.Type < 0) {
        panic("Invalid type %d!", uart_ctl.TypeData.Type);
    }
    if (uart_ctl.TypeData.Mode < 0 || MAX_DEVICE_MODES <= uart_ctl.TypeData.Mode) {
        panic("Invalid mode %d for device type %d!", uart_ctl.TypeData.Mode, uart_ctl.TypeData.Type);
    }

    printf("port: %d type: %-3d mode: %d name: %s\n", (uint8_t) port, uart_ctl.TypeData.Type,
           uart_ctl.TypeData.Mode, uart_ctl.TypeData.Name);
}

void uart_device_get_info(uart_device_t* uart_device, in_port_t port)
{
    if (uart_device->uart->Status[(size_t) port] & UART_PORT_CHANGED) {
        uart_device_refresh_port_info(uart_device, port);
    }
}

uint8_t uart_device_get_value(uart_device_t* uart_device, in_port_t port)
{
    size_t log_idx = uart_device->uart->Actual[(size_t) port];
    return (uint8_t) uart_device->uart->Raw[(size_t) port][log_idx][0];
}
