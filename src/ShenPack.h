
#include <arpa/inet.h>
#include <stdint.h>
#include <endian.h>
#include <string>
#include <stdio.h>

namespace Binary
{

int64_t htobe16_signed(int64_t val){
	uint64_t* p = (uint64_t*) & val;
	uint64_t res = htobe64(*p);
	int64_t* q = (int64_t*) & res;
	return *q;
}

#pragma pack(push,1)


/**
 * 价格,N13(4)
 **/
typedef struct Price{
	int64_t val;	///<  N13(4)
	double get(){
		return 0.0001*(double)htobe64(val);
	}
}* LPPrice;

/**
 * 数量,N15(2)
 **/
typedef struct Qty{
	int64_t val; 	///< N15(2)
	double get(){
		return 0.01 * htobe64(val);
	}
}* LPQty;

/**
 * 金额,N18(4)
 **/
typedef struct Amt{
	int64_t val;	///< N18(4)
	double get(){
		return 0.0001 * htobe64(val);
	}
}* LPAmt;


/**
 * 消息序号
 **/
typedef struct SeqNum{
	int64_t val;	///< 消息序列号
	int64_t get(){
		return htobe64(val);
	}
}* LPSeqNum;

/**
 * 1=True/Yes,0=False/No
 **/
typedef struct Boolean{
	uint16_t val;
	bool get(){
		uint16_t vv = htons(val);
		return vv == 1?true:false;
	}
}* LPBoolean;

/**
 * 长度
 * 表示字节为单位的数据长度,正数
 **/
typedef struct Length{
	uint32_t val;
	uint32_t get(){
		uint32_t vv = htonl(val);
		return vv;
	}
}* LPLength;

/**
 * 本地时间戳 YYYYMMDDHHMMSSsss(毫秒),
 * YYYY = 0000-9999, MM = 01-12, DD = 01-31, HH = 00-23, MM = 00-59, SS = 00-60 (秒),sss=000-999 (毫秒)。
 **/
typedef struct LocalTimeStamp{
	int64_t val;
	std::string get(){
		uint64_t vv = htobe64(val);
		char buff[20];
		sprintf(buff, "%llu", vv);
		return std::string(buff);
	}
}* LPLocalTimeStamp;

/**
 * 重复数
 * 表示重复组的个数,正数
 **/
typedef struct NumInGroup{
	uint32_t val;
	uint32_t get(){
		return htonl(val);
	}
}* LPNumInGroup;

/**
 * 本地市场日期
 * 格式 YYYYMMDD,YYYY = 0000-9999, MM = 01-12, DD = 01-31
 **/
typedef struct LocalMktDate{
	uint32_t val;
	std::string get(){
		uint32_t vv = htonl(val);
		char buff[10];
		sprintf(buff, "%lu", vv);
		return std::string(buff);
	}
	void get(char* buff){
		sprintf(buff, "%lu", htonl(val));
	}
		
}* LPLocalMktDate;

/**
 * 证券代码
 **/
typedef struct SecurityID{
	char data[8];
	void get(char* buff){
		snprintf(buff, 7, "%s", data);
	}
}* LPSecurityID;



/**
* @brief Head 包头
*/
typedef struct Head
{
	uint32_t msgType; ///< 消息ID
	uint32_t bodyLength; ///< 包体长度，不包含包头（本结构），不包含包尾（校验）
	char next[0];
	void setType(uint32_t typ){
		msgType = htonl(typ);
	}
	uint32_t getType(){
		return ntohl(msgType);
	}
	void setBodyLength(uint32_t bl){
		bodyLength = htonl(bl);
	}
	uint32_t getBodyLength(){
		return ntohl(bodyLength);
	}
}*LPHead; 

/**
* @brief End 包尾
*/
typedef struct End
{
	uint32_t checkSum; ///< 校验字， 校验从包头开始到数据体
	void setCheckSum(uint32_t s){
		checkSum = htonl(s);
	}
	uint32_t getCheckSum(){
		return ntohl(checkSum);
	}
}*LPChecksum;

/**
* @brief CompID CompID
*/
typedef struct CompID
{
	char data[20];
	void get(char* buff){
		snprintf(buff, 21, "%s", data);
	}
}*LPCompID;

/**
* @brief Login 登录结构
*/
typedef struct Login
{
	CompID senderCompID; ///< 发送方代码
	CompID targetCompID; ///< 接收方代码
	uint32_t heartBiInt; ///< 心跳间隔，单位是秒。用户行情系统登陆时提供给行情网关。
	char password[16]; ///< 密码
	/**
	* 二进制协议版本，即数据接口规范版本
	*	填写为 n.xy
	*	其中 n.xy 为数据接口规范版本，，具体参见 Binary 行情数据接口规范首页中的版本号，
	*	如数据接口规范版本为(Ver1.00)时,该字段应设置为 1.00，如数据接口规范版本为（Ver1.01）时，
	*	该字段应设置为 1.01
	*/
	char defaultAppVerID[32];  ///< 二进制协议版本
	char next[0];
	void setHeartBit(uint32_t hb){
		heartBiInt = htonl(hb);
	}
	uint32_t getHeartBit(){
		return ntohl(heartBiInt);
	}
}*LPLogin;

typedef struct Snapshot
{
	LocalTimeStamp origTime; 		///< YYYYMMDDHHMMSSsss, 数据生成时间
	uint16_t channelNo;		///< 频道代码
	char mdStreamID[3];		///< 行情类别
	SecurityID securityID;		///< 证券代码
	char securityIDSource[4];	///< 证券代码源·102深圳，103香港
	char tradingPhaseCode[8];	///<
								///< 产品所处的交易阶段代码
								///< 第0位
								///< 	S ＝ 启动（开市前）
								///< 	O ＝ 开盘集合竞价
								///< 	T ＝ 连续竞价
								///< 	B ＝ 休市
								///< 	C ＝ 收盘集合竞价
								///< 	E ＝ 已闭市
								///< 	H ＝ 临时停牌
								///< 	A = 盘后交易
								///< 	V = 波动性中断
								///< 第1位
								///< 	0 = 正常状态
								///< 	1 = 全天停牌
								///< 
	Price prevClosePx;	///< 昨收价, N13(4)
	int64_t numTrades;		///< 成交笔数
	Qty totalVolumeTrade;	///< 成交总量
	Amt totalValueTrade;	///< 成交总金额, N18(4)
	char next[0];			/// Extend Fields
public:
	void getMDStreamID(char* buff){
		snprintf(buff, 4, "%s", mdStreamID);
	}
	void getSecurityIDSource(char* buff){
		snprintf(buff, 5, "%s", securityIDSource);
	}
	void getTradingPhaseCode(char* buff){
		snprintf(buff, 9, "%s", tradingPhaseCode);
	}
	uint16_t getChannelNo(){
		return htonl(channelNo);
	}
	uint64_t getNumTrades(){
		return htobe64(numTrades);
	}
}* LPSnapshot;

typedef struct SN300111Ext
{
	char mdEntryType[2];		///< 行情条目个数
								///< 0, 买入
								///< 1，卖出
								///< 2，最近价
								///< 4，开盘价
								///< 7，最高价
								///< 8，最低价
								///< x1,升跌一
								///< x2,升跌二
								///< x3,买入汇总(总量及加权平均价)
								///< x4,卖出汇总(总量及加权平均价)
								///< x5,股票市盈率一
								///< x6,股票市盈率二
								///< x7,基金T-1日净值
								///< x8,基金实时参考净值（包括etf的IOPV）
								///< x9,权证溢价率
								///< xe，涨停价
								///< xf，跌停价
								///< xg，合约持仓量

	int64_t mdEntryPx;			///< 价格, N18(6)
	Qty mdEntrySize;		///< 数量
	uint16_t mdPriceLevel;		///< 买卖盘档位
	int64_t numberOfOrders;		///< 价位总委托笔数，为0表示不揭示
	NumInGroup noOrders;			///< 价位揭示委托笔数，为0表示不揭示
	Qty orderQty[0];		///< 委托数量
public:
	void getMDEntryType(char* buff){
		snprintf(buff, 3, "%s", mdEntryType);
	}
	double getMDEntryPx(){
		return 0.000001 * htobe64(mdEntryPx);
	}
	uint16_t getMDPriceLevel(){
		return htons(mdPriceLevel);
	}
	uint64_t getNumberOfOrders(){
		return htobe64(numberOfOrders);
	}
}* LPSN300111Ext;

typedef struct SN300111
{
	uint32_t noMDEntries;
	SN300111Ext data[0];
public:
	uint32_t getNoMDEntries(){
		return htonl(noMDEntries);
	}
}* LPSN300111;


#pragma pack(pop)

/**
* @brief GetCheckSumLen 返回需要校验的数据长度
* @param len 数据体长度
*/
uint32_t GetCheckSumLen(uint32_t len)
{
	return sizeof(Binary::Head)+len;
};

/**
* @brief GenerateCheckSum 生成校验码
*/
uint32_t GenerateCheckSum(char *buf, uint32_t bufLen)
{
	uint32_t idx;
	uint32_t cks;
	for( idx = 0L, cks = 0; idx < bufLen; cks += (uint32_t)buf[ idx++ ] );
	return (cks % 256 );
};

/**
* @brief GetPackageLength 根据数据体长度计算整包长
*/
uint32_t GetPackageLength(uint32_t dataLen)
{          //
	return sizeof(Binary::Head)+dataLen+sizeof(Binary::End);
};
}

