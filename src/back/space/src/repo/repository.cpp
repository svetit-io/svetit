#include "repository.hpp"

#include "table_space.hpp"
#include "table_space_user.hpp"
#include "table_space_invitation.hpp"
#include "table_space_link.hpp"

#include "userver/components/component_config.hpp"
#include "userver/components/component_context.hpp"
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::space {

namespace pg = storages::postgres;
using pg::ClusterHostType;

/*static*/ yaml_config::Schema Repository::GetStaticConfigSchema()
{
	return yaml_config::MergeSchemas<components::LoggableComponentBase>(R"(
type: object
description: Main repository component
additionalProperties: false
properties:
  some-var:
    type: string
    description: some desc
)");
}

Repository::Repository(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx)
	: components::LoggableComponentBase{conf, ctx}
	, _pg{ctx.FindComponent<components::Postgres>("database").GetCluster()}
	, _space{_pg}
	, _spaceUser{_pg}
	, _spaceInvitation{_pg}
	, _spaceLink{_pg}
{}

table::Space& Repository::Space() {
	return _space;
}

table::SpaceUser& Repository::SpaceUser() {
	return _spaceUser;
}

table::SpaceInvitation& Repository::SpaceInvitation() {
	return _spaceInvitation;
}

table::SpaceLink& Repository::SpaceLink() {
	return _spaceLink;
}

const pg::Query kSelectSpaceAvailable{
	R"~(
		SELECT s.id, s.name, s.key, s.requestsAllowed, s.createdAt
		FROM space.space s
		LEFT JOIN space.user su ON s.id = su.spaceId AND su.userId = $1
		WHERE s.requestsAllowed = true AND su.spaceId IS NULL
		OFFSET $2 LIMIT $3;
	)~",
	pg::Query::Name{"select_space_available"},
};

const pg::Query kCountSpaceAvailable{
	R"~(
		SELECT COUNT(*)
		FROM space.space s
		LEFT JOIN space.user su ON s.id = su.spaceId AND su.userId = $1
		WHERE s.requestsAllowed = true AND su.spaceId IS NULL;
	)~",
	pg::Query::Name{"count_space_available"},
};

PagingResult<model::Space> Repository::SelectAvailable(const std::string& userId, int offset, int limit)
{
	PagingResult<model::Space> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectSpaceAvailable, userId, offset, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCountSpaceAvailable, userId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

const pg::Query kSelectSpaceAvailableBySpaceName{
	R"~(
		SELECT s.id, s.name, s.key, s.requestsAllowed, s.createdAt
		FROM space.space s
		LEFT JOIN space.user su ON s.id = su.spaceId AND su.userId = $1
		WHERE s.requestsAllowed = true AND su.spaceId IS NULL AND s.name LIKE $2
		OFFSET $3 LIMIT $4;
	)~",
	pg::Query::Name{"select_space_available_by_space_name"},
};

const pg::Query kCountSpaceAvailableBySpaceName{
	R"~(
		SELECT COUNT(*)
		FROM space.space s
		LEFT JOIN space.user su ON s.id = su.spaceId AND su.userId = $1
		WHERE s.requestsAllowed = true AND su.spaceId IS NULL AND s.name LIKE $2;
	)~",
	pg::Query::Name{"count_space_available_by_space_name"},
};

PagingResult<model::Space> Repository::SelectAvailableBySpaceName(const std::string& spaceName, const std::string& userId, int offset, int limit)
{
	PagingResult<model::Space> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectSpaceAvailableBySpaceName, userId, '%' + spaceName + '%', offset, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCountSpaceAvailableBySpaceName, userId, '%' + spaceName + '%');
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

const pg::Query kSelectByUserId{
	R"~(
		SELECT s.id, s.name, s.key, s.requestsAllowed, s.createdAt
		FROM space.space s
		LEFT JOIN space.user su ON s.id = su.spaceId
		WHERE su.userId = $1
		OFFSET $2 LIMIT $3
	)~",
	pg::Query::Name{"select_space_by_user_id"},
};

const pg::Query kCountByUserId{
	R"~(
		SELECT COUNT(*)
		FROM space.space s
		LEFT JOIN space.user su ON s.id = su.spaceId
		WHERE su.userId = $1
	)~",
	pg::Query::Name{"count_space_by_user_id"},
};

