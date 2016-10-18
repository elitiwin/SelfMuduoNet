.SUFFIXES: .sh .h .c .cc .cpp
# 程序名
BIN = pro
# 程序依赖object文件，与src目录下的.cc文件同名
OBJ = main.o Hello.o

AR = ar
AR_FLAGS = crs
CXX = g++
CXXFLAGS = -std=c++11 \
			 -ggdb3 \
			 -I ../inc \
			 -I ../lib \
			 -Wall \
			 -Werror
CXX_MACRO	=

BASE_DIR = ../
INC_DIR = $(BASE_DIR)inc
SRC_DIR = $(BASE_DIR)src
OBJ_DIR = obj
LIB_DIR = lib
BIN_DIR = bin

BASE_INC = -I$(INC_DIR)
MUDUO_INC = -I$(BASE_DIR)
MUDUO_LIB = -L$(LIB_DIR) -lmuduo -lpthread

INC = $(BASE_INC) $(MUDUO_INC)
LIB = $(MUDUO_LIB)

MUDUOLIB = libmuduo.a

all: debug release

debug release gprof: dir $(addprefix $(LIB_DIR)/,$(MUDUOLIB)) $(addprefix $(BIN_DIR)/,$(BIN))

dir:
	@-mkdir -p $(OBJ_DIR)
	@-mkdir -p $(LIB_DIR)
	@-mkdir -p $(BIN_DIR)

$(OBJ_DIR)/%.o:$(BASE_DIR)muduo/base/%.cc
	$(CXX) $(CXXFLAGS) $(CXX_MACRO) -o $@ $(MUDUO_INC) -c $<

$(OBJ_DIR)/%.o:$(BASE_DIR)muduo/net/%.cc
	$(CXX) $(CXXFLAGS) $(CXX_MACRO) -o $@ $(MUDUO_INC) -c $<

$(OBJ_DIR)/%.o:$(BASE_DIR)muduo/net/poller/%.cc
	$(CXX) $(CXXFLAGS) $(CXX_MACRO) -o $@ $(MUDUO_INC) -c $<

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) $(CXX_MACRO) -o $@ $(INC) -c $<

$(LIB_DIR)/libmuduo.a:$(addprefix $(OBJ_DIR)/, \
	Thread.o ThreadPool.o CountDownLatch.o Timestamp.o LogFile.o Logging.o \
	FileUtil.o LogStream.o TimeZone.o Date.o ProcessInfo.o Exception.o \
	AsyncLogging.o Timestamp.o Condition.o EventLoop.o TcpServer.o \
	InetAddress.o TcpConnection.o TimerQueue.o Channel.o Socket.o \
	Poller.o DefaultPoller.o Acceptor.o EventLoopThreadPool.o SocketsOps.o \
	Timer.o PollPoller.o EPollPoller.o EventLoopThread.o Buffer.o \
	TcpClient.o Connector.o) 
	$(AR) $(AR_FLAGS) $@ $^

$(BIN_DIR)/$(BIN):$(addprefix $(OBJ_DIR)/, $(OBJ))
	$(CXX) $(CXXFLAGS) $(CXX_MACRO) -o $@ $^ $(LIB)

strip:
	strip $(addprefix $(BIN_DIR)/,$(BIN))

clean:
	rm -rf  $(OBJ_DIR)
	rm -rf  $(LIB_DIR)
	rm -rf  $(BIN_DIR)

