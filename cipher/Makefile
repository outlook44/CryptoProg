CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I/usr/include/cryptopp
LDFLAGS = -lcryptopp

TARGET = cipher
SRCS = cipher.cpp

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: clean
