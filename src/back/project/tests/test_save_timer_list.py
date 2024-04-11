import pytest

endpoint = '/project/save-timer/list'

@pytest.mark.pgsql('app', files=['test_data.sql'])
async def test_save_timer_list(service_client):
	"""Discovery request to save_timer list endpoint"""
	"""With valid params"""
	url = endpoint + '?start=0&limit=5'
	res = await service_client.get(url)
	assert res.status == 200
	assert b'"total":1' in res.content