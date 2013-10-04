override TOP_DIR := $(CURDIR)/..

include $(TOP_DIR)/Makefile.inc

AR := ar
CC := gcc

CPPFLAGS :=
CFLAGS   := \
	-Wall -Wextra -Wno-sign-compare \
	-g3 \
	-I$(TOP_DIR)/include \
	$(shell pkg-config --cflags check)
LDFLAGS	 :=
LDLIBS	 := $(shell pkg-config --libs check)

libev3         := libev3.a
libev3_DIR     := $(TOP_DIR)/src
libev3_SOURCES := \
	$(libev3_DIR)/panic.c $(libev3_DIR)/allocator.c $(libev3_DIR)/array.c \
	$(libev3_DIR)/atomic_queue.c $(libev3_DIR)/event_dispatcher.c

test	     := test
test_SOURCES := test.c \
	check_allocator.c check_array.c check_atomic_queue.c \
	check_event_dispatcher.c

TARGETS := $(test) $(libev3)

all: test

$(libev3): $(libev3_SOURCES:.c=.c.o)
	$(Q)echo "Packaging $@.."
	$(Q)$(AR) rs $@ $(subst $(libev3_DIR),$(CURDIR),$^)

$(test): $(test_SOURCES:.c=.c.o) $(libev3)
	$(Q)echo "Linking $@.."
	$(Q)$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.c.o: %.c
	$(Q)echo "Compiling $(<).."
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) -o $(subst $(libev3_DIR),$(CURDIR),$@) -c $<

clean:
	$(Q)rm -f $(TARGETS)
	$(Q)rm -f *.o

.PHONY: all clean
