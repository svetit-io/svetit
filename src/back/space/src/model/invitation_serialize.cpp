#include "invitation_serialize.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <optional>

namespace svetit::space::model {

formats::json::Value Serialize(
	const SpaceInvitation& si,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};

	int roleId = 0;
	if (si.roleId.has_value())
		roleId = si.roleId.value();

	builder["id"] = si.id;
	builder["userId"] = si.userId;
	builder["roleId"] = roleId;
	builder["creatorId"] = si.creatorId;
	builder["createdAt"] = std::chrono::duration_cast<std::chrono::seconds>(si.createdAt.time_since_epoch()).count();

	return builder.ExtractValue();
}

SpaceInvitation Parse(
	const formats::json::Value& json,
	formats::parse::To<SpaceInvitation>)
{
	const auto spaceIdStr = json["spaceId"].As<std::string>();
	const auto spaceId = spaceIdStr.empty() ? boost::uuids::uuid{} : utils::BoostUuidFromString(spaceIdStr);

	const std::chrono::system_clock::time_point createdAt{std::chrono::seconds{json["createdAt"].As<int64_t>(0)}};

	const auto roleIdParsed = json["roleId"].As<int>(0);
	std::optional<int> roleIdOptional;
	if (roleIdParsed != 0)
		roleIdOptional = roleIdParsed;

	return {
		.id = json["id"].As<int>(0),
		.spaceId = spaceId,
		.creatorId = json["creatorId"].As<std::string>(""),
		.userId = json["userId"].As<std::string>(),
		.roleId = roleIdOptional,
		.createdAt = createdAt
	};
}

} // namespace svetit::space::model