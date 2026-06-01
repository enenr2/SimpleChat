#pragma once
#include"connst.h"
#include"MysqlDao.h"
class MysqlMgr:public Singleton<MysqlMgr>
{
	friend class Singleton<MysqlMgr>;
public:
	std::shared_ptr<UserInfo> GetUser(int uid);
	bool CheckEmail(const std::string& name, const std::string& email);

	bool UpdatePwd(const std::string& name, const std::string& pwd);

	int RegUser(const std::string& name, const std::string& email, const std::string& pwd);

	bool CheckPwd(const std::string &email,const std::string &pwd,UserInfo&userInfo);

private:
	MysqlMgr();
	
	MysqlDao _dao;
};

