# compilador e flags
CXX     := g++
CXXFLAGS:= -std=c++17 -O2 -Wall -Wextra

# binário final
TARGET  := run

# fontes e objetos
SRC     := main.cpp astar.cpp pq.cpp problem1.cpp problem2.cpp heuristics.cpp
OBJ     := $(SRC:.cpp=.o)

# regra padrão
all: $(TARGET)

# linkagem
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# compilar cada .cpp em .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# limpar objetos e binário
clean:
	rm -f $(OBJ) $(TARGET)

# rodar com problema 1 ou 2
run1: $(TARGET)
	./$(TARGET) 1

run2: $(TARGET)
	./$(TARGET) 2
