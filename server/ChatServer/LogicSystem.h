#pragma once
#include"connst.h"
#include"VerilyGrpcClient.h"
#include"CSession.h"
#include"data.h"
class HttpConnection;

typedef std::function<void(std::shared_ptr< HttpConnection>)>  Httphandler; 
typedef function<void(shared_ptr<CSession>, const short& msg_id, const string& msg_data)> FunCallBack;

class LogicSystem:public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem() {};
	bool isPureDigit(const std::string& str);
	void RegisterCallbacks();
	void GetUserByUid(std::string uid_str, Json::Value& rtvalue);
	void GetUserByName(std::string name, Json::Value& rtvalue);
	void LoginHandler(std::shared_ptr<CSession> session, const short& msg_id, const string& msg_data);
	void SearchInfo(std::shared_ptr<CSession> session,const short&msg_id,const string &msg_data);
	bool HandleGet(std::string, std::shared_ptr< HttpConnection>);
	void RegGet(std::string, Httphandler handler);
	void RegPost(std::string url, Httphandler handler);
	bool HandlePost(std::string, std::shared_ptr< HttpConnection>);
	bool GetBaseInfo(std::string base_key,int uid ,std::shared_ptr<UserInfo>&userinfo);
	bool GetFriendApplyInfo(int to_uid, std::vector<std::shared_ptr<ApplyInfo>>& list);
	bool GetFriendList(int self_uid, std::vector<std::shared_ptr<UserInfo>>& user_list);
	void AuthFriendApply(std::shared_ptr<CSession> session, const short& msg_id, const string& msg_data);
	void AddFriendApply(std::shared_ptr<CSession> session, const short& msg_id, const string& msg_data);
	void DealChatTextMsg(std::shared_ptr<CSession> session, const short& msg_id, const string& msg_data);
private:
	LogicSystem();
	std::thread _worker_thread;
	std::queue<shared_ptr<LogicNode>> _msg_que;
	std::mutex _mutex;
	std::condition_variable _consume;
	bool _b_stop;
	std::map<short, FunCallBack> _fun_callbacks;
	std::shared_ptr<CServer> _p_server;
	std::map<std::string, Httphandler>_post_handlers;
	std::map<std::string, Httphandler>_get_handlers;
};

