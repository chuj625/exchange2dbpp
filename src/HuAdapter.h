
#include <string>
#include <fstream>
#include<cstdio>

#include "GlobalParameters.h"
#include "AdapterEX.h"
#include "unistd.h"

#ifndef __HuAdapter
#define __HuAdapter

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
		while(true){
			std::ifstream fin(_file, std::ios::in);
			if(fin.is_open()){
				// 读文件内容
				fin.read(_buff, _max);
				num = strlen(_buff);
				//_buff[20] = '\0';
				//printf("buff:[%s]\n", _buff);
				printf("buff len:[%d],max[%d], buff size[%d] [%s]\n", num, _max, sizeof(_buff), _buff);
				fin.close();
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


