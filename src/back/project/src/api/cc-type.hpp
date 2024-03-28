#pragma once

#include <string>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project {
class Service;
} // namespace svetit::project

namespace svetit::project::handlers {

class CcType final : public server::handlers::HttpHandlerJsonBase {
public:
	static constexpr std::string_view kName = "handler-cc-type";

	explicit CcType(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	formats::json::Value HandleRequestJsonThrow(
		const server::http::HttpRequest& req,
		const formats::json::Value& body,
		server::request::RequestContext&) const override;

private:
	Service& _s;
};

} // namespace svetit::project::handlers