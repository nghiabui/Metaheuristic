CXX = mpic++
OBJSPATH = ../OBJ/
CXXFLAGS = -D_DEBUG=0 -Wall -O -g
SOURCES= edaSource.cpp

OBJS=${SOURCES:.cpp=.o}
all: libmetasearch.a move

libmetasearch.a: ${OBJS}
	ar rcs $@ ${OBJS}

.cpp\.o:
	${CXX} ${CXXFLAGS} -c -o $@ $<
move:
	@mv -f ${OBJS} ${OBJSPATH}
	@mv -f libmetasearch.a ${OBJSPATH}
clean:
	@rm -rf ${OBJS} *~ libmetasearch.a
	@rm -rf ${OBJSPATH}*~ ${OBJSPATH}libmetasearch.a


