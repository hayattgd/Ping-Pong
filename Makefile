NAME = main
SRC = src/main.cpp

build_run:
	g++ -Isrc/Include -Lsrc/lib -o bin/$(NAME) $(SRC) -lmingw32 -lSDL2main -lSDL2
	bin/$(NAME).exe