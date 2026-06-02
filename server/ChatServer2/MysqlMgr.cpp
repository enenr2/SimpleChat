#include "MysqlMgr.h"
MysqlMgr::MysqlMgr(){ }

int MysqlMgr::RegUser(const std::string& name, const std::string& email, const std::string& pwd)
{
	return _dao.RegUser(name, email, pwd);
}

std::shared_ptr<UserInfo> MysqlMgr::GetUser(int uid)
{
    return _dao.GetUser(uid);
}

std::shared_ptr<UserInfo> MysqlMgr::GetUser(std::string name)
{
    return _dao.GetUser(name);
}

bool MysqlMgr::CheckEmail(const std::string& name, const std::string& email) {
    return _dao.CheckEmail(name, email);
}

bool MysqlMgr::UpdatePwd(const std::string& name, const std::string& pwd) {
    return _dao.UpdatePwd(name, pwd);
}

bool MysqlMgr::CheckPwd(const std::string& email, const std::string& pwd, UserInfo& userInfo) {
    return _dao.CheckPwd(email, pwd, userInfo);
}

bool MysqlMgr::AddFriendApply(const int& from, const int& to)
{
    return _dao.AddFriendApply(from,to);
}

bool MysqlMgr::AuthFriendApply(const int& from, const int& to)
{
    return _dao.AuthFriendApply(from,to);
}

bool MysqlMgr::AddFriend(const int& from, const int& to, const std::string& back_name)
{
    return _dao.AddFriend(from,to,back_name);
}

bool MysqlMgr::GetApplyList(const int& to_uid, std::vector<std::shared_ptr<ApplyInfo>>& list, int begin, int limit)
{
    return _dao.GetApplyList(to_uid,list,begin,limit);
}

bool MysqlMgr::GetFriendList(int self_uid, std::vector<std::shared_ptr<UserInfo>>& user_list) {
    return _dao.GetFriendList(self_uid, user_list);
}
