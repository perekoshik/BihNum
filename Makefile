# Компилятор и флаги сборки
CXX := g++
CXXFLAGS := -std=c++17 -Wall -I include -MMD

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
CALC_PI_SRC := $(SRC_DIR)/calc_pi.cpp
TEST_SRC := $(SRC_DIR)/main.cpp
COMMON_SRCS := $(filter-out $(CALC_PI_SRC) $(TEST_SRC), $(SRCS))

COMMON_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(COMMON_SRCS))
CALC_PI_OBJ := $(OBJ_DIR)/calc_pi.o
TEST_OBJ := $(OBJ_DIR)/main.o

CALC_PI_EXE := $(BIN_DIR)/calc_pi
TEST_EXE := $(BIN_DIR)/test

.PHONY: all clean

all: $(CALC_PI_EXE) $(TEST_EXE)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Built $@ from $<"

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(CALC_PI_EXE): $(CALC_PI_OBJ) $(COMMON_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Linked $@"

$(TEST_EXE): $(TEST_OBJ) $(COMMON_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Linked $@"

$(OBJ_DIR)/%.o: Makefile

-include $(wildcard $(OBJ_DIR)/*.d)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Cleaned all build files."
