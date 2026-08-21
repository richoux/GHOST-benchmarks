EXEC=wallin

# Directories
SRCDIR=src
OBJDIR=obj
BINDIR=bin

# Compiler flags
MYFLAGS=
CXXFIRSTFLAGS= -O3 -W -Wall -Wextra -pedantic -Wno-sign-compare -Wno-unused-parameter -Wno-use-after-free
CXXFIRSTFLAGSDEBUG= -g -O0 -W -Wall -Wextra -pedantic -Wno-sign-compare -Wno-unused-parameter -Wno-use-after-free

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CXX=g++
	CXXFLAGS= -std=c++20 $(CXXFIRSTFLAGS) $(MYFLAGS)
	CXXFLAGSDEBUG= -std=c++20 $(CXXFIRSTFLAGSDEBUG) $(MYFLAGS)
	LDFLAGS=-pthread -lghost_static
	LDFLAGSDEBUG=-pthread -lghostd
endif
ifeq ($(UNAME_S),Darwin)
	CXX=clang++
	CXXFLAGS= -std=c++20  -stdlib=libc++ $(CXXFIRSTFLAGS) $(MYFLAGS)
	CXXFLAGSDEBUG= -std=c++20  -stdlib=libc++ $(CXXFIRSTFLAGSDEBUG) $(MYFLAGS)
	LDFLAGS=-pthread -lghost_static -lc++ -lc++abi
	LDFLAGSDEBUG=-pthread -lghostd -lc++ -lc++abi
endif

# Files
SOURCES=$(foreach sdir, $(SRCDIR), $(wildcard $(sdir)/*.cpp))
OBJECTS=$(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(SOURCES)))

# For rm
SOURCESTILDE=$(foreach sdir, $(SRCDIR), $(wildcard $(sdir)/*.cpp~))

vpath %.cpp $(SRCDIR)

# Reminder, 'cause it is easy to forget makefile's fucked-up syntax...
# $@ is what triggered the rule, ie the target before :
# $^ is the whole dependencies list, ie everything after :
# $< is the first item in the dependencies list

# Rules
all: $(BINDIR)/$(EXEC)

debug: CXXFLAGS= $(CXXFLAGSDEBUG)
debug: $(BINDIR)/$(EXEC)

$(BINDIR)/$(EXEC): $(OBJDIR)/wall-in.o $(OBJDIR)/builder_wallin.o $(OBJDIR)/building.o $(OBJDIR)/wallinConstraint.o $(OBJDIR)/wallinObjective.o
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/wall-in.o: wall-in.cpp $(OBJDIR)/builder_wallin.o $(OBJDIR)/building.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/builder_wallin.o: builder_wallin.cpp $(OBJDIR)/wallinConstraint.o $(OBJDIR)/wallinObjective.o $(OBJDIR)/building.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/wallinConstraint.o: wallinConstraint.cpp $(OBJDIR)/building.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/wallinObjective.o: wallinObjective.cpp $(OBJDIR)/building.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/building.o: building.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean 

clean:
	rm -fr core *~ $(EXEC) $(OBJECTS) $(SOURCESTILDE)
