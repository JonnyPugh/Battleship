# Compiler and C++ standard for this project
CC = g++
STANDARD = -std=c++14

# Linking, compiling, and debugging flags
LFLAGS = $(STANDARD)
CFLAGS = $(STANDARD) -c
DFLAGS = -g -Wall -Werror -pedantic-errors

# Objects, executable, and test files
OBJS = agent.o player.o cpu.o field.o
BS = battleship
TESTDIR = tests
TESTINFILES = $(wildcard $(TESTDIR)/*_in.txt)
TESTOUTFILES = $(patsubst %in.txt,%out.txt,$(TESTINFILES))

# Phony targets
.PHONY: force test generate_correct_output run

# By default build with debugging flags
debug: LFLAGS += $(DFLAGS)
debug: release

# Build without debugging flags
release: $(BS)

# Run test cases
test: debug
	@for in_file in $(TESTINFILES); do \
		echo Running $${in_file%_in.txt}...; \
		./$(BS) < $$in_file > $${in_file%_in.txt}_out.txt; \
		diff $${in_file%_in.txt}_out.txt $${in_file%_in.txt}_out_correct.txt; \
	done

# Generate test case output
generate_correct_output: debug
	@for in_file in $(TESTINFILES); do \
		echo Generating correct output for $${in_file%_in.txt}...; \
		./$(BS) < $$in_file > $${in_file%_in.txt}_out_correct.txt; \
	done

# Run battleship with computers many times
run: debug
	@echo 0 > computer_in.txt
	@for n in $(shell seq 1 1000); do \
		./$(BS) < computer_in.txt > out.txt; \
	done
	@echo Finished running $(BS) many times.

# Remove Executable, objects, test files, and flags
clean:
	@rm -f $(BS) $(OBJS) $(TESTOUTFILES) computer_in.txt out.txt flags
	@clear

# To recompile when the compiler flags change, save off flags in a file
flags: force
	@echo '$(LFLAGS)' | cmp -s - $@ || echo '$(LFLAGS)' > $@

# Build the executable
$(BS): battleship.cpp $(OBJS) flags
	$(CC) battleship.cpp $(OBJS) $(LFLAGS) -o $(BS)

# Rebuild objects if the flags have changed
$(OBJS): flags

agent.o: agent.cpp
	$(CC) agent.cpp $(CFLAGS)

player.o: player.cpp
	$(CC) player.cpp $(CFLAGS)

cpu.o: cpu.cpp
	$(CC) cpu.cpp $(CFLAGS)

field.o: field.cpp
	$(CC) field.cpp $(CFLAGS)
