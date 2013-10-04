override TOP_DIR := $(CURDIR)/..

include $(TOP_DIR)/Makefile.inc

CPPFLAGS += -DPIC
CFLAGS	 += -Wno-sign-compare -fPIC
LDLIBS	 :=
LDFLAGS	 :=

libev3	       := libev3.so
libev3_SOURCES := \
	panic.c allocator.c array.c atomic_queue.c event_dispatcher.c \
	pwm_device.c uart_device.c motor.c

TARGETS := $(libev3)

all: $(TARGETS)

$(libev3): $(libev3_SOURCES:.c=.c.o)
	$(Q)$(CC) -shared -o $@ $^

%.c.o: %.c
	$(Q)echo "Compiling $(<).."
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

clean:
	$(Q)rm -f $(TARGETS)
	$(Q)rm -f *.o

.PHONY: all clean
