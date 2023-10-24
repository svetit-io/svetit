#include <userver/components/minimal_server_component_list.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include "repo/repository.hpp"
#include "service/service.hpp"

#include "api/check-key.hpp"
#include "api/create.hpp"
#include "api/delete.hpp"
#include "api/info.hpp"
#include "api/invitation.hpp"
#include "api/invitation-manage.hpp"
#include "api/link.hpp"
#include "api/link-manage.hpp"
#include "api/list-available.hpp"
#include "api/list.hpp"

using namespace svetit::space;

int main(int argc, char* argv[]) {
	auto component_list = components::MinimalServerComponentList()
		.Append<server::handlers::Ping>()
		.Append<components::TestsuiteSupport>()
		.Append<components::HttpClient>()
		.Append<components::Postgres>("database")
		.Append<clients::dns::Component>()
		.Append<server::handlers::TestsControl>()

		.Append<Repository>()
		.Append<Service>()
		.Append<handlers::CheckKey>()
		.Append<handlers::Create>()
		.Append<handlers::Delete>()
		.Append<handlers::Info>()
		.Append<handlers::InvitationManage>()
		.Append<handlers::Invitation>()
		.Append<handlers::LinkManage>()
		.Append<handlers::Link>()
		.Append<handlers::ListAvailable>()
		.Append<handlers::List>()
		;

	return utils::DaemonMain(argc, argv, component_list);
}
