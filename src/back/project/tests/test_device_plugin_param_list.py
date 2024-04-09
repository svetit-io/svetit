import pytest

endpoint = '/project/device-plugin-param/list'

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_device_plugin_param_list(service_client):
	"""Device plugin param list endpoint"""

	"""Without params"""
	res = await service_client.get(endpoint)
	assert res.status == 400

	"""With invalid params"""
	url = endpoint + '?start=-1&limit=abc&keepDeleted=x'
	res = await service_client.get(url)
	assert res.status == 400

	"""With valid params"""
	url = endpoint + '?start=0&limit=5&keepDeleted=true'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'DeviceId' in res.content
	assert b'"total":1' in res.content