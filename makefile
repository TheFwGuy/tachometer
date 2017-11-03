CC=msp430-gcc
CFLAGS=-mmcu=msp430x169 -O0 -Wall -g

OBJS=main.o system.o lcd_new.o set.o

all: $(OBJS)
	$(CC) $(CFLAGS) -o rpm.elf $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -fr rpm.elf $(OBJS)

