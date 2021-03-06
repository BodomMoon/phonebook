CC ?= gcc
CFLAGS_common ?= -Wall -std=gnu99
CFLAGS_orig = -O2
CFLAGS_opt  = -O2
CFLAGS_bst  = -O2
CFLAGS_huf  = -O2

EXEC = phonebook_orig phonebook_opt phonebook_bst phonebook_ent phonebook_huf

GIT_HOOKS := .git/hooks/applied
.PHONY: all
all: $(GIT_HOOKS) $(EXEC)

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

SRCS_common = main.c

phonebook_orig: $(SRCS_common) phonebook_orig.c phonebook_orig.h
	$(CC) $(CFLAGS_common) $(CFLAGS_orig) \
		-DIMPL="\"$@.h\"" -o $@ \
		$(SRCS_common) $@.c
phonebook_opt: $(SRCS_common) phonebook_opt.c phonebook_opt.h
	$(CC) $(CFLAGS_common) $(CFLAGS_opt) \
		-DIMPL="\"$@.h\"" -o $@ \
		$(SRCS_common) $@.c
phonebook_bst: $(SRCS_common) phonebook_bst.c phonebook_bst.h
	$(CC) $(CFLAGS_common) $(CFLAGS_bst) \
		-g -DIMPL="\"$@.h\"" -o $@ -g \
		$(SRCS_common) -g $@.c
phonebook_ent: $(SRCS_common) phonebook_opt.c phonebook_opt.h
	$(CC) $(CFLAGS_common) $(CFLAGS_bst) \
		-DIMPL="\"phonebook_opt.h\"" -o $@ \
		-D ENT=1 $(SRCS_common) phonebook_opt.c

phonebook_huf: $(SRCS_common) phonebook_huf.c phonebook_huf.h
	$(CC) $(CFLAGS_common) $(CFLAGS_huf) \
		-DIMPL="\"$@.h\"" -o $@ -g \
		$(SRCS_common) -g $@.c



run: $(EXEC)
	echo 3 | sudo tee /proc/sys/vm/drop_caches
	watch -d -t "./phonebook_orig && echo 3 | sudo tee /proc/sys/vm/drop_caches"
run2: $(EXEC)
	echo 3 | sudo tee /proc/sys/vm/drop_caches
	watch -d -t "./phonebook_opt && echo 3 | sudo tee /proc/sys/vm/drop_caches"
run3: $(EXEC)
	echo 3 | sudo tee /proc/sys/vm/drop_caches
	watch -d -t "./phonebook_bst && echo 3 | sudo tee /proc/sys/vm/drop_caches"
run4: $(EXEC)
	echo 3 | sudo tee /proc/sys/vm/drop_caches
	watch -d -t "./phonebook_huf && echo 3 | sudo tee /proc/sys/vm/drop_caches"



cache-test: $(EXEC)
	perf stat --repeat 100 \
		-e cache-misses,cache-references,instructions,cycles \
		./phonebook_orig
	perf stat --repeat 100 \
		-e cache-misses,cache-references,instructions,cycles \
		./phonebook_opt
	perf stat --repeat 10 \
		-e cache-misses,cache-references,instructions,cycles \
		./phonebook_bst
	perf stat --repeat 20 \
		-e cache-misses,cache-references,instructions,cycles \
		./phonebook_huf
			
cache-test2: $(EXEC)
	perf stat --repeat 10 \
		-e cache-misses,cache-references,instructions,cycles \
		./phonebook_opt

cache-test3: $(EXEC)
	perf stat --repeat 10 \
		-e cache-misses,cache-references,instructions,cycles \
		./phonebook_bst
cache-test4: $(EXEC)
	perf stat --repeat 20 \
		-e cache-misses,cache-references,instructions,cycles \
		./phonebook_huf

output.txt: cache-test calculate
	./calculate

plot: output.txt
	gnuplot scripts/runtime.gp

calculate: calculate.c
	$(CC) $(CFLAGS_common) $^ -o $@

.PHONY: clean
clean:
	$(RM) $(EXEC) *.o perf.* \
	      	calculate orig.txt opt.txt output.txt runtime.png
