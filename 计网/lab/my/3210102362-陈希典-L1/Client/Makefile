CXX = g++
CXXFLAGS = -std=c++11 -Wall

C_SRC = ClientDemo.cpp
OBJ = $(patsubst %.cpp,%.o,$(C_SRC))

EXECUTABLE = client

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXECUTABLE)