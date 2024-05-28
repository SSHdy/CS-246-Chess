CXX = g++-11 -std=c++20
CXXFLAGS = -Wall -g -MMD -Werror=vla
LIBFLAGS = -lX11
SOURCES = $(wildcard *.cpp)
OBJECTS = ${SOURCES:.cpp=.o}
DEPENDS = ${OBJECTS:.o=.d}
EXEC=chess

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC)  $(LIBFLAGS)

%.o: %.cc
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(LIBFLAGS)

-include ${DEPENDS}

.PHONY: clean tests
clean:
	rm  -f $(OBJECTS) $(DEPENDS) $(EXEC)
