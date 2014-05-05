
SRCDIR = ./SCollector/SCollector
BUILDDIR = ./build
BINDIR = ./bin/

CC = clang++
CPPFLAGS = -Wall -Wextra -g -DLINUX -DTIXML_USE_STL -std=c++11
lib = -L/usr/local/lib -L/usr/lib -lsfml-graphics-d -lsfml-window-d -lsfml-audio-d -lsfml-system-d -lsfgui -ltmxparser -ltinyxml -lz
includes = -I/usr/local/include -I/usr/include/TmxParser/include

SCOLLECTOR = $(BINDIR)SCollector

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o,$(SRCS))

$(SCOLLECTOR): $(OBJS)
	$(CC) -g -o $@ $^ $(lib)
	cp SCollector/SCollector/assets $(BINDIR)/assets -r

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(includes) $(CPPFLAGS) -c $< -o $@

clean:
	rm  $(OBJS) $(SCOLLECTOR)
