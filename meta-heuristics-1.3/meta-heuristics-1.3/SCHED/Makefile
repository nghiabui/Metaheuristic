CXX = mpic++
OBJSPATH = ../OBJ/
CXXFLAGS = -Wall -D_DEBUG=0 -O -g
MPIFLAGS = -Wall -D_DEBUG=0 -O -g -DCOMM_MPI
LDFLAGS = -O -g -L${OBJSPATH} -lsched  -lmetasearch

SOURCES =	sched2Opt.cpp\
			schedSwap.cpp\
			schedInsertion.cpp\
			schedSolution.cpp\
			schedSolution.cpp\
			schedGenne.cpp\
			schedFitness.cpp\
			schedSolution.cpp\
			schedProblem.cpp\
			
			
OBJS=$(patsubst %.cpp,${OBJSPATH}%.o,${SOURCES})

schedHCSearchSource = schedHCSearch.cpp
schedSASearchSource = schedSASearch.cpp
schedTSSearchSource = schedTSSearch.cpp
schedVNSSearchSource = schedVNSSearch.cpp
schedGASearchSource = schedGASearch.cpp
schedMASearchSource = schedMASearch.cpp
schedWF01SearchSource = schedWF01Search.cpp
schedWF02SearchSource = schedWF02Search.cpp
schedWF03SearchSource = schedWF03Search.cpp
schedWF04SearchSource = schedWF04Search.cpp

all: sequential parallel

parallel: CXXFLAGS = ${MPIFLAGS}

sequential: schedTSSearch

parallel: schedMPI01Search 
${OBJSPATH}libsched.a: ${OBJS}
	ar rcs $@ ${OBJS} 

${OBJSPATH}%.o: %.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $<	

${OBJSPATH}%.obj: %.cpp
	${CXX} ${MPIFLAGS} -c -o $@ $<	

	

schedTSSearch:${OBJSPATH}$(schedTSSearchSource:.cpp=.o) ${OBJSPATH}libsched.a
	${CXX} ${CXXFLAGS} -o $@ $< ${LDFLAGS}	
	@mv -f schedTSSearch ../RUN

schedMPI01Search:${OBJSPATH}$(schedWF01SearchSource:.cpp=.obj) ${OBJSPATH}libsched.a
	${CXX} ${CXXFLAGS} -o $@ $< ${LDFLAGS}	
	@mv -f schedMPI01Search ../RUN	


	
clean:
	@rm -rf ${OBJS} *~ libsched.a
	@rm -rf ${OBJSPATH}sched*.o ${OBJSPATH}sched*.obj ${OBJSPATH}*~ ${OBJSPATH}libsched.a 
	@rm -f ../RUN/sched*Search*
