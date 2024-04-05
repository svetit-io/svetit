import pytest

endpoint = '/project/cc-param'

body_invalid = {
	'cc_id': 'abc',
	'param_id': -1,
	'is_deleted': 'test'
}

body_valid = {
	'cc_id': 1,
	'param_id': 5,
	'is_deleted': False
}

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_cc_param(service_client):
	"""Cc param endpoint"""

	"""Get without params"""
	res = await service_client.get(endpoint)
	assert res.status == 400

	"""Get with invalid params"""
	url = endpoint + '?ccId=abc&paramId=xyz'
	res = await service_client.get(url)
	assert res.status == 400

	"""Get with valid params"""
	url = endpoint + '?ccId=1&paramId=1'
	res = await service_client.get(url)
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post(endpoint)
	assert res.status == 400

	"""Post with invalid body"""
	res = await service_client.post(endpoint, json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	res = await service_client.post(endpoint, json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch(endpoint)
	assert res.status == 400

	"""Patch with invalid body"""
	res = await service_client.patch(endpoint, json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	data = body_valid.copy()
	data['is_deleted'] = True
	res = await service_client.patch(endpoint, json=data)
	assert res.status == 200

	"""Delete without params"""
	res = await service_client.delete(endpoint)
	assert res.status == 400

	"""Delete with invalid params"""
	url = endpoint + '?ccId=abc&paramId=xyz'
	res = await service_client.delete(url)
	assert res.status == 400

	"""Delete with valid params"""
	url = endpoint + '?ccId=1&paramId=5'
	res = await service_client.delete(url)
	assert res.status == 200