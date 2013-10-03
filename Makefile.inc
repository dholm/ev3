AR := arm-none-linux-gnueabi-ar
CC := arm-none-linux-gnueabi-gcc

CPPFLAGS :=
CFLAGS   := \
         -O2 -g \
         -march=armv5te -mtune=arm9tdmi -msoft-float \
         -Wall -Wextra \
         -I$(TOP_DIR)/lms2012 -I$(TOP_DIR)/include

Q := @
ifeq ($(V),1)
  Q :=
endif
