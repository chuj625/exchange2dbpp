g++ -g -DHAVE_NETINET_IN_H -I. -I/usr/local/include/thrift -L/usr/local/lib IDManager_thrift.cpp IDManager_thrift_types.cpp IDManager_thrift_constants.cpp IDManager_thrift_server.skeleton.cpp -o server -lthrift