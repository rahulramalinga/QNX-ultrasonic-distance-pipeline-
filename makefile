
#	Makefile 

DEBUG = -g
CC = qcc
LD = qcc


TARGET = -Vgcc_ntoaarch64le

CFLAGS += $(DEBUG) $(TARGET) -Wall
LDFLAGS+= $(DEBUG) $(TARGET)


BINS = ultrasonic

SRCS = \
	src/ultrasonic/main.c \
	src/ultrasonic/display.c \
	src/ultrasonic/gpio.c \
	src/ultrasonic/hc_sr04.c \
	src/ultrasonic/processing.c \
	src/ultrasonic/sensor.c

OBJS = $(SRCS:.c=.o)

all: $(BINS)

$(BINS): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

clean:
	rm -f $(OBJS) $(BINS)
