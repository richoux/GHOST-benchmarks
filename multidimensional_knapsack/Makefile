# Compiler flags
MYFLAGS=
CXXFIRSTFLAGS= -O3 -W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter
#CXXFIRSTFLAGSDEBUG= -g -O0 -DDEBUG -W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -fsanitize=address -v -da -Q
CXXFIRSTFLAGSDEBUG= -g -O0 -DDEBUG -W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter
CXXFIRSTFLAGSINFO= -g -O2 -W -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter
#LDFLAGSDEBUG=-pthread -lghost_staticd -fsanitize=address -static-libasan
LDFLAGSDEBUG=-pthread -lghost_staticd
LDFLAGSINFO=-pthread -lghost

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CXX=g++
	CXXFLAGS= -std=c++17 $(CXXFIRSTFLAGS) $(MYFLAGS)
	CXXFLAGSDEBUG= -std=c++17 $(CXXFIRSTFLAGSDEBUG) $(MYFLAGS)
	CXXFLAGSINFO= -std=c++17 $(CXXFIRSTFLAGSINFO) $(MYFLAGS)
	LDFLAGS=-lghost_static -pthread
endif
ifeq ($(UNAME_S),Darwin)
	CXX=clang++
	CXXFLAGS= -std=c++17  -stdlib=libc++ $(CXXFIRSTFLAGS) $(MYFLAGS)
	CXXFLAGSDEBUG= -std=c++17  -stdlib=libc++ $(CXXFIRSTFLAGSDEBUG) $(MYFLAGS)
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

all: $(BINDIR)/multidimensional_knapsack

debug: CXXFLAGS=$(CXXFLAGSDEBUG)
debug: LDFLAGS=$(LDFLAGSDEBUG)
debug: $(BINDIR)/multidimensional_knapsack

info: CXXFLAGS=$(CXXFLAGSINFO)
info: LDFLAGS=$(LDFLAGSINFO)
info: $(BINDIR)/multidimensional_knapsack

$(BINDIR)/multidimensional_knapsack: $(OBJDIR)/multi_knapsack.o $(OBJDIR)/multi_knapsack_model_builder.o $(OBJDIR)/sat_multi_knapsack_model_builder.o $(OBJDIR)/multi_knapsack_objective.o $(OBJDIR)/zo_knapsack_print.o $(OBJDIR)/check_solution.o 
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $< -o $@

.PHONY: clean

clean:
	rm -fr core $(BINDIR)/multidimensional_knapsack $(OBJECTS) 
