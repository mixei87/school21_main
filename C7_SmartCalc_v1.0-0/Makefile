OS:= $(shell uname -s)
DEBUG=-g
WFLAGS=-Wall -Wextra -Werror
STD=-std=c11
COV=--coverage
LFLAGS=-c
CC=gcc $(STD) $(WFLAGS)
ifeq ($(OS), Darwin)
LCHECK=-lcheck
else
LCHECK=-lcheck -lpthread -lrt -lsubunit -lm
endif
D2=tests
S1=reverse_polish_notation
S2=$(D2)/test_calc
S3=qt_calc
EXE2=$(S2).out
BD = ../build

.PHONY: all build open rebuild install uninstall clean dvi dist \
tests gcov_report cpplint leaks cppcheck

all: build open

build: mkdir copy
	cd $(BD); qmake $(S3).pro
	make -C $(BD)

open:
	@if [[ -d "$(HOME)/.brew/bin" && -d "$(HOME)/.brew/bin/$(S3).app" ]]; then\
		open $(HOME)/.brew/bin/$(S3).app;\
	else\
		open $(BD)/$(S3).app;\
	fi	

rebuild: clean build

copy: 
	cp -rf $(S3).pro *.c *.h *.cpp *.ui *.qrc resources $(BD)

install: build
	@if [ -d "$(HOME)/.brew/bin" ]; then\
		cp -rf $(BD)/$(S3).app $(HOME)/.brew/bin;\
	fi

uninstall:
	@if [ -d "$(HOME)/.brew/bin" ]; then\
		rm -r "$(HOME)/.brew/bin/$(S3).app";\
	else\
		rm -r "$(BD)/$(S3).app";\
	fi
dvi: 
	open -a "Google Chrome" description.html

dist: mkdir
	tar -cvzf $(BD)/$(S3).tgz Makefile *.c *.h *.pro *.user *.html *.cpp *.ui resources *.qrc

tests: clean $(S1)
	./$(EXE2)

$(S1): $(S1).a $(S2).o
	$(CC) $(S1).a $(S2).o -o $(EXE2) $(LCHECK)

$(S1).a: $(S1).o
	ar r $(S1).a $(S1).o

$(S1).o:
	$(CC) $(LFLAGS) $(S1).c -o $(S1).o

$(S2).o:
	$(CC) $(LFLAGS) $(S2).c -o $(S2).o

gcov_report: tests
	$(CC) --coverage $(S1).c $(S2).c -o test $(LCHECK)
	./test
	lcov -t "test" -c -d . -o test.info
	genhtml test.info -o report
	open -a "Google Chrome" report/index.html

clean:
	rm -f *.a *.gcda *.gcno *.o *.info *.out *.gcov *.cfg ../.DS_Store \
	moc_*.cpp moc_*.h ui_*.h qrc_resource.cpp tests/*.out tests/*.o
	rm -rf report* *.dSYM ./test ./gcov_report $(BD) tests/*.dSYM

cppcheck: clean
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h *.cpp 

leaks: clean $(S1)
	CK_FORK=no leaks -atExit -- ./$(EXE2)

cpplint:
	cp ../materials/linters/CPPLINT.cfg .
	python3 ../materials/linters/cpplint.py --extensions=c *.c
	python3 ../materials/linters/cpplint.py --extensions=h *.h
	python3 ../materials/linters/cpplint.py --extensions=cpp *.cpp
	rm -f CPPLINT.cfg

mkdir:
	mkdir -p $(BD)
