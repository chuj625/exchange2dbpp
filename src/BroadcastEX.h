#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include <iostream>
#include <set>

#include "GlobalParameters.h"

/*#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>*/
#include <websocketpp/common/thread.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using websocketpp::lib::thread;
using websocketpp::lib::mutex;
using websocketpp::lib::lock_guard;
using websocketpp::lib::unique_lock;
using websocketpp::lib::condition_variable;

#ifndef __BroadcastEX
#define __BroadcastEX
namespace EXHQ{

class BroadcastEX {
public:
    BroadcastEX() {
        // Initialize Asio Transport
        m_server.init_asio();

        // Register handler callbacks
        m_server.set_open_handler(bind(&BroadcastEX::on_open,this,::_1));
        m_server.set_close_handler(bind(&BroadcastEX::on_close,this,::_1));
        m_server.set_message_handler(bind(&BroadcastEX::on_message,this,::_1,::_2));
    }

    void run(uint16_t port) {
        // listen on specified port
        m_server.listen(port);

        // Start the server accept loop
        m_server.start_accept();

        // Start the ASIO io_service run loop
        try {
            m_server.run();
        } catch (const std::exception & e) {
            std::cout << e.what() << std::endl;
        }
    }

    void on_open(connection_hdl hdl) {
		//TODO do nothing
		fprintf(stderr, "register []\n");
		m_connections.insert(hdl);
		/*
        {
            lock_guard<mutex> guard(m_action_lock);
            std::cout << "on_open" << std::endl;
            m_actions.push(action(SUBSCRIBE,hdl));
        }
        m_action_cond.notify_one();
		// */
    }

    void on_close(connection_hdl hdl) {
		fprintf(stderr, "unregister []\n");
		m_connections.erase(hdl);
		/*
        {
            lock_guard<mutex> guard(m_action_lock);
            std::cout << "on_close" << std::endl;
            m_actions.push(action(UNSUBSCRIBE,hdl));
        }
        m_action_cond.notify_one();
		// */ 
    }

    void on_message(connection_hdl hdl, server::message_ptr msg) {
        // queue message up for sending by processing thread
		/*TODO do nothing
		// */
    }

    void process_messages(const std::string& msg) {
		fprintf(stderr, "char HQBroadcast msg len: %d conn size %d, msg[%s]\n", msg.size(), m_connections.size(), msg.c_str());
		con_list::iterator it;
		try{
			for(it = m_connections.begin(); it != m_connections.end(); ++it){
				m_server.send(*it, msg, websocketpp::frame::opcode::text);
			}
		}catch(websocketpp::exception const & e){
			std::cerr<< e.what() << std::endl;
		}
    }

	void process_messages(const char* msg, size_t leng){
		fprintf(stderr, "char HQBroadcast msg len: %d", leng);
		con_list::iterator it;
		for(it = m_connections.begin(); it != m_connections.end(); ++it){
			m_server.send(*it, msg, leng, websocketpp::frame::opcode::binary);
		}
	}
private:
    typedef std::set<connection_hdl,std::owner_less<connection_hdl> > con_list;

    server m_server;
    con_list m_connections;

    mutex m_action_lock;
    mutex m_connection_lock;
    condition_variable m_action_cond;
};

}	///< namespace EXHQ

#endif	///< BroadcastEX

/*
int main() {
    try {
    BroadcastEX server_instance;

    // Start a thread to run the processing loop
    thread t(bind(&BroadcastEX::process_messages,&server_instance));

    // Run the asio loop with the main thread
    server_instance.run(9002);

    t.join();

    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    }
}
// */
