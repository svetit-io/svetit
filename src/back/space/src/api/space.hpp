#pragma once

#include <string>

#include "../model/space_serialize.hpp"

#include <boost/lexical_cast.hpp>
#include <regex>
#include <boost/uuid/uuid.hpp>
#include <boost/algorithm/string.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::space {
class Service;
} // namespace svetit::space

namespace svetit::space::handlers {

class Space final : public server::handlers::HttpHandlerJsonBase {
public:
	static constexpr std::string_view kName = "handler-space";

	explicit Space(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	formats::json::Value HandleRequestJsonThrow(
		const server::http::HttpRequest& req,
		const formats::json::Value& body,
		server::request::RequestContext&) const override;

	formats::json::Value Get(
		const server::http::HttpRequest& req,
		const formats::json::Value& body) const;

	formats::json::Value Post(
		const server::http::HttpRequest& req,
		const formats::json::Value& body) const;

	formats::json::Value Delete(
		const server::http::HttpRequest& req,
		const formats::json::Value& body) const;

private:
	Service& _s;
};

} // namespace svetit::space::handlers