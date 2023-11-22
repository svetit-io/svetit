#pragma once

#include "../model/role.hpp"
#include "../model/space.hpp"
#include "../model/space_invitation.hpp"
#include "../model/space_link.hpp"
#include "../model/space_user.hpp"

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/http/url.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::space {

class Repository;

class Service final : public components::LoggableComponentBase {
public:
	static constexpr std::string_view kName = "main-service";
	static yaml_config::Schema GetStaticConfigSchema();

	explicit Service(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);
	std::vector<model::Space> GetList(const std::string& userId, const unsigned int start, const unsigned int limit);
	std::vector<model::Space> GetAvailableList(const std::string& userId, const unsigned int start, const unsigned int limit);
	int GetCount(const std::string& userId);
	int GetAvailableCount(const std::string& userId);
	std::vector<model::SpaceInvitation> GetInvitationList(const unsigned int start, const unsigned int limit);
	int GetInvitationsCount();
	std::vector<model::SpaceLink> GetLinkList(const unsigned int start, const unsigned int limit);
	int GetLinksCount();
	std::vector<model::SpaceUser> GetUserList(const std::string& userId, const std::string& spaceId, const unsigned int start, const unsigned int limit);
	int GetUserCount(const std::string& userId, const std::string& spaceId);
	bool isSpaceExistsByKey(const std::string& key);
	bool isCanCreate();
	int CountInvitationAvailable(const std::string& userId);
	bool CheckKeyByRegex(const std::string& key);
	bool IsKeyValid(const std::string& key);
	bool KeyAdditionalCheck(const std::string& key, const std::string& userId);
	bool IsUserTimeouted(const std::string& userId);
	bool IsLimitReached(const std::string& userId);
	void Create(const std::string& name, const std::string& key, const bool requestsAllowed, const std::string& userId);
	void Delete(const std::string& id);
	bool IsSpaceOwner(const std::string& id, const std::string& userId);
	bool ValidateRole(const Role::Type& role);
	void Invite(const std::string& creatorId, const boost::uuids::uuid& spaceId, const std::string& userId, const Role::Type& role);
	void ChangeRoleInInvitation(const int id, const Role::Type& role);
	void ApproveInvitation(const int id);
	void DeleteInvitation(const int id);
	bool CheckExpiredAtValidity(const int64_t expiredAt);
	void CreateInvitationLink(const boost::uuids::uuid& spaceId, const std::string& creatorId, const std::string& name, const int64_t expiredAt);
	void DeleteInvitationLink(const std::string& id);
	model::Space GetById(const std::string& id, const std::string& userId);
	model::Space GetByKey(const std::string& key, const std::string& userId);
	model::Space GetByLink(const std::string& link);
	bool IsLinkExpired(const std::string& link);
	void InviteByLink(const std::string& creatorId, const std::string& link);
	bool CanDeleteUser(const std::string& requestUserId, const std::string& spaceId, const std::string& userId);
	void DeleteUser(const std::string& spaceId, const std::string& userId);
	bool CanUpdateUser(const bool isRoleMode, const bool isOwner, const boost::uuids::uuid& spaceUuid, const std::string& userId, const std::string& headerUserId);
	void UpdateUser(const bool isRoleMode, const Role::Type& role, const bool isOwner, const boost::uuids::uuid& spaceId, const std::string& userId, const std::string& headerUserid);

private:
	std::vector<model::SpaceUser> _users;
	Repository& _repo;
	bool _canCreate;
	std::string _defaultSpace;
	int _spacesLimitForUser;
	int _itemsLimitForList;
};

} // namespace svetit::space