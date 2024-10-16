#include <userver/components/minimal_server_component_list.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include "service/tokenizer.hpp"
#include "service/oidc.hpp"
#include "repo/repository.hpp"
#include "service/service.hpp"
#include "api/token_introspect.hpp"
#include "api/token_refresh.hpp"
#include "api/login.hpp"
#include "api/login_callback.hpp"
#include "api/logout.hpp"
#include "api/logout_callback.hpp"
#include "api/user_info.hpp"
#include "api/user_byid.hpp"
#include "api/user_list.hpp"

using namespace svetit::auth;

int main(int argc, char* argv[]) {
	auto component_list = components::MinimalServerComponentList()
		.Append<server::handlers::Ping>()
		.Append<components::TestsuiteSupport>()
		.Append<components::HttpClient>()
		.Append<components::Postgres>("database")
		.Append<clients::dns::Component>()
		.Append<server::handlers::TestsControl>()

		.Append<Tokenizer>()
		.Append<OIDConnect>()
		.Append<RepositoryComponent>()
		.Append<Service>()
		.Append<handlers::Login>()
		.Append<handlers::LoginCallback>()
		.Append<handlers::Logout>()
		.Append<handlers::LogoutCallback>()
		.Append<handlers::TokenRefresh>()
		.Append<handlers::TokenIntrospect>()
		.Append<handlers::UserInfo>()
		.Append<handlers::UserById>()
		.Append<handlers::UserList>()
		;

	return utils::DaemonMain(argc, argv, component_list);
}
