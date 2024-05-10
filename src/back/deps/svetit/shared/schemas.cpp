#include "schemas.hpp"
#include "errors.hpp"
#include "userver/formats/json/serialize.hpp"
#include "userver/formats/json/value.hpp"
#include "userver/formats/json/value_builder.hpp"
#include "userver/server/http/http_method.hpp"

#include <fmt/core.h>
#include <ranges>
#include <utility>
#include <string>
#include <fmt/format.h>
#include <boost/algorithm/string.hpp>

#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/fs/blocking/read.hpp>
#include <userver/utils/text.hpp>

namespace svetit {

formats::json::Value getBodySchema(
	const formats::json::Value& requestSchema,
	const std::string& path
) {
	if (!requestSchema.HasMember("requestBody"))
		return {};

	const auto requestBodyParam = requestSchema["requestBody"];
	const auto requestBodyPath = path + '/' + boost::algorithm::to_lower_copy(requestBodyParam.As<std::string>());
	return formats::json::blocking::FromFile(requestBodyPath);
}

constexpr inline auto enum_range = [](auto front, auto back) {
	return std::views::iota(std::to_underlying(front), std::to_underlying(back) + 1)
		| std::views::transform([](auto e) { return decltype(front)(e); });
};

std::map<server::http::HttpMethod, RequestAndJsonSchema> LoadSchemas(
	const std::string_view& handlerName, const std::string& schemasFolder)
{
	std::map<server::http::HttpMethod, RequestAndJsonSchema> res;
	const auto schemasPath = boost::algorithm::trim_right_copy_if(schemasFolder, [](auto c) { return c == '/'; });
	const auto pathFmt = fmt::format("{}/{}-{{}}.json", schemasPath, handlerName);
	for (const auto m : enum_range(server::http::HttpMethod::kDelete, server::http::HttpMethod::kUnknown))
	{
		const auto methodStr = utils::text::ToLower(server::http::ToString(m));
		const auto filePath = fmt::vformat(pathFmt, fmt::make_format_args(methodStr));
		if (!fs::blocking::FileExists(filePath))
			continue;

		auto req = formats::json::blocking::FromFile(filePath);

		RequestAndJsonSchema schemas;

		if (req.HasMember("properties"))
		{
			schemas.requestProps = req["properties"];
			schemas.request.reset(new formats::json::Schema{req});
		}

		auto body = getBodySchema(req, schemasPath);
		if (!body.IsEmpty())
			schemas.body.reset(new formats::json::Schema{body});

		res.emplace(m, std::move(schemas));
	}

	return res;
}

formats::json::Value strToJson(const std::string& value, const std::string& type)
{
	if (type == "string")
	{
		formats::json::ValueBuilder res;
		res = value;
		return res.ExtractValue();
	}

	return formats::json::FromString(value);
}

formats::json::Value requestParamsToJson(
	const formats::json::Value& properties,
	const server::http::HttpRequest& req
) {
	formats::json::ValueBuilder res;
	for (auto it = properties.begin(); it != properties.end(); ++it)
	{
		const auto param = it.GetName();
		const auto type = (*it)["type"].As<std::string>();
		const auto in = (*it)["in"].As<std::string>();
		if (in == "query") {
			if (req.HasArg(param))
				res[param] = strToJson(req.GetArg(param), type);
		} else if (in == "header") {
			if (req.HasHeader(param))
				res[param] = strToJson(req.GetHeader(param), type);
		} else if (in == "path") {
			if (req.HasPathArg(param))
				res[param] = strToJson(req.GetPathArg(param), type);
		} else {
			LOG_WARNING() << "Unknown parameter destination: " << in
				<< " Name: " << param
				<< " Url: " << req.GetUrl();
		}
	}

	return res.ExtractValue();
}

void ValidateRequest(
	const std::map<server::http::HttpMethod, RequestAndJsonSchema>& schemasMap,
	const server::http::HttpRequest& req
) {
	static const formats::json::Value emptyBody;
	ValidateRequest(schemasMap, req, emptyBody);
}

void ValidateRequest(
	const std::map<server::http::HttpMethod, RequestAndJsonSchema>& schemasMap,
	const server::http::HttpRequest& req,
	const formats::json::Value& body
) {
	const auto& schemas = schemasMap.at(req.GetMethod());
	try {
		if (schemas.request)
		{
			const auto reqParams = requestParamsToJson(schemas.requestProps, req);
			if (!formats::json::Validate(reqParams, *schemas.request))
				throw errors::BadRequest400("Wrong params/headers");
		}

		if (schemas.body && !formats::json::Validate(body, *schemas.body))
			throw errors::BadRequest400("Wrong body");
	} catch (formats::json::ParseException& e) {
		LOG_ERROR()
			<< "Method: " << req.GetMethodStr()
			<< " Url: " << req.GetUrl()
			<< " Err: " << e.what()
			<< " Body: " << req.RequestBody().substr(0, 1024);
		throw errors::BadRequest400("Wrong params/headers");
	}
}

} // namespace svetit
