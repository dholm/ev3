CC	:= arm-none-linux-gnueabi-gcc
CFLAGS	:= -I$(HOME)/Projects/ev3sources/lms2012/lms2012/source
LDFLAGS	:=

TARGETS	:= motor

all: $(TARGETS)

motor: motor.c.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.c.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	@rm -f $(TARGETS)
	@rm -f *.o
