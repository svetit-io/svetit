#include "invitation-manage.hpp"

namespace svetit::space::handlers {

InvitationManage::InvitationManage(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
{}

formats::json::Value InvitationManage::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	res = formats::json::FromString(R"({
    "items":[]
  	})");

	return res.ExtractValue();
}

} // namespace svetit::space::handlers
