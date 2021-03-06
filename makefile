CXX ?= g++
CXXFLAGS ?= $(shell imlib2-config --cflags) -O2 -g
LDFLAGS += -lImlib2 -lX11 -lXfixes -lXrandr
SOURCES = main.cpp x.cpp cmdline.c im.cpp
OBJECTS = main.o x.o cmdline.o im.o
EXECUTABLE = maim
BINDIR = "/usr/bin"

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	$(RM) $(OBJECTS)

dist-clean: clean
	$(RM) $(EXECUTABLE)

install: all
	mkdir -p $(DESTDIR)$(BINDIR)
	cp $(CURDIR)/$(EXECUTABLE) $(DESTDIR)$(BINDIR)
