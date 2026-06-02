#pragma once
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include<memory>
#include<iostream>
#include"Singletion.h"
#include<functional>
#include<map>
#include<unordered_map>
#include<json/json.h>
#include<json/value.h>
#include<json/reader.h>
#include<boost/filesystem.hpp>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/ini_parser.hpp>
#include<atomic>
#include<queue>
#include<mutex>
#include<condition_variable>
#include"hiredis.h"
#include<cassert>


namespace beast = boost::beast;         
namespace http = beast::http;           
namespace net = boost::asio;            
using tcp = boost::asio::ip::tcp;       

enum ErrorCodes {
	Success = 0,
	Error_Json = 1001,  
	RPCFailed = 1002,  
	VarifyExpired = 1003, 
	VarifyCodeErr = 1004, 
	UserExist = 1005,       
	PasswdErr = 1006,    
	EmailNotMatch = 1007,  
	PasswdUpFailed = 1008,  
	PasswdInvalid = 1009,   
	TokenInvalid = 1010,   
	UidInvalid = 1011,  
	CREATE_CHAT_FAILED = 1012, 
	LOAD_CHAT_FAILED = 1013, 
	RPCGetFailed=1014, 
	RECFailed=1015, 
};

#include "data.h"


class Defer {
public:
	
	Defer(std::function<void()> func) : func_(func) {}

	
	~Defer() {
		func_();
	}

private:
	std::function<void()> func_;
};

#define MAX_LENGTH  (1024*2 + 1)

#define HEAD_TOTAL_LEN 4

#define HEAD_ID_LEN 2

#define HEAD_DATA_LEN 2
#define MAX_RECVQUE  10000
#define MAX_SENDQUE 1000


enum MSG_IDS {
	MSG_CHAT_LOGIN = 1005, 
	MSG_CHAT_LOGIN_RSP = 1006, 
	ID_SEARCH_USER_REQ = 1007, 
	ID_SEARCH_USER_RSP = 1008, 
	ID_ADD_FRIEND_REQ = 1009, 
	ID_ADD_FRIEND_RSP = 1010, 
	ID_NOTIFY_ADD_FRIEND_REQ = 1011,  
	ID_AUTH_FRIEND_REQ = 1013,  
	ID_AUTH_FRIEND_RSP = 1014,  
	ID_NOTIFY_AUTH_FRIEND_REQ = 1015, 
	ID_TEXT_CHAT_MSG_REQ = 1017, 
	ID_TEXT_CHAT_MSG_RSP = 1018, 
	ID_NOTIFY_TEXT_CHAT_MSG_REQ = 1019, 
	ID_NOTIFY_OFF_LINE_REQ = 1021, 
	ID_HEART_BEAT_REQ = 1023,      
	ID_HEARTBEAT_RSP = 1024,       
	ID_LOAD_CHAT_THREAD_REQ = 1025, 
	ID_LOAD_CHAT_THREAD_RSP = 1026, 
	ID_CREATE_PRIVATE_CHAT_REQ = 1027, 
	ID_CREATE_PRIVATE_CHAT_RSP = 1028, 

	ID_LOAD_CHAT_MSG_REQ = 1029,      
	ID_LOAD_CHAT_MSG_RSP = 1030,      
};

#define USERIPPREFIX  "uip_"
#define USERTOKENPREFIX  "utoken_"
#define IPCOUNTPREFIX  "ipcount_"
#define USER_BASE_INFO "ubaseinfo_"
#define LOGIN_COUNT  "logincount"
#define NAME_INFO  "nameinfo_"
#define LOCK_PREFIX "lock_"
#define USER_SESSION_PREFIX "usession_"
#define LOCK_COUNT "lockcount"
#define CODEPREFIX  "code_"


#define LOCK_TIME_OUT 10

#define ACQUIRE_TIME_OUT 5