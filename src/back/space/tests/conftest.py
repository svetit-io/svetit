import pytest

from testsuite.databases.pgsql import discover

pytest_plugins = ['pytest_userver.plugins.core', 'pytest_userver.plugins.postgresql']

@pytest.fixture(scope='session')
def pgsql_local(service_source_dir, pgsql_local_create):
    databases = discover.find_schemas(
        'space', [service_source_dir.joinpath('db/migrations')],
    )
    return pgsql_local_create(list(databases.values()))

@pytest.fixture(scope="session")
def allowed_url_prefixes_extra():
    return ["http://localhost:8083/"]