#include "cc-di-list.hpp"
#include "../service/service.hpp"
// #include <shared/errors.hpp>
// #include <shared/errors_catchit.hpp>
// #include <shared/headers.hpp>

namespace svetit::project::handlers {

CcDiList::CcDiList(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value CcDiList::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	return res.ExtractValue();
}

} // namespace svetit::project::handlers
