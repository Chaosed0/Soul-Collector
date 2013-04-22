
SRCDIR = ./SCollector/SCollector
BUILDDIR = ../SCollector/build
BINDIR = ../SCollector/bin/

CC = g++
CCFLAGS = -Wall -Wextra -g -D LINUX -std=c++11
lib = -L/usr/local/lib -L/usr/lib -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system -lsfgui -ltmxparser -ltinyxml
includes = -I/usr/local/include -I/usr/include/TmxParser

SCOLLECTOR = $(BINDIR)SCollector

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o,$(SRCS))

$(SCOLLECTOR): $(OBJS)
	$(CC) -g -o $@ $^ $(lib)
	cp SCollector/SCollector/assets/* $(BINDIR) -r

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -g $(includes) $(CCFLAGS) -c $< -o $@

clean:
	rm $(CLIENTOBJS) $(SERVEROBJS) $(CLIENT) $(SERVER)
