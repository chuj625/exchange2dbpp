# ============================================================
# 这个 example 的目录结构是这样
# - Makefile
# - lib
#   |- lib.cpp
#   |- lib.h
# - src
#   |- foo
#      |- foo.cpp
#      |- foo.h
#   |- bar
#      |- bar.cpp
#      |- bar.h
# 其中 main 函数在 foo.cpp 中
# 
# 下面给出编译这样结构的 project 的 Makefile 写法，最终结果是这样
#  * 所有的 .o 文件都被放到 .build 下，并且 .build 下的目录结构与 cpp 文件
#    的目录结构是一样的
#  * bin 文件会被放到 bin/ 文件夹下
#
# 以下 Makefile 最核心的是下面几块
#  * BINS, CPPS, OBJS, DEPS 这几个变量的设置
#  * 编译规则的设定
# ============================================================

#------------------------------------------------------------
# make 的命令行参数可以在这个变量里指定
#  -j 10 表示用十个进程跑 make
#------------------------------------------------------------
MAKEFLAGS = "-j 10"

CC = g++

#------------------------------------------------------------
# 所有包含 cpp 文件的目录
#------------------------------------------------------------
LIB_DIR = lib
FOO_DIR = src
OTHER_LIB_DIR = /home/data/hanzhonghua/real_time_quotes/exchange2db/tools/websocketpp

CFLAGS = -std=c++11 -pthread -I $(LIB_DIR) -Wall -lboost_system -lboost_thread -I $(OTHER_LIB_DIR) -pg
LIBS = -lpthread -lboost_system -lboost_thread

#------------------------------------------------------------
# 所有的 .o 文件都放到这个根目录下
#------------------------------------------------------------
OBJ_DIR = .build

#------------------------------------------------------------
# 所有 bin 文件都放到这个根目录下
#------------------------------------------------------------
BIN_DIR = bin

BINS = $(BIN_DIR)/exchange2db_server
all: $(BINS)

#------------------------------------------------------------
# $(CPPS) 展开为 lib/lib.cpp src/foo/foo.cpp src/bar/bar.cpp
# 
# 如果有文件不想被 include 进来，可以这样
# EXCLUDE_CPPS = $(wildcard $(FOO_DIR)/*useless.cpp)
# CPPS = $(filter-out $(EXCLUDE_CPPS), $(CPPS))
#------------------------------------------------------------
CPPS = $(wildcard $(LIB_DIR)/*.cpp) $(wildcard $(FOO_DIR)/*.cpp)

# 一个不错的 debug 手段，会将 CPPS 包含的内容打印出来
$(info CPPS is ${CPPS})

#------------------------------------------------------------
# $(OBJS) 展开为 .build/lib/lib.o .build/src/foo/foo.o .build/src/bar/bar.o
#------------------------------------------------------------
OBJS = $(CPPS:%.cpp=$(OBJ_DIR)/%.o)

#------------------------------------------------------------
# $(DEPS) 展开为 .build/lib/lib.d .build/src/foo/foo.d .build/src/bar/bar.d
#------------------------------------------------------------
DEPS = $(OBJS:.o=.d)

#------------------------------------------------------------
# 下面这个语句会把所有的 .d 文件 include 进来
#
# 前面的 - 号，是为了表示如果 $(DEPS) 不存在这里不会报错，主要用在第一次运行
# make 时，因为第一次运行时，那些 .d 文件都是不存在的
#------------------------------------------------------------
-include $(DEPS)

# ------------------------------------------------------------
# 这个 rule 的意思是对于所有你见到的 .o 文件都用如下规则生成
#
# 这里为什么需要 $(OBJ_DIR) 呢？原因是这样，'%' 相当于一个 wildcard，在字符串
# 匹配中会被替换成相应的子串，如果用 'xx%xx' 匹配 'xx111xx' 的话，'%' 就表示
# '111'，对于这里的 case，如果只写 %.o: %.cpp 的话，为了生成 .build/lib/lib.o，
# '%' 会被替换成 '.build/lib/lib' 而 '.build/lib/lib.cpp' 是不存在的，而如果写成
# '$(OBJ_DIR)/%.o'，则 '%' 会被替换成 'lib/lib'，这样就对了
# 
# 另一种常用的基于 pattern matching 的做法是
#  $(OBJ_DIR)/foo_test $(OBJ_DIR)/bar_test: $(OBJ_DIR)/%_test: %.cpp %_test.cpp
# 这种写法显式指定 $(OBJ_DIR)/foo_test $(OBJ_DIR)/bar_test 这几个文件按
# $(OBJ_DIR)/%_test: %.cpp %_test.cpp 的规则来匹配
# ------------------------------------------------------------
$(OBJ_DIR)/%.o: %.cpp
	@#------------------------------------------------------------
	@# mkdir 是必须的，make 不会自动创建你需要的文件夹，$(@D) 表示目标所在
	@# 目录，对于 '.build/lib/lib.cpp' 目录就是 '.build/lib/'
	@#------------------------------------------------------------
	@mkdir -p $(@D)
	@#------------------------------------------------------------
	@# 下面这个命令用于创建 .d 文件，比如 .build/src/foo/foo.d 会包含如下内容
	@#   .build/src/foo/foo.o: src/foo/foo.cpp lib/lib.h src/bar/bar.h
	@# -MM 表示要创建 .d 文件
	@# -MT $@ 表示 .d 文件中的目标会设为 $(OBJ_DIR)/%.o 展开后的结果，如
	@#     .build/src/foo/foo.o 否则会是去掉目录后的 .o 文件，如 foo.o
	@# -MF 表示输出到指定的 .d 文件中
	@#------------------------------------------------------------
	$(CC) $(CFLAGS) -MM -MT $@ -MF $(patsubst %.o,%.d,$@) $<
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/exchange2db_server: $(OBJS)
	@mkdir -p $(@D)
	$(CC) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	rm -rf $(BINS) $(OBJ_DIR)
