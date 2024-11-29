# Compiler flags
MYFLAGS=
CXXFIRSTFLAGS= -O3 -W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-use-after-free
#CXXFIRSTFLAGSDEBUG= -g -O0 -DDEBUG -W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-use-after-free -fsanitize=address -v -da -Q
CXXFIRSTFLAGSDEBUG= -g -O0 -DDEBUG -W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-use-after-free
CXXFIRSTFLAGSINFO= -g -O2 -W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-use-after-free
#LDFLAGSDEBUG=-pthread -lghost_staticd -fsanitize=address -static-libasan
LDFLAGSDEBUG=-pthread -lghost_staticd
LDFLAGSINFO=-pthread -lghost

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CXX=g++
	CXXFLAGS= -std=c++20 $(CXXFIRSTFLAGS) $(MYFLAGS)
	CXXFLAGSDEBUG= -std=c++20 $(CXXFIRSTFLAGSDEBUG) $(MYFLAGS)
	CXXFLAGSINFO= -std=c++20 $(CXXFIRSTFLAGSINFO) $(MYFLAGS)
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

# Reminder, 'cause it is easy to forget makefile's fucked-up syntax...
# $@ is what triggered the rule, ie the target before :
# $^ is the whole dependencies list, ie everything after :
# $< is the first item in the dependencies list

# Rules

all: $(BINDIR)/knapsack $(BINDIR)/alldiff_knapsack

debug: CXXFLAGS=$(CXXFLAGSDEBUG)
debug: LDFLAGS=$(LDFLAGSDEBUG)
debug: $(BINDIR)/knapsack $(BINDIR)/alldiff_knapsack

info: CXXFLAGS=$(CXXFLAGSINFO)
info: LDFLAGS=$(LDFLAGSINFO)
info: $(BINDIR)/knapsack $(BINDIR)/alldiff_knapsack

$(BINDIR)/knapsack: $(OBJDIR)/knapsack.o $(OBJDIR)/knapsack_model_builder.o $(OBJDIR)/knapsack_objective.o $(OBJDIR)/knapsack_print.o $(OBJDIR)/check_solution.o 
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BINDIR)/alldiff_knapsack: $(OBJDIR)/knapsack.o $(OBJDIR)/alldiff_knapsack_model_builder.o $(OBJDIR)/knapsack_objective.o $(OBJDIR)/knapsack_print.o $(OBJDIR)/alldiff_check_solution.o
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $< -o $@

.PHONY: clean

clean:
	rm -fr core $(BINDIR)/knapsack $(BINDIR)/alldiff_knapsack $(OBJECTS) 
