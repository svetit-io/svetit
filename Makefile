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
	make -C src/back/$* service-start-debug
