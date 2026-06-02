#include "MysqlDao.h"
#include "ConfigMgr.h"
#include "MysqlMgr.h"
using namespace std;
class ConnectionGuard {
public:
	ConnectionGuard(MySqlPool* pool, std::unique_ptr<SqlConnection> conn)
		: pool_(pool), conn_(std::move(conn)) {}

	~ConnectionGuard() {
		if (conn_ && pool_) {
			pool_->returnConnection(std::move(conn_));
		}
	}

	SqlConnection* operator->() { return conn_.get(); }
	bool operator==(std::nullptr_t) const { return conn_ == nullptr; }
	bool operator!=(std::nullptr_t) const { return conn_ != nullptr; }

	ConnectionGuard(const ConnectionGuard&) = delete;
	ConnectionGuard& operator=(const ConnectionGuard&) = delete;

private:
	MySqlPool* pool_;
	std::unique_ptr<SqlConnection> conn_;
};

MysqlDao::MysqlDao()
{
	auto& cfg = ConfigMgr::Inst();
	const auto& host = cfg["Mysql"]["Host"];
	const auto& port = cfg["Mysql"]["Port"];
	const auto& pwd = cfg["Mysql"]["Passwd"];
	const auto& schema = cfg["Mysql"]["Schema"];
	const auto& user = cfg["Mysql"]["User"];
	pool_.reset(new MySqlPool(host + ":" + port, user, pwd, schema, 5));
}

MysqlDao::~MysqlDao() {
	pool_->Close();
}

int MysqlDao::RegUser(const std::string& name, const std::string& email, const std::string& pwd)
{
	auto con = pool_->getConnection();
	try {
		if (con == nullptr) {
			return false;
		}
		
		std::unique_ptr < sql::PreparedStatement > stmt(con->_con->prepareStatement("CALL reg_user(?,?,?,@result)"));
		
		stmt->setString(1, name);
		stmt->setString(2, email);
		stmt->setString(3, pwd);

		

		  
		stmt->execute();
		
	   
		std::unique_ptr<sql::Statement> stmtResult(con->_con->createStatement());
		std::unique_ptr<sql::ResultSet> res(stmtResult->executeQuery("SELECT @result AS result"));
		if (res->next()) {
			int result = res->getInt("result");
			cout << "Result: " << result << std::endl;
			pool_->returnConnection(std::move(con));
			return result;
		}
		pool_->returnConnection(std::move(con));
		return -1;
	}
	catch (sql::SQLException& e) {
		pool_->returnConnection(std::move(con));
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return -1;
	}
}

int MysqlDao::RegUserTransaction(const std::string& name, const std::string& email, const std::string& pwd,
	const std::string& icon)
{
	auto con = pool_->getConnection();
	if (con == nullptr) {
		return false;
	}

	Defer defer([this, &con] {
		pool_->returnConnection(std::move(con));
		});

	try {
		
		con->_con->setAutoCommit(false);
		
			

		std::unique_ptr<sql::PreparedStatement> pstmt_email(con->_con->prepareStatement("SELECT 1 FROM user WHERE email = ?"));

		
		pstmt_email->setString(1, email);

		
		std::unique_ptr<sql::ResultSet> res_email(pstmt_email->executeQuery());

		auto email_exist = res_email->next();
		if (email_exist) {
			con->_con->rollback();
			std::cout << "email " << email << " exist";
			return 0;
		}

		
		std::unique_ptr<sql::PreparedStatement> pstmt_name(con->_con->prepareStatement("SELECT 1 FROM user WHERE name = ?"));

		
		pstmt_name->setString(1, name);

		
		std::unique_ptr<sql::ResultSet> res_name(pstmt_name->executeQuery());

		auto name_exist = res_name->next();
		if (name_exist) {
			con->_con->rollback();
			std::cout << "name " << name << " exist";
			return 0;
		}

		
		std::unique_ptr<sql::PreparedStatement> pstmt_upid(con->_con->prepareStatement("UPDATE user_id SET id = id + 1"));

		
		pstmt_upid->executeUpdate();

		
		std::unique_ptr<sql::PreparedStatement> pstmt_uid(con->_con->prepareStatement("SELECT id FROM user_id"));
		std::unique_ptr<sql::ResultSet> res_uid(pstmt_uid->executeQuery());
		int newId = 0;
		
		if (res_uid->next()) {
			newId = res_uid->getInt("id");
		}
		else {
			std::cout << "select id from user_id failed" << std::endl;
			con->_con->rollback();
			return -1;
		}

		
		std::unique_ptr<sql::PreparedStatement> pstmt_insert(con->_con->prepareStatement("INSERT INTO user (uid, name, email, pwd, nick, icon) "
			"VALUES (?, ?, ?, ?,?,?)"));
		pstmt_insert->setInt(1, newId);
		pstmt_insert->setString(2, name);
		pstmt_insert->setString(3, email);
		pstmt_insert->setString(4, pwd);
		pstmt_insert->setString(5, name);
		pstmt_insert->setString(6, icon);
		
		pstmt_insert->executeUpdate();
		
		con->_con->commit();
		std::cout << "newuser insert into user success" << std::endl;
		return newId;
	}
	catch (sql::SQLException& e) {
		
		if (con) {
			con->_con->rollback();
		}
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return -1;
	}
}

