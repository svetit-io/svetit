#include "table_di_plugin_param.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

DiPluginParam::DiPluginParam(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kGet{
	"SELECT space_id, di_type_id, param_id FROM project.di_plugin_param WHERE space_id = $1 AND di_type_id = $2 AND param_id = $3",
	pg::Query::Name{"select_di_plugin_param"},
};

model::DiPluginParam DiPluginParam::Get(const boost::uuids::uuid& spaceId, int64_t diTypeId, int64_t paramId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, spaceId, diTypeId, paramId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::DiPluginParam>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.di_plugin_param (space_id, di_type_id, param_id) "
	"VALUES ($1, $2, $3)",
	pg::Query::Name{"insert_di_plugin_param"},
};

void DiPluginParam::Create(const model::DiPluginParam& item) {
	_pg->Execute(ClusterHostType::kMaster, kCreate, item.spaceId, item.diTypeId, item.paramId);
}

void DiPluginParam::Update(const model::DiPluginParam&) {
	throw errors::Forbidden403();
}

const pg::Query kDelete {
	"DELETE FROM project.di_plugin_param WHERE space_id = $1 AND di_type_id = $2 AND param_id = $3",
	pg::Query::Name{"delete_di_plugin_param"},
};

void DiPluginParam::Delete(const boost::uuids::uuid& spaceId, int64_t diTypeId, int64_t paramId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, spaceId, diTypeId, paramId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectDiPluginParams{
	"SELECT space_id, di_type_id, param_id FROM project.di_plugin_param "
	"WHERE space_id = $1 AND di_type_id = $2"
	"OFFSET $3 LIMIT $4",
	pg::Query::Name{"select_di_plugin_params"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.di_plugin_param "
	"WHERE space_id = $1 AND di_type_id = $2",
	pg::Query::Name{"count_di_plugin_params"},
};

PagingResult<model::DiPluginParam> DiPluginParam::GetList(const boost::uuids::uuid& spaceId, int64_t diTypeId, int start, int limit) {
	PagingResult<model::DiPluginParam> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectDiPluginParams, spaceId, diTypeId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount, spaceId, diTypeId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
