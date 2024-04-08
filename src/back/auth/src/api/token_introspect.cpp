#include "token_introspect.hpp"
#include "../service/service.hpp"
#include "../model/consts.hpp"
#include <shared/headers.hpp>

namespace svetit::auth::handlers {

TokenIntrospect::TokenIntrospect(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

std::string TokenIntrospect::HandleRequestThrow(
	const server::http::HttpRequest& req,
	server::request::RequestContext&) const
{
	auto& token = req.GetCookie(Consts::SessionCookieName);

	try {
		const auto data = _s.Session().Token().Verify(token);

		req.GetHttpResponse().SetHeader(headers::kUserId, data._userId);
		req.GetHttpResponse().SetHeader(headers::kSessionId, data._sessionId);
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to get user ID from token: " << e.what();
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return "Invalid authorization token";
	}
	return "Ok";
}

} // namespace svetit::auth::handlers
