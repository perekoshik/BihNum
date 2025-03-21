CXX := g++
CXXFLAGS := -std=c++17 -Wall -I include

.PHONY: all clean

# Сборка всех целей
all: bin/calc_pi bin/test

# Цель для сборки calc_pi (число пи)
bin/calc_pi: src/calc_pi.cpp src/BigNumber.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

# Цель для сборки тестов/демо
bin/test: src/main.cpp src/BigNumber.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f bin/calc_pi bin/test
