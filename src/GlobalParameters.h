
#include <string>
#include <queue>
#include "IDCache.h"

#ifndef __GlobalParameters
#define __GlobalParameters

namespace EXHQ{
	class BroadcastEX;

	BroadcastEX* broadcast_server_p;

	//class IDCache;
	IDCache* IDCache_p;

	// 3M
	static const size_t MAX_MSG_LENGTH = 1024*1024*4;
	static const double FILE_CHECK_INTERVAL = 1000;		///< 微秒

}	///< namespace EXHQ
#endif	///< GlobalParameters

