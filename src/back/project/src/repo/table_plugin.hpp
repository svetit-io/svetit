#pragma once

#include "../model/plugin.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class Plugin final {
public:
	explicit Plugin(storages::postgres::ClusterPtr pg);
	model::Plugin Get(int64_t id);
	int64_t Create(const model::Plugin& plugin);
	void Update(const model::Plugin& plugin);
	void Delete(int64_t id);
	PagingResult<model::Plugin> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
