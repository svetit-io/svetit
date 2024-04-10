#pragma once

#include "di_plugin_param.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const DiPluginParam& item,
	formats::serialize::To<formats::json::Value>);

DiPluginParam Parse(const formats::json::Value& json,
	formats::parse::To<DiPluginParam>);

} // namespace svetit::project::model