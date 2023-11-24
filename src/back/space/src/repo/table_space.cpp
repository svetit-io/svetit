#include "table_space.hpp"
#include "../../../shared/errors.hpp"
#include "../../../shared/paging.hpp"
#include <chrono>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

#include <boost/uuid/uuid_io.hpp>

namespace svetit::space::table {

namespace pg = storages::postgres;

Space::Space(storages::postgres::ClusterPtr pg)
	: _pg{std::move(pg)}
{
	constexpr auto kCreateTable = R"~(
CREATE TABLE IF NOT EXISTS space (
	id UUID PRIMARY KEY,
	name TEXT NOT NULL,
	key TEXT NOT NULL,
	requestsAllowed BOOLEAN NOT NULL,
	createdAt BIGINT NOT NULL
);
)~";

	using storages::postgres::ClusterHostType;
	_pg->Execute(ClusterHostType::kMaster, kCreateTable);

	//InsertDataForMocks();
}

const storages::postgres::Query kInsertSpace{
	"INSERT INTO space (id, name, key, requestsAllowed, createdAt) "
	"VALUES ($1, $2, $3, $4, $5) ",
	storages::postgres::Query::Name{"insert_space"},
};

void Space::Insert(
	const boost::uuids::uuid& uuid,
	const std::string& name,
	const std::string& key,
	const bool requestsAllowed,
	const int64_t createdAt)
{
	_pg->Execute(storages::postgres::ClusterHostType::kMaster, kInsertSpace, uuid, name, key, requestsAllowed, createdAt);
}

const storages::postgres::Query kSelectSpaceAvailable{
	R"~(
		SELECT s.id, s.name, s.key, s.requestsAllowed, s.createdAt
		FROM space s
		LEFT JOIN space_user su ON s.id = su.spaceId AND su.userId = $1
		WHERE s.requestsAllowed = true AND su.spaceId IS NULL
		OFFSET $2 LIMIT $3;
	)~",
	storages::postgres::Query::Name{"select_space_available"},
};

const storages::postgres::Query kCountSpaceAvailable{
	R"~(
		SELECT count(*)
		FROM space s
		LEFT JOIN space_user su ON s.id = su.spaceId AND su.userId = $1
		WHERE s.requestsAllowed = true AND su.spaceId IS NULL;
	)~",
	storages::postgres::Query::Name{"count_space_available"},
};

PagingResult<model::Space> Space::SelectAvailable(const std::string& userId, const int offset, const int limit)
{
	PagingResult<model::Space> data;

	auto trx = _pg->Begin(storages::postgres::Transaction::RO);
	auto res = trx.Execute(kSelectSpaceAvailable, userId, offset, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCountSpaceAvailable, userId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

const storages::postgres::Query kSelectByUserId{
	R"~(
		SELECT s.id, s.name, s.key, s.requestsAllowed, s.createdAt
		FROM space s
		LEFT JOIN space_user su ON s.id = su.spaceId
		WHERE su.userId = $1
		OFFSET $2 LIMIT $3
	)~",
	storages::postgres::Query::Name{"select_space_by_user_id"},
};

const storages::postgres::Query kCountByUserId{
	R"~(
		SELECT count(*)
		FROM space s
		LEFT JOIN space_user su ON s.id = su.spaceId
		WHERE su.userId = $1
	)~",
	storages::postgres::Query::Name{"count_space_by_user_id"},
};

