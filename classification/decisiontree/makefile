SRC_DIR=$(HOME)/ml

CXX=g++
#CXXFLAGS=-c -g -W -I. -I$(SRC_DIR)/common -I$(SRC_DIR)/utility
CXXFLAGS=-c -W -I$(SRC_DIR)/common -I$(SRC_DIR)/utility -I. -O2

TESTER=DecisionTreeTester
SRCS=$(SRC_DIR)/common/IndexOutOfBound.cpp	\
	$(SRC_DIR)/common/StringDataReader.cpp	\
	$(SRC_DIR)/common/Samples.cpp	\
	$(SRC_DIR)/utility/Timing.cpp	\
	$(SRC_DIR)/utility/Utils.cpp	\
	Node.cpp	\
	DecisionTree.cpp	\
	test/$(TESTER).cpp

OBJS=$(SRC_DIR)/common/IndexOutOfBound.o	\
	$(SRC_DIR)/common/StringDataReader.o	\
	$(SRC_DIR)/common/Samples.o	\
	$(SRC_DIR)/utility/Timing.o	\
	$(SRC_DIR)/utility/Utils.o	\
	Node.o	\
	DecisionTree.o	\
	test/$(TESTER).o

.PHONY: dt
$(TESTER): $(OBJS)
	$(CXX) $^ -o $@

$(OBJS): %.o : %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TESTER)
