
#include "json.hpp"
#include <iostream>
#include <fstream>

#ifndef ConfigEX_H
#define ConfigEX_H

class ConfigEX{
public:
	bool init(const char* pp){
		std::ifstream ff(pp);
		if(!ff.is_open()){
			std::cerr<<"open conf error [" << pp << "]" << std::endl;
			return false;
		}
		try{
			std::string a = "";
			std::string line = "";
			while(std::getline(ff, line)){
				a += line;
			}
			nlohmann::json js;
			js = nlohmann::json::parse(a);
			std::cout << js.dump(4) << std::endl;
			// parse
			hu_file = js["hu_file"];
			shen_ip_port = js["shen_ip_port"];
			idcache_ip = js["idcache_ip"];
			idcache_port = js["idcache_port"];
			broadcast_port = js["broadcast_port"];
			hu_file_check_interval = js["hu_file_check_interval"];
			hu_file_read_waitime = js["hu_file_read_waitime"];
			// */
		}catch(const std::exception& ex){
			std::cerr<<"load conf error [" << pp << "]: " << ex.what()<< std::endl;
			return false;
		}
		std::cerr<< "load conf over["<< pp<< "]" <<std::endl;
		return true;
	}
public:
	std::string hu_file;
	std::string shen_ip_port;
	std::string idcache_ip;
	int idcache_port;
	int broadcast_port;

	int hu_file_check_interval;
	int hu_file_read_waitime;
};

#endif

