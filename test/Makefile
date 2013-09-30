override TOP_DIR := $(CURDIR)/..

include $(TOP_DIR)/Makefile.inc

CPPFLAGS := -DPIC
CFLAGS	 := -Wall -Wextra -I$(TOP_DIR)/lms2012 -I$(TOP_DIR)/include
LDLIBS	 := -lev3
LDFLAGS	 := -L$(TOP_DIR)/src

test	     := test
test_SOURCES := test.c

TARGETS := $(test)

all: test

$(test): $(test_SOURCES:.c=.c.o)
	$(Q)echo "Linking $@.."
	$(Q)$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.c.o: %.c
	$(Q)echo "Compiling $(<).."
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

clean:
	$(Q)rm -f $(TARGETS)
	$(Q)rm -f *.o

.PHONY: all clean
