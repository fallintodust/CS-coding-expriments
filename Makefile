CC = gcc
CFLAGS = -Wall -O3 -march=native -fomit-frame-pointer
LIBS = -lm

SRCS = sort.c quick_sort_opt.c utils.c
OBJS = $(SRCS:.c=.o)

all: test_robustness

test_robustness: $(OBJS) test_robustness.c
	$(CC) $(CFLAGS) -o $@ $(OBJS) test_robustness.c $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o test_robustness

.PHONY: all clean

test: test_robustness
	./test_robustness
