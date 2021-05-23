CXX				:= g++
FLAGS			:= -std=c++17 -c
DEBUG_FLAGS		:= -g3 -Wall
RELEASE_FLAGS	:= -O3
LIBS			:= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
SRC				:= $(shell find "$(shell pwd)" -name \*.cpp)
BIN				:= pacman
BUILD_DIR		:= build/
DEBUG_DIR		:= $(BUILD_DIR)/debug
RELEASE_DIR		:= $(BUILD_DIR)/release

debug: build-debug execute-debug

release: build-release execute-release

build-debug: compile-debug link-debug

compile-debug:
	@ mkdir -p $(DEBUG_DIR)/bin
	@ mkdir -p $(DEBUG_DIR)/temp
	@ printf "Compiling source code... "
	@ cd $(DEBUG_DIR)/temp/ && $(CXX) $(FLAGS) $(DEBUG_FLAGS) $(SRC)
	@ echo "\033[1;32mDone\033[1;0m"

link-debug:
	@ printf "Linking... "
	@ $(CXX) $(DEBUG_DIR)/temp/* -o $(DEBUG_DIR)/bin/$(BIN) $(LIBS)
	@ echo "\033[1;32mDone\033[1;0m"

execute-debug:
	@ $(DEBUG_DIR)/bin/$(BIN)

build-release: compile-release link-release

compile-release:
	@ mkdir -p $(RELEASE_DIR)/bin
	@ mkdir -p $(RELEASE_DIR)/temp
	@ printf "Compiling source code... "
	@ cd $(RELEASE_DIR)/temp/ && $(CXX) $(FLAGS) $(RELEASE_FLAGS) $(SRC)
	@ echo "\033[1;32mDone\033[1;0m"

link-release:
	@ printf "Linking... "
	@ $(CXX) $(RELEASE_DIR)/temp/* -o $(RELEASE_DIR)/bin/$(BIN) $(LIBS)
	@ echo "\033[1;32mDone\033[1;0m"

execute-release:
	@ $(RELEASE_DIR)/bin/$(BIN)

clean:
	@ printf "Removing build directory..."
	@ rm -rf $(BUILD_DIR) >> /dev/null 2> /dev/null || true
	@ echo "\033[1;32mDone\033[1;0m"
