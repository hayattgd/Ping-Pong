NAME = main
SRC = src/main.cpp
BUILD_PATH = ./

build_run:
	g++ -Isrc/Include -Lsrc/lib -o $(BUILD_PATH)$(NAME) $(SRC) -lmingw32 -lSDL2main -lSDL2
	$(BUILD_PATH)$(NAME).exe