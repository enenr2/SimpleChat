
#include "VerilyGrpcClient.h"
#include"ConfigMgr.h"

VerilyGrpcClient::VerilyGrpcClient()
{
	auto& gCfcMgr = ConfigMgr::Inst();
	std::string host=gCfcMgr["VarifyServer"]["Host"];
	std::string port=gCfcMgr["VarifyServer"]["Port"];
	pool_.reset(new RPConPool(5,host,port));
}
