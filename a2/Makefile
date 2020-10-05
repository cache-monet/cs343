CXX = u++					# compiler
CXXFLAGS = -g -Wall -Wextra -MMD
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS1 = q1main.o q1filter.o q1reader.o q1writer.o q1hexdumpfilter.o q1whitespacefilter.o q1trianglefilter.o 
# object files forming 1st executable with prefix "q1"
EXEC1 = filter

OBJECTS2 = q2main.o q2potato.o q2player.o # object files forming 2nd executable with prefix "q2"
EXEC2 = hotpotato

OBJECTS3 = q3.o # object files forming 2nd executable with prefix "q2"
EXEC3 = q3 

OBJECTS = ${OBJECTS1} ${OBJECTS2} ${OBJECTS3}		# all object files
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
EXECS = ${EXEC1} ${EXEC2} ${EXEC3}			# all executables

.PHONY : all clean

all : ${EXECS}					# build all executables

#############################################################

${EXEC1} : ${OBJECTS1}				# link step 1st executable
	${CXX} ${CXXFLAGS} $^ -o $@

${EXEC2} : ${OBJECTS2}				# link step 2nd executable
	${CXX} ${CXXFLAGS} $^ -o $@

${EXEC3} : ${OBJECTS3}				# link step 34d executable
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${EXECS}
