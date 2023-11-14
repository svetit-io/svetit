#pragma once

#include "role.hpp"
#include "space_user.hpp"
#include <vector>

#include <boost/uuid/uuid_io.hpp>

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include <userver/formats/json/value_builder.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::space::model {

formats::json::Value Serialize(
	const SpaceUser& su,
	formats::serialize::To<formats::json::Value>);

SpaceUser Parse(const formats::json::Value& json,
	formats::parse::To<SpaceUser>);

} // namespace svetit::space::model