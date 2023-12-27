import pytest

# Start via `make test-debug` or `make test-release`
@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_basic(service_client):
    assert 200 == 200

@pytest.mark.pgsql('V0001__Init', files=['test_data.sql'])
async def test_postgres(service_client):
    response = await service_client.get(
        '/space/available/list?start=0&limit=5', headers={'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'},
    )
    assert response.status == 200
    assert b'22222222-2222-2222-2222-222222222222' in response.content
