CXX=g++
CXXFLAGS=-c -g -W -I. -I../../common -I../../utility
#CXXFLAGS=-c -W -I../utility -I. -O2

#TESTER=InputDataSetTester
TESTER=DecisionTreeTester
SRCS=../../common/IndexOutOfBound.cpp	\
	../../utility/Timing.cpp	\
	../../utility/Utils.cpp	\
	InputDataSet.cpp	\
	Item.cpp	\
	Node.cpp	\
	DecisionTree.cpp	\
	test/$(TESTER).cpp

OBJS=../../common/IndexOutOfBound.o	\
	../../utility/Timing.o	\
	../../utility/Utils.o	\
	InputDataSet.o	\
	Item.o	\
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
