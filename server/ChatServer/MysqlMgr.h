#pragma once
#include"connst.h"
#include"MysqlDao.h"

class MysqlMgr:public Singleton<MysqlMgr>
{
	friend class Singleton<MysqlMgr>;
public:
	std::shared_ptr<UserInfo> GetUser(int uid);

	std::shared_ptr<UserInfo> GetUser(std::string name);

	bool CheckEmail(const std::string& name, const std::string& email);

	bool UpdatePwd(const std::string& name, const std::string& pwd);

	int RegUser(const std::string& name, const std::string& email, const std::string& pwd);

	bool CheckPwd(const std::string &email,const std::string &pwd,UserInfo&userInfo);

	bool AddFriendApply(const int& from, const int& to);

	bool AuthFriendApply(const int& from, const int& to);

	bool AddFriend(const int& from, const int& to, const std::string& back_name);

	bool GetFriendList(int self_uid, std::vector<std::shared_ptr<UserInfo>>& user_list);

	bool GetApplyList(const int& to_uid, std::vector<std::shared_ptr<ApplyInfo>>& list,int begin,int limit=10);
private:
	MysqlMgr();
	
	MysqlDao _dao;
};

