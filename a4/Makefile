VIMPL:=BAR
OUTPUT:=OUTPUT

CXX = u++					# compiler
CXXFLAGS = -g -multi -O2 -Wall -Wextra -MMD -D${VIMPL} -D${OUTPUT} # compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

# OBJECTS = q3tallyVotes${VIMPL}.o # list of object files for question 3 prefixed with "q3"
OBJECTS = q3tallyVotes${VIMPL}.o q3voter.o q3printer.o q3main.o  # list of object files for question 3 prefixed with "q3"
EXEC = vote


DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"

#############################################################

.PHONY : all clean

all : ${EXEC}					# build all executables

-include VoteImpl

ifeq (${LOCKVIMPL},${VIMPL})			# same implementation type as last time ?
${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@
else						# implementation type has changed => rebuilt
.PHONY : ${EXEC}
${EXEC} :
	rm -f VoteImpl
	touch q3tallyVotes.h
	${MAKE} ${EXEC} VIMPL="${VIMPL}"
endif

VoteImpl :
	echo "LOCKVIMPL=${VIMPL}" > VoteImpl
	sleep 1

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d ${OBJECTS} ${EXEC} VoteImpl
