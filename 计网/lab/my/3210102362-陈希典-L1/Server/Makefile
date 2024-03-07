CXX = g++
CXXFLAGS = -std=c++11 -Wall

C_SRC = serverDemo.cpp
OBJ = $(patsubst %.cpp,%.o,$(C_SRC))

EXECUTABLE = server	

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXECUTABLE)