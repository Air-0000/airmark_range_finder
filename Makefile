# Makefile for AirMark Range Finder
# Targets:
#   make test     - Compile and run algorithm unit tests on host PC (Linux/macOS)
#   make clean    - Clean build artifacts

CC ?= gcc
CFLAGS = -Wall -Wextra -O2 -DUSE_HOST_TEST -I Core/Inc -I User_Code/Inc
LDFLAGS = -lm

SRC = User_Code/Src/triangulation.c \
      User_Code/Src/subpixel_center.c \
      User_Code/Src/mark_detection.c \
      User_Code/Src/calibration.c

TEST_SRC = tests/test_ranging.c $(SRC)
TEST_BIN = build/test_ranging

.PHONY: all test clean

all: test

$(TEST_BIN): $(TEST_SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test: $(TEST_BIN)
	@echo "Running AirMark ranging algorithm tests..."
	@./$(TEST_BIN)

clean:
	rm -rf build *.o
