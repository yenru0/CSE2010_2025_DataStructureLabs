학번 = 2024062806
BUILD_DIR := build

CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -fsanitize=address -g

all:
	echo "Hello, World!"

compile:
	@if [ "$(labs)" = "" ]; then \
		echo "Usage: make compile labs={lab_number} (proc={proc_number})"; \
	elif [ "$(proc)" = "" ]; then \
		$(CC) $(CFLAGS) lab$(labs)/$(학번)_p$(labs).c -o $(BUILD_DIR)/p$(labs).out; \
	else \
		$(CC) $(CFLAGS) lab$(labs)/$(학번)_p$(labs)_$(proc).c -o $(BUILD_DIR)/p$(labs)_$(proc).out; \
	fi

run:
	@if [ "$(labs)" = "" ]; then \
		echo "Usage: make run labs={lab_number} (proc={problem_number})"; \
	elif [ "$(proc)" = "" ]; then \
		$(MAKE) compile labs=$(labs) && \
		$(BUILD_DIR)/p$(labs).out $(args); \
	else \
		$(MAKE) compile labs=$(labs) proc=$(proc) && \
		$(BUILD_DIR)/p$(labs)_$(proc).out $(args); \
	fi
