CXX = g++
CXXFLAGS = -Wall -O3 -std=c++17 -I./src

SOURCES = src/main.cpp \
          src/core/estruturas.cpp \
          src/core/algoritmos.cpp \
          src/generators/gerador.cpp \
          src/io/gerenciador_csv.cpp

OBJECTS = $(SOURCES:.cpp=.o)
TARGET = pipeline_mst

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)