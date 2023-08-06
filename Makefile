CC = gcc
CFLAGS := -g
Link_flags := -lgbm -lEGL -lGLESv2 -lm
Inc_flags := -I/usr/include/GLES2
all:
	$(CC) $(CFLAGS) test.c gles2_helper.c $(Link_flags) $(Inc_flags)
clean:
	rm a.out