CXX = g++
CXXFLAGS = -Wall -std=c++11

SRCDIR = src
OBJDIR = obj

UPDATEDB_EXEC = updatedb
LOCATE_EXEC = locate

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
UPDATEDB_SOURCES = $(SRCDIR)/bwt.cpp $(SRCDIR)/converter.cpp $(SRCDIR)/deserializer.cpp $(SRCDIR)/file_manager.cpp  $(SRCDIR)/file_mapper.cpp $(SRCDIR)/pattern_finder.cpp $(SRCDIR)/suffix_array_builder.cpp  $(SRCDIR)/utils.cpp $(SRCDIR)/thread_pool.cpp $(SRCDIR)/main_updatedb.cpp
LOCATE_SOURCES = $(SRCDIR)/bwt.cpp $(SRCDIR)/converter.cpp $(SRCDIR)/deserializer.cpp $(SRCDIR)/file_manager.cpp  $(SRCDIR)/file_mapper.cpp $(SRCDIR)/pattern_finder.cpp $(SRCDIR)/suffix_array_builder.cpp  $(SRCDIR)/utils.cpp $(SRCDIR)/thread_pool.cpp  $(SRCDIR)/main_locate.cpp

OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
UPDATEDB_OBJECTS := $(UPDATEDB_SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
LOCATE_OBJECTS := $(LOCATE_SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

all: MKDIR EXEC

MKDIR:
	mkdir $(OBJDIR)

EXEC: $(OBJECTS) 
	$(CXX) $(UPDATEDB_OBJECTS) -o $(UPDATEDB_EXEC) -lboost_system -lboost_thread -lboost_filesystem
	$(CXX) $(LOCATE_OBJECTS) -o $(LOCATE_EXEC) -lboost_system -lboost_thread -lboost_filesystem

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -rf $(OBJDIR)
	rm -rf $(UPDATEDB_EXEC)
	rm -rf $(LOCATE_EXEC)


