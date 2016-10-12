#include "stdio.h" 
#include "stdlib.h"
#include<iostream> 
#include<string>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

#ifndef __RealTimeCell_h
#define __RealTimeCell_h

class RealTimeCell{
	
	public:
		json result;  //存放解析完毕的json结果
		char buff[128];		// buff
	
	public:
		void to_next();  //初始化一个数组，存放一条数据
		void pop_null();  //删除最后一条数据
		void to_string(string&  str_result);  //将json对象结果转换成string
		void set_typ(const char* ss){         //存放typ 
			result["typ"]=ss;
		}
		void set_tim(const char* ss){         //存放tim
			memcpy(buff, ss, 2);
			memcpy(buff+3, ss + 2, 2);
			memcpy(buff+6, ss + 4, 2);
			buff[2] = buff[5] = ':';
			buff[8] = '\0';
			result["tim"]=buff;
		}
		void set_dat(const char* ss){         //存放dat
			memcpy(buff, ss, 4);		///< YYYY
			memcpy(buff+5, ss+4, 2);	///< MM
			memcpy(buff+8, ss+6, 2);	///< DD
			buff[4] = buff[7] = '-';
			buff[10] = '\0';
			result["dat"]=buff;
		}
		void set_src(const char* src){
			result["src"] = src;
		}

		void set_id(const char* ss){          					//0,产品代码 
			result["res"][result["res"].size()-1][0]=ss;
		}
		void set_volume(int ss){              					//1,成交数量
			result["res"][result["res"].size()-1][1]=ss;
		}                                    
		void set_money(double ss){           				    //2,成交金额   
			result["res"][result["res"].size()-1][2]=ss;
		}
		void set_newest_price(double ss){     					//3,最新价
			result["res"][result["res"].size()-1][3]=ss;
		}
		void set_pre_close_price(double ss){  					//4,昨日收盘价
			result["res"][result["res"].size()-1][4]=ss;
		}
		void set_open_price(double ss){       					//5,开盘价
			result["res"][result["res"].size()-1][5]=ss;
		}
		void set_high(double ss){            					//6,最高价
			result["res"][result["res"].size()-1][6]=ss;
		}
		void set_low(double ss){              					//7,最低价
			result["res"][result["res"].size()-1][7]=ss;
		}
		void set_close_price(double ss){      					//8,收盘价
			result["res"][result["res"].size()-1][8]=ss;
		}
		void set_buy_price1(double ss){       					//9,申买价一
			result["res"][result["res"].size()-1][9]=ss;
		}
		void set_buy_volume1(int ss){							//10,申买量一
			result["res"][result["res"].size()-1][10]=ss;
		}
		void set_buy_price2(double ss){							//11,申买价二
			result["res"][result["res"].size()-1][11]=ss;
		}
		void set_buy_volume2(int  ss){							//12,申买量二
			result["res"][result["res"].size()-1][12]=ss;
		}
		void set_buy_price3(double ss){							//13,申买价三
			result["res"][result["res"].size()-1][13]=ss;
		}
		void set_buy_volume3(int ss){							//14,申买量三
			result["res"][result["res"].size()-1][14]=ss;
		}
		void set_buy_price4(double ss){							//15,申买价四
			result["res"][result["res"].size()-1][15]=ss;
		}
		void set_buy_volume4(int ss){							//16,申买价四
			result["res"][result["res"].size()-1][16]=ss;
		}
		void set_buy_price5(double ss){							//17,申买价五
			result["res"][result["res"].size()-1][17]=ss;
		}
		void set_buy_volume5(int ss){							//18,申买量五
			result["res"][result["res"].size()-1][18]=ss;
		}
		void set_sell_price1(double ss){						//19,申卖价一
			result["res"][result["res"].size()-1][19]=ss;
		}
		void set_sell_volume1(int ss){							//20,申卖量一
			result["res"][result["res"].size()-1][20]=ss;
		}
		void set_sell_price2(double ss){						//21,申卖价一
			result["res"][result["res"].size()-1][21]=ss;
		}
		void set_sell_volume2(int ss){							//22,申卖量一
			result["res"][result["res"].size()-1][22]=ss;
		}
		void set_sell_price3(double ss){						//23,申卖价一
			result["res"][result["res"].size()-1][23]=ss;
		}
		void set_sell_volume3(int ss){							//24,申卖量一
			result["res"][result["res"].size()-1][24]=ss;
		}
		void set_sell_price4(double ss){						//25,申卖价一
			result["res"][result["res"].size()-1][25]=ss;
		}
		void set_sell_volume4(int ss){							//26,申卖量一
			result["res"][result["res"].size()-1][26]=ss;
		}
		void set_sell_price5(double ss){						//27,申卖价一
			result["res"][result["res"].size()-1][27]=ss;
		}
		void set_sell_volume5(int ss){							//28,申卖量一
			result["res"][result["res"].size()-1][28]=ss;
		}
		void set_transactions(int ss){							//29,成交笔数  ，深交所独有字段
			result["res"][result["res"].size()-1][29]=ss;
		}
		void set_contract(int ss){								//30,合约持仓量， 深交所独有字段
			result["res"][result["res"].size()-1][30]=ss;
		}
		void set_status(const char* ss){						//31,公司交易状态
			result["res"][result["res"].size()-1][31]=ss;
		}
		void set_company_id(const char* ss){					//32,公司代码
			result["res"][result["res"].size()-1][32]=ss;
		}
		
};

void RealTimeCell::to_next(){
	json j;
	int n=33;
	j.clear();
	while(n>0){
		j.push_back(NULL);
		--n;
	}
	j[0]="";
	j[31]="";
	j[32]="";
	result["res"].push_back(j);
}
void RealTimeCell::pop_null(){

	if(!result["res"].empty()){
		result["res"].erase(result["res"].size()-1);
	}
}

void RealTimeCell::to_string(string&  str_result){
	str_result=result.dump();
}

#endif


