#pragma once

#include "userver/formats/json/value.hpp"
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit {

struct Paging {
	int start, limit;
};

template<typename T>
struct PagingResult {
	std::vector<T> items;
	int64_t total = 0;
};

Paging parsePaging(const server::http::HttpRequest& req);
Paging parsePaging(const formats::json::Value& params);

} // namespace svetit
