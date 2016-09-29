#include <cstdio>
#include <iostream>
#include <string>
#include <thread>

#include "GlobalParameters.h"
#include "BroadcastEX.h"
#include "ShenAdapter.h"
#include "HuAdapter.h"

using namespace std;


/// {{{ 子任务
/**
 * 深交所socket 线程
 **/
void thread_shen(){
	string ip = "127.0.0.1:9898";
	ShenAdapter shen(EXHQ::MAX_MSG_LENGTH);
	if(shen.init(ip) == false){
		fprintf(stderr, "open shen adapter error");
		exit(-1);
	}
	shen.read();
}

/**
 * 上交所文件监控线程
 **/
void thread_hu(){
	//string files = "/home/data/hanzhonghua/real_time_quotes/exchange2db/input/mktdt00.txt";
	string files = "/home/data/hanzhonghua/real_time_quotes/tools/level1_hu/data/mktdt00.txt";
	HuAdapter hu(EXHQ::MAX_MSG_LENGTH);
	if(!hu.init(files)){
		fprintf(stderr, "open hu adapter error");
		exit(-1);
	}
	hu.read();
}

/// }}}

int main(){
	// broadcast server
	EXHQ::broadcast_server_p = new EXHQ::BroadcastEX();

	// client to get message
	std::thread t_shen(thread_shen);
	t_shen.join();

	/*
	std::thread t_hu(thread_hu);
	t_hu.join();
	// */
	
	EXHQ::broadcast_server_p->run(1986);
	return 0;
}

// */
