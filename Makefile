CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -lm

TARGETS=image_editor

build: $(TARGETS)

image_editor: image_check.c helper.c
	$(CC) $(CFLAGS) image_check.c -lm -o image_editor
pack:
	zip	-FSr 312CA_ChirilaTiberiuAndrei_Tema3.zip README Makefile *.c

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
