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
S1=s21_matrix
S2=parser
S3=viewer
S4=tests
EXE2=$(S2).out
BD = ../build

.PHONY: all build open rebuild install uninstall clean dvi dist \
tests gcov_report cpplint leaks cppcheck

all: build

build: mkdir copy
	cd $(BD); qmake $(S3).pro
	make -C $(BD)

rebuild: clean build

copy: 
	cp -Rf $(S3).pro *.c *.h *.cpp *.fsh *.vsh *.ui *.qrc *.png *.icns *.jpeg \
	QtGifImage objects $(BD) 

install: build
	cp -rf $(BD)/$(S3).app $(HOME)/Desktop

uninstall:
	rm -r "$(HOME)/Desktop/$(S3).app"
	
dvi: 
	open documentation.html

dist: mkdir
	tar -cvzf $(BD)/$(S3).tgz Makefile *.c *.h $(S3).pro *.cpp *.ui *.qrc *.fsh *.vsh \
	*.png *.icns *.jpeg QtGifImage

tests: clean $(S1)
	./$(EXE2)

$(S1): $(S1).a $(S2).o
	$(CC) $(S1).a $(S2).o $(S4).c -o $(EXE2) $(LCHECK)

$(S1).a: $(S1).o
	ar r $(S1).a $(S1).o

$(S1).o:
	$(CC) $(LFLAGS) $(S1).c -o $(S1).o

$(S2).o:
	$(CC) $(LFLAGS) $(S2).c -o $(S2).o

gcov_report: tests
	$(CC) --coverage $(S1).c $(S2).c $(S4).c -o test $(LCHECK)
	./test
	lcov -t "test" -c -d . -o test.info
	genhtml test.info -o report
	open -a "Google Chrome" report/index.html

clean:
	rm -f *.a *.gcda *.gcno *.o *.info *.out *.gcov *.cfg ../.DS_Store \
	moc_*.cpp moc_*.h ui_*.h qrc_resource.cpp tests/*.out tests/*.o
	rm -rf report* *.dSYM ./test ./gcov_report $(BD) tests/*.dSYM

check: cppcheck leaks cpplint

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
