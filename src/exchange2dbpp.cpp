#include <cstdio>
#include <iostream>
#include <thread>

#include "GlobalParameters.h"
#include "BroadcastEX.h"
#include "ShenAdapter.h"

using namespace std;


/// {{{ 子任务
/**
 * 深交所socket 线程
 **/
void thread_shen(){
	string ip = "127.0.0.1:9898";
	ShenAdapter shen(102400);
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
	;
}

/// }}}

int main(){
	// broadcast server
	EXHQ::broadcast_server_p = new EXHQ::BroadcastEX();

	// client to get message
	std::thread t_shen(thread_shen);
	std::thread t_hu(thread_hu);

	t_shen.join();
	t_hu.join();
	
	EXHQ::broadcast_server_p->run(1986);
	return 0;
}

// */
