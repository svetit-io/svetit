#include "session_refresh_serialize.hpp"

#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/strong_typedef.hpp>

namespace svetit::auth::model {

formats::json::Value Serialize(
	const SessionRefresh& s,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};
	builder["token"] = s._token;

	return builder.ExtractValue();
}

SessionRefresh Parse(
	const formats::json::Value& json,
	formats::parse::To<SessionRefresh>)
{
	return {
		._token = json["token"].As<std::string>()
	};
}

} // namespace svetit::auth::model