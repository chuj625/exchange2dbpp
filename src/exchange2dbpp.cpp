#include <cstdio>
#include <iostream>
#include <string>
#include <thread>

#include "GlobalParameters.h"
#include "BroadcastEX.h"
#include "ShenAdapter.h"
#include "HuAdapter.h"
#include "ConfigEX.h"

using namespace std;


/// {{{ 子任务
/**
 * 深交所socket 线程
 **/
void thread_shen(const ConfigEX & conf){
	//string ip = "127.0.0.1:9898";
	string ip = conf.shen_ip_port;
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
void thread_hu(const ConfigEX & conf){
	//string files = "/home/data/hanzhonghua/real_time_quotes/exchange2db/input/mktdt00.txt";
	//string files = "/home/data/hanzhonghua/real_time_quotes/tools/level1_hu/data/mktdt00.txt";
	//string files = "/home/data/hanzhonghua/real_time_quotes/exchange2db/input/mktdt00.txt";
	string files = conf.hu_file;
	HuAdapter hu(EXHQ::MAX_MSG_LENGTH);
	if(!hu.init(files)){
		fprintf(stderr, "open hu adapter error");
		exit(-1);
	}
	hu.set_hu_file_check_interval(conf.hu_file_check_interval);
	hu.set_hu_file_read_waitime(conf.hu_file_read_waitime);
	hu.read();
}

/// }}}

int main(int argc, char* argv[]){
	ConfigEX conf;
	if(argc != 2 || !conf.init(argv[1])){
		std::cerr<< argv[0] << " config" <<endl;
		exit(-1);
	}
	EXHQ::IDCache_p = new IDCache();
	//EXHQ::IDCache_p->init("192.168.2.7",7777);
	cout<< conf.idcache_ip << ":" << conf.idcache_port <<endl;
	EXHQ::IDCache_p->init(conf.idcache_ip, conf.idcache_port);
	// broadcast server
	EXHQ::broadcast_server_p = new EXHQ::BroadcastEX();

	// client to get message
	std::thread t_shen(thread_shen, conf);
	std::thread t_hu(thread_hu, conf);

	EXHQ::broadcast_server_p->run(conf.broadcast_port);
	t_shen.join();
	t_hu.join();
	// */
	
	return 0;
}

// */
