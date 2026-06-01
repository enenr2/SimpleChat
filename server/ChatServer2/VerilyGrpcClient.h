#pragma once
#include<grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include"connst.h"
#include"Singletion.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;

class RPConPool {
public:
	RPConPool(size_t poolsize, std::string host, std::string port) :
		poolSize_(poolsize), host_(host), port_(port) {
		b_stop_ = false;
		std::string target = host_ + ":" + port_;
		std::cout<<"RPConPool init with target: " << target << std::endl;
		for (size_t i = 0; i < poolSize_; ++i) {
			std::shared_ptr<Channel> channel = grpc::CreateChannel(target, grpc::InsecureChannelCredentials());
			connections_.push(VarifyService::NewStub(channel));
		}
	};
	~RPConPool() {
		std::lock_guard<std::mutex> lock(mutex_);
		Close();
		while (!connections_.empty()) {
			connections_.pop();
		}
	}

	void Close() {
		b_stop_ = true;
		cond_.notify_all();
	}

	std::unique_ptr<VarifyService::Stub> getConnection() {
		std::unique_lock<std::mutex> lock(mutex_);
		cond_.wait(lock, [this]() {
			return b_stop_ || !connections_.empty();
			});

		if (b_stop_) {
			return nullptr;
		}

		auto context = std::move(connections_.front());
		connections_.pop();
		return std::move(context);
	}

	void returnConnection(std::unique_ptr<VarifyService::Stub> context) {
		std::lock_guard<std::mutex> lock(mutex_);
		if (b_stop_)
			return;

		connections_.push(std::move(context));
		cond_.notify_one();
	}
private:
	std::atomic<bool> b_stop_;
	size_t poolSize_;
	std::string host_;
	std::string port_;
	std::condition_variable cond_;
	std::mutex mutex_;
	std::queue<std::unique_ptr<VarifyService::Stub>> connections_;
};

class VerilyGrpcClient : public Singleton<VerilyGrpcClient>
{
	friend class Singleton<VerilyGrpcClient>;
public:
	GetVarifyRsp GetVarifyCode(std::string email) {
		ClientContext context;
		GetVarifyRsp reply;
		GetVarifyReq request;
		request.set_email(email);
		auto stub_ = pool_->getConnection();

		if (!stub_) {
			// 无可用连接或已停止
			reply.set_error(ErrorCodes::RECFailed);
			return reply;
		}

		Status status = stub_->GetVarifyCode(&context, request, &reply);

		// 不论成功与否，都把连接放回池中（以便重用）
		pool_->returnConnection(std::move(stub_));

		if (status.ok()) {
			return reply;
		}
		else {
			reply.set_error(ErrorCodes::RECFailed);
			return reply;
		}
	}
private:
	VerilyGrpcClient();
	std::unique_ptr<RPConPool> pool_;
};