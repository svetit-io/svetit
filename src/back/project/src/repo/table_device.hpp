#pragma once

#include "../model/device.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class Device final {
public:
	explicit Device(storages::postgres::ClusterPtr pg);
	model::Device Get(int64_t id);
	int64_t Create(const model::Device& device);
	void Update(const model::Device& device);
	void Delete(int64_t id);
	PagingResult<model::Device> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
