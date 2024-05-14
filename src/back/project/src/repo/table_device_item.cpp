#include "table_device_item.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

DeviceItem::DeviceItem(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kGet{
	"SELECT id, device_id, type_id, name FROM project.device_item WHERE id = $1",
	pg::Query::Name{"select_device_item"}
};

model::DeviceItem DeviceItem::Get(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::DeviceItem>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO project.device_item (device_id, type_id, name) "
	"VALUES ($1, $2, $3)",
	pg::Query::Name{"insert_device_item"},
};

void DeviceItem::Insert(
	int deviceId,
	int typeId,
	const std::string& name)
{
	_pg->Execute(ClusterHostType::kMaster, kInsert, deviceId, typeId, name);
}

const pg::Query kUpdate {
	"UPDATE project.device_item SET device_id = $2, type_id = $3, name = $4 "
	"WHERE id = $1",
	pg::Query::Name{"update_device_item"},
};

void DeviceItem::Update(const model::DeviceItem& deviceItem) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, deviceItem.id, deviceItem.deviceId, deviceItem.typeId, deviceItem.name);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.device_item WHERE id = $1",
	pg::Query::Name{"delete_device_item"},
};

void DeviceItem::Delete(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectDeviceItems{
	"SELECT id, device_id, type_id, name FROM project.device_item "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_device_items"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.device_item",
	pg::Query::Name{"count_device_items"},
};

PagingResult<model::DeviceItem> DeviceItem::GetList(int start, int limit) {
	PagingResult<model::DeviceItem> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectDeviceItems, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
