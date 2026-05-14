#pragma once
#include"connst.h"
#include"VerilyGrpcClient.h"

class HttpConnection;

typedef std::function<void(std::shared_ptr< HttpConnection>)>  Httphandler; 

class LogicSystem:public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem() {};
	bool HandleGet(std::string, std::shared_ptr< HttpConnection>);
	void RegGet(std::string, Httphandler handler);
	void RegPost(std::string url, Httphandler handler);
	bool HandlePost(std::string, std::shared_ptr< HttpConnection>);
private:
	LogicSystem();
	std::map<std::string, Httphandler>_post_handlers;
	std::map<std::string, Httphandler>_get_handlers;
};

