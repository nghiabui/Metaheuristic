CXX = mpic++
OBJSPATH = ../OBJ/
CXXFLAGS = -Wall -D_DEBUG=0 -O -g
MPIFLAGS = -Wall -D_DEBUG=0 -O -g -DCOMM_MPI
LDFLAGS = -O -g -L${OBJSPATH} -ltsp -lmetasearch

SOURCES = 	tsp2Opt.cpp\
			tspInsertion.cpp\
			tspSolution.cpp\
			tspSolution.cpp\
			tspGenne.cpp\
			tspSolution.cpp\
			tspProblem.cpp
	
OBJS=$(patsubst %.cpp,${OBJSPATH}%.o,${SOURCES})

tspHCSearchSource = tspHCSearch.cpp
tspSASearchSource = tspSASearch.cpp
tspTSSearchSource = tspTSSearch.cpp
tspVNSSearchSource = tspVNSSearch.cpp
tspGASearchSource = tspGASearch.cpp
tspMASearchSource = tspMASearch.cpp
tspWF01SearchSource = tspWF01Search.cpp
tspWF02SearchSource = tspWF02Search.cpp
tspWF03SearchSource = tspWF03Search.cpp
tspWF04SearchSource = tspWF04Search.cpp

all: sequential parallel

parallel: CXXFLAGS = ${MPIFLAGS}

sequential: tspHCSearch tspSASearch tspTSSearch tspVNSSearch tspGASearch tspMASearch tspSEQ01Search tspSEQ02Search tspSEQ03Search tspSEQ04Search

parallel: tspMPI01Search tspMPI02Search tspMPI03Search tspMPI04Search tspMPIMASearch

${OBJSPATH}libtsp.a: ${OBJS}
	ar rcs $@ ${OBJS} 

${OBJSPATH}%.o: %.cpp
	${CXX} ${CXXFLAGS} -c -o $@ $<	

${OBJSPATH}%.obj: %.cpp
	${CXX} ${MPIFLAGS} -c -o $@ $<	

	
tspHCSearch:${OBJSPATH}$(tspHCSearchSource:.cpp=.o) ${OBJSPATH}libtsp.a
	${CXX} ${CXXFLAGS} -o $@ $< ${LDFLAGS}	
	@mv -f tspHCSearch ../RUN

tspSASearch:${OBJSPATH}$(tspSASearchSource:.cpp=.o) ${OBJSPATH}libtsp.a
	${CXX} ${CXXFLAGS} -o $@ $< ${LDFLAGS}	
	@mv -f tspSASearch ../RUN

tspTSSearch:${OBJSPATH}$(tspTSSearchSource:.cpp=.o) ${OBJSPATH}libtsp.a
	${CXX} ${CXXFLAGS} -o $@ $< ${LDFLAGS}	
	@mv -f tspTSSearch ../RUN

tspVNSSearch:${OBJSPATH}$(tspVNSSearchSource:.cpp=.o) ${OBJSPATH}libtsp.a
	${CXX} ${CXXFLAGS} -o $@ $< ${LDFLAGS}	
	@mv -f tspVNSSearch ../RUN

tspGASearch:${OBJSPATH}$(tspGASearchSource:.cpp=.o) ${OBJSPATH}libtsp.a
	${CXX} ${CXXFLAGS} -o $@ $< ${LDFLAGS}	
	@mv -f tspGASearch ../RUN

tspMASearch:${OBJSPATH}$(tspMASearchSource:.cpp=.o) ${OBJSPATH}libtsp.a
	${CXX} ${CXXFLAGS} -o $@ $< ${LDFLAGS}	
	@mv -f tspMASearch ../RUN	
	
tspSEQ01Search:${OBJSPATH}$(tspWF01SearchSource:.cpp=.o) ${OBJSPATH}libtsp.a
	${CXX} ${CXXFLAGS} -o $@ $< ${LDFLAGS}	
	@mv -f tspSEQ01Search ../RUN	

tspSEQ02Search:${OBJSPATH}$(tspWF02SearchSource:.cpp=.o) ${OBJSPATH}libtsp.a
	${CXX} ${CXXFLAGS} -o $@ $< ${LDFLAGS}	
	@mv -f tspSEQ02Search ../RUN	
	
tspSEQ03Search:${OBJSPATH}$(tspWF03SearchSource:.cpp=.o) ${OBJSPATH}libtsp.a
	${CXX} ${CXXFLAGS} -o $@ $< ${LDFLAGS}	
	@mv -f tspSEQ03Search ../RUN		

tspSEQ04Search:${OBJSPATH}$(tspWF04SearchSource:.cpp=.o) ${OBJSPATH}libtsp.a
	${CXX} ${CXXFLAGS} -o $@ $< ${LDFLAGS}	
	@mv -f tspSEQ04Search ../RUN	
	
tspMPIMASearch:${OBJSPATH}$(tspMASearchSource:.cpp=.obj) ${OBJSPATH}libtsp.a
	${CXX} ${CXXFLAGS} -o $@ $< ${LDFLAGS}	
	@mv -f tspMPIMASearch ../RUN	
	
tspMPI01Search:${OBJSPATH}$(tspWF01SearchSource:.cpp=.obj) ${OBJSPATH}libtsp.a
	${CXX} ${CXXFLAGS} -o $@ $< ${LDFLAGS}	
	@mv -f tspMPI01Search ../RUN	

tspMPI02Search:${OBJSPATH}$(tspWF02SearchSource:.cpp=.obj) ${OBJSPATH}libtsp.a
	${CXX} ${CXXFLAGS} -o $@ $< ${LDFLAGS}	
	@mv -f tspMPI02Search ../RUN	
	
tspMPI03Search:${OBJSPATH}$(tspWF03SearchSource:.cpp=.obj) ${OBJSPATH}libtsp.a
	${CXX} ${CXXFLAGS} -o $@ $< ${LDFLAGS}	
	@mv -f tspMPI03Search ../RUN		

tspMPI04Search:${OBJSPATH}$(tspWF04SearchSource:.cpp=.obj) ${OBJSPATH}libtsp.a
	${CXX} ${CXXFLAGS} -o $@ $< ${LDFLAGS}	
	@mv -f tspMPI04Search ../RUN
	
clean:
	@rm -rf ${OBJS} *~ libtsp.a
	@rm -rf ${OBJSPATH}tsp*.o ${OBJSPATH}tsp*.obj ${OBJSPATH}*~ ${OBJSPATH}libtsp.a 
	@rm -f ../RUN/tsp*Search*
