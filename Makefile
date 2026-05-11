SRC=ejemplo.cpp

ejemplo: $(SRC) ssoomaforos.hpp
	g++ $(SRC) -Og -g -Wall -o ejemplo

.PHONY: run

run: ejemplo
	./ejemplo
