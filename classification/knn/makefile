SRC_DIR=$(HOME)/ml

CXX=g++
#CXXFLAGS=-c -g -W -I$(SRC_DIR)/utility -I.
CXXFLAGS=-c -O2 -W -I$(SRC_DIR)/utility -I. -DCHECK_IO

TESTER=KNearestNeighboursTester
SRCS=NMinHeap.cpp	\
	InputDataSet.cpp	\
	KNearestNeighbours.cpp	\
	$(SRC_DIR)/utility/Timing.cpp	\
	test/$(TESTER).cpp

OBJS=NMinHeap.o	\
	InputDataSet.o	\
	KNearestNeighbours.o	\
	$(SRC_DIR)/utility/Timing.o	\
	test/$(TESTER).o

.PHONY: knn
$(TESTER): $(OBJS)
	$(CXX) $^ -o $@

$(OBJS): %.o : %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	rm $(OBJS) $(TESTER)
