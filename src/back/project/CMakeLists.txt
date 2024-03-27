cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(svetit_project CXX)

set(SERVICE_PORT 8084 CACHE INT "Service port for listening")

# Enable userver libraries that are needed in this project
set(USERVER_FEATURE_POSTGRESQL ON CACHE BOOL "" FORCE)

# Compatibility mode: some systems don't support these features
set(USERVER_FEATURE_CRYPTOPP_BLAKE2 OFF CACHE BOOL "" FORCE)
set(USERVER_FEATURE_GRPC_CHANNELZ OFF CACHE BOOL "" FORCE)
set(USERVER_FEATURE_REDIS_HI_MALLOC ON CACHE BOOL "" FORCE)
set(USERVER_FEATURE_CLICKHOUSE OFF CACHE BOOL "" FORCE)


# Adding userver dependency
add_subdirectory(third_party/userver)
userver_setup_environment()


# Common sources
add_library(${PROJECT_NAME}_objs OBJECT
	third_party/svetit/shared/errors.hpp
	third_party/svetit/shared/errors.cpp
	third_party/svetit/shared/errors_catchit.hpp
	third_party/svetit/shared/errors_catchit.cpp
	third_party/svetit/shared/headers.hpp
	third_party/svetit/shared/paging.hpp
	third_party/svetit/shared/paging_serialize.hpp
	third_party/svetit/shared/paging.cpp
	third_party/svetit/shared/parse/request.hpp
	third_party/svetit/shared/parse/request.cpp

	src/repo/repository.cpp
	src/repo/repository.hpp

	src/service/service.hpp
	src/service/service.cpp

	src/api/details.hpp
	src/api/details.cpp
	src/api/project.hpp
	src/api/project.cpp
	src/api/project-list.hpp
	src/api/project-list.cpp
	src/api/param-type.hpp
	src/api/param-type.cpp
	src/api/param-type-list.hpp
	src/api/param-type-list.cpp
	src/api/project-param.hpp
	src/api/project-param.cpp
	src/api/project-param-list.hpp
	src/api/project-param-list.cpp
	src/api/section.hpp
	src/api/section.cpp
	src/api/section-list.hpp
	src/api/section-list.cpp
	src/api/section-param.hpp
	src/api/section-param.cpp
	src/api/section-param-list.hpp
	src/api/section-param-list.cpp
	src/api/cc-type.hpp
	src/api/cc-type.cpp
	src/api/cc-type-list.hpp
	src/api/cc-type-list.cpp
	src/api/control-circuit.hpp
	src/api/control-circuit.cpp
	src/api/control-circuit-list.hpp
	src/api/control-circuit-list.cpp
	src/api/plugin.hpp
	src/api/plugin.cpp
	src/api/plugin-list.hpp
	src/api/plugin-list.cpp
	src/api/device.hpp
	src/api/device.cpp
	src/api/device-list.hpp
	src/api/device-list.cpp
	src/api/device-plugin-param.hpp
	src/api/device-plugin-param.cpp
	src/api/device-plugin-param-list.hpp
	src/api/device-plugin-param-list.cpp
	src/api/code.hpp
	src/api/code.cpp
	src/api/code-list.hpp
	src/api/code-list.cpp
	src/api/measure.hpp
	src/api/measure.cpp
	src/api/measure-list.hpp
	src/api/measure-list.cpp
	src/api/save-timer.hpp
	src/api/save-timer.cpp
	src/api/save-timer-list.hpp
	src/api/save-timer-list.cpp
	src/api/cc-type-param.hpp
	src/api/cc-type-param.cpp
	src/api/cc-type-param-list.hpp
	src/api/cc-type-param-list.cpp
	src/api/di-type.hpp
	src/api/di-type.cpp
	src/api/di-type-list.hpp
	src/api/di-type-list.cpp
	src/api/di-plugin-param.hpp
	src/api/di-plugin-param.cpp
	src/api/di-plugin-param-list.hpp
	src/api/di-plugin-param-list.cpp
	src/api/cc-type-di-type.hpp
	src/api/cc-type-di-type.cpp
	src/api/cc-type-di-type-list.hpp
	src/api/cc-type-di-type-list.cpp
	src/api/device-item.hpp
	src/api/device-item.cpp
	src/api/device-item-list.hpp
	src/api/device-item-list.cpp
	src/api/cc-mode-type.hpp
	src/api/cc-mode-type.cpp
	src/api/cc-mode-type-list.hpp
	src/api/cc-mode-type-list.cpp
	src/api/cc-di.hpp
	src/api/cc-di.cpp
	src/api/cc-di-list.hpp
	src/api/cc-di-list.cpp
	src/api/cc-param.hpp
	src/api/cc-param.cpp
	src/api/cc-param-list.hpp
	src/api/cc-param-list.cpp
	src/api/cc-status-category.hpp
	src/api/cc-status-category.cpp
	src/api/cc-status-category-list.hpp
	src/api/cc-status-category-list.cpp
	src/api/cc-status-type.hpp
	src/api/cc-status-type.cpp
	src/api/cc-status-type-list.hpp
	src/api/cc-status-type-list.cpp
	src/api/value-view.hpp
	src/api/value-view.cpp
	src/api/value-view-list.hpp
	src/api/value-view-list.cpp
	src/api/translation.hpp
	src/api/translation.cpp
	src/api/translation-list.hpp
	src/api/translation-list.cpp
)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC userver-core userver-postgresql)

target_include_directories(${PROJECT_NAME}_objs PUBLIC third_party/svetit)

# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)


# Unit Tests
add_executable(${PROJECT_NAME}_unittest

)
target_link_libraries(${PROJECT_NAME}_unittest PRIVATE ${PROJECT_NAME}_objs userver-utest)
add_google_tests(${PROJECT_NAME}_unittest)


# Benchmarks
add_executable(${PROJECT_NAME}_benchmark
)
target_link_libraries(${PROJECT_NAME}_benchmark PRIVATE ${PROJECT_NAME}_objs userver-ubench)
add_google_benchmark_tests(${PROJECT_NAME}_benchmark)

# Functional Tests
include(UserverTestsuite)

userver_testsuite_add_simple(
	PYTEST_ARGS --service-port=${SERVICE_PORT}
	REQUIREMENTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/requirements.txt"
)


# Install
include(GNUInstallDirs)

if(DEFINED ENV{PREFIX})
	message(STATUS "Set install prefix: $ENV{PREFIX}")
	file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
	set(CMAKE_INSTALL_PREFIX ${PREFIX_PATH})
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})

file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)
install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})

# Generate and install run scripts
set(CONFIG_PATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}/static_config.yaml)
set(CONFIG_VARS_PATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}/config_vars.yaml)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CONFIG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/configs/static_config.yaml)
	set(CONFIG_VARS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/configs/config_vars.yaml)
	configure_file(
		${CMAKE_CURRENT_SOURCE_DIR}/configs/config_vars_from_env.sh
		${CMAKE_CURRENT_BINARY_DIR}/config_vars_from_env.sh
		COPYONLY)
endif()
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/configs/run_with_env.sh.in
	${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.sh)
install(PROGRAMS
	${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.sh
	${CMAKE_CURRENT_SOURCE_DIR}/configs/config_vars_from_env.sh
	DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})
