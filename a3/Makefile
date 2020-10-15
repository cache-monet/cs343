OPT:= # -O2 -nodebug
SIMPL:=CBEGIN
TYPE:=int
BIMPL:=NOBUSY
SENTINEL:=-1

CXX = u++					# compiler
CXXFLAGS = -g -multi ${OPT} -Wall -Wextra -MMD -D"${SIMPL}" -D"${BIMPL}" \
		   -DTYPE="${TYPE}" -DSENTINEL="${SENTINEL}" # compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS1 = # object files forming 1st executable with prefix "q2"
EXEC1 = quicksort				# 1st executable name

OBJECTS2 = # object files forming 2nd executable with prefix "q3"
EXEC2 = buffer					# 2nd executable name

OBJECTS = ${OBJECTS1} ${OBJECTS2}		# all object files
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
EXECS = ${EXEC1} ${EXEC2}			# all executables

#############################################################

.PHONY : all clean

all : ${EXECS}					# build all executables

-include QuickImpl

ifeq (${shell if [ "${IMPLTYPE}" = "${TYPE}" -a "${IMPLSIMPL}" = "${SIMPL}" ] ; \
		then echo true ; fi },true)
${EXEC1} : ${OBJECTS1}
	${CXX} ${CXXFLAGS} $^ -o $@
else
.PHONY : ${EXEC1}
${EXEC1} :
	rm -f QuickImpl
	touch q2quicksort.h
	${MAKE} ${EXEC1} SIMPL="${SIMPL}" TYPE="${TYPE}"
endif

QuickImpl :
	echo "IMPLSIMPL=${SIMPL}\nIMPLTYPE=${TYPE}" > QuickImpl
	sleep 1

-include BufImpl

ifeq (${BUFIMPL},${BIMPL})			# same implementation type as last time ?
${EXEC2} : ${OBJECTS2}
	${CXX} ${CXXFLAGS} $^ -o $@
else
.PHONY : ${EXEC2}
${EXEC2} :
	rm -f BufImpl
	touch q3buffer.h
	sleep 1
	${MAKE} ${EXEC2} BIMPL="${BIMPL}"
endif

BufImpl :
	echo "BUFIMPL=${BIMPL}" > BufImpl
	sleep 1

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${EXECS} QuickImpl BufImpl