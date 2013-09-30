AR := arm-none-linux-gnueabi-ar
CC := arm-none-linux-gnueabi-gcc

Q := @
ifeq ($(V),1)
  Q :=
endif
