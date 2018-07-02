CFLAGS_SECRET  = $(shell pkg-config --cflags --libs libsecret-1)

CFLAGS = -g -Wall -Werror

all: keyring-crack 

keyring-crack: keyring_crack.c
	gcc $< -o $@ $(CFLAGS)  $(CFLAGS_SECRET)

clean:
	rm -f keyring-crack


.PHONY: all clean
