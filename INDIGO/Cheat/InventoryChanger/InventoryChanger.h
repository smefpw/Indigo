#pragma once

#include "../../Engine/Engine.h"
#include "../../protobuf/base_gcmessages.pb.h"
#include "../../protobuf/cstrike15_gcmessages.pb.h"
#include "../../protobuf/econ_gcmessages.pb.h"
#include "../../protobuf/engine_gcmessages.pb.h"
#include "../../protobuf/gcsystemmsgs.pb.h"
#include "../../protobuf/gcsdk_gcmessages.pb.h"
#include "../../protobuf/netmessages.pb.h"
#include "../../protobuf/steammessages.pb.h"

#pragma comment(lib, "libprotobuf.lib")
#pragma comment(lib, "libprotoc.lib")

class CInventoryChanger {
public:
	void PostRetrieveMessage(uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize);
	bool PreSendMessage(uint32_t &unMsgType, void* pubData, uint32_t &cubData);
	void ApplyMedals(CMsgSOCacheSubscribed::SubscribedType* pInventoryCacheObject);
	void ApplySkins(CMsgSOCacheSubscribed::SubscribedType* pInventoryCacheObject);
};