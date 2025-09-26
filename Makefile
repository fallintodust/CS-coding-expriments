CC = gcc
CFLAGS = -Wall -O3 -march=native -fomit-frame-pointer
LIBS = -lm

SRCS = sorting/sort.c sorting/quick_sort_opt.c sorting/utils.c
OBJS = $(SRCS:.c=.o)

all: test_robustness

test_robustness: $(OBJS) tests/test_robustness.c
	$(CC) $(CFLAGS) -o $@ $(OBJS) tests/test_robustness.c $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o test_robustness

.PHONY: all clean

test: test_robustness
	./test_robustness
