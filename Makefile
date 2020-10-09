#  FILE = "./Makefile"                                              #
#  Created by Dionysis Taxiarchis Balaskas.                         #
#  Copyright © 2019 D.T.Balaskas. All rights reserved.              #
#####################################################################

.PHONY: clean default run

# Define exec names.
LSH_EXEC= lsh
CUBE_EXEC= cube
CLUSTER_EXEC= cluster

# Define filetype and compiler type.
CXX= g++ -std=c++11 
CODETYPE = cpp
HEADERTYPE = h

# Define commands and tags.
CXXFLAGS= -Wall -g3
LDLIBS= -lpthread
RM= rm -f
GDBFLAGS = -ggdb3

# Organization of directories and files.
DOC= doc
README= $(DOC)/README.md
OUTPUT= ./doc/output
INPUT= ./doc/input
QUERY= ./doc/query
BDIR= bin
SRCDIR= src
SRC= $(wildcard $(SRCDIR)/*.$(CODETYPE))
IDIR= include
DEPS= $(wildcard $(IDIR)/*.h)
ODIR= build
DEMANDED_OBJECTS= $(ODIR)/lsh_classifier.o $(ODIR)/hc_classifier.o $(ODIR)/exhaustive_knn.o $(ODIR)/lsh_hashtable.o \
 $(ODIR)/hc_hashtable.o $(ODIR)/numc.o $(ODIR)/pandac.o $(ODIR)/prediction_results.o

default: exec
	@echo "============================================================================"
	@echo "Compiled Project..."
info:
	@echo "============================================================================"
	@echo "source directory: $(SRCDIR)."
	@echo "include directory: $(IDIR)."
	@echo "object directory: $(ODIR)."
	@echo "documents/data directory: $(DOC)."
	@echo "input directory: $(INPUT)."
	@echo "output directory: $(OUTPUT)."
	@echo "query directory: $(QUERY)."
	@echo "executable directory: $(BDIR)."
	@echo "At compilation we are adding the libraries: $(LDLIBS)."
	@echo "README path: $(README)."
	@echo "============================================================================"
	@echo "make: compiles the executable."
	@echo "make <filename>.o: compiles the object file of $(SRCDIR)/<filename>.$(CODETYPE) on $(ODIR)/<filename>.o."
	@echo "make clean: Clears the project from the executables and the object files."
	@echo "make clean_cache: Clears the project from the output files."
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
clean_cache:
	@echo "============================================================================"
	@echo "Removing cached outputs..."
	$(RM) $(OUTPUT)/* 
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

