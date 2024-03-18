.PHONY: *

default init-env init create-user up down stop migrate:
	make -C pipeline $@

migrate-%:
	make -C pipeline migrate-$*

# docker-build-% docker-status
docker-%:
	make -C pipeline $*

run-%:
	make -C pipeline stop-$*
	make -C src/back/$* build-debug
	env TESTSUITE_POSTGRESQL_PORT=15434 make -C src/back/$* service-start-debug

run-bin-%:
	make -C pipeline stop-$*
	make -C src/back/$* build-debug
	./src/back/$*/build_debug/svetit_$* \
		--config "./src/back/space/configs/static_config.yaml" \
		--config_vars "./src/back/space/configs/config_vars.yaml"

test-%:
	env TESTSUITE_POSTGRESQL_PORT=15434 make -C src/back/$* test-debug
