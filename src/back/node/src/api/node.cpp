#include "node.hpp"
#include "../model/node_serialize.hpp"
#include "../service/service.hpp"
#include <shared/headers.hpp>
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/paging.hpp>
#include <shared/parse/request.hpp>
#include <shared/schemas.hpp>
#include <shared/parse/uuid.hpp>

namespace svetit::node::handlers {

Node::Node(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}

formats::json::Value Node::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		const auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);

		switch (req.GetMethod()) {
			case server::http::HttpMethod::kGet:
				return Get(res, params);
			case server::http::HttpMethod::kPost:
				return Post(req, body, res, params);
			case server::http::HttpMethod::kPut:
				return Put(req, body, res, params);
			case server::http::HttpMethod::kDelete:
				return Delete(req, res, params);
			default:
				throw std::runtime_error("Unsupported");
				break;
		}
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

formats::json::Value Node::Get(
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto userId = params[headers::kUserId].As<std::string>();
	const auto id = params["id"].As<boost::uuids::uuid>();
	const auto spaceId = params[headers::kSpaceId].As<boost::uuids::uuid>();

	res = _s.Get(id, userId, spaceId);
	return res.ExtractValue();
}

formats::json::Value Node::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto userId = params[headers::kUserId].As<std::string>();
	const auto spaceId = params[headers::kSpaceId].As<boost::uuids::uuid>();

	const auto id = params["id"].As<boost::uuids::uuid>();

	_s.Delete(id, userId, spaceId);

	req.SetResponseStatus(server::http::HttpStatus::kNoContent);
	return res.ExtractValue();
}

formats::json::Value Node::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto userId = params[headers::kUserId].As<std::string>();
	const auto spaceId = params[headers::kSpaceId].As<boost::uuids::uuid>();

	auto node = body.As<model::Node>();

	_s.Create(node, userId, spaceId);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

formats::json::Value Node::Put(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto userId = params[headers::kUserId].As<std::string>();
	const auto spaceId = params[headers::kSpaceId].As<boost::uuids::uuid>();

	auto node = body.As<model::Node>();

	_s.Update(node, userId, spaceId);

	req.SetResponseStatus(server::http::HttpStatus::kNoContent);
	return res.ExtractValue();
}

} // namespace svetit::node::handlers
