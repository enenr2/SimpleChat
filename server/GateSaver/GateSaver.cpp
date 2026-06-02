#include <iostream>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include"CServer.h"
#include"ConfigMgr.h"

void TestRedis() {
    

    redisContext* c = redisConnect("127.0.0.1", 6380);
    if (c->err)
    {
        printf("Connect to redisServer faile:%s\n", c->errstr);
        redisFree(c);        return;
    }
    printf("Connect to redisServer Success\n");

    std::string redis_password = "123456";
    redisReply* r = (redisReply*)redisCommand(c, "AUTH %s", redis_password);
    if (r->type == REDIS_REPLY_ERROR) {
        printf("Redis认证失败！\n");
    }
    else {
        printf("Redis认证成功！\n");
    }

    
    const char* command1 = "set stest1 value1";

    
    r = (redisReply*)redisCommand(c, command1);

    
    if (NULL == r)
    {
        printf("Execut command1 failure\n");
        redisFree(c);        return;
    }

    
    if (!(r->type == REDIS_REPLY_STATUS && (strcmp(r->str, "OK") == 0 || strcmp(r->str, "ok") == 0)))
    {
        printf("Failed to execute command[%s]\n", command1);
        freeReplyObject(r);
        redisFree(c);        return;
    }

    
    freeReplyObject(r);
    printf("Succeed to execute command[%s]\n", command1);

    const char* command2 = "strlen stest1";
    r = (redisReply*)redisCommand(c, command2);

    
    if (r->type != REDIS_REPLY_INTEGER)
    {
        printf("Failed to execute command[%s]\n", command2);
        freeReplyObject(r);
        redisFree(c);        return;
    }

    
    int length = r->integer;
    freeReplyObject(r);
    printf("The length of 'stest1' is %d.\n", length);
    printf("Succeed to execute command[%s]\n", command2);

    
    const char* command3 = "get stest1";
    r = (redisReply*)redisCommand(c, command3);
    if (r->type != REDIS_REPLY_STRING)
    {
        printf("Failed to execute command[%s]\n", command3);
        freeReplyObject(r);
        redisFree(c);        return;
    }
    printf("The value of 'stest1' is %s\n", r->str);
    freeReplyObject(r);
    printf("Succeed to execute command[%s]\n", command3);

    const char* command4 = "get stest2";
    r = (redisReply*)redisCommand(c, command4);
    if (r->type != REDIS_REPLY_NIL)
    {
        printf("Failed to execute command[%s]\n", command4);
        freeReplyObject(r);
        redisFree(c);        return;
    }
    freeReplyObject(r);
    printf("Succeed to execute command[%s]\n", command4);

    
    redisFree(c);

}

int main()
{
	auto & gCfgMgr=ConfigMgr::Inst();
	std::string gate_post_str= gCfgMgr["GateServer"]["Port"];
	unsigned short gate_port = atoi(gate_post_str.c_str());
	try {
		unsigned short port = static_cast<unsigned short>(8080);
		net::io_context ioc{ 1 };
		boost::asio::signal_set signals(ioc, SIGTERM);
		signals.async_wait([&ioc](const boost::system::error_code& error, int signal_number) {
			if (error) {
				return;
			}
			ioc.stop();
			});
		std::make_shared<CServer>(ioc, port)->Start();
		std::cout << "Gate Server listen on port:" << port << std::endl;
		ioc.run();
	}
	catch (std::exception const &ec) {
		std::cout << "ERROR:" << ec.what() << std::endl;
		return EXIT_FAILURE;
	}
}
