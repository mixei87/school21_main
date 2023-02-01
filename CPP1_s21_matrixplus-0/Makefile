OS=$(shell uname -s)
git_branch=$(shell git branch --show-current 2>/dev/null)
CC=g++
CFLAGS=-c -std=c++17 -Wall -Wextra -Werror 
LDFLAGS=--coverage
ifeq ($(OS), Darwin)
	LIBTEST=-lgtest
else
	LIBTEST=-lgtest -lgtest_main -lstdc++ -L. -lm -lrt -lpthread
endif
SOURCES=s21_additional_functions.cc s21_arithmetic_functions.cc \
		s21_constructors.cc s21_for_test.cc s21_operators.cc \
		s21_setters_getters.cc
TEST=test
OBJECTS=$(SOURCES:.cc=.o)
LIB=s21_matrix_oop
EXE=s21_matrix.out
R=report

.PHONY: all $(LIB).a git clean cpplint leaks test rebuild install_gtest \
install_valgrind

all: test

$(LIB).a: $(SOURCES) $(LIB)
	
$(LIB): $(OBJECTS) 
	ar r $@.a $^ 

.cc.o:
	$(CC) $(CFLAGS) $< -o $@

test: $(TEST).o $(LIB).a   
	$(CC) -o $(EXE) $^ $(LIBTEST) 
	./$(EXE)

$(TEST).o:
	$(CC) $(CFLAGS) $(TEST).cc -o $@

gcov_report: clean
	$(CC) $(LDFLAGS) $(SOURCES) $(TEST).cc -o test.out $(LIBTEST)
	./test.out
	lcov -t "test" --no-external -o $(R).info -c -d .
	genhtml -o $(R) $(R).info	
ifeq ($(OS), Darwin)
	open -a "Google Chrome" $(R)/index.html
else
	google-chrome $(R)/index.html
endif	

cpplint:
	cp ../materials/linters/CPPLINT.cfg .
	python3 ../materials/linters/cpplint.py --extensions=cc *.cc
	python3 ../materials/linters/cpplint.py --extensions=h *.h
	rm -f CPPLINT.cfg

clean:
	rm -rf *.dSYM *.out *.o *.a *.gcda *.gcno report.* report/

leaks: rebuild
ifeq ($(OS), Darwin)
	CK_FORK=no leaks -atExit -- ./$(EXE)
else	
	valgrind --leak-check=full --show-leak-kinds=all \
         --track-origins=yes --verbose ./$(EXE)       
endif

git: clean
	@if [ -n "$(git_branch)" ]; then \
		git add . ; \
		git status ; \
		git commit -m'$(m)' ; \
		git push origin $(git_branch) ; \
	fi

rebuild: clean test

install_gtest:
	sudo apt install -y libgtest-dev
	sudo apt install -y cmake
	cd /usr/src/gtest
	sudo cmake CMakeLists.txt
	sudo make
	sudo cp lib/*.a /usr/lib

install_valgrind:
	sudo apt install -y valgrind