# include this script in your Makefile with "include helpers.mk"

# File locations
BASENAME?=main # set BASENAME only if it was not set before
EXE_LOC?=./$(BASENAME) # set EXE_LOC only if it was not set before

TEST_DIR=./test

# The number of lines to show (for 'run' and 'test')
N_LINES=30

# run gdb
gdb: all
	gdb $(EXE_LOC)

# run valgrind on all .in files
valgrind: all
	for input in $(TEST_DIR)/*.in ; do \
		valgrind $(EXE_LOC) < $$input ; \
	done

# run the program on all .in files
run: all
	for input in $(TEST_DIR)/*.in ; do \
		echo "Running on $$input"; \
		myout=$${input%.in}.myout; \
		$(EXE_LOC) < $$input > $$myout ; \
		echo "Showing the first $(N_LINES) lines. Run 'cat $$myout' to see the full output."; \
		head -n $(N_LINES) $$myout; \
	done

# run the program on all .in files, comparing it with the expected output
test: all
	for input in $(TEST_DIR)/*.in ; do \
		echo "Running on $$input"; \
		referene=$${input%.in}.out; \
		myout=$${input%.in}.myout; \
		differences=$${input%.in}.diff; \
		$(EXE_LOC) < $$input > $$myout ; \
		tmp_ref=$$referene.tmp; \
		tmp_out=$$myout.tmp; \
		nl $$referene > $$tmp_ref; \
		nl $$myout > $$tmp_out; \
		diff --side-by-side --ignore-trailing-space $$tmp_ref $$tmp_out >$$differences; \
		if [ $$? -eq 0 ]; \
		then \
			echo "Output correct for file $$input"; \
		else \
			echo "Showing the first $(N_LINES) lines of the difference for file $$input. Run 'cat $$differences' to see the full report."; \
			head -n $(N_LINES) $$differences; \
		fi; \
		rm $$tmp_ref; \
		rm $$tmp_out; \
	done

