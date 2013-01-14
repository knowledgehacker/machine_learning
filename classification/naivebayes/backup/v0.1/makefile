CXX=g++
CXXFLAGS=-c -g -W -I. -I../common -I../utility -I/usr/include
#CXXFLAGS=-c -W -I../utility -I. -O2

TESTER=OfflineNBCTester
SRCS=../common/IndexOutOfBound.cpp	\
	Document.cpp	\
	Vector.cpp	\
	InputDataSet.cpp	\
	../utility/Timing.cpp	\
	../utility/Utils.cpp	\
	FileTokenizer.cpp	\
	OfflineNaiveBayesClassifier.cpp	\
	test/$(TESTER).cpp

OBJS=../common/IndexOutOfBound.o	\
	Document.o	\
	Vector.o	\
	InputDataSet.o	\
	../utility/Timing.o	\
	../utility/Utils.o	\
	FileTokenizer.o	\
	OfflineNaiveBayesClassifier.o	\
	test/$(TESTER).o

.PHONY: nbc
$(TESTER): $(OBJS)
	$(CXX) $^ -o $@

$(OBJS): %.o : %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TESTER)
