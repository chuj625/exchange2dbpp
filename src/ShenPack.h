
#include <stdint.h>

namespace Binary
{
#pragma pack(push,1)

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

