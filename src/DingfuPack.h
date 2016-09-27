
#include <stdint.h>

namespace DF{
#pragma pack(push, 1)

typedef struct HEADER_DF
{
	uint32_t msgType;
	uint32_t bodyLength;
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
} HEADER;

#pragma pack(pop)

}

