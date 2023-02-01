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
DEBUG=-g
LFLAGS=-c#-D_GNU_SOURCE

F1=test_result
F2=test_diff
F3=misc
F4=example_test

R1=s21_grep
R2=test.sh
R3=create_flags

S3=$(F3)/$(R3)

T1=$(F4)/test1.txt
T2=$(F4)/test2.txt
T3=$(F4)/test3.txt
T4=$(F4)/test4.txt
T5=$(F4)/test5.txt
T6=$(F4)/test6.txt

.PHONY: all $(R1) test leaks cpplint cppcheck clean rebuild git

all: clean $(R1)

test: test0 test1 test2 test3 test4 test5 test6 test7 test8 test9 \
	test11 test12 test13 test14 test16 test17 test18
	rm -rf $(F1)/* $(F2)/*
E0=he
E1=-e and
E2=-e apple
E3=-e Pineapple
E4=-e ou
E5=-e 1
E6=-e y
E7=",the"
E8=-e "[a-g]owe"
E9=-e "[[:punct:]]s"
E11=-e "[^e]en"
E12=-f $(F4)/file1.txt
E13=-f $(F4)/file2.txt
E14=-f $(F4)/file3.txt
E15=-f $(F4)/file4.txt
E16=-e abc
E17=-e tr

test0: clean $(R1)
	bash $(R2) $(E0) $(T5)
test1: clean $(R1)
	bash $(R2) $(E1) $(T1)
test2: clean $(R1)
	bash $(R2) $(E2) $(T2)
test3: clean $(R1)
	bash $(R2) $(E3) $(T3)
test4: clean $(R1)
	bash $(R2) $(E4) $(T1) $(T2)
test5: clean $(R1)
	bash $(R2) $(E5) $(T1) $(T2) $(T3)
test6: clean $(R1)
	bash $(R2) $(E6) $(T4)
test7: clean $(R1)
	bash $(R2) $(E7) $(T2)
test8: clean $(R1)
	bash $(R2) $(E8) $(T2)
test9: clean $(R1)
	bash $(R2) $(E9) $(T2)	
test11: clean $(R1)
	bash $(R2) $(E11) $(T2)
test12: clean $(R1)
	bash $(R2) $(E17) $(T1) $(T2)
test13: clean $(R1)
	bash $(R2) $(E13) $(T2)
test14: clean $(R1)
	bash $(R2) $(E14) $(T2)
test16: clean $(R1)
	bash $(R2) $(E1) $(E5) $(T2)
test17: clean $(R1)
	bash $(R2) $(E2) $(E13) $(T2)
test18: clean $(R1)
	bash $(R2) $(E16) $(T1) $(T6)

$(R1): $(R1).o
	$(CC) $^ -o $@
$(R3): $(S3).o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(LFLAGS) $(WFLAGS) $< -o $@

create_flag: clean $(R3)
	./$(R3)

clean:
	rm -f *.a *.gcda *.gcno *.o *.info .DS_Store ../../.DS_Store \
	*.out *.exe $(R1) $(R3) misc/*.o $(F1)/* $(F2)/*
	rm -f -r *.dSYM

rebuild: clean all

cpplint:
	cp ../../materials/linters/CPPLINT.cfg .
	python3 ../../materials/linters/cpplint.py --extensions=c *.c
	python3 ../../materials/linters/cpplint.py --extensions=h *.h
	rm -f CPPLINT.cfg

leaks: clean $(R1)
	$(LEAKS) ./$(R1) -e "[a-g]owe" -ivclnhso $(T1) $(E12)
	$(LEAKS) ./$(R1) $(E0) $(T5) -ivo
	$(LEAKS) ./$(R1) $(E1) $(T1) -nhso
	$(LEAKS) ./$(R1) $(E2) $(T2) -o
	$(LEAKS) ./$(R1) $(E3) $(T3) -h
	$(LEAKS) ./$(R1) $(E4) $(T1) -s $(T2)
	$(LEAKS) ./$(R1) $(E5) $(T1) -l $(T2) $(T3)
	$(LEAKS) ./$(R1) $(E6) $(T4) -n
	$(LEAKS) ./$(R1) $(E7) $(T2) -o
	$(LEAKS) ./$(R1) $(E8) $(T2) -v
	$(LEAKS) ./$(R1) $(E9) $(T2) -i
	$(LEAKS) ./$(R1) $(E0) $(T1) $(E12) -il
	$(LEAKS) ./$(R1) $(E11) $(T2) -ho
	$(LEAKS) ./$(R1) $(E13) $(T2) -cl
	$(LEAKS) ./$(R1) $(E14) $(T2) -nc
	$(LEAKS) ./$(R1) $(E1) $(E5) $(T2) -ni
	$(LEAKS) ./$(R1) $(E2) $(E13) $(T2) -so
	$(LEAKS) ./$(R1) $(E16) $(T1) $(T6) -ivclnhso

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h

git: clean
	git add .
	git status
	git commit -m'$(m)'
	git push origin develop

m:=FIRE! FIRE! FIRE!
