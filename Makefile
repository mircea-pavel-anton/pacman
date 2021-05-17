MAIN		:= src/main.cpp
#SRC		:=
BIN			:= pacman
BUILD_DIR	:= ./build/
CXX			:= g++
FLAGS		:= -std=c++17 -c
LIBS		:= -lsfml-graphics -lsfml-window -lsfml-system

run: build execute clean

build: compile link

compile:
	@ mkdir $(BUILD_DIR) -p
	@ printf "Compiling source code... "
	@ cd $(BUILD_DIR) && $(CXX) $(FLAGS) ../$(MAIN)
	@ echo "\033[1;32mDone\033[1;0m"

link:
	@ printf "Linking... "
	@ $(CXX) $(BUILD_DIR)/* -o $(BIN) $(LIBS)
	@ echo "\033[1;32mDone\033[1;0m"

clean:
	@ printf "Removing build dir..."
	@ rm -rf $(BUILD_DIR) >> /dev/null 2> /dev/null || true
	@ echo "\033[1;32mDone\033[1;0m"
	@ printf "Removing binary..."
	@ rm $(BIN) >> /dev/null 2> /dev/null || true
	@ echo "\033[1;32mDone\033[1;0m"

dependencies:
	@ printf "Installing dependencies... "
	@ sudo apt install libsfml-dev -y >> /dev/null 2> /dev/null
	@ echo "\033[1;32mDone\033[1;0m"

execute:
	@ ./$(BIN)