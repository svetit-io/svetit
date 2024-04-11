import pytest

endpoint = '/project/device-plugin-param'

body = {
	'deviceId': 1,
	'paramId': 3,
	'isDeleted': False
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_device_plugin_param(service_client):
	"""Device plugin param endpoint"""
	"""Get with valid params"""
	url = endpoint + '?deviceId=1&paramId=1'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'deviceId' in res.content

	"""Post with valid body"""
	res = await service_client.post(endpoint, json=body)
	assert res.status == 201

	"""Patch with valid body"""
	data = body.copy()
	data['isDeleted'] = True
	res = await service_client.patch(endpoint, json=data)
	assert res.status == 200

	"""Delete with valid params"""
	url = endpoint + '?deviceId=1&paramId=4'
	res = await service_client.delete(url)
	assert res.status == 200