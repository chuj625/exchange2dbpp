#include "IDManagerClient.h"
#include <map>

#ifndef IDCache_H
#define IDCache_H

class IDCache{
	public:
		map<string,string>  compid;
		IDManagerClient *idmc;
	public:
		IDCache(){
			idmc=NULL;
		}
		void init(const string& host,const int& port){
			idmc =new IDManagerClient();
			idmc->init(host,port);
			
		}
		string get_company_id(const string& qu){
			if(compid.find(qu)==compid.end()){
				compid[qu]=idmc->get_id(qu);
			}
			return compid[qu];
			
		}

		~IDCache(){
			delete idmc;
		}
};

#endif

