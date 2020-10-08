#  FILE = "./Makefile"                                              #
#  Created by Dionysis Taxiarchis Balaskas.                         #
#  Copyright © 2019 D.T.Balaskas. All rights reserved.              #
#####################################################################

.PHONY: clean default run
CXX= g++ -std=c++11 
CODETYPE = cpp
HEADERTYPE = h

CXXFLAGS= -Wall -g3
LDLIBS= -lpthread
RM= rm -f
GDBFLAGS = -ggdb3

BDIR= bin
SRCDIR= src
SRC= $(wildcard $(SRCDIR)/*.$(CODETYPE))
IDIR= include
DEPS= $(wildcard $(IDIR)/*.h)
ODIR= build
DOC= doc
README= $(DOC)/README.md
DEMANDED_OBJECTS= $(ODIR)/rbtree.o $(ODIR)/stats_entity.o $(ODIR)/record.o $(ODIR)/hash_table.o \
 $(ODIR)/func.o $(ODIR)/record_date_func.o $(ODIR)/bounded_buffer.o

default: exec
	@echo "============================================================================"
	@echo "Compiled Project..."
info:
	@echo "============================================================================"
	@echo "source directory: $(SRCDIR)."
	@echo "include directory: $(IDIR)."
	@echo "object directory: $(ODIR)."
	@echo "documents/data directory: $(DOC)."
	@echo "executable directory: $(BDIR)."
	@echo "At compilation we are adding the libraries: $(LDLIBS)."
	@echo "README path: $(DOC)/README.txt."
	@echo "============================================================================"
	@echo "make: compiles the executable."
	@echo "make <filename>.o: compiles the object file of $(SRCDIR)/<filename>.$(CODETYPE) on $(ODIR)/<filename>.o."
	@echo "make clean: Clears the project from the executables, the object files, and the data."
	@echo "make clean_cache: Clears the project from the data (logfiles, outputs and named pipes)."
	@echo "make debug: compiles the project with $(GDBFLAGS) and runs gdb on the executable."
	@echo "make README: Prints the README file of the project."
	@echo "============================================================================"

$(ODIR)/%.o: $(SRCDIR)/%.$(CODETYPE)
	@echo "Creating object" $(ODIR)/$@ "..."
	$(CXX) -c -o $@ $< $(CXXFLAGS)
%.o: $(SRCDIR)/%.$(CODETYPE)
	@echo "Creating object" $(ODIR)/$@ "..."
	$(CXX) -c -o $(ODIR)/$@ $< $(CXXFLAGS)
exec: $(DEMANDED_OBJECTS)
	@echo "============================================================================"
	@echo "Creating executables..."
	$(CXX) -c -o $(ODIR)/master.o $(SRCDIR)/master.$(CODETYPE) $(CXXFLAGS)
	$(CXX) -o $(BDIR)/master $(ODIR)/master.o $^ $(LDLIBS) $(CXXFLAGS)
	$(CXX) -c -o $(ODIR)/worker.o $(SRCDIR)/worker.$(CODETYPE) $(CXXFLAGS)
	$(CXX) -o $(BDIR)/worker $(ODIR)/worker.o $^ $(LDLIBS) $(CXXFLAGS)
	$(CXX) -c -o $(ODIR)/whoClient.o $(SRCDIR)/whoClient.$(CODETYPE) $(CXXFLAGS)
	$(CXX) -o $(BDIR)/whoClient $(ODIR)/whoClient.o $^ $(LDLIBS) $(CXXFLAGS)
	$(CXX) -c -o $(ODIR)/whoServer.o $(SRCDIR)/whoServer.$(CODETYPE) $(CXXFLAGS)
	$(CXX) -o $(BDIR)/whoServer $(ODIR)/whoServer.o $^ $(LDLIBS) $(CXXFLAGS)
clean:
	@echo "============================================================================"
	@echo "Cleaning up..."
	$(RM) $(ODIR)/*
	$(RM) $(BDIR)/*
	$(RM) ./doc/logs/logfile.*
	$(RM) ./doc/output/output.txt
	$(RM) ./worker_*
clean_cache:
	@echo "============================================================================"
	@echo "Removing datafiles..."
	$(RM) ./doc/logs/logfile.*
	$(RM) ./doc/output/output.txt
	$(RM) ./worker_*
debug: $(DEMANDED_OBJECTS)
	@echo "============================================================================"
	@echo "Debugging..."
	$(CXX) -c -o $(ODIR)/main.o $(SRCDIR)/main.$(CODETYPE) $(CXXFLAGS) $(GDBFLAGS)
	$(CXX) -o $(BDIR)/$@ $(ODIR)/main.o $^ $(LDLIBS) $(CXXFLAGS) $(GDBFLAGS)
	$(CXX) -c -o $(ODIR)/worker.o $(SRCDIR)/worker.$(CODETYPE) $(CXXFLAGS) $(GDBFLAGS)
	$(CXX) -o $(BDIR)/worker $(ODIR)/worker.o $^ $(LDLIBS) $(CXXFLAGS) $(GDBFLAGS)
	$(CXX) -c -o $(ODIR)/whoClient.o $(SRCDIR)/whoClient.$(CODETYPE) $(CXXFLAGS)
	$(CXX) -o $(BDIR)/whoClient $(ODIR)/whoClient.o $^ $(LDLIBS) $(CXXFLAGS)
	$(CXX) -c -o $(ODIR)/whoServer.o $(SRCDIR)/whoServer.$(CODETYPE) $(CXXFLAGS)
	$(CXX) -o $(BDIR)/whoServer $(ODIR)/whoServer.o $^ $(LDLIBS) $(CXXFLAGS)
	gdb $(BDIR)/master
README:
	@echo "============================================================================"
	@cat $(README)
	@echo "============================================================================"

