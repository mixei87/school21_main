ifneq ($(OS),Windows_NT)
OS:= $(shell uname -s)
endif
ifeq ($(OS), Darwin)
LEAKS:=leaks -atExit --
else
LEAKS:=valgrind --leak-check=full --show-leak-kinds=all
endif

CC=gcc# $(DEBUG)
WFLAGS=#-Wall -Wextra -Werror
DEBUG=#-g
LFLAGS=-c

F1=test_result
F2=test_diff
F3=misc
F4=example_test

R1=s21_cat
R2=test.sh
R3=create_flags
R4=create_ascii

S3=$(F3)/$(R3)
S4=$(F3)/$(R4)

T1=$(F4)/test1.txt
T2=$(F4)/test2.txt
T3=$(F4)/test3.txt
T4=$(F4)/test4.txt
T5=$(F4)/test5.txt

ERROR=0

.PHONY: all $(R1) test leaks cpplint cppcheck clean rebuild git

all: $(R1)

test: test1 test2 test3 test4 test5

test1: clean $(R1)
	bash $(R2) $(T1)
test2: clean $(R1)
	bash $(R2) $(T2)
test3: clean $(R1)
	bash $(R2) $(T3)
test4: clean $(R1)
	bash $(R2) $(T1) $(T4)
test5: clean $(R1)
	bash $(R2) $(T5)
  
$(R1): $(R1).o
	$(CC) $^ -o $@
$(R3): $(S3).o
	$(CC) $^ -o $@
$(R4): $(S4).o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(LFLAGS) $(WFLAGS) $< -o $@

create_flag: clean $(R3)
	./$(R3)
create_asc: clean $(R4)
	./$(R4)

clean:
	rm -f *.a *.gcda *.gcno *.o *.info .DS_Store ../../.DS_Store \
	$(F1)/* $(F2)/*	*.out *.exe $(R1) $(R3) $(R4)
	rm -f -r *.dSYM

rebuild: clean all

cpplint:
	cp ../../materials/linters/CPPLINT.cfg .
	python3 ../../materials/linters/cpplint.py --extensions=c *.c
	python3 ../../materials/linters/cpplint.py --extensions=h *.h
	rm -f CPPLINT.cfg

leaks: clean $(R1)
	$(LEAKS) ./$(R1) -b -n -e -s -t $(T1)

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h

git: clean
	git add .
	git status
	git commit -m'$(m)'
	git push origin develop

m:=FIRE! FIRE! FIRE!
