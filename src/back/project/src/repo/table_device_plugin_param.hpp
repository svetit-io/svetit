#pragma once

#include "../model/device_plugin_param.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class DevicePluginParam final {
public:
	explicit DevicePluginParam(storages::postgres::ClusterPtr pg);
	model::DevicePluginParam Get(int deviceId, int paramId);
	void Insert(int deviceId, int paramId);
	void Delete(int deviceId, int paramId);
	PagingResult<model::DevicePluginParam> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
