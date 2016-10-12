//#include "../gen-cpp/IDManager_thrift.h"
#include "IDManager_thrift.h"
#include<iostream>
#include <string>
#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>

#ifndef __IDManagerClient_H
#define __IDManagerClient_H

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using boost::shared_ptr;
using namespace std;
class IDManagerClient{
	public:
		const static string cla;
		//string host;
		//string port;
		//boost::shared_ptr<TTransport> transport;
		IDManager_thriftClient *client;
	public:
		IDManagerClient(){
			client=NULL;
		}
		void init(const string& host,const int& port){
			boost::shared_ptr<TSocket> socket(new TSocket(host,port));
			boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
			//boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
			boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
			IDManager_thriftClient *i_client=new IDManager_thriftClient(protocol);
			client =i_client;
			transport->open();

		}
		string get_id(const string& qu){
			string id_return;
			client->getid(id_return,cla,qu);
			return id_return;
		}
		~IDManagerClient(){
			delete client;
			//transport->close();
		}
};
const string IDManagerClient::cla="business.stock_ticker_symbol";

#endif ///< __IDManagerClient_H
