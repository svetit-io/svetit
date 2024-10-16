#pragma once

#include "role.hpp"
#include <boost/uuid/uuid.hpp>
#include <chrono>

namespace svetit::space::model {

struct SpaceUser {
	boost::uuids::uuid spaceId;
	std::string userId;
	bool isOwner;
	std::chrono::system_clock::time_point joinedAt;
	int roleId;
};

} // namespace svetit::space::model