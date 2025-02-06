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
	LDFLAGS=-pthread -lghost_static
endif
ifeq ($(UNAME_S),Darwin)
	CXX=clang++
	CXXFLAGS= -std=c++20  -stdlib=libc++ $(CXXFIRSTFLAGS) $(MYFLAGS)
	CXXFLAGSDEBUG= -std=c++20  -stdlib=libc++ $(CXXFIRSTFLAGSDEBUG) $(MYFLAGS)
	LDFLAGS=-pthread -lghost_static -lc++ -lc++abi
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

all: $(BINDIR)/nqueens_permut $(BINDIR)/nqueens_alldiff $(BINDIR)/nqueens_complete

debug: CXXFLAGS=$(CXXFLAGSDEBUG)
debug: LDFLAGS=$(LDFLAGSDEBUG)
debug: $(BINDIR)/nqueens_permut $(BINDIR)/nqueens_alldiff $(BINDIR)/nqueens_complete

info: CXXFLAGS=$(CXXFLAGSINFO)
info: LDFLAGS=$(LDFLAGSINFO)
info: $(BINDIR)/nqueens_permut $(BINDIR)/nqueens_alldiff $(BINDIR)/nqueens_complete

$(BINDIR)/nqueens_permut: $(OBJDIR)/nqueens.o $(OBJDIR)/builder_nqueens_permut.o $(OBJDIR)/constraint_diagonal.o $(OBJDIR)/print_nqueens.o
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BINDIR)/nqueens_alldiff: $(OBJDIR)/nqueens.o $(OBJDIR)/builder_nqueens_alldiff.o $(OBJDIR)/constraint_diagonal.o $(OBJDIR)/print_nqueens.o
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BINDIR)/nqueens_complete: $(OBJDIR)/nqueens_complete.o $(OBJDIR)/builder_nqueens_alldiff.o $(OBJDIR)/constraint_diagonal.o $(OBJDIR)/print_nqueens.o
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/nqueens.o: $(OBJDIR)/builder_nqueens_permut.o $(OBJDIR)/builder_nqueens_alldiff.o $(OBJDIR)/print_nqueens.o
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/nqueens.cpp -o $@

$(OBJDIR)/nqueens_complete.o: $(OBJDIR)/builder_nqueens_alldiff.o $(OBJDIR)/print_nqueens.o
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -DCOMPLETESEARCH -c $(SRCDIR)/nqueens.cpp -o $@

$(OBJDIR)/builder_nqueens_permut.o: $(OBJDIR)/constraint_diagonal.o
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/builder_nqueens_permut.cpp -o $@

$(OBJDIR)/builder_nqueens_alldiff.o: $(OBJDIR)/constraint_diagonal.o
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/builder_nqueens_alldiff.cpp -o $@

$(OBJDIR)/constraint_diagonal.o:
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/constraint_diagonal.cpp -o $@

$(OBJDIR)/print_nqueens.o:
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/print_nqueens.cpp -o $@

.PHONY: clean

clean:
	rm -fr core $(BINDIR)/nqueens_permut $(BINDIR)/nqueens_alldiff $(BINDIR)/nqueens_complete $(OBJDIR)/*
