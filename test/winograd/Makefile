.PHONY: graph \
		functional_winograd functional_winogradp functional_strassen functional_strassenp functional \
		speed_compare speed_compare_slow level_load level_load_slow valgrind clean

CC=g++
# OPTIMIZATION=-Ofast -march=native
OPTIMIZATION=-O3
STANDART=-std=c++17
FLAGS=-Wall -Werror -Wextra $(STANDART)
BLAS_FLAGS=-lpthread -lm -ldl -lblas
GTEST_FLAGS=-lgtest -lgtest_main -pthread


graph:
	cmake -S . -B build
	make -C build
	./build/graph

functional_winograd_build:
	$(CC) $(FLAGS) -D WINOGRAD functional.cc -o functional_winograd $(GTEST_FLAGS)

functional_winogradp_build:
	$(CC) $(FLAGS) -D WINOGRADP functional.cc -o functional_winogradp $(GTEST_FLAGS)

functional_strassen_build:
	$(CC) $(FLAGS) -D STRASSEN functional.cc -o functional_strassen $(GTEST_FLAGS)

functional_strassenp_build:
	$(CC) $(FLAGS) -D STRASSENP functional.cc -o functional_strassenp $(GTEST_FLAGS)

functional_winograd: functional_winograd_build
	./functional_winograd

functional_winogradp: functional_winogradp_build
	./functional_winogradp

functional_strassen: functional_strassen_build
	./functional_strassen

functional_strassenp: functional_strassenp_build
	./functional_strassenp

functional: functional_winograd functional_winogradp functional_strassen functional_strassenp

speed_compare:
	$(CC) $(FLAGS) speed_compare.cc -o speed_compare_optimized $(BLAS_FLAGS) $(OPTIMIZATION)
	./speed_compare_optimized

speed_compare_slow:
	$(CC) $(FLAGS) speed_compare.cc -o speed_compare_slow $(BLAS_FLAGS)
	./speed_compare_slow

level_load:
	$(CC) $(FLAGS) level_load.cc -o level_load_optimized $(OPTIMIZATION)
	./level_load_optimized

level_load_slow:
	$(CC) $(FLAGS) level_load.cc -o level_load
	./level_load_slow

valgrind: functional_winograd_build functional_winogradp_build functional_strassen_build functional_strassenp_build
	valgrind --leak-check=full ./functional_winograd
	valgrind --leak-check=full ./functional_winogradp
	valgrind --leak-check=full ./functional_strassen
	valgrind --leak-check=full ./functional_strassenp

clean:
	rm -f functional_winograd functional_winogradp functional_strassen functional_strassenp
	rm -f speed_compare_optimized speed_compare_slow level_load_optimized level_load_slow
