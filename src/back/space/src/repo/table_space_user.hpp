#pragma once

#include "../model/space_user.hpp"
#include "../model/role.hpp"

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::space::table {

class SpaceUser final {
public:
	explicit SpaceUser(storages::postgres::ClusterPtr pg);
	void Insert(
		const boost::uuids::uuid& spaceId,
		const std::string& userId,
		const bool isOwner,
		int64_t joinedAt,
		const Role::Type& role);
	void InsertDataForMocks();
	void DeleteBySpace(const boost::uuids::uuid& spaceUuid);
	bool IsOwner(const boost::uuids::uuid& spaceUuid, const std::string& userId);
	bool IsUserInside(const boost::uuids::uuid& spaceUuid, const std::string& userId);
	model::SpaceUser GetByIds(const boost::uuids::uuid& spaceUuid, const std::string& userId);
	bool IsAdmin(const boost::uuids::uuid& spaceUuid, const std::string& userId);
	void Delete(const boost::uuids::uuid& spaceUuid, const std::string& userId);
	void Update(const model::SpaceUser& user);
	std::vector<model::SpaceUser> Get(const boost::uuids::uuid& spaceUuid, const int start, const int limit);
	int CountBySpaceId(const boost::uuids::uuid& spaceId);

private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::space::table
