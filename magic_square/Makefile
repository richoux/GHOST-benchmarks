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

all: $(BINDIR)/magic_square $(BINDIR)/min_magic_square $(BINDIR)/magic_square_complete $(BINDIR)/min_magic_square_complete

debug: CXXFLAGS=$(CXXFLAGSDEBUG)
debug: LDFLAGS=$(LDFLAGSDEBUG)
debug: $(BINDIR)/magic_square $(BINDIR)/min_magic_square $(BINDIR)/magic_square_complete $(BINDIR)/min_magic_square_complete

info: CXXFLAGS=$(CXXFLAGSINFO)
info: LDFLAGS=$(LDFLAGSINFO)
info: $(BINDIR)/magic_square $(BINDIR)/min_magic_square $(BINDIR)/magic_square_complete $(BINDIR)/min_magic_square_complete

$(BINDIR)/magic_square: $(OBJDIR)/magic_square.o $(OBJDIR)/builder_ms.o $(OBJDIR)/print_ms.o
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BINDIR)/min_magic_square: $(OBJDIR)/min_magic_square.o $(OBJDIR)/min_builder_ms.o $(OBJDIR)/print_ms.o $(OBJDIR)/min_sum_corners.o
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BINDIR)/magic_square_complete: $(OBJDIR)/magic_square_complete.o $(OBJDIR)/builder_ms_complete.o $(OBJDIR)/print_ms.o
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BINDIR)/min_magic_square_complete: $(OBJDIR)/min_magic_square_complete.o $(OBJDIR)/min_builder_ms_complete.o $(OBJDIR)/print_ms.o $(OBJDIR)/min_sum_corners.o
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/magic_square.o: $(OBJDIR)/builder_ms.o $(OBJDIR)/print_ms.o
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/magic_square.cpp -o $@

$(OBJDIR)/min_magic_square.o: $(OBJDIR)/min_builder_ms.o $(OBJDIR)/print_ms.o $(OBJDIR)/min_sum_corners.o
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/magic_square.cpp -o $@

$(OBJDIR)/magic_square_complete.o: $(OBJDIR)/builder_ms_complete.o $(OBJDIR)/print_ms.o
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -DCOMPLETESEARCH -c $(SRCDIR)/magic_square.cpp -o $@

$(OBJDIR)/min_magic_square_complete.o: $(OBJDIR)/min_builder_ms_complete.o $(OBJDIR)/print_ms.o $(OBJDIR)/min_sum_corners.o
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -DCOMPLETESEARCH -c $(SRCDIR)/magic_square.cpp -o $@

$(OBJDIR)/builder_ms.o:
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/builder_ms.cpp -o $@

$(OBJDIR)/min_builder_ms.o:
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -DMINMS -c $(SRCDIR)/builder_ms.cpp -o $@

$(OBJDIR)/builder_ms_complete.o:
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -DCOMPLETESEARCH -c $(SRCDIR)/builder_ms.cpp -o $@

$(OBJDIR)/min_builder_ms_complete.o:
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -DCOMPLETESEARCH -DMINMS -c $(SRCDIR)/builder_ms.cpp -o $@

$(OBJDIR)/print_ms.o:
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/print_ms.cpp -o $@

$(OBJDIR)/min_sum_corners.o:
	$(CXX) $(CXXFLAGS) -I$(HPPDIR) -c $(SRCDIR)/min_sum_corners.cpp -o $@

.PHONY: clean

clean:
	rm -fr core $(BINDIR)/magic_square $(BINDIR)/min_magic_square $(BINDIR)/magic_square_complete $(BINDIR)/min_magic_square_complete $(OBJECTS) $(OBJDIR)/min_magic_square.o $(OBJDIR)/min_builder_ms.o
