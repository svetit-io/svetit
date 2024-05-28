#pragma once

#include <string>
#include <string_view>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::space {
class Service;
} // namespace svetit::space

namespace svetit::space::handlers {

class Introspect final : public server::handlers::HttpHandlerBase {
public:
	static constexpr std::string_view kName = "handler-introspect";

	explicit Introspect(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	std::string HandleRequestThrow(
		const server::http::HttpRequest& req,
		server::request::RequestContext&) const override;

private:
	Service& _s;
};

} // namespace svetit::space::handlers
