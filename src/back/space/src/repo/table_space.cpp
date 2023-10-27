#include "table_space.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::space::table {

namespace pg = storages::postgres;

Space::Space(storages::postgres::ClusterPtr pg)
	: _pg{std::move(pg)}
{
	constexpr auto kCreateTable = R"~(
CREATE TABLE IF NOT EXISTS space (
	id uuid PRIMARY KEY,
	name TEXT NOT NULL,
	key TEXT NOT NULL,
	requestsAllowed BOOLEAN NOT NULL,
	createdAt TIMESTAMPTZ NOT NULL DEFAULT NOW()
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
	std::chrono::system_clock::time_point createdAt)
{
	storages::postgres::Transaction transaction =
		_pg->Begin("insert_space_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	transaction.Execute(kInsertSpace, uuid, name, key, requestsAllowed, createdAt);
	transaction.Commit();
}

const storages::postgres::Query kSelectSpace{
	"SELECT id, name, key, requestsAllowed, createdAt FROM space OFFSET $1 LIMIT $2",
	storages::postgres::Query::Name{"select_space"},
};

std::vector<model::Space> Space::Select(const int& offset, const int& limit)
{
	storages::postgres::Transaction transaction =
		_pg->Begin("select_space_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	auto res = transaction.Execute(kSelectSpace, offset, limit);
	if (res.IsEmpty())
	{
		transaction.Commit();
		return {};
	}

	transaction.Commit();
	return res.AsContainer<std::vector<model::Space>>(pg::kRowTag);
}

const storages::postgres::Query kCountSpace{
	"SELECT count(id) FROM space",
	storages::postgres::Query::Name{"count_space"},
};

int Space::Count() {
	storages::postgres::Transaction transaction =
		_pg->Begin("count_space_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	auto res = transaction.Execute(kCountSpace);

	auto id = res.Front()[0].As<int64_t>();
	transaction.Commit();

	return id;
}

const storages::postgres::Query kSelectSpaceByKey{
	"SELECT * FROM space WHERE key=$1",
	storages::postgres::Query::Name{"select_space_by_key"},
};

bool Space::isExists(std::string key) {
	storages::postgres::Transaction transaction =
		_pg->Begin("select_space_by_key_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	auto res = transaction.Execute(kSelectSpaceByKey, key);

	transaction.Commit();
	return !res.IsEmpty();
}

void Space::InsertDataForMocks() {
	// insert test data
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "Пространство №1", "key1", true, std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("22222222-2222-2222-2222-222222222222"), "Пространство №2", "key2", true, std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("33333333-3333-3333-3333-333333333333"), "Пространство №3", "key3", true, std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("44444444-4444-4444-4444-444444444444"), "Пространство №4", "key4", true, std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("55555555-5555-5555-5555-555555555555"), "Пространство №5", "key5", true, std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("66666666-6666-6666-6666-666666666666"), "Пространство №6", "key6", true, std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("77777777-7777-7777-7777-777777777777"), "Пространство №7", "key7", true, std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("88888888-8888-8888-8888-888888888888"), "Пространство №8", "key8", true, std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("99999999-9999-9999-9999-999999999999"), "Пространство №9", "key9", true, std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("10000000-1000-1000-1000-100000000000"), "Пространство №10", "key10", true, std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("11000000-1100-1100-1100-110000000000"), "Пространство №11", "key11", true, std::chrono::system_clock::now());
}

} // namespace svetit::space::table