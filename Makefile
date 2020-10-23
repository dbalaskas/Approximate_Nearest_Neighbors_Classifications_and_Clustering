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
README= README.md
OUTPUT= ./doc/output
INPUT= ./doc/input
QUERY= ./doc/query
CONF= ./doc/configuration
BDIR= bin
SRCDIR= src
SRC= $(wildcard $(SRCDIR)/*.$(CODETYPE))
IDIR= include
DEPS= $(wildcard $(IDIR)/*.h)
ODIR= build
DEMANDED_OBJECTS= $(ODIR)/exhaustive_knn.o $(ODIR)/hashtable.o $(ODIR)/hash_function.o $(ODIR)/numc.o $(ODIR)/pandac.o $(ODIR)/prediction_results.o

default: $(CUBE_EXEC) $(LSH_EXEC) $(CLUSTER_EXEC)
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
	@echo "configuration directory: $(CONF)."
	@echo "executable directory: $(BDIR)."
	@echo "At compilation we are adding the libraries: $(LDLIBS)."
	@echo "README path: $(README)."
	@echo "============================================================================"
	@echo "make: compiles the executable."
	@echo "make <filename>.o: compiles the object file of $(SRCDIR)/<filename>.$(CODETYPE) on $(ODIR)/<filename>.o."
	@echo "make clean: Clears the project from the executables and the object files."
	@echo "make clean_cache: Clears the project from the output files."
	@echo "make README: Prints the README file of the project."
	@echo "============================================================================"

$(ODIR)/%.o: $(SRCDIR)/%.$(CODETYPE)
	@echo "Creating object" $(ODIR)/$@ "..."
	$(CXX) -c -o $@ $< $(CXXFLAGS)
%.o: $(SRCDIR)/%.$(CODETYPE)
	@echo "Creating object" $(ODIR)/$@ "..."
	$(CXX) -c -o $(ODIR)/$@ $< $(CXXFLAGS)
$(CUBE_EXEC): $(DEMANDED_OBJECTS) $(ODIR)/hc_classifier.o
	@echo "============================================================================"
	@echo "Creating $(CUBE_EXEC)..."
	$(CXX) -c -o $(ODIR)/cube.o $(SRCDIR)/cube.$(CODETYPE) $(CXXFLAGS)
	$(CXX) -o $(BDIR)/cube $(ODIR)/cube.o $^ $(LDLIBS) $(CXXFLAGS)

$(LSH_EXEC): $(DEMANDED_OBJECTS) $(ODIR)/lsh_classifier.o
	@echo "============================================================================"
	@echo "Creating $(LSH_EXEC)..."
	$(CXX) -c -o $(ODIR)/lsh.o $(SRCDIR)/lsh.$(CODETYPE) $(CXXFLAGS)
	$(CXX) -o $(BDIR)/lsh $(ODIR)/lsh.o $^ $(LDLIBS) $(CXXFLAGS)

$(CLUSTER_EXEC): $(DEMANDED_OBJECTS) $(ODIR)/lsh_classifier.o $(ODIR)/hc_classifier.o
	@echo "============================================================================"
	@echo "Creating $(CLUSTER_EXEC)..."
	$(CXX) -c -o $(ODIR)/cluster.o $(SRCDIR)/cluster.$(CODETYPE) $(CXXFLAGS)
	$(CXX) -o $(BDIR)/cluster $(ODIR)/cluster.o $^ $(LDLIBS) $(CXXFLAGS)
exec: $(CUBE_EXEC)
	@echo "============================================================================"
	@echo "Creating executables..."
clean:
	@echo "============================================================================"
	@echo "Cleaning up..."
	$(RM) $(ODIR)/*
	$(RM) $(BDIR)/*
clean_cache:
	@echo "============================================================================"
	@echo "Removing cached outputs..."
	$(RM) $(OUTPUT)/*
README:
	@echo "============================================================================"
	@cat $(README)
	@echo "============================================================================"

