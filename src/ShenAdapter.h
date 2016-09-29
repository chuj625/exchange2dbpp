
#include <string>
#include <iostream>
#include <fstream>

#include "GlobalParameters.h"
#include "MessageWraper.h"
#include "AdapterEX.h"
#include "SocketDF.h"

#ifndef __ShenAdapter
#define __ShenAdapter

int shen_cnt = 0;
void SplitWithFlag(const char* str1,char c,std::vector<std::string>& arr)  
{  
    std::stringstream ss(str1);  
	std::string str;    
    while(getline(ss,str,c))    
    {    
        arr.push_back(str);    
    }    
}  

class ShenAdapter: public AdapterEX
{
public:
	ShenAdapter(unsigned int size)
			: _wraper(size), _sock()
	{
	}
	bool init(std::string& ip_port){
		std::vector<std::string> ip;
		SplitWithFlag(ip_port.c_str(), ':', ip);
		std::string ips = ip[0];
		int port = atoi(ip[1].c_str());
		// connect
		if(! _sock.connects(ips, port)) {
			fprintf(stderr, "Error while connect to %s:%d\n", ips.c_str(), port);
			return false;
		}
		// login
		LoginWraper login("dingfu", "shenid", 30, "111111", "1.01");
		_sock.writes(login.getHeader(), login.mem_size());
		return true;
	}

	int read(){
		char* buff = new char[EXHQ::MAX_MSG_LENGTH];
		while(true){
			size_t n = _sock.reads(buff, EXHQ::MAX_MSG_LENGTH);
			//printf("shen read [%d]\n", n);
			analysis(buff, n);
			// hear beat
			_sock.writes(HEARTBEAT.getHeader(), HEARTBEAT.mem_size());
		}
		if(buff != NULL){
			delete []buff;
		}
	}

	bool analysis(char* buff, size_t len){
		/**
		 * MsgTyp
		 * 1: 登录消息
		 * 2: 注销消息
		 * 3，心跳消息
		 * 390094，重传消息
		 * 390093，用户信息报告消息
		 * 390090，快照行情频道统计
		 * 8，业务拒绝消息
		 * 390019，实时行情
		 * 390013，证券实时状态
		 * 390012，公告
		 * 3xxx11，快照行情
		 * 30xx91，逐笔成交行情
		 *
		 **/
		Binary::LPHead h = (Binary::LPHead)buff;
		//printf("shen read type[%d], bodylength[%d], totalen[%d]\n", h->getType(), h->getBodyLength(), len);
		printf("shen read type[%d], bodylength[%d], totalen[%d]\n", h->getType(), h->getBodyLength(), len);
		char bb[1024];
		sprintf(bb, "./log/m%d.%d", h->getType(), shen_cnt++);
		std::string ff(bb);
		std::fstream fout(ff,std::ios::out|std::ios::binary);
		if (!fout.is_open()){
			printf("open error: %s\n", ff.c_str());
		}
		else{
			fout.write(buff, len);
			fout.close();
		}
		//fprintf(stdout, "%s\n", buff, len);
		fprintf(stdout, "%s\n", buff);
		return true;
	}

	const char* header(){
		return _wraper.getHeader();
	}

	void setBodyLength(uint32_t bl){
		_wraper.setBodyLength(bl);
	}

	size_t size(){
		return _wraper.size();
	}

private:
	SocketDF _sock;
	MessageWraper _wraper;
};


#endif


