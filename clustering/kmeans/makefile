SRC_DIR=$(HOME)/ml

CXX=g++
CXXFLAGS=-c -g -W -I$(SRC_DIR)/common -I$(SRC_DIR)/utility -I. #-DOKM_DBG
#CXXFLAGS=-c -W -I$(SRC_DIR)/common -I$(SRC_DIR)/utility -I. -O2

TESTER=OfflineKMeansTester
SRCS=$(SRC_DIR)/common/IndexOutOfBound.cpp	\
	$(SRC_DIR)/common/FloatDataReader.cpp	\
	$(SRC_DIR)/common/Samples.cpp	\
	$(SRC_DIR)/utility/Timing.cpp	\
	$(SRC_DIR)/utility/Utils.cpp	\
	OfflineKMeans.cpp	\
	test/$(TESTER).cpp

OBJS=$(SRC_DIR)/common/IndexOutOfBound.o	\
	$(SRC_DIR)/common/FloatDataReader.o	\
	$(SRC_DIR)/common/Samples.o	\
	$(SRC_DIR)/utility/Timing.o	\
	$(SRC_DIR)/utility/Utils.o	\
	OfflineKMeans.o	\
	test/$(TESTER).o

.PHONY: okm
$(TESTER): $(OBJS)
	$(CXX) $^ -o $@

$(OBJS): %.o : %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TESTER)
