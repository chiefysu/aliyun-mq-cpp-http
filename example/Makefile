TOPDIR := ..
SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRCS))
TARGET := $(patsubst %.cpp, $(TOPDIR)/example/%, $(SRCS))

CXXFLAGS := -g -Wall -fPIC -Wno-deprecated -fstack-protector -O1 -DONS -std=c++11
CPPFLAGS := -I$(TOPDIR)/include 

# LDLIBS := -Wl,-Bstatic -llog4cpp -Wl,-Bdynamic -lrocketmq -lpthread
ifeq ($(static), 1)
LDLIBS := -Wl,-static -lonsclient4cpp -lrestclientcpp -L$(TOPDIR)/lib/lib-boost-static/ -Wl,-Bdynamic  -lpthread -ldl -lrt -lcurl -lcrypto
else
LDLIBS :=  -lonsclient4cpp -L$(TOPDIR)/lib/lib-boost-share/ -lboost_system -lboost_thread -lboost_chrono -lboost_filesystem -lpthread -ldl -lrt 
endif
##====================================================================
all:$(TARGET)

%:%.o
	$(CXX) $(LDFLAGS) -o $@ $<  $(LIBPATH) $(LDLIBS)

%.o:%.cpp
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $<


clean:
	$(RM) -rf $(TARGET)
	$(RM) -rf $(OBJS)



