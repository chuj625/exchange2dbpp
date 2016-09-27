#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>

#ifndef __SocketDF
#define __SocketDF
void error(const char *msg)
{
	perror(msg);
}

class SocketDF{
public:
	SocketDF(): _ip(), _port(0), _sockfd(0), _serv_addr(), _server(NULL)
	{
	}
	~SocketDF(){
		;
	}
	bool connects(const std::string& ip, unsigned int port){
		_ip = ip;
		_port = port;
		_sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if(_sockfd < 0){
			error("ERROR opening socket");
			return false;
		}
		_server = gethostbyname(_ip.c_str());
		if(_server == NULL) {
			error("ERROR, no such host\n");
			return false;
		}
		bzero((char *) &_serv_addr, sizeof(_serv_addr));
		_serv_addr.sin_family = AF_INET;
		bcopy((char *)_server->h_addr,
				(char *)&_serv_addr.sin_addr.s_addr,
				_server->h_length);
		_serv_addr.sin_port = htons(_port);
		if (connect(_sockfd, (struct sockaddr *) &_serv_addr, sizeof(_serv_addr)) < 0){
			error("ERROR connecting");
			return false;
		}
		return true;
	}
	int writes(char* buff, unsigned int bufsize){
		int res = write(_sockfd, buff, bufsize);
		return res;
	}
	int reads(char* buff, unsigned int maxsize){
		int res = read(_sockfd, buff, maxsize);
		return res;
	}
	void closes(){
		close(_sockfd);
	}
private:
	std::string _ip;
	unsigned int _port;

	int _sockfd;
	struct sockaddr_in _serv_addr;
	struct hostent *_server;
};

#endif ///< SocketDF

