#include "LogicSystem.h"
#include"HttpConnection.h"
#include"RedisMgr.h"
#include"VerilyGrpcClient.h"
#include"MysqlMgr.h"
#include"StatusGrpcClient.h"
void LogicSystem::RegGet(std::string url, Httphandler handler) {
	_get_handlers.insert(make_pair(url,handler));
}

void LogicSystem::RegPost(std::string url, Httphandler handler) {
	_post_handlers.insert(make_pair(url, handler));
}

LogicSystem::LogicSystem() {
	RegGet("/get_test", [](std::shared_ptr<HttpConnection>connection){
		std::cout << "get test" << std::endl;
		beast::ostream(connection->_response.body()) << "receive get_text req";
		int i = 0;
		for (auto& elem : connection->_get_params) {
			i++;
			beast::ostream(connection->_response.body()) << "params" << i << "Key is" << elem.first;
			beast::ostream(connection->_response.body()) << "params" << i << "value is" << elem.second<<std::endl;
		}
		});
	RegPost("/get_varifycode", [](std::shared_ptr<HttpConnection>connection) {
		std::cout<< "get varify code" << std::endl;
		auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
		std::cout << "receive body is" << body_str << std::endl;
		connection->_response.set(http::field::content_type, "text/json");
		Json::Value root;
		Json::Reader reader;
		Json::Value src_root;
		bool parse_success = reader.parse(body_str, src_root);
		if (!parse_success) {
			std::cout << "failed to parse json data!" << std::endl;
			root["error"] = ErrorCodes::Error_Json;
			std::string _jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << _jsonstr;
			return true;
		}
		if (!src_root.isMember("email")) {
			std::cout << "failed to parse json data!" << std::endl;
			root["error"] = ErrorCodes::Error_Json;
			std::string _jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << _jsonstr;
			return true;
		}
		auto email = src_root["email"].asString();
		GetVarifyRsp rsp = VerilyGrpcClient::GetInstance()->GetVarifyCode(email);
		std::cout << "email is" << email << std::endl;
		root["error"] = rsp.error();
		root["email"] = src_root["email"];
		std::string _jsonstr = root.toStyledString();
		beast::ostream(connection->_response.body()) << _jsonstr;
		return true;
		});

	RegPost("/user_register", [](std::shared_ptr<HttpConnection> connection) {
		std::cout<< "user register" << std::endl;
		auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
		std::cout << "receive body is " << body_str << std::endl;
		connection->_response.set(http::field::content_type, "text/json");
		Json::Value root;
		Json::Reader reader;
		Json::Value src_root;
		bool parse_success = reader.parse(body_str, src_root);
		if (!parse_success) {
			std::cout << "Failed to parse JSON data!" << std::endl;
			root["error"] = ErrorCodes::Error_Json;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}

		auto pwd= src_root["passwd"].asString();
		auto confirm = src_root["confirm"].asString();

		if(pwd != confirm) {
			std::cout << "Password and confirm password do not match!" << std::endl;
			root["error"] = ErrorCodes::PasswdErr;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}

		
		std::string  varify_code;
		bool b_get_varify = RedisMgr::GetInstance()->Get(CODEPREFIX + src_root["email"].asString(), varify_code);
		if (!b_get_varify) {
			std::cout << " get varify code expired" << std::endl;
			root["error"] = ErrorCodes::VarifyExpired;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}

		if (varify_code != src_root["varifycode"].asString()) {
			std::cout << " varify code error" << std::endl;
			root["error"] = ErrorCodes::VarifyCodeErr;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}

		
		bool b_usr_exist = RedisMgr::GetInstance()->ExistsKey(src_root["user"].asString());
		if (b_usr_exist) {
			std::cout << src_root["user"].asString() << std::endl;
			std::cout << " user exist" << std::endl;
			root["error"] = ErrorCodes::UserExist;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}

		
		int uid = MysqlMgr::GetInstance()->RegUser(src_root["name"].asString(), src_root["email"].asString(), pwd);
		if(uid==0||uid==-1) {
			std::cout << " user exist" << std::endl;
			root["error"] = ErrorCodes::UserExist;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}
		root["error"] = 0;
		root["email"] = src_root["email"];
		root["uid"] = uid;
		root["user"] = src_root["user"].asString();
		root["passwd"] = src_root["passwd"].asString();
		root["confirm"] = src_root["confirm"].asString();
		root["varifycode"] = src_root["varifycode"].asString();
		std::string jsonstr = root.toStyledString();
		beast::ostream(connection->_response.body()) << jsonstr;
		std::cout << "success reguser" << std::endl;
		return true;
		});

	
	RegPost("/user_login", [](std::shared_ptr<HttpConnection> connection) {
		auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
		std::cout << "receive body is " << body_str << std::endl;
		connection->_response.set(http::field::content_type, "text/json");
		Json::Value root;
		Json::Reader reader;
		Json::Value src_root;
		bool parse_success = reader.parse(body_str, src_root);
		if (!parse_success) {
			std::cout << "Failed to parse JSON data!" << std::endl;
			root["error"] = ErrorCodes::Error_Json;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}

		auto name = src_root["user"].asString();
		auto pwd = src_root["passwd"].asString();
		UserInfo userInfo;
		
		bool pwd_valid = MysqlMgr::GetInstance()->CheckPwd(name, pwd, userInfo);
		if (!pwd_valid) {
			std::cout << " user pwd not match" << std::endl;
			root["error"] = ErrorCodes::PasswdInvalid;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}

		
		auto reply = StatusGrpcClient::GetInstance()->GetChatServer(userInfo.uid);
		if (reply.error()) {
			std::cout << " grpc get chat server failed, error is " << reply.error() << std::endl;
			root["error"] = ErrorCodes::RPCGetFailed;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}

		std::cout << "succeed to load userinfo uid is " << userInfo.uid << std::endl;
		root["error"] = 0;
		root["user"] = name;
		root["uid"] = userInfo.uid;
		root["token"] = reply.token();
		root["host"] = reply.host();
		root["port"] = reply.port();
		std::string jsonstr = root.toStyledString();
		beast::ostream(connection->_response.body()) << jsonstr;
		return true;
		});

	RegPost("/reset_pwd", [](std::shared_ptr<HttpConnection> connection) {
		auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
		std::cout << "receive body is " << body_str << std::endl;
		connection->_response.set(http::field::content_type, "text/json");
		Json::Value root;
		Json::Reader reader;
		Json::Value src_root;
		bool parse_success = reader.parse(body_str, src_root);
		if (!parse_success) {
			std::cout << "Failed to parse JSON data!" << std::endl;
			root["error"] = ErrorCodes::Error_Json;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}
		auto email = src_root["email"].asString();
		auto newpwd = src_root["newpwd"].asString();
		auto pwd = src_root["pwd"].asString();

		std::string  varify_code;
		bool b_get_varify = RedisMgr::GetInstance()->Get(CODEPREFIX + email, varify_code);
		if (!b_get_varify) {
			std::cout << " get varify code expired" << std::endl;
			root["error"] = ErrorCodes::VarifyExpired;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}

		if (varify_code != src_root["varifycode"].asString()) {
			std::cout << " varify code error" << std::endl;
			root["error"] = ErrorCodes::VarifyCodeErr;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}

		bool email_valid = MysqlMgr::GetInstance()->CheckEmail(src_root["user"].asString(), email);
		if (!email_valid) {
			std::cout << " email not match" << std::endl;
			root["error"] = ErrorCodes::EmailNotMatch;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}

		bool b_up= MysqlMgr::GetInstance()->UpdatePwd(email, newpwd);
		if (!b_up) {
			std::cout << " reset pwd failed" << std::endl;
			root["error"] = ErrorCodes::PasswdUpFailed;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}

		std::cout<< " reset pwd success" << std::endl;
		root["error"] = 0;
		root["email"] = email;
		root["user"] = src_root["user"].asString();
		root["passwd"] = pwd;
		root["varifycode"] = src_root["varifycode"].asString();
		std::string jsonstr = root.toStyledString();
		beast::ostream(connection->_response.body()) << jsonstr;
		return true;
		});
}

bool LogicSystem::HandleGet(std::string path, std::shared_ptr< HttpConnection> con) {
	if (_get_handlers.find(path) == _get_handlers.end()) {
		return false;
	}
	_get_handlers[path](con);
	return true;
}

bool LogicSystem::HandlePost(std::string path, std::shared_ptr<HttpConnection>con)
{
	if (_post_handlers.find(path) == _post_handlers.end()) {
		return false;
	}
	_post_handlers[path](con);
	return true;
}