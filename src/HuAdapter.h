
#include <string>
#include <fstream>
#include<cstdio>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

#include "GlobalParameters.h"
#include "AdapterEX.h"
#include "unistd.h"

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
		while(true){
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
				fin.close();
				/*
				// TODO 处理掉文件
				const char *sysCommand = _cmd.data();
				FILE *fp;
				if ((fp = popen(sysCommand, "r")) == NULL) {  
					printf("rm failed\n");
				}
				else{
					char ln[1024];
					while (fgets(ln, sizeof(ln)-1, fp) != NULL){
						printf("%s\n", ln);
					}
					pclose(fp);
					printf("rm over\n");
				}
				// */
			}
			else{
				//usleep(100000);		///< 微秒
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


