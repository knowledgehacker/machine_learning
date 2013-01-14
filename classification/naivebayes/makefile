SRC_DIR=$(HOME)/ml

CXX=g++
CXXFLAGS=-c -g -W -I$(SRC_DIR)/common -I$(SRC_DIR)/utility -I/usr/include -I. -DNBC_DEBUG -DUSE_VECTOR
#CXXFLAGS=-c -O2 -W -I$(SRC_DIR)/common -I$(SRC_DIR)/utility -I/usr/include -I.

TESTER=OfflineNBCTester
SRCS=$(SRC_DIR)/common/IndexOutOfBound.cpp	\
	$(SRC_DIR)/utility/Timing.cpp	\
	$(SRC_DIR)/utility/Utils.cpp	\
	Vector.cpp	\
	InputDataSet.cpp	\
	FileTokenizer.cpp	\
	OfflineNaiveBayesClassifier.cpp	\
	test/$(TESTER).cpp

OBJS=$(SRC_DIR)/common/IndexOutOfBound.o	\
	$(SRC_DIR)/utility/Timing.o	\
	$(SRC_DIR)/utility/Utils.o	\
	Vector.o	\
	InputDataSet.o	\
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
