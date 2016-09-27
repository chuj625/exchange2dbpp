
/**
 * 消息包装器
 **/

#include <stdint.h>
#include "DingfuPack.h"
#include "ShenPack.h"

#ifndef __MessageWraper
#define __MessageWraper

class ALLWraper{
public:
	ALLWraper(unsigned int size){
		_size = size;
		_wraper = new char[size];
	}
	~ALLWraper(){
		if(_wraper != NULL){
			delete []_wraper;
		}
	}

	size_t mem_size(){
		return _size;
	}

	char* getHeader(){
		return _wraper;
	}

protected:
	size_t _size;
	char* _wraper;
};

class MessageWraper{
public:
	MessageWraper(unsigned int size){
		_size = size;
		_wraper = new char[sizeof(DF::HEADER)+ size];
		_header = (DF::HEADER*) _wraper;
		_msg = _header->next;
		_head_size = sizeof(DF::HEADER);
	}
	~MessageWraper(){
		if(_wraper!= NULL){
			delete []_wraper;
		}
	}

	char* getBody(){
		return _msg;
	}
	char* getHeader(){
		return _wraper;
	}
	void setType(uint32_t typ){
		_header->setType(typ);
	}
	void setBodyLength(uint32_t bl){
		_header->setBodyLength(bl);
		_body_size = bl;
	}
	uint32_t header_size(){
		return _head_size;
	}
	uint32_t size(){
		return _head_size + _body_size;
	}
	
private:
	size_t _size;		///< 消息体大小
	char* _wraper;		///< wraper
	DF::HEADER* _header;	///< 消息头
	size_t _head_size;	///< 消息头大小
	size_t _body_size;	///< 消息大小
	char* _msg;			///< 消息体
};

class LoginWraper: public ALLWraper
{
public:
	LoginWraper(std::string senderid
			, std::string targetid, uint32_t heartbit, std::string passwd, std::string version)
				: ALLWraper(sizeof(Binary::Head) + sizeof(Binary::End) + sizeof(Binary::Login))
	{
		phead = (Binary::LPHead)_wraper;
		phead->setType(1);
		phead->setBodyLength(sizeof(Binary::Login));
		plogin = (Binary::LPLogin)phead->next;
		//strcpy((char*)(&plogin->senderCompID), "dingfu                  ", 20);
		memcpy(&plogin->senderCompID, "dingfu                  ", 20);
		//strcpy(&plogin->targetCompID, "nothing                 ", 20);
		memcpy(&plogin->targetCompID, "nothing                 ", 20);
		plogin->setHeartBit(30);
		memcpy(&plogin->password, "123456         ", 16);
		memcpy(&plogin->defaultAppVerID, "1.01                             ", 32);
		ptail = (Binary::LPChecksum)plogin->next;
		ptail->setCheckSum(Binary::GenerateCheckSum(_wraper
					, sizeof(Binary::Login)+sizeof(Binary::Head)));
	}

private:
	Binary::LPHead phead;
	Binary::LPLogin plogin;
	Binary::LPChecksum ptail;
};

#endif ///< __MessageWraper
