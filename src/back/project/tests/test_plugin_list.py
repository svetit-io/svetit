import pytest

endpoint = '/project/plugin/list'

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_plugin_list(service_client):
	"""Plugin list endpoint"""
	"""With valid params"""
	url = endpoint + '?start=0&limit=5'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'"total":2' in res.content