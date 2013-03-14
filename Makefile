
SRCDIR = ./SCollector/SCollector
BUILDDIR = ./build
BINDIR = ./bin/

CC = g++
CCFLAGS = -Wall -Wextra -D LINUX
lib = -L../SFGUI/lib -L../TmxParser -lsfml-graphics -lsfml-window -lsfml-system -lsfgui -ltmxparser -ltinyxml
includes = -I../SFGUI/include -I../TmxParser/TmxParser

SCOLLECTOR = $(BINDIR)SCollector

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o,$(SRCS))

$(SCOLLECTOR): $(OBJS)
	$(CC) -g -o $@ $^ $(lib)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -g $(includes) $(CCFLAGS) -c $< -o $@

clean:
	rm $(CLIENTOBJS) $(SERVEROBJS) $(CLIENT) $(SERVER)
