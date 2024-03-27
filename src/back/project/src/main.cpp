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
#include "api/details.hpp"
#include "api/project.hpp"
#include "api/project-list.hpp"
#include "api/param-type.hpp"
#include "api/param-type-list.hpp"
#include "api/project-param.hpp"
#include "api/project-param-list.hpp"
#include "api/section.hpp"
#include "api/section-list.hpp"
#include "api/section-param.hpp"
#include "api/section-param-list.hpp"
#include "api/cc-type.hpp"
#include "api/cc-type-list.hpp"
#include "api/control-circuit.hpp"
#include "api/control-circuit-list.hpp"
#include "api/plugin.hpp"
#include "api/plugin-list.hpp"
#include "api/device.hpp"
#include "api/device-list.hpp"
#include "api/device-plugin-param.hpp"
#include "api/device-plugin-param-list.hpp"
#include "api/code.hpp"
#include "api/code-list.hpp"
#include "api/measure.hpp"
#include "api/measure-list.hpp"
#include "api/save-timer.hpp"
#include "api/save-timer-list.hpp"
#include "api/cc-type-param.hpp"
#include "api/cc-type-param-list.hpp"
#include "api/di-type.hpp"
#include "api/di-type-list.hpp"
#include "api/di-plugin-param.hpp"
#include "api/di-plugin-param-list.hpp"
#include "api/cc-type-di-type.hpp"
#include "api/cc-type-di-type-list.hpp"
#include "api/device-item.hpp"
#include "api/device-item-list.hpp"
#include "api/cc-mode-type.hpp"
#include "api/cc-mode-type-list.hpp"
#include "api/cc-di.hpp"
#include "api/cc-di-list.hpp"
#include "api/cc-param.hpp"
#include "api/cc-param-list.hpp"
#include "api/cc-status-category.hpp"
#include "api/cc-status-category-list.hpp"
#include "api/cc-status-type.hpp"
#include "api/cc-status-type-list.hpp"
#include "api/value-view.hpp"
#include "api/value-view-list.hpp"
#include "api/translation.hpp"
#include "api/translation-list.hpp"


using namespace svetit::project;

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
		.Append<handlers::Details>()
		.Append<handlers::Project>()
		.Append<handlers::ProjectList>()
		.Append<handlers::ParamType>()
		.Append<handlers::ParamTypeList>()
		.Append<handlers::ProjectParam>()
		.Append<handlers::ProjectParamList>()
		.Append<handlers::Section>()
		.Append<handlers::SectionList>()
		.Append<handlers::SectionParam>()
		.Append<handlers::SectionParamList>()
		.Append<handlers::CcType>()
		.Append<handlers::CcTypeList>()
		.Append<handlers::ControlCircuit>()
		.Append<handlers::ControlCircuitList>()
		.Append<handlers::Plugin>()
		.Append<handlers::PluginList>()
		.Append<handlers::Device>()
		.Append<handlers::DeviceList>()
		.Append<handlers::DevicePluginParam>()
		.Append<handlers::DevicePluginParamList>()
		.Append<handlers::Code>()
		.Append<handlers::CodeList>()
		.Append<handlers::Measure>()
		.Append<handlers::MeasureList>()
		.Append<handlers::SaveTimer>()
		.Append<handlers::SaveTimerList>()
		.Append<handlers::CcTypeParam>()
		.Append<handlers::CcTypeParamList>()
		.Append<handlers::DiType>()
		.Append<handlers::DiTypeList>()
		.Append<handlers::DiPluginParam>()
		.Append<handlers::DiPluginParamList>()
		.Append<handlers::CcTypeDiType>()
		.Append<handlers::CcTypeDiTypeList>()
		.Append<handlers::DeviceItem>()
		.Append<handlers::DeviceItemList>()
		.Append<handlers::CcModeType>()
		.Append<handlers::CcModeTypeList>()
		.Append<handlers::CcDi>()
		.Append<handlers::CcDiList>()
		.Append<handlers::CcParam>()
		.Append<handlers::CcParamList>()
		.Append<handlers::CcStatusCategory>()
		.Append<handlers::CcStatusCategoryList>()
		.Append<handlers::CcStatusType>()
		.Append<handlers::CcStatusTypeList>()
		.Append<handlers::ValueView>()
		.Append<handlers::ValueViewList>()
		.Append<handlers::Translation>()
		.Append<handlers::TranslationList>()
		;

	return utils::DaemonMain(argc, argv, component_list);
}
