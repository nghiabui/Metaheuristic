echo "start"
mpic++ -D_DEBUG=1 -Wall -O -c -g -o edaSource.o edaSource.cpp
mpic++ -D_DEBUG=1 -Wall -O -c -g -o edaSeqWorkflow.o edaSeqWorkflow.cpp
rm ../OBJ/libmetasearch.a
ar -rcs ../OBJ/libmetasearch.a edaSource.o edaSeqWorkflow.o
rm edaSource.o
rm edaSeqWorkflow.o