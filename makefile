학번 = 2024062806
BUILD_DIR := build

CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -fsanitize=leak,address,undefined -g

all: compile

init:
	echo "Initialization of build"
	if [ ! -d "$(BUILD_DIR)" ]; then
		echo "Create BUILD_DIR"
		mkdir $(BUILD_DIR)
	fi

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
		$(MAKE) compile labs=$(labs); \
		if [ "$(stdin)" = "false" ]; then \
			$(BUILD_DIR)/p$(labs).out $(args) > $(BUILD_DIR)/out.txt; \
		else \
			$(BUILD_DIR)/p$(labs).out $(args); \
		fi \
	else \
		$(MAKE) compile labs=$(labs) proc=$(proc) && \
		$(BUILD_DIR)/p$(labs)_$(proc).out $(args); \
	fi

diff:
	@if [ "$(labs)" = "" ] || [ "$(tc)" = "" ]; then \
		echo "Usage: make diff labs={lab_number} tc={testcase_number}"; \
	else \
		diff -u -s lab$(labs)/${tc}.out.txt $(BUILD_DIR)/out.txt; \
	fi