SRC_DIR=$(HOME)/ml

CXX=g++
CXXFLAGS=-c -g -W -I$(SRC_DIR)/common -I$(SRC_DIR)/utility -I. -DAP_DBG
#CXXFLAGS=-c -W -I$(SRC_DIR)/common -I$(SRC_DIR)/utility -I. -O2

TESTER=AprioriTester
SRCS=$(SRC_DIR)/common/IndexOutOfBound.cpp	\
	$(SRC_DIR)/utility/Timing.cpp	\
	Apriori.cpp	\
	test/$(TESTER).cpp

OBJS=$(SRC_DIR)/common/IndexOutOfBound.o	\
	$(SRC_DIR)/utility/Timing.o	\
	Apriori.o	\
	test/$(TESTER).o

.PHONY: apriori
$(TESTER): $(OBJS)
	$(CXX) $^ -o $@

$(OBJS): %.o : %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TESTER)
