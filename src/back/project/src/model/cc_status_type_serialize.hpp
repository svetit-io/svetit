#pragma once

#include "cc_status_type.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const CcStatusType& ccStatusType,
	formats::serialize::To<formats::json::Value>);

CcStatusType Parse(const formats::json::Value& json,
	formats::parse::To<CcStatusType>);

} // namespace svetit::project::model