PagingResult<model::Space> Space::SelectByUserId(const std::string& userId, const int offset, const int limit)
{
	PagingResult<model::Space> data;

	auto trx = _pg->Begin(storages::postgres::Transaction::RO);
	auto res = trx.Execute(kSelectByUserId, userId, offset, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCountByUserId, userId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

const storages::postgres::Query kSelectSpaceByKey{
	"SELECT 1 FROM space WHERE key=$1",
	storages::postgres::Query::Name{"select_space_by_key"},
};

bool Space::IsExists(const std::string& key) {
	auto res = _pg->Execute(storages::postgres::ClusterHostType::kMaster, kSelectSpaceByKey, key);
	return !res.IsEmpty();
}

const storages::postgres::Query kSelectWithDateClauseForOwner {
	R"~(
		SELECT 1
		FROM space s
		LEFT JOIN space_user su ON s.id = su.spaceId
		WHERE s.createdAt >= $1 AND su.userId = $2 AND su.isOwner = true
	)~",
	storages::postgres::Query::Name{"select_space_with_date_clause_for_owner"},
};

bool Space::IsReadyForCreationByTime(const std::string& userId) {
	const auto minuteAgo = std::chrono::system_clock::now() - std::chrono::minutes(1);
	const auto minuteAgoTimestamp = std::chrono::duration_cast<std::chrono::seconds>(minuteAgo.time_since_epoch()).count();

	auto res = _pg->Execute(storages::postgres::ClusterHostType::kMaster, kSelectWithDateClauseForOwner, minuteAgoTimestamp, userId);
	return res.IsEmpty();
}

const storages::postgres::Query kCountSpacesWithUser {
	R"~(
		SELECT count(*)
		FROM space s
		LEFT JOIN space_user su ON s.id = su.spaceId
		WHERE su.userId = $1
	)~",
	storages::postgres::Query::Name{"count_spaces_with_user"},
};

int64_t Space::GetCountSpacesWithUser(const std::string& userId) {
	const auto res = _pg->Execute(storages::postgres::ClusterHostType::kMaster, kCountSpacesWithUser, userId);
	if (res.IsEmpty())
		return 0;

	return res.AsSingleRow<int64_t>();
}

const storages::postgres::Query kDelete {
	"DELETE FROM space WHERE id = $1",
	storages::postgres::Query::Name{"delete_space"},
};

void Space::Delete(const boost::uuids::uuid& spaceUuid) {
	auto res = _pg->Execute(storages::postgres::ClusterHostType::kMaster, kDelete, spaceUuid);
	if (!res.RowsAffected())
		throw errors::NotFound();
}

const storages::postgres::Query kSelectById{
	"SELECT id, name, key, requestsAllowed, createdAt FROM space WHERE id = $1",
	storages::postgres::Query::Name{"select_by_id"},
};

model::Space Space::SelectById(const boost::uuids::uuid& id) {
	auto res = _pg->Execute(storages::postgres::ClusterHostType::kMaster, kSelectById, id);
	if (res.IsEmpty())
		throw errors::NotFound{};

	return res.AsSingleRow<model::Space>(pg::kRowTag);
}

const storages::postgres::Query kSelectByKey{
	"SELECT id, name, key, requestsAllowed, createdAt FROM space WHERE key = $1",
	storages::postgres::Query::Name{"select_by_key"},
};

model::Space Space::SelectByKey(const std::string& key) {
	auto res = _pg->Execute(storages::postgres::ClusterHostType::kMaster, kSelectByKey, key);
	if (res.IsEmpty())
		throw errors::NotFound{};

	return res.AsSingleRow<model::Space>(pg::kRowTag);
}

void Space::InsertDataForMocks() {
	const auto p1 = std::chrono::system_clock::now();
	const auto now = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
	// insert test data
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "Пространство №1", "key1", true, now);
	Insert(utils::BoostUuidFromString("22222222-2222-2222-2222-222222222222"), "Пространство №2", "key2", true, now);
	Insert(utils::BoostUuidFromString("33333333-3333-3333-3333-333333333333"), "Пространство №3", "key3", true, now);
	Insert(utils::BoostUuidFromString("44444444-4444-4444-4444-444444444444"), "Пространство №4", "key4", true, now);
	Insert(utils::BoostUuidFromString("55555555-5555-5555-5555-555555555555"), "Пространство №5", "key5", true, now);
	Insert(utils::BoostUuidFromString("66666666-6666-6666-6666-666666666666"), "Пространство №6", "key6", true, now);
	Insert(utils::BoostUuidFromString("77777777-7777-7777-7777-777777777777"), "Пространство №7", "key7", true, now);
	Insert(utils::BoostUuidFromString("88888888-8888-8888-8888-888888888888"), "Пространство №8", "key8", true, now);
	Insert(utils::BoostUuidFromString("99999999-9999-9999-9999-999999999999"), "Пространство №9", "key9", true, now);
	Insert(utils::BoostUuidFromString("10000000-1000-1000-1000-100000000000"), "Пространство №10", "key10", true, now);
	Insert(utils::BoostUuidFromString("11000000-1100-1100-1100-110000000000"), "Пространство №11", "key11", true, now);
}

} // namespace svetit::space::table