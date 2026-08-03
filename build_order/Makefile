EXEC=build_order

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

$(BINDIR)/$(EXEC): $(OBJDIR)/build_order_problem.o $(OBJDIR)/actionData.o $(OBJDIR)/actionMap.o $(OBJDIR)/builder_bo.o $(OBJDIR)/constraint_local_dependency.o $(OBJDIR)/constraint_merge.o $(OBJDIR)/objective_makespan.o
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/actionData.o: actionData.cpp
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/actionData.cpp -o $@

$(OBJDIR)/build_order_problem.o: $(OBJDIR)/builder_bo.o build_order_problem.cpp
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/build_order_problem.cpp -o $@

$(OBJDIR)/actionMap.o: $(OBJDIR)/actionData.o actionMap.cpp
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/actionMap.cpp -o $@

$(OBJDIR)/builder_bo.o: $(OBJDIR)/actionData.o $(OBJDIR)/constraint_local_dependency.o $(OBJDIR)/constraint_merge.o $(OBJDIR)/objective_makespan.o builder_bo.cpp
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/builder_bo.cpp -o $@

$(OBJDIR)/constraint_local_dependency.o: $(OBJDIR)/actionMap.o constraint_local_dependency.cpp
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/constraint_local_dependency.cpp -o $@

$(OBJDIR)/constraint_merge.o: $(OBJDIR)/actionMap.o constraint_merge.cpp
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/constraint_merge.cpp -o $@

$(OBJDIR)/objective_makespan.o: $(OBJDIR)/actionData.o $(OBJDIR)/actionMap.o objective_makespan.cpp
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/objective_makespan.cpp -o $@

.PHONY: clean 

clean:
	rm -fr core *~ $(BINDIR)/$(EXEC) $(OBJECTS) $(SOURCESTILDE)
