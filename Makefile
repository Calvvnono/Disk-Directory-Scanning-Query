CXX = g++
CXXFLAGS = -g3 -std=c++11 -Wall -Wextra -Iinclude -D__DEBUG__

BIN = Disk-Scan-Simulation

SRC = main.cpp Controller.cpp Scanner.cpp sqlGenerator.cpp DirTree.cpp Tick.cpp Formatter.cpp
OBJ = $(SRC:.cpp=.o)

BUILD_DIR = build

.PHONY: all
all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(OBJ) -o $(BIN) $(CXXFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJ) $(BIN)
	rm -rf $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) -c $< -o $@ $(CXXFLAGS)
