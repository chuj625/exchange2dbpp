
#ifndef __AdapterEX
#define __AdapterEX

class AdapterEX{
public:
	/**
	 * 初始化方法，目前只有一个参数
	 **/
	virtual bool init(std::string& argv) = 0;

	/**
	 * read 子类实现
	 * return <0: error, return is lens of read, not length of message
	 **/
	virtual int read()=0;
};

//class ShenAdapter: public AdapterEX
//{
//public:
//	ShenAdapter(unsigned int size)
//			: _wraper(size){
//		;
//	}
//
//	/**
//	 * 初始化方法，目前只有一个参数
//	 **/
//	bool init(std::string& argv) = 0;
//
//	/**
//	 * read 子类实现
//	 * return <0: error, return is lens of read, not length of message
//	 **/
//	int read()=0;
//
//	const char* header(){
//		return _wraper.getHeader();
//	}
//
//	void setBodyLength(uint32_t bl){
//		_wraper.setBodyLength(bl);
//	}
//
//	size_t size(){
//		return _wraper.size();
//	}
//
//private:
//	MessageWraper _wraper;
//}

#endif	///< __AdapterEX

