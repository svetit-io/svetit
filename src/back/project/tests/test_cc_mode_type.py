import pytest

h = {'X-User': 'd2f9924d-a69a-4aec-b9c0-c80171d3ed86'}
endpoint = '/project/cc-mode-type'

body = {
	'id': 3,
	'ccTypeId': 1,
	'key': 'abc123',
	'name': 'Test'
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_cc_mode_type(service_client):
	"""Cc mode type endpoint"""
	"""Get with valid param"""
	url = endpoint + '?id=1'
	res = await service_client.get(url, headers=h)
	assert res.status == 200
	assert b'ccModeType1' in res.content

	"""Post with valid body"""
	data = body.copy()
	data['id'] = 0
	res = await service_client.post(endpoint, json=data, headers=h)
	assert res.status == 201

	"""Patch with valid body"""
	data = body.copy()
	data['name'] = 'Another name'
	res = await service_client.patch(endpoint, json=data, headers=h)
	assert res.status == 204

	"""Delete with valid param"""
	url = endpoint + '?id=2'
	res = await service_client.delete(url, headers=h)
	assert res.status == 200