# Compiler flags
MYFLAGS=
CXXFIRSTFLAGS= -O3 -W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-use-after-free
CXXFIRSTFLAGSDEBUG= -g -O0 -DDEBUG -W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-use-after-free
LDFLAGSDEBUG=-pthread -lghostd
# CXXFIRSTFLAGSDEBUG= -g -O0 -DDEBUG -W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-use-after-free -fsanitize=address,undefined,leak
# LDFLAGSDEBUG=-pthread -lghost_staticd -fsanitize=address,undefined,leak -static-libasan
CXXFIRSTFLAGSINFO= -g -O2 -W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-use-after-free
LDFLAGSINFO=-pthread -lghost

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CXX=g++
	CXXFLAGS= -std=c++20 $(CXXFIRSTFLAGS) $(MYFLAGS)
	CXXFLAGSDEBUG= -std=c++20 $(CXXFIRSTFLAGSDEBUG) $(MYFLAGS)
	LDFLAGS=-lghost_static -pthread
endif
ifeq ($(UNAME_S),Darwin)
	CXX=clang++
	CXXFLAGS= -std=c++20  -stdlib=libc++ $(CXXFIRSTFLAGS) $(MYFLAGS)
	CXXFLAGSDEBUG= -std=c++20  -stdlib=libc++ $(CXXFIRSTFLAGSDEBUG) $(MYFLAGS)
	LDFLAGS=-lghost_static -lc++ -lc++abi -pthread
endif

# Directories
SRCDIR=src
HPPDIR=src
OBJDIR=obj
BINDIR=bin

# Files
SOURCES=$(foreach sdir, $(SRCDIR), $(wildcard $(sdir)/*.cpp))
OBJECTS=$(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(SOURCES)))

vpath %.cpp $(SRCDIR)
vpath %.o $(OBJDIR)

# Reminder, 'cause it is easy to forget makefile's fucked-up syntax...
# $@ is what triggered the rule, ie the target before :
# $^ is the whole dependencies list, ie everything after :
# $< is the first item in the dependencies list

# Rules

all: $(BINDIR)/magic_hexagon $(BINDIR)/min_magic_hexagon 

debug: CXXFLAGS=$(CXXFLAGSDEBUG)
debug: LDFLAGS=$(LDFLAGSDEBUG)
debug: $(BINDIR)/magic_hexagon $(BINDIR)/min_magic_hexagon 

info: CXXFLAGS=$(CXXFLAGSINFO)
info: LDFLAGS=$(LDFLAGSINFO)
info: $(BINDIR)/magic_hexagon $(BINDIR)/min_magic_hexagon 

$(BINDIR)/magic_hexagon: $(OBJDIR)/magic_hexagon.o $(OBJDIR)/builder_mh.o $(OBJDIR)/hexagon.o $(OBJDIR)/print_mh.o
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BINDIR)/min_magic_hexagon: $(OBJDIR)/min_magic_hexagon.o $(OBJDIR)/min_builder_mh.o $(OBJDIR)/hexagon.o $(OBJDIR)/print_mh.o $(OBJDIR)/min_sum_corners.o
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/magic_hexagon.o: $(SRCDIR)/magic_hexagon.cpp $(OBJDIR)/builder_mh.o $(OBJDIR)/print_mh.o
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/magic_hexagon.cpp -o $@

$(OBJDIR)/min_magic_hexagon.o: $(SRCDIR)/magic_hexagon.cpp $(OBJDIR)/min_builder_mh.o $(OBJDIR)/print_mh.o 
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/magic_hexagon.cpp -o $@

$(OBJDIR)/builder_mh.o: $(SRCDIR)/builder_mh.cpp $(OBJDIR)/hexagon.o
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/builder_mh.cpp -o $@

$(OBJDIR)/min_builder_mh.o: $(SRCDIR)/builder_mh.cpp $(OBJDIR)/hexagon.o
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -DMINMH -c $(SRCDIR)/builder_mh.cpp -o $@

$(OBJDIR)/hexagon.o: $(SRCDIR)/hexagon.cpp
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/hexagon.cpp -o $@

$(OBJDIR)/print_mh.o: $(SRCDIR)/print_mh.cpp
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/print_mh.cpp -o $@

$(OBJDIR)/min_sum_corners.o: $(SRCDIR)/min_sum_corners.cpp
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/min_sum_corners.cpp -o $@

.PHONY: clean

clean:
	rm -fr core $(BINDIR)/magic_hexagon $(BINDIR)/min_magic_hexagon $(OBJECTS) $(OBJDIR)/min_magic_hexagon.o $(OBJDIR)/min_builder_mh.o
