#include "device-item-list.hpp"
#include "../service/service.hpp"
#include "../model/device_item_serialize.hpp"
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/paging.hpp>
#include <shared/paging_serialize.hpp>
#include <shared/parse/request.hpp>

namespace svetit::project::handlers {

DeviceItemList::DeviceItemList(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value DeviceItemList::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		auto paging = parsePaging(req);
		res = _s.GetDeviceItemList(paging.start, paging.limit);
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

} // namespace svetit::project::handlers
