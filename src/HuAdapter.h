
#include <string>
#include <fstream>
#include<cstdio>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <boost/algorithm/string.hpp>
#include <unistd.h>

#include "GlobalParameters.h"
#include "AdapterEX.h"
#include "RealTimeCell.h"

#ifndef __HuAdapter
#define __HuAdapter


bool return_until_it_changed(long& last_time, const std::string& ff){
	long cur;
	int fd;
	struct stat buf;
	while(true){
		FILE * fp;
		fp = fopen(ff.c_str(), "r");
		fd = fileno(fp);
		fstat(fd, &buf);
		cur = buf.st_mtime;
		fclose(fp);
		if(last_time == cur){
			usleep(1000);			///< 微秒
		}
		else{
			printf("curtime [%ld], last_time [%ld]\n", cur, last_time);
			break;
		}
	}
	last_time = cur;
	return true;
}

void analysis(std::string outs, const char* buff, size_t len){
	//stringstream stream(buff);
	std::istringstream input(buff);
	std::string line;
	std::getline(input, line);
	printf("head[%s], [%d]\n", line.c_str(), line.size());
	if(line.size() < 80 || line[0] != 'H' || line[5] != 'R'){
		fprintf(stderr, "get HEADER ERROR [%s]", line.c_str());
		return;
	}
	std::string tim = line.substr(58, 8);
	tim = tim.substr(0, 2) + tim.substr(3,2) + tim.substr(6,2);
	std::string dat = line.substr(49, 8);
	printf("time[%s]\n", tim.c_str());
	printf("date[%s]\n", dat.c_str());
	RealTimeCell rtc;
	rtc.set_tim(tim.c_str());
	rtc.set_dat(dat.c_str());

	std::vector<std::string> res;
	while(std::getline(input, line)){
		res.clear();
		if(strncmp("MD002", line.c_str(), 5) != 0){
			//TODO drop
			continue;
		}
		//TODO MD002
		boost::algorithm::split(res, line, boost::algorithm::is_any_of("|"));
		if(res.size() < 32){
			continue;
		}
		rtc.to_next();
		rtc.set_id(res[1].c_str());			///< 1
		rtc.set_volume(atoi(res[3].c_str()));	///< 3
		rtc.set_money(atof(res[4].c_str()));	///< 4
		rtc.set_pre_close_price(atof(res[5].c_str()));	///<5
		rtc.set_open_price(atof(res[6].c_str()));		///<6
		rtc.set_high(atof(res[7].c_str()));				///<7
		rtc.set_low(atof(res[8].c_str()));				///<8
		rtc.set_newest_price(atof(res[9].c_str()));		///<9
		rtc.set_close_price(atof(res[10].c_str()));		///<10
		rtc.set_buy_price1(atof(res[11].c_str()));		///<11
		rtc.set_buy_volume1(atoi(res[12].c_str()));		///<12
		rtc.set_buy_price2(atof(res[13].c_str()));		///<13
		rtc.set_buy_volume2(atoi(res[14].c_str()));		///<14
		rtc.set_buy_price3(atof(res[15].c_str()));		///<15
		rtc.set_buy_volume3(atoi(res[16].c_str()));		///<16
		rtc.set_buy_price4(atof(res[17].c_str()));		///<17
		rtc.set_buy_volume4(atoi(res[18].c_str()));		///<18
		rtc.set_buy_price5(atof(res[19].c_str()));		///<19
		rtc.set_buy_volume5(atoi(res[20].c_str()));		///< 20
		rtc.set_sell_price1(atof(res[21].c_str()));		///< 21
		rtc.set_sell_volume1(atoi(res[22].c_str()));	///< 22
		rtc.set_sell_price2(atof(res[23].c_str()));		///< 23
		rtc.set_sell_volume2(atoi(res[24].c_str()));	///< 24
		rtc.set_sell_price3(atof(res[25].c_str()));		///< 25
		rtc.set_sell_volume3(atoi(res[26].c_str()));	///< 26
		rtc.set_sell_price4(atof(res[27].c_str()));		///< 27
		rtc.set_sell_volume4(atoi(res[28].c_str()));	///< 28
		rtc.set_sell_price5(atof(res[29].c_str()));		///< 29
		rtc.set_sell_volume5(atoi(res[30].c_str()));	///< 30
		rtc.set_status(res[31].c_str());				///< 31
	}
	rtc.to_string(outs);
}

class HuAdapter: public AdapterEX
{
public:
	HuAdapter(unsigned int size)
			: _buff(NULL), _max(size)
	{
		_buff = new char[size];
	}
	~HuAdapter(){
		if(_buff != NULL){
			delete []_buff;
		}
	}
	bool init(std::string& file_path){
		_file = file_path;
		_cmd = std::string("rm ") + _file;
		return true;
	}

	int read(){
		/**
		 * 检测文件_file，如果存在则处理，处理完成后删除
		 **/
		int num = 0;
		long last_time = 0;
		std::string outs = "";
		while(true){
			outs.clear();
			printf("check files %s\n", _file.c_str());
			return_until_it_changed(last_time, _file);
			printf("file changed %s\n", _file.c_str());
			std::ifstream fin(_file, std::ios::in);
			if(fin.is_open()){
				// 读文件内容
				fin.read(_buff, _max);
				num = strlen(_buff);
				//_buff[20] = '\0';
				//printf("buff:[%s]\n", _buff);
				//printf("buff len:[%d],max[%d], buff size[%d] [%s]\n", num, _max, sizeof(_buff), _buff);
				printf("buff len:[%d],max[%d], buff size[%d]\n", num, _max, sizeof(_buff));
				analysis(outs, _buff, num);
				EXHQ::broadcast_server_p->process_messages(outs);
				fin.close();
			}
			else{
				usleep(EXHQ::FILE_CHECK_INTERVAL);		///< 微秒
			}
		}
		return num;
	}

private:
	std::string _file;			///< 文件路径
	std::string _cmd;			///< rm _file
	char* _buff;
	size_t _max;
};


#endif


