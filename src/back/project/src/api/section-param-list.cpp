#include "section-param-list.hpp"
#include "../service/service.hpp"

namespace svetit::project::handlers {

SectionParamList::SectionParamList(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value SectionParamList::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	return res.ExtractValue();
}

} // namespace svetit::project::handlers
