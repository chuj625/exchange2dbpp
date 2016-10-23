
#include <string>
#include <iostream>
#include <fstream>

#include "GlobalParameters.h"
#include "MessageWraper.h"
#include "AdapterEX.h"
#include "SocketDF.h"
#include "RealTimeCell.h"

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

void ana300111(std::string& outs, char* bbuff){
	RealTimeCell rtc;
	char tbuf[1024];
	Binary::LPSnapshot psnapshot = (Binary::LPSnapshot)bbuff;
	std::string dt = psnapshot->origTime.get();
	rtc.set_tim(dt.substr(8,6).c_str());
	rtc.set_dat(dt.substr(0,8).c_str());
	rtc.set_src("shen");
	rtc.set_typ("rt");
	rtc.to_next();
	//cout<< "origTime:\t" <<  psnapshot->origTime.get() <<endl;
	//cout<< "channelNo:\t" << psnapshot->getChannelNo()<<endl;
	psnapshot->getMDStreamID(tbuf);
	//cout<< "mdStreamID:\t" << tbuf <<endl;
	psnapshot->securityID.get(tbuf);
	rtc.set_id(tbuf);
	string ticker(tbuf);
	string df = EXHQ::IDCache_p->get_company_id(ticker);
	rtc.set_company_id(df.c_str());
	//cout<< "securityID:\t" << tbuf <<endl;
	psnapshot->getSecurityIDSource(tbuf);
	//cout<< "securityIDSource:\t" << tbuf <<endl;
	psnapshot->getTradingPhaseCode(tbuf);
	//cout<< "tradingPhaseCode:\t" << tbuf<<endl;
	//cout<< "prevClosePx:\t" << psnapshot->prevClosePx.get()<<endl;
	rtc.set_pre_close_price(psnapshot->prevClosePx.get());
	//cout<< "numTrades:\t"<< psnapshot->getNumTrades()<<endl;
	rtc.set_transactions(psnapshot->getNumTrades());
	//cout<< "totalVolumeTrade\t"<< psnapshot->totalVolumeTrade.get()<<endl;
	rtc.set_volume(psnapshot->totalVolumeTrade.get());
	//cout<< "totalValueTrade\t" << psnapshot->totalValueTrade.get()<<endl;
	rtc.set_money(psnapshot->totalValueTrade.get());
	
	Binary::LPSN300111 sn300111 = (Binary::LPSN300111)psnapshot->next;
	uint32_t exnum = sn300111->getNoMDEntries();
	Binary::LPSN300111Ext ext = sn300111->data;
	for(uint32_t i =0; i< exnum; i++){
		//cout << "******************************" <<endl;
		ext->getMDEntryType(tbuf);
		switch(tbuf[0])
		{
		case '0':	///< buy
			switch(ext->getMDPriceLevel()){
				case 1:
					rtc.set_buy_price1(ext->getMDEntryPx());
					rtc.set_buy_volume1(ext->mdEntrySize.get());
					break;
				case 2:
					rtc.set_buy_price2(ext->getMDEntryPx());
					rtc.set_buy_volume2(ext->mdEntrySize.get());
					break;
				case 3:
					rtc.set_buy_price3(ext->getMDEntryPx());
					rtc.set_buy_volume3(ext->mdEntrySize.get());
					break;
				case 4:
					rtc.set_buy_price4(ext->getMDEntryPx());
					rtc.set_buy_volume4(ext->mdEntrySize.get());
					break;
				case 5:
					rtc.set_buy_price5(ext->getMDEntryPx());
					rtc.set_buy_volume5(ext->mdEntrySize.get());
					break;
			}
			break;
		case '1':	///< sell
			switch(ext->getMDPriceLevel()){
				case 1:
					rtc.set_sell_price1(ext->getMDEntryPx());
					rtc.set_sell_volume1(ext->mdEntrySize.get());
					break;
				case 2:
					rtc.set_sell_price2(ext->getMDEntryPx());
					rtc.set_sell_volume2(ext->mdEntrySize.get());
					break;
				case 3:
					rtc.set_sell_price3(ext->getMDEntryPx());
					rtc.set_sell_volume3(ext->mdEntrySize.get());
					break;
				case 4:
					rtc.set_sell_price4(ext->getMDEntryPx());
					rtc.set_sell_volume4(ext->mdEntrySize.get());
					break;
				case 5:
					rtc.set_sell_price5(ext->getMDEntryPx());
					rtc.set_sell_volume5(ext->mdEntrySize.get());
					break;
			}
			
			break;
		case '2':
			rtc.set_newest_price(ext->getMDEntryPx());
			break;
		case '4':
			rtc.set_open_price(ext->getMDEntryPx());
			break;
		case '7':
			rtc.set_high(ext->getMDEntryPx());
			break;
		case '8':
			rtc.set_low(ext->getMDEntryPx());
			break;
		case 'x':
		default:
			break;
		}
		//cout << "**mdEntryType\t" << tbuf<<endl;
		//cout << "**mdEntryPx\t" << ext->getMDEntryPx()<<endl;
		//cout << "**mdEntrySize\t"<< ext->mdEntrySize.get()<<endl;
		//cout << "**mdPriceLevel\t"<< ext->getMDPriceLevel()<<endl;
		//cout << "**numberOfOrders\t"<<ext->getNumberOfOrders()<<endl;
		uint32_t noOrders = ext->noOrders.get();
		//cout << "**noOrders\t" << noOrders<<endl;
		for(uint32_t j = 0; j<noOrders; ++j){
			//cout << "------------------------------"<<endl;
			//cout<< "----orderQty:\t" <<  ext->orderQty[j].get()<<endl;
		}
		// TODO next ext
		ext = (Binary::LPSN300111Ext)((char*)ext+ sizeof(Binary::SN300111Ext) + sizeof(Binary::Qty)*noOrders);
	}
	rtc.to_string(outs);
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
		char* obuf = new char[EXHQ::MAX_MSG_LENGTH];
		std::string outs;
		while(true){
			size_t n = _sock.reads(buff, EXHQ::MAX_MSG_LENGTH);
			//printf("shen read [%d]\n", n);
			analysis(outs, buff, n);
			EXHQ::broadcast_server_p->process_messages(outs);
			// hear beat
			_sock.writes(HEARTBEAT.getHeader(), HEARTBEAT.mem_size());
		}
		if(buff != NULL){
			delete []buff;
		}
		if(obuf != NULL){
			delete []obuf;
		}
	}

	bool analysis(std::string& outs, char* buff, size_t len){
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
		if(h->getType() != 300111){
			return false;
		}
		ana300111(outs, h->next);
		EXHQ::broadcast_server_p->process_messages(outs);
		/*
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
		// */
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


