#pragma once
#include"message.grpc.pb.h"
#include"message.pb.h"
#include<grpcpp/grpcpp.h>
#include<mutex>
#include"data.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;
using grpc::ServerContext;

using message::AddFriendReq;
using message::AddFriendRsp;

using message::AuthFriendReq;
using message::AuthFriendRsp;

using message::GetChatServerRsp;
using message::LoginReq;
using message::LoginRsp;
using message::ChatService;

using message::TextChatMsgReq;
using message::TextChatMsgRsp;
using message::TextChatData;


class ChatServiceImpl:public ChatService::Service
{
public:
	ChatServiceImpl();
	Status NotifyAddFriend(grpc::ServerContext* context, const AddFriendReq* request,
		AddFriendRsp* reply) override;

	Status NotifyAuthFriend(grpc::ServerContext* context,
		const AuthFriendReq* request, AuthFriendRsp* reply) override;

	Status NotifyTextChatMsg(::grpc::ServerContext* context,
		const TextChatMsgReq* request, TextChatMsgRsp* response) override;

	bool GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo);
};

