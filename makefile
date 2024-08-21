##############################################################################
# Exemplo de makefile para um projeto C++ com OpenGL/freeglut
# Para simplificar, todos os arquivos estão em um mesmo diretório
##############################################################################
# Arquivo principal: main.cpp
##############################################################################


# definição de variáveis auxiliares do makefile
MAIN = main
OBJECTS	= $(MAIN).o
FLAGS = -Wall -Wextra -std=c++17 -pedantic-errors
MATH = -lm
DEBUG = -g
CC = g++

ifeq ($(OS),Windows_NT)
	OPENGL_FLAG = -lopengl32 -lglu32 -lfreeglut
	RM = del
else
	OPENGL_FLAG = -lGL -lGLU -lglut
	RM = rm -rf

endif
# regra de compilação do projeto completo
all: main.exe

# regra para geração do arquivo executavel
main.exe: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(MAIN).exe $(FLAGS) $(OPENGL_FLAG)

# regra de compilação do objeto main.o
main.o: $(MAIN).cpp
	$(CC) -c $(MAIN).cpp -o $(MAIN).o 

# regra para limpeza do projeto (excluir arquivos .o e .exe). 
# (OBS.: SO windows)
clean:
	$(RM)  $(OBJECTS)
	$(RM) *.exe

# regra para compilar todo o projeto (se necessário) e, posteriormente, executar
# (OBS.: SO windows)
run: all
	./$(MAIN).exe