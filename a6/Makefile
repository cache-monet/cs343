CXX = u++ # compiler
CXXFLAGS = -g -multi -O2 -std=c++11 -Wall -Wextra -MMD  # compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

# OBJECTS1 = bank.o bottlingplant.o config.o groupoff.o nameserver.o parent.o printer.o student.o truck.o vendingmachine.o watcard.o watcardoffice.o soda.o
OBJECTS1 = bank.o config.o  parent.o printer.o soda.o watcard.o watcardoffice.o
EXEC1 = soda

OBJECTS = ${OBJECTS1}						# all object files
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
EXECS = ${EXEC1}								# all executables

#############################################################

.PHONY : all clean

all: ${EXECS}

${EXEC1} : ${OBJECTS1}
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}								# include *.d files containing program dependences

clean :														# remove files that can be regenerated
	rm -f *.d *.o ${EXECS}
