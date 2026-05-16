CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -I include
LDFLAGS  :=

SRC_DIR  := src
INC_DIR  := include
OBJ_DIR  := build
BIN      := smartbank

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

.PHONY: all clean run

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

run: $(BIN)
	./$(BIN)

clean:
	rm -rf $(OBJ_DIR) $(BIN)
