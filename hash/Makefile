CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
LDFLAGS = -lcrypto++

SRC = hash_224.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = hash_224

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

install-deps:
	sudo apt-get install libcrypto++-dev libcrypto++-doc libcrypto++-utils
