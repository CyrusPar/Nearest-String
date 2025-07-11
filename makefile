CC = gcc
CFLAGS = -o3
OPTIONS =

TARGET = a.out

TYPE ?= brute
TXT ?= 3_strings_4.txt
V ?= 0
ifeq ($(V), 1)
	OPTIONS = -v
endif

.PHONY: all run clean

all: $(TARGET)

${TARGET}: near_string_$(TYPE).c
	$(CC) $(CFLAGS) near_string_${TYPE}.c -o ${TARGET}

run: all
	@clear
	@echo "Running ${TARGET}..."
	@./${TARGET} input_files/$(TXT) ${OPTIONS}
	@echo "Execution completed."
	@echo "Cleaning up..."
	@rm -f ${TARGET}

run_all: all
	@clear
	@echo "Running batches..."
	@for i in 3 4 5 6; do \
		echo "Running tests for $$i strings..."; \
		for len in 4 8 12 16; do \
			echo "String length: $$len"; \
			for j in 1 2 3; do \
				./${TARGET} "input_files/$${i}_strings_$${len}.txt" ${OPTIONS}; \
			done; \
		done; \
	done
	@echo "Execution completed."
	@echo "Cleaning up..."
	@rm -f ${TARGET}

run_len: all
	@clear
	@echo "Running same string len..."
	@for i in 2 4 6 8 10; do \
		echo "Running tests for $$i strings"; \
		for j in 1 2 3; do \
			./${TARGET} "input_same_len/$${i}_strings_4.txt" ${OPTIONS}; \
		done; \
	done
	@echo "Execution completed."
	@echo "Cleaning up..."
	@rm -f ${TARGET}

run_num: all
	@clear
	@echo "Running same string num..."
	@for i in 2 4 6 8 10; do \
		echo "Running tests for $$i string length"; \
		for j in 1 2 3; do \
			./${TARGET} "input_same_num/4_strings_$${i}.txt" ${OPTIONS}; \
		done; \
	done
	@echo "Execution completed."
	@echo "Cleaning up..."
	@rm -f ${TARGET}