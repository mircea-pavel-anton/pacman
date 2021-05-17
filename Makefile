MAIN			:= src/main.cpp
#SRC			:=
BIN				:= pacman
BUILD_DIR		:= ./build/
CXX				:= g++
FLAGS			:= -std=c++17 -c
DEBUG_FLAGS		:= -g -Wall
RELEASE_FLAGS	:= -O3
LIBS			:= -lsfml-graphics -lsfml-window -lsfml-system

run: build execute clean

debug: build-debug link-debug

release: build-release link-release

build-debug:
	@ mkdir -p $(BUILD_DIR)/debug/bin
	@ mkdir -p $(BUILD_DIR)/debug/temp
	@ printf "Compiling source code... "
	@ $(CXX) $(FLAGS) $(DEBUG_FLAGS) $(MAIN)
	@ mv *.o $(BUILD_DIR)/debug/temp/
	@ echo "\033[1;32mDone\033[1;0m"

link-debug:
	@ printf "Linking... "
	@ $(CXX) $(BUILD_DIR)/debug/temp/* -o $(BUILD_DIR)/debug/bin/$(BIN) $(LIBS)
	@ echo "\033[1;32mDone\033[1;0m"

build-release:
	@ mkdir -p $(BUILD_DIR)/release/bin
	@ mkdir -p $(BUILD_DIR)/release/temp
	@ printf "Compiling source code... "
	@ $(CXX) $(FLAGS) $(RELEASE_FLAGS) $(MAIN)
	@ echo "\033[1;32mDone\033[1;0m"
	@ mv *.o $(BUILD_DIR)/release/temp/

link-release:
	@ printf "Linking... "
	@ $(CXX) $(BUILD_DIR)/release/temp/* -o $(BUILD_DIR)/release/bin/$(BIN) $(LIBS)
	@ echo "\033[1;32mDone\033[1;0m"

clean:
	@ printf "Removing build dir..."
	@ rm -rf $(BUILD_DIR) >> /dev/null 2> /dev/null || true
	@ echo "\033[1;32mDone\033[1;0m"
	@ printf "Removing binary..."
	@ rm $(BIN) >> /dev/null 2> /dev/null || true
	@ echo "\033[1;32mDone\033[1;0m"

prerequisites:
	@ printf "Installing dependencies... "
	@ sudo apt install libsfml-dev -y >> /dev/null 2> /dev/null
	@ echo "\033[1;32mDone\033[1;0m"