bool MysqlDao::CheckEmail(const std::string& name, const std::string& email) {
	auto con = pool_->getConnection();
	if (con == nullptr) {
		return false;
	}
	ConnectionGuard guard(pool_.get(), std::move(con));

	try {
		std::unique_ptr<sql::PreparedStatement> pstmt(guard->_con->prepareStatement("SELECT email FROM user WHERE name = ?"));
		pstmt->setString(1, name);

		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

		while (res->next()) {
			std::cout << "Check Email: " << res->getString("email") << std::endl;
			if (email != res->getString("email")) {
				return false;
			}
			return true;
		}
		return false;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
}
bool MysqlDao::UpdatePwd(const std::string& email, const std::string& newpwd) {
	auto con = pool_->getConnection();
	try {
		if (con == nullptr) {
			return false;
		}

		
		std::unique_ptr<sql::PreparedStatement> pstmt(con->_con->prepareStatement("UPDATE user SET pwd = ? WHERE email = ?"));

		
		pstmt->setString(2, email);
		pstmt->setString(1, newpwd);

		
		int updateCount = pstmt->executeUpdate();

		std::cout << "Updated rows: " << updateCount << std::endl;
		pool_->returnConnection(std::move(con));
		return true;
	}
	catch (sql::SQLException& e) {
		pool_->returnConnection(std::move(con));
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
}

bool MysqlDao::CheckPwd(const std::string& email, const std::string& pwd, UserInfo& userInfo) {
	auto con = pool_->getConnection();
	if (con == nullptr) {
		return false;
	}

	Defer defer([this, &con]() {
		pool_->returnConnection(std::move(con));
		});

	try {
		std::unique_ptr<sql::PreparedStatement> pstmt(con->_con->prepareStatement("SELECT * FROM user WHERE email = ?"));
		pstmt->setString(1, email);

		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

		if (!res->next()) {
			return false;
		}

		std::string origin_pwd = res->getString("pwd");
		std::cout << "Password: " << origin_pwd << std::endl;

		if (pwd != origin_pwd) {
			return false;
		}
		userInfo.name = res->getString("name");
		userInfo.email = res->getString("email");
		userInfo.uid = res->getInt("uid");
		userInfo.pwd = origin_pwd;
		return true;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
}
bool MysqlDao::TestProcedure(const std::string& email, int& uid, string& name) {
	auto con = pool_->getConnection();
	try {
		if (con == nullptr) {
			return false;
		}

		Defer defer([this, &con]() {
			pool_->returnConnection(std::move(con));
			});
		
		std::unique_ptr < sql::PreparedStatement > stmt(con->_con->prepareStatement("CALL test_procedure(?,@userId,@userName)"));
		
		stmt->setString(1, email);

		

		  
		stmt->execute();
		
	   
		std::unique_ptr<sql::Statement> stmtResult(con->_con->createStatement());
		std::unique_ptr<sql::ResultSet> res(stmtResult->executeQuery("SELECT @userId AS uid"));
		if (!(res->next())) {
			return false;
		}

		uid = res->getInt("uid");
		cout << "uid: " << uid << std::endl;

		stmtResult.reset(con->_con->createStatement());
		res.reset(stmtResult->executeQuery("SELECT @userName AS name"));
		if (!(res->next())) {
			return false;
		}

		name = res->getString("name");
		cout << "name: " << name << std::endl;
		return true;

	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
}

std::shared_ptr<UserInfo> MysqlDao::GetUser(int uid) {
	auto con = pool_->getConnection();
	if (con == nullptr) {
		return nullptr;
	}

	Defer defer([this, &con]() {
		pool_->returnConnection(std::move(con));
		});

	try {
		
		std::unique_ptr<sql::PreparedStatement> pstmt(con->_con->prepareStatement("SELECT * FROM user WHERE uid = ?"));
		pstmt->setInt(1, uid); 

		
		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
		std::shared_ptr<UserInfo> user_ptr = nullptr;
		
		while (res->next()) {
			user_ptr.reset(new UserInfo);
			user_ptr->pwd = res->getString("pwd");
			user_ptr->email = res->getString("email");
			user_ptr->name = res->getString("name");
			user_ptr->nick = res->getString("nick");
			user_ptr->desc = res->getString("desc");
			user_ptr->sex = res->getInt("sex");
			user_ptr->icon = res->getString("icon");
			user_ptr->uid = uid;
			break;
		}
		return user_ptr;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return nullptr;
	}
}

std::shared_ptr<UserInfo> MysqlDao::GetUser(std::string name) {
	auto con = pool_->getConnection();
	if (con == nullptr) {
		return nullptr;
	}

	Defer defer([this, &con]() {
		pool_->returnConnection(std::move(con));
		});
	try {
		
		std::unique_ptr<sql::PreparedStatement> pstmt(con->_con->prepareStatement("SELECT * FROM user WHERE name = ?"));
		pstmt->setString(1, name); 

		
		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
		std::shared_ptr<UserInfo> user_ptr = nullptr;
		
		while (res->next()) {
			user_ptr.reset(new UserInfo);
			user_ptr->pwd = res->getString("pwd");
			user_ptr->email = res->getString("email");
			user_ptr->name = res->getString("name");
			user_ptr->nick = res->getString("nick");
			user_ptr->desc = res->getString("desc");
			user_ptr->sex = res->getInt("sex");
			user_ptr->icon = res->getString("icon");
			user_ptr->uid = res->getInt("uid");
			break;
		}
		return user_ptr;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return nullptr;
	}
}

bool MysqlDao::AddFriendApply(const int& from, const int& to) {
	auto con= pool_->getConnection();
	if (con == nullptr) {
		return false;
	}
	Defer defer([this, &con]() {
		pool_->returnConnection(std::move(con));
		});
	try {
		std::unique_ptr<sql::PreparedStatement> pstmt(con->_con->prepareStatement("INSERT INTO friend_apply (from_uid, to_uid) VALUES (?, ?)"
		"ON DUPLICATE KEY UPDATE from_uid=from_uid"));
		pstmt->setInt(1, from);
		pstmt->setInt(2, to);
		int rowAddected=pstmt->executeUpdate();
		if (rowAddected < 0) {
			return false;
		}
	}catch (sql::SQLException& e) {
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
	return true;
}

bool MysqlDao::GetApplyList(const int& to_uid, std::vector<std::shared_ptr<ApplyInfo>>& list, int begin, int limit) {
	auto con = pool_->getConnection();
	if (con == nullptr) {
		return false;
	}

	Defer defer([this, &con]() {
		pool_->returnConnection(std::move(con));
		});

	try {
		std::unique_ptr<sql::PreparedStatement> pstmt(con->_con->prepareStatement("Select apply.from_uid ,apply.status,user.name,user.nick,user.sex from friend_apply as "
			"apply join user on apply.from_uid = user.uid where apply.to_uid = ? and apply.id > ? order by apply.id desc limit ?"));
		pstmt->setInt(1, to_uid);
		pstmt->setInt(2, begin);
		pstmt->setInt(3, limit);
		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
		while (res->next()) {
			int uid = res->getInt("from_uid");
			int status = res->getInt("status");
			std::string name = res->getString("name");
			std::string nick = res->getString("nick");
			auto sex = res->getInt("sex");
			auto apply_ptr = std::make_shared<ApplyInfo>(uid, name, "", "", nick, sex, status);
			list.push_back(apply_ptr);
		}
	}

	catch (sql::SQLException& e) {
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}

	return true;
}

bool MysqlDao::AuthFriendApply(const int& from, const int& to) {
	auto con = pool_->getConnection();
	if (con == nullptr) {
		return false;
	}

	Defer defer([this, &con]() {
		pool_->returnConnection(std::move(con));
		});

	try {
		std::unique_ptr<sql::PreparedStatement> pstmt(con->_con->prepareStatement(
			"UPDATE friend_apply SET status = 1 WHERE from_uid = ? AND to_uid = ? AND status = 0"));
		pstmt->setInt(1, from);
		pstmt->setInt(2, to);
		int rows = pstmt->executeUpdate();
		if (rows <= 0) {
			return false;
		}

		
		std::unique_ptr<sql::PreparedStatement> pstmt2(con->_con->prepareStatement(
			"INSERT INTO friend (self_id, friend_id) VALUES (?, ?), (?, ?)"));
		pstmt2->setInt(1, from);
		pstmt2->setInt(2, to);
		pstmt2->setInt(3, to);
		pstmt2->setInt(4, from);
		pstmt2->executeUpdate();

		return true;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
}

bool MysqlDao::AddFriend(const int& from, const int& to, const std::string& back_name) {
	auto con = pool_->getConnection();
	if (con == nullptr) {
		return false;
	}

	Defer defer([this, &con]() {
		pool_->returnConnection(std::move(con));
		});

	try {
		
		std::unique_ptr<sql::PreparedStatement> pstmt(con->_con->prepareStatement(
			"INSERT INTO friend_apply (from_uid, to_uid) VALUES (?, ?)"
			" ON DUPLICATE KEY UPDATE from_uid=from_uid"));
		pstmt->setInt(1, from);
		pstmt->setInt(2, to);
		int rows = pstmt->executeUpdate();
		if (rows < 0) {
			return false;
		}

		
		std::unique_ptr<sql::PreparedStatement> pstmt2(con->_con->prepareStatement(
			"INSERT INTO friend (self_id, friend_id, back) VALUES (?, ?, ?), (?, ?, ?)"));
		pstmt2->setInt(1, from);
		pstmt2->setInt(2, to);
		pstmt2->setString(3, back_name);
		pstmt2->setInt(4, to);
		pstmt2->setInt(5, from);
		pstmt2->setString(6, back_name);
		pstmt2->executeUpdate();

		return true;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
}
bool MysqlDao::GetFriendList(int self_id, std::vector<std::shared_ptr<UserInfo>>& user_info_list) {
	auto con = pool_->getConnection();
	if (con == nullptr) {
		return false;
	}

	Defer defer([this, &con]() {
		pool_->returnConnection(std::move(con));
		});

	try {
		std::unique_ptr<sql::PreparedStatement> pstmt(con->_con->prepareStatement(
			"SELECT u.uid, u.name, u.email, u.pwd, u.nick, u.desc, u.sex, u.icon, f.back "
			"FROM friend f JOIN user u ON f.friend_id = u.uid "
			"WHERE f.self_id = ?"));

		pstmt->setInt(1, self_id);

		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
		while (res->next()) {
			auto user_info = std::make_shared<UserInfo>();
			user_info->uid = res->getInt("uid");
			user_info->name = res->getString("name");
			user_info->email = res->getString("email");
			user_info->pwd = res->getString("pwd");
			user_info->nick = res->getString("nick");
			user_info->desc = res->getString("desc");
			user_info->sex = res->getInt("sex");
			user_info->icon = res->getString("icon");
			user_info->back = res->getString("back");
			user_info_list.push_back(user_info);
		}
		return true;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}

	return true;
}
