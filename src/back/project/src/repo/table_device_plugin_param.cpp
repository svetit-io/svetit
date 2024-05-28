#include "table_device_plugin_param.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

DevicePluginParam::DevicePluginParam(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kGet{
	"SELECT space_id, device_id, param_id FROM project.device_plugin_param WHERE space_id = $1 AND device_id = $2 AND param_id = $3",
	pg::Query::Name{"select_device_plugin_param"},
};

model::DevicePluginParam DevicePluginParam::Get(const boost::uuids::uuid& spaceId, int64_t deviceId, int64_t paramId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, spaceId, deviceId, paramId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::DevicePluginParam>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.device_plugin_param (space_id, device_id, param_id) "
	"VALUES ($1, $2, $3)",
	pg::Query::Name{"insert_device_plugin_param"},
};

void DevicePluginParam::Create(const model::DevicePluginParam& item) {
	_pg->Execute(ClusterHostType::kMaster, kCreate, item.spaceId, item.deviceId, item.paramId);
}

void DevicePluginParam::Update(const model::DevicePluginParam&) {
	throw errors::Forbidden403();
}

const pg::Query kDelete {
	"DELETE FROM project.device_plugin_param WHERE space_id = $1 AND device_id = $2 AND param_id = $3",
	pg::Query::Name{"delete_device_plugin_param"},
};

void DevicePluginParam::Delete(const boost::uuids::uuid& spaceId, int64_t deviceId, int64_t paramId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, spaceId, deviceId, paramId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectDevicePluginParams{
	"SELECT space_id, device_id, param_id FROM project.device_plugin_param "
	"WHERE space_id = $1 AND device_id = $2"
	"OFFSET $3 LIMIT $4",
	pg::Query::Name{"select_device_plugin_params"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.device_plugin_param "
	"WHERE space_id = $1 AND device_id = $2",
	pg::Query::Name{"count_device_plugin_params"},
};

PagingResult<model::DevicePluginParam> DevicePluginParam::GetList(const boost::uuids::uuid& spaceId, int64_t deviceId, int start, int limit) {
	PagingResult<model::DevicePluginParam> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectDevicePluginParams, spaceId, deviceId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount, spaceId, deviceId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