PagingResult<model::Space> Repository::SelectByUserId(const std::string& userId, int offset, int limit)
{
	PagingResult<model::Space> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectByUserId, userId, offset, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCountByUserId, userId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

const pg::Query kSelectWithDateClauseForOwner {
	R"~(
		SELECT 1
		FROM space.space s
		LEFT JOIN space.user su ON s.id = su.spaceId
		WHERE s.createdAt >= $1 AND su.userId = $2 AND su.isOwner = true
	)~",
	pg::Query::Name{"select_space_with_date_clause_for_owner"},
};

bool Repository::IsReadyForCreationByTime(const std::string& userId) {
	const auto minuteAgo = std::chrono::system_clock::now() - std::chrono::minutes(1);
	const auto minuteAgoTimestamp = std::chrono::duration_cast<std::chrono::seconds>(minuteAgo.time_since_epoch()).count();

	auto res = _pg->Execute(ClusterHostType::kMaster, kSelectWithDateClauseForOwner, minuteAgoTimestamp, userId);
	return res.IsEmpty();
}

const pg::Query kCountSpacesWithUser {
	R"~(
		SELECT COUNT(*)
		FROM space.space s
		LEFT JOIN space.user su ON s.id = su.spaceId
		WHERE su.userId = $1
	)~",
	pg::Query::Name{"count_spaces_with_user"},
};

int64_t Repository::GetCountSpacesWithUser(const std::string& userId) {
	const auto res = _pg->Execute(ClusterHostType::kMaster, kCountSpacesWithUser, userId);
	if (res.IsEmpty())
		return 0;

	return res.AsSingleRow<int64_t>();
}

const pg::Query kInsertSpace{
	"INSERT INTO space.space (name, key, requestsAllowed) "
	"VALUES ($1, $2, $3) RETURNING id",
	pg::Query::Name{"insert_space"},
};

const pg::Query kInsertSpaceUser{
	"INSERT INTO space.user (spaceId, userId, isOwner, role) "
	"VALUES ($1, $2, $3, $4) ",
	pg::Query::Name{"insert_space_user"},
};

void Repository::CreateSpaceAndItsOwner(const std::string& name, const std::string& key, bool requestsAllowed, const std::string& userId)
{
	auto trx = _pg->Begin(pg::Transaction::RW);
	auto res = trx.Execute(kInsertSpace, name, key, requestsAllowed);
	const auto spaceUuid = res.AsSingleRow<boost::uuids::uuid>();
	res = trx.Execute(kInsertSpaceUser, spaceUuid, userId, true, Role::Admin);
	trx.Commit();
}

const pg::Query kSelectSpaceInvitation{
	R"~(
		SELECT si.id, si.spaceId, si.creatorId, si.userId, si.role, si.createdAt
		FROM space.invitation si
		WHERE si.userId = $1 OR si.spaceId IN (
			SELECT su.spaceId FROM space.user su WHERE su.userId = $1 AND su.role = 3
		) OFFSET $2 LIMIT $3
	)~",
	pg::Query::Name{"select_space.invitation"},
};

const pg::Query kCountSpaceInvitation{
	R"~(
		SELECT COUNT(*)
		FROM space.invitation si
		WHERE si.userId = $1 OR si.spaceId IN (
			SELECT su.spaceId FROM space.user su WHERE su.userId = $1 AND su.role = 3
		)
	)~",
	pg::Query::Name{"count_space.invitation"},
};

PagingResult<model::SpaceInvitation> Repository::SelectInvitationsForSpaceList(int start, int limit, const std::string& userId) {
	PagingResult<model::SpaceInvitation> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectSpaceInvitation, userId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCountSpaceInvitation, userId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

const pg::Query kSelectByLink{
	"SELECT id, name, key, requestsAllowed, createdAt FROM space.space WHERE id = (SELECT spaceId FROM space.link WHERE id = $1)",
	pg::Query::Name{"select_by_linl"},
};

model::Space Repository::SelectByLink(const boost::uuids::uuid& link) {
	const auto res = _pg->Execute(ClusterHostType::kMaster, kSelectByLink, link);
	if (res.IsEmpty())
		throw errors::NotFound{};

	return res.AsSingleRow<model::Space>(pg::kRowTag);
}

} // namespace svetit::space