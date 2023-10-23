#pragma once

#include "../model/model.hpp"

#include <string>
#include <string_view>
#include <vector>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/http/url.hpp>

namespace svetit::space {

class Service final : public components::LoggableComponentBase {
public:
	static constexpr std::string_view kName = "main-service";
	static yaml_config::Schema GetStaticConfigSchema();

	explicit Service(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	std::vector<svetit::space::Space> GetList();
	std::vector<svetit::space::SpaceInvitation> GetInvitationList();
	std::vector<svetit::space::SpaceLink> GetLinkList();
	std::vector<svetit::space::SpaceUser> GetUserList();

private:
	std::string _someUrl;
	std::vector<svetit::space::Space> _spaces;
	std::vector<svetit::space::SpaceInvitation> _invitations;
	std::vector<svetit::space::SpaceLink> _links;
	std::vector<svetit::space::SpaceUser> _users;
};

} // namespace svetit::space
