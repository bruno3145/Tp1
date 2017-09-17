CC=gcc
OPCOES_COMPILADOR=-I. -lglut -lGLU -lGLEW -lGL -lSOIL -lm -ISDL -lSDL_mixer `sdl-config --cflags --libs`
TODOS_ARQUIVOS_PONTOH = textures.h characterMoves.h drawing.h
TODOS_ARQUIVOS_OBJ = textures.o characterMoves.o drawing.o main.o

%.o: ../%.c $(TODOS_ARQUIVOS_PONTOH)
	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)

all: $(TODOS_ARQUIVOS_OBJ)
	gcc -o KILLTHEWALKERS.out $^ $(OPCOES_COMPILADOR)

run: all
	./KILLTHEWALKERS.out

clean:
	rm -f *.o KILLTHEWALKERS
