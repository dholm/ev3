AR	:= arm-none-linux-gnueabi-ar
CC	:= arm-none-linux-gnueabi-gcc
CFLAGS	:= -Wall -Wextra -I$(HOME)/Projects/ev3sources/lms2012/lms2012/source
LDFLAGS	:=

Q := @
ifeq ($(V),1)
  Q :=
endif

LIBEV3		:= libev3.a
LIBEV3_SOURCES	:= pwm_device.c motor.c

TARGETS	:= $(LIBEV3) test

all: $(TARGETS)

test: test.c.o $(LIBEV3)
	$(Q)echo "Linking $@.."
	$(Q)$(CC) -o $@ $^ $(LDFLAGS)

$(LIBEV3): $(LIBEV3_SOURCES:.c=.c.o)
	$(Q)$(AR) -rs $@ $^

%.c.o: %.c
	$(Q)$(CC) -c -o $@ $< $(CFLAGS)

clean:
	$(Q)rm -f $(TARGETS)
	$(Q)rm -f *.o

.PHONY: all clean
