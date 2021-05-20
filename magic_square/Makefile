EXEC=magic-square_hamming magic-square_num

# Compiler flags
MYFLAGS=
CXXFIRSTFLAGS= -O3 -W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter
#CXXFIRSTFLAGSDEBUG= -g -O0 -DDEBUG -W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter
#LDFLAGSDEBUG=-pthread -lghostd
CXXFIRSTFLAGSDEBUG= -g -O0 -DDEBUG -W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -fsanitize=address,undefined,leak
LDFLAGSDEBUG=-pthread -lghost_staticd -fsanitize=address,undefined,leak -static-libasan

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CXX=g++
	CXXFLAGS= -std=c++2a $(CXXFIRSTFLAGS) $(MYFLAGS)
	CXXFLAGSDEBUG= -std=c++2a $(CXXFIRSTFLAGSDEBUG) $(MYFLAGS)
	LDFLAGS=-lghost_static -pthread
endif
ifeq ($(UNAME_S),Darwin)
	CXX=clang++
	CXXFLAGS= -std=c++2a  -stdlib=libc++ $(CXXFIRSTFLAGS) $(MYFLAGS)
	CXXFLAGSDEBUG= -std=c++2a  -stdlib=libc++ $(CXXFIRSTFLAGSDEBUG) $(MYFLAGS)
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

# For rm
SOURCESTILDE=$(foreach sdir, $(SRCDIR), $(wildcard $(sdir)/*.cpp~))

vpath %.cpp $(SRCDIR)
vpath %.o $(OBJDIR)

# Reminder, 'cause it is easy to forget makefile's fucked-up syntax...
# $@ is what triggered the rule, ie the target before :
# $^ is the whole dependencies list, ie everything after :
# $< is the first item in the dependencies list

# Rules

all: $(BINDIR)/magic-square_hamming $(BINDIR)/magic-square_num

debug: CXXFLAGS=$(CXXFLAGSDEBUG)
debug: LDFLAGS=$(LDFLAGSDEBUG)
debug: $(BINDIR)/magic-square_hamming $(BINDIR)/magic-square_num

$(BINDIR)/magic-square_hamming: $(OBJDIR)/magic-square_hamming.o $(OBJDIR)/factory_ms.o $(OBJDIR)/print_ms.o $(OBJDIR)/linear-eq_hamming.o
	$(CXX) -o $@ $^ $(LDFLAGS) -DHAMMING

$(BINDIR)/magic-square_num: $(OBJDIR)/magic-square_num.o $(OBJDIR)/factory_ms.o $(OBJDIR)/print_ms.o $(OBJDIR)/linear-eq_num.o
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/magic-square_num.o: $(OBJDIR)/factory_ms.o $(OBJDIR)/print_ms.o $(OBJDIR)/linear-eq_num.o
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/magic_square.cpp -o $@

$(OBJDIR)/magic-square_hamming.o: $(OBJDIR)/factory_ms.o $(OBJDIR)/print_ms.o $(OBJDIR)/linear-eq_hamming.o
	$(CXX) $(CXXFLAGS) -DHAMMING -I$(HPPDIR) -c $(SRCDIR)/magic_square.cpp -o $@

$(OBJDIR)/factory_ms.o:
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/factory_ms.cpp -o $@

$(OBJDIR)/print_ms.o:
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/print_ms.cpp -o $@

$(OBJDIR)/linear-eq_hamming.o:
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/linear-eq_hamming.cpp -o $@

$(OBJDIR)/linear-eq_num.o:
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/linear-eq_num.cpp -o $@

# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $< -o $@

# $(OBJDIR)/%.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $< -o $@

.PHONY: clean

clean:
	rm -fr core *~ $(BINDIR)/$(EXEC) $(OBJECTS) 
