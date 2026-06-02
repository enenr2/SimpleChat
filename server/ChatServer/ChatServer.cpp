#include "LogicSystem.h"
#include <csignal>
#include <thread>
#include <mutex>
#include "AsioIOServicePool.h"
#include "CServer.h"
#include "ConfigMgr.h"
#include"RedisMgr.h"
#include"ChatServiceImpl.h"
using namespace std;
bool bstop = false;
std::condition_variable cond_quit;
std::mutex mutex_quit;

int main()
{
    auto&cfg=ConfigMgr::Inst();
    auto server_name = cfg["SelfServer"]["Name"];
    try {
        auto& cfg = ConfigMgr::Inst();
        RedisMgr::GetInstance()->HSet(LOGIN_COUNT,server_name,"0");
        auto pool = AsioIOServicePool::GetInstance();

        std::string server_address(cfg["SelfServer"]["Host"] + ":" + cfg["SelfServer"]["RPCPort"]);
        ChatServiceImpl service;
        grpc::ServerBuilder builder;
        
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);
   
        
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        std::cout << "RPC Server listening on " << server_address << std::endl;

        
        std::thread  grpc_server_thread([&server]() {
            server->Wait();
            });

        boost::asio::io_context  io_context;
        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&io_context, pool](auto, auto) {
            io_context.stop();
            LogicSystem::GetInstance()->Stop();
            pool->Stop();
            
            });
        auto port_str = cfg["SelfServer"]["Port"];
        CServer s(io_context, atoi(port_str.c_str()));
        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

}