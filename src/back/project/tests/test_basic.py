import pytest

h = {}
json = {}

# run by make test-project or make run-project
async def test_details(service_client):
	"""Details endpoint"""
	"""Without params in body"""
	res = await service_client.post('/project/details')
	assert res.status == 400

	"""Invalid uuids"""
	project_uuids_invalid = [
		'abc123',
		123
	]
	res = await service_client.post('/project/details', json=project_uuids_invalid)
	assert res.status == 400

	"""Valid uuids"""
	project_uuids = [
		'11111111-1111-1111-1111-111111111111',
		'22222222-2222-2222-2222-222222222222'
	]
	res = await service_client.post('/project/details', json=project_uuids)
	assert res.status == 200

async def test_project(service_client):
	"""Project endpoint"""
	"""Without params"""
	res = await service_client.get('/project')
	assert res.status == 400

	"""By id with invalid uuid"""
	res = await service_client.get('/project' + '?id=123')
	assert res.status == 400

	"""By id with valid uuid"""
	res = await service_client.get('/project' + '?id=11111111-1111-1111-1111-111111111111')
	assert res.status == 200

	"""By key with empty key"""
	res = await service_client.get('/project' + '?key=')
	assert res.status == 400

	"""By key with valid key"""
	res = await service_client.get('/project' + '?key=project123')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid={
		'space_id': 'abc123',
		'key': 123,
		'name': True,
		'description': '',
		'sync': 'invalid_sync'
	}
	res = await service_client.post('/project', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid={
		'space_id': '33333333-3333-3333-3333-333333333333',
		'key': "test",
		'name': "Test project",
		'description': 'Text',
		'sync': 'project_to_node'
	}
	res = await service_client.post('/project', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid={
		'id': 'abc',
		'space_id': 'abc123',
		'key': 123,
		'name': True,
		'description': '',
		'sync': 'invalid_sync'
	}
	res = await service_client.patch('/project', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid={
		'id': '11111111-1111-1111-1111-111111111111',
		'space_id': '33333333-3333-3333-3333-333333333333',
		'key': "test",
		'name': "Test project",
		'description': 'Text',
		'sync': 'project_to_node'
	}
	res = await service_client.post('/project', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project')
	assert res.status == 400

	"""Delete with invalid param (invalid uuid)"""
	res = await service_client.delete('/project' + '?id=123')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project' + '?id=11111111-1111-1111-1111-111111111111')
	assert res.status == 200


async def test_project_list(service_client):
	"""Project list endpoint"""
	"""Without params"""
	res = await service_client.get('/project/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/list' + '?start=abc&limit=-1')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/list' + '?start=0&limit=5')
	assert res.status == 200


async def test_param_type(service_client):
	"""Param_type endpoint"""

	"""Get without param"""
	res = await service_client.get('/project/param-type')
	assert res.status == 400

	"""Get with invalid param"""
	res = await service_client.get('/project/param-type' + '?id=abc')
	assert res.status == 400

	"""Get with valid param"""
	res = await service_client.get('/project/param-type' + '?id=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/param-type')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'parent_id': 'abc',
		'key': 123,
		'name': True,
		'description': -1,
		'value_type': 'unknown',
		'is_deleted': 123
	}
	res = await service_client.post('/project/param-type', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'parent_id': 1,
		'key': 'abc123',
		'name': 'Test name',
		'description': 'Test description',
		'value_type': 'int',
		'is_deleted': False
	}
	res = await service_client.post('/project/param-type', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/param-type')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': -1,
		'parent_id': 'abc',
		'key': 123,
		'name': True,
		'description': -1,
		'value_type': 'unknown',
		'is_deleted': 123
	}
	res = await service_client.patch('/project/param-type', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 2,
		'parent_id': 1,
		'key': 'abc123',
		'name': 'Test name',
		'description': 'Test description',
		'value_type': 'int',
		'is_deleted': False
	}
	res = await service_client.patch('/project/param-type', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project/param-type')
	assert res.status == 400

	"""Delete with invalid param"""
	res = await service_client.delete('/project/param-type' + '?id=abc')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project/param-type' + '?id=1')
	assert res.status == 200


async def test_param_type_list(service_client):
	"""Param_type list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/param-type/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/param-type/list' + '?start=-1&limit=abc&keepDeleted=x')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/param-type/list' + '?start=0&limit=5&keepDeleted=true')
	assert res.status == 200


async def test_project_param(service_client):
	"""Project_param endpoint"""
	"""Get without params"""
	res = await service_client.get('/project/project-param')
	assert res.status == 400

	"""Get with invalid params"""
	res = await service_client.get('/project/project-param' + '&projectId=abc&paramId=xyz')
	assert res.status == 400

	"""Get with valid params"""
	res = await service_client.get('/project/project-param' + '&projectId=11111111-1111-1111-1111-111111111111&paramId=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/project-param')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'project_id': 'abc',
		'param_id': -1,
		'is_deleted': 'test'
	}
	res = await service_client.post('/project/project-param', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'project_id': '11111111-1111-1111-1111-111111111111',
		'param_id': 1,
		'is_deleted': False
	}
	res = await service_client.post('/project/project-param', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/project-param')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'project_id': 'abc',
		'param_id': -1,
		'is_deleted': 'test'
	}
	res = await service_client.patch('/project/project-param', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'project_id': '11111111-1111-1111-1111-111111111111',
		'param_id': 1,
		'is_deleted': True
	}
	res = await service_client.patch('/project/project-param', json=body_valid)
	assert res.status == 200

	"""Delete without params"""
	res = await service_client.delete('/project/project-param')
	assert res.status == 400

	"""Delete with invalid params"""
	res = await service_client.delete('/project/project-param' + '?projectId=abc&paramId=xyz')
	assert res.status == 400

	"""Delete with valid params"""
	res = await service_client.delete('/project/project-param' + '?projectId=11111111-1111-1111-1111-111111111111&paramId=1')
	assert res.status == 200


async def test_project_param_list(service_client):
	"""Project_param list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/project-param/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/project-param/list' + '?start=-1&limit=abc&keepDeleted=x')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/project-param/list' + '?start=0&limit=5&keepDeleted=true')
	assert res.status == 200


async def test_section(service_client):
	"""Section endpoint"""

	"""Get without param"""
	res = await service_client.get('/project/section')
	assert res.status == 400

	"""Get with invalid param"""
	res = await service_client.get('/project/section' + '?id=abc')
	assert res.status == 400

	"""Get with valid param"""
	res = await service_client.get('/project/section' + '?id=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/section')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'project_id': 'abc',
		'name': 123,
		'is_deleted': -1
	}
	res = await service_client.post('/project/section', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'project_id': '11111111-1111-1111-1111-111111111111',
		'name': "Test section",
		'is_deleted': False
	}
	res = await service_client.post('/project/section', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/section')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': -1,
		'project_id': 'abc',
		'name': 123,
		'is_deleted': -1
	}
	res = await service_client.patch('/project/section', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 1,
		'project_id': '11111111-1111-1111-1111-111111111111',
		'name': "Test section",
		'is_deleted': True
	}
	res = await service_client.patch('/project/section', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project/section')
	assert res.status == 400

	"""Delete with invalid param"""
	res = await service_client.delete('/project/section' + '?id=abc')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project/section' + '?id=1')
	assert res.status == 200


async def test_section_list(service_client):
	"""Section list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/section/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/section/list' + '?start=-1&limit=abc&keepDeleted=x')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/section/list' + '?start=0&limit=5&keepDeleted=true')
	assert res.status == 200


async def test_section_param(service_client):
	"""Section_param endpoint"""

	"""Get without params"""
	res = await service_client.get('/project/section-param')
	assert res.status == 400

	"""Get with invalid params"""
	res = await service_client.get('/project/section-param' + '&sectionId=abc&paramId=xyz')
	assert res.status == 400

	"""Get with valid params"""
	res = await service_client.get('/project/section-param' + '&sectionId=1&paramId=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/section-param')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'section_id': 'abc',
		'param_id': -1,
		'is_deleted': 'test'
	}
	res = await service_client.post('/project/section-param', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'section_id': 1,
		'param_id': 1,
		'is_deleted': False
	}
	res = await service_client.post('/project/section-param', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/section-param')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'section_id': 'abc',
		'param_id': -1,
		'is_deleted': 'test'
	}
	res = await service_client.patch('/project/section-param', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'section_id': 1,
		'param_id': 1,
		'is_deleted': False
	}
	res = await service_client.patch('/project/section-param', json=body_valid)
	assert res.status == 200

	"""Delete without params"""
	res = await service_client.delete('/project/section-param')
	assert res.status == 400

	"""Delete with invalid params"""
	res = await service_client.delete('/project/section-param' + '?sectionId=abc&paramId=xyz')
	assert res.status == 400

	"""Delete with valid params"""
	res = await service_client.delete('/project/section-param' + '?sectionId=1&paramId=1')
	assert res.status == 200


async def test_section_param_list(service_client):
	"""Section_param list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/section-param/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/section-param/list' + '?start=-1&limit=abc&keepDeleted=x')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/section-param/list' + '?start=0&limit=5&keepDeleted=true')
	assert res.status == 200


async def test_cc_type(service_client):
	"""Cc_type endpoint"""

	"""Get without param"""
	res = await service_client.get('/project/cc-type')
	assert res.status == 400

	"""Get with invalid param"""
	res = await service_client.get('/project/cc-type' + '?id=abc')
	assert res.status == 400

	"""Get with valid param"""
	res = await service_client.get('/project/cc-type' + '?id=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/cc-type')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'project_id': 'abc',
		'key': 123,
		'name': -1,
		'description': 1,
		'is_deleted': 'abc'
	}
	res = await service_client.post('/project/cc-type', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'project_id': '11111111-1111-1111-1111-111111111111',
		'key': "abc123",
		'name': "Test cc_type",
		'description': "Description",
		'is_deleted': False
	}
	res = await service_client.post('/project/cc-type', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/cc-type')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': '',
		'project_id': 'abc',
		'key': 123,
		'name': -1,
		'description': 1,
		'is_deleted': 'abc'
	}
	res = await service_client.patch('/project/cc-type', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 1,
		'project_id': '11111111-1111-1111-1111-111111111111',
		'key': "abc123",
		'name': "Test cc_type",
		'description': "Description",
		'is_deleted': False
	}
	res = await service_client.patch('/project/cc-type', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project/cc-type')
	assert res.status == 400

	"""Delete with invalid param"""
	res = await service_client.delete('/project/cc-type' + '?id=abc')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project/cc-type' + '?id=1')
	assert res.status == 200


async def test_cc_type_list(service_client):
	"""Cc_type list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/cc-type/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/cc-type/list' + '?start=-1&limit=abc&keepDeleted=x')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/cc-type/list' + '?start=0&limit=5&keepDeleted=true')
	assert res.status == 200


async def test_control_circuit(service_client):
	"""Control_circuit endpoint"""

	"""Get without param"""
	res = await service_client.get('/project/control-circuit')
	assert res.status == 400

	"""Get with invalid param"""
	res = await service_client.get('/project/control-circuit' + '?id=abc')
	assert res.status == 400

	"""Get with valid param"""
	res = await service_client.get('/project/control-circuit' + '?id=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/control-circuit')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'type_id': 'abc',
		'section_id': 'xyz',
		'name': -1,
		'is_deleted': 123
	}
	res = await service_client.post('/project/control-circuit', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'type_id': 1,
		'section_id': 1,
		'name': "Test control curcuit",
		'is_deleted': False
	}
	res = await service_client.post('/project/control-circuit', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/control-circuit')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': 'def',
		'type_id': 'abc',
		'section_id': 'xyz',
		'name': -1,
		'is_deleted': 123
	}
	res = await service_client.patch('/project/control-circuit', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 1,
		'type_id': 1,
		'section_id': 1,
		'name': "Test control curcuit",
		'is_deleted': False
	}
	res = await service_client.patch('/project/control-circuit', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project/control-circuit')
	assert res.status == 400

	"""Delete with invalid param"""
	res = await service_client.delete('/project/control-circuit' + '?id=abc')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project/control-circuit' + '?id=1')
	assert res.status == 200


async def test_control_circuit_list(service_client):
	"""Control_circuit list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/control-circuit/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/control-circuit/list' + '?start=-1&limit=abc&keepDeleted=x')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/control-circuit/list' + '?start=0&limit=5&keepDeleted=true')
	assert res.status == 200


async def test_plugin(service_client):
	"""Plugin endpoint"""

	"""Get without param"""
	res = await service_client.get('/project/plugin')
	assert res.status == 400

	"""Get with invalid param"""
	res = await service_client.get('/project/plugin' + '?id=abc')
	assert res.status == 400

	"""Get with valid param"""
	res = await service_client.get('/project/plugin' + '?id=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/plugin')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'project_id': 'abc',
		'name': 123,
		'description': False,
		'key': 0,
		'is_deleted': 5
	}
	res = await service_client.post('/project/plugin', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'project_id': '11111111-1111-1111-1111-111111111111',
		'name': 'Test plugin',
		'description': 'Description',
		'key': "abc123",
		'is_deleted': False
	}
	res = await service_client.post('/project/plugin', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/plugin')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': 'xyz',
		'project_id': 'abc',
		'name': 123,
		'description': False,
		'key': 0,
		'is_deleted': 5
	}
	res = await service_client.patch('/project/plugin', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 1,
		'project_id': '11111111-1111-1111-1111-111111111111',
		'name': 'Test plugin',
		'description': 'Description',
		'key': "abc123",
		'is_deleted': False
	}
	res = await service_client.patch('/project/plugin', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project/plugin')
	assert res.status == 400

	"""Delete with invalid param"""
	res = await service_client.delete('/project/plugin' + '?id=abc')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project/plugin' + '?id=1')
	assert res.status == 200

async def test_plugin_list(service_client):
	"""Plugin list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/plugin/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/plugin/list' + '?start=-1&limit=abc&keepDeleted=x')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/plugin/list' + '?start=0&limit=5&keepDeleted=true')
	assert res.status == 200


async def test_device(service_client):
	"""Device endpoint"""

	"""Get without param"""
	res = await service_client.get('/project/device')
	assert res.status == 400

	"""Get with invalid param"""
	res = await service_client.get('/project/device' + '?id=abc')
	assert res.status == 400

	"""Get with valid param"""
	res = await service_client.get('/project/device' + '?id=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/device')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'project_id': 'klm',
		'plugin_id': 'def',
		'name': 123,
		'check_interval_msec': 'xyz',
		'is_deleted': 'abc'
	}
	res = await service_client.post('/project/device', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'project_id': '11111111-1111-1111-1111-111111111111',
		'plugin_id': 1,
		'name': 'Test device',
		'check_interval_msec': 1,
		'is_deleted': False
	}
	res = await service_client.post('/project/device', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/device')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': 'zzz',
		'project_id': 'klm',
		'plugin_id': 'def',
		'name': 123,
		'check_interval_msec': 'xyz',
		'is_deleted': 'abc'
	}
	res = await service_client.patch('/project/device', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 1,
		'project_id': '11111111-1111-1111-1111-111111111111',
		'plugin_id': 1,
		'name': 'Test device',
		'check_interval_msec': 1,
		'is_deleted': False
	}
	res = await service_client.patch('/project/device', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project/device')
	assert res.status == 400

	"""Delete with invalid param"""
	res = await service_client.delete('/project/device' + '?id=abc')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project/device' + '?id=1')
	assert res.status == 200


async def test_device_list(service_client):
	"""Device list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/device/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/device/list' + '?start=-1&limit=abc&keepDeleted=x')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/device/list' + '?start=0&limit=5&keepDeleted=true')
	assert res.status == 200


async def test_device_plugin_param(service_client):
	"""Device_plugin_param endpoint"""

	"""Get without params"""
	res = await service_client.get('/project/device-plugin-param')
	assert res.status == 400

	"""Get with invalid params"""
	res = await service_client.get('/project/device-plugin-param' + '&deviceId=abc&paramId=xyz')
	assert res.status == 400

	"""Get with valid params"""
	res = await service_client.get('/project/device-plugin-param' + '&deviceId=1&paramId=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/device-plugin-param')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'device_id': 'abc',
		'param_id': -1,
		'is_deleted': 'test'
	}
	res = await service_client.post('/project/device-plugin-param', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'device_id': 1,
		'param_id': 1,
		'is_deleted': False
	}
	res = await service_client.post('/project/device-plugin-param', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/device-plugin-param')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'device_id': 'abc',
		'param_id': -1,
		'is_deleted': 'test'
	}
	res = await service_client.patch('/project/device-plugin-param', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'device_id': 1,
		'param_id': 1,
		'is_deleted': False
	}
	res = await service_client.patch('/project/device-plugin-param', json=body_valid)
	assert res.status == 200

	"""Delete without params"""
	res = await service_client.delete('/project/device-plugin-param')
	assert res.status == 400

	"""Delete with invalid params"""
	res = await service_client.delete('/project/device-plugin-param' + '?deviceId=abc&paramId=xyz')
	assert res.status == 400

	"""Delete with valid params"""
	res = await service_client.delete('/project/device-plugin-param' + '?deviceId=1&paramId=1')
	assert res.status == 200


async def test_device_plugin_param_list(service_client):
	"""Device_plugin_param list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/device-plugin-param/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/device-plugin-param/list' + '?start=-1&limit=abc&keepDeleted=x')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/device-plugin-param/list' + '?start=0&limit=5&keepDeleted=true')
	assert res.status == 200


async def test_code(service_client):
	"""Code endpoint"""

	"""Get without param"""
	res = await service_client.get('/project/code')
	assert res.status == 400

	"""Get with invalid param"""
	res = await service_client.get('/project/code' + '?id=abc')
	assert res.status == 400

	"""Get with valid param"""
	res = await service_client.get('/project/code' + '?id=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/code')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'project_id': 'abc',
		'repository_id': 123,
		'commit_hash': False
	}
	res = await service_client.post('/project/code', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'project_id': '11111111-1111-1111-1111-111111111111',
		'repository_id': '4572def0053fcba64e4becb1800a1d160502e99f',
		'commit_hash': False
	}
	res = await service_client.post('/project/code', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/code')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': 'xyz',
		'project_id': 'abc',
		'repository_id': 123,
		'commit_hash': False
	}
	res = await service_client.patch('/project/code', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 1,
		'project_id': '11111111-1111-1111-1111-111111111111',
		'repository_id': '4572def0053fcba64e4becb1800a1d160502e99f',
		'commit_hash': False
	}
	res = await service_client.patch('/project/code', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project/code')
	assert res.status == 400

	"""Delete with invalid param"""
	res = await service_client.delete('/project/code' + '?id=abc')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project/code' + '?id=1')
	assert res.status == 200


async def test_code_list(service_client):
	"""Code list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/code/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/code/list' + '?start=-1&limit=abc')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/code/list' + '?start=0&limit=5')
	assert res.status == 200


async def test_measure(service_client):
	"""Measure endpoint"""

	"""Get without param"""
	res = await service_client.get('/project/measure')
	assert res.status == 400

	"""Get with invalid param"""
	res = await service_client.get('/project/measure' + '?id=abc')
	assert res.status == 400

	"""Get with valid param"""
	res = await service_client.get('/project/measure' + '?id=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/measure')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'project_id': 'abc',
		'name': 123,
		'is_deleted': -1
	}
	res = await service_client.post('/project/measure', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'project_id': '11111111-1111-1111-1111-111111111111',
		'name': 'Test measure',
		'is_deleted': False
	}
	res = await service_client.post('/project/measure', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/measure')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': -1,
		'project_id': 'abc',
		'name': 123,
		'is_deleted': -1
	}
	res = await service_client.patch('/project/measure', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 1,
		'project_id': '11111111-1111-1111-1111-111111111111',
		'name': 'Test measure',
		'is_deleted': True
	}
	res = await service_client.patch('/project/measure', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project/measure')
	assert res.status == 400

	"""Delete with invalid param"""
	res = await service_client.delete('/project/measure' + '?id=abc')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project/measure' + '?id=1')
	assert res.status == 200


async def test_measure_list(service_client):
	"""Measure list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/measure/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/measure/list' + '?start=-1&limit=abc&keepDeleted=x')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/measure/list' + '?start=0&limit=5&keepDeleted=true')
	assert res.status == 200


async def test_save_timer(service_client):
	"""Save_timer endpoint"""

	"""Get without param"""
	res = await service_client.get('/project/save-timer')
	assert res.status == 400

	"""Get with invalid param"""
	res = await service_client.get('/project/save-timer' + '?id=abc')
	assert res.status == 400

	"""Get with valid param"""
	res = await service_client.get('/project/save-timer' + '?id=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/save-timer')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'project_id': 'abc',
		'interval_msec': 'xyz'
	}
	res = await service_client.post('/project/save-timer', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'project_id': '11111111-1111-1111-1111-111111111111',
		'interval_msec': 1
	}
	res = await service_client.post('/project/save-timer', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/save-timer')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': 1,
		'project_id': 'abc',
		'interval_msec': 'xyz'
	}
	res = await service_client.patch('/project/save-timer', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 1,
		'project_id': '11111111-1111-1111-1111-111111111111',
		'interval_msec': 'xyz'
	}
	res = await service_client.patch('/project/save-timer', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project/save-timer')
	assert res.status == 400

	"""Delete with invalid param"""
	res = await service_client.delete('/project/save-timer' + '?id=abc')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project/save-timer' + '?id=1')
	assert res.status == 200


async def test_save_timer_list(service_client):
	"""Discovery request to save_timer list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/save-timer/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/save-timer/list' + '?start=-1&limit=abc')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/save-timer/list' + '?start=0&limit=5')
	assert res.status == 200


async def test_cc_type_param(service_client):
	"""Cc_type_param endpoint"""

	"""Get without params"""
	res = await service_client.get('/project/cc-type-param')
	assert res.status == 400

	"""Get with invalid params"""
	res = await service_client.get('/project/cc-type-param' + '&ccTypeId=abc&paramId=xyz')
	assert res.status == 400

	"""Get with valid params"""
	res = await service_client.get('/project/cc-type-param' + '&ccTypeId=1&paramId=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/cc-type-param')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'cc_type_id': 'abc',
		'param_id': -1,
		'is_deleted': 'test'
	}
	res = await service_client.post('/project/cc-type-param', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'cc_type_id': 1,
		'param_id': 1,
		'is_deleted': False
	}
	res = await service_client.post('/project/cc-type-param', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/cc-type-param')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'cc_type_id': 'abc',
		'param_id': -1,
		'is_deleted': 'test'
	}
	res = await service_client.patch('/project/cc-type-param', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'cc_type_id': 1,
		'param_id': 1,
		'is_deleted': False
	}
	res = await service_client.patch('/project/cc-type-param', json=body_valid)
	assert res.status == 200

	"""Delete without params"""
	res = await service_client.delete('/project/cc-type-param')
	assert res.status == 400

	"""Delete with invalid params"""
	res = await service_client.delete('/project/cc-type-param' + '?ccTypeId=abc&paramId=xyz')
	assert res.status == 400

	"""Delete with valid params"""
	res = await service_client.delete('/project/cc-type-param' + '?ccTypeId=1&paramId=1')
	assert res.status == 200


async def test_cc_type_param_list(service_client):
	"""Cc_type_param list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/cc-type-param/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/cc-type-param/list' + '?start=-1&limit=abc&keepDeleted=x')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/cc-type-param/list' + '?start=0&limit=5&keepDeleted=true')
	assert res.status == 200


async def test_di_type(service_client):
	"""Di_type endpoint"""

	"""Get without param"""
	res = await service_client.get('/project/di-type')
	assert res.status == 400

	"""Get with invalid param"""
	res = await service_client.get('/project/di-type' + '?id=abc')
	assert res.status == 400

	"""Get with valid param"""
	res = await service_client.get('/project/di-type' + '?id=1')
	assert res.status == 200

	"""Post without body"""
	res = await service_client.post('/project/di-type')
	assert res.status == 400

	"""Post with invalid body"""
	body_invalid = {
		'measure_id': 'abc',
		'save_timer_id': 'xyz',
		'key': 123,
		'name': 456,
		'mode': 'invalid',
		'save_algorithm': 'invalid',
		'is_deleted': 5
	}
	res = await service_client.post('/project/di-type', json=body_invalid)
	assert res.status == 400

	"""Post with valid body"""
	body_valid = {
		'measure_id': 1,
		'save_timer_id': 1,
		'key': 'abc123',
		'name': 'Test',
		'mode': 'readonly_flag',
		'save_algorithm': 'off',
		'is_deleted': False
	}
	res = await service_client.post('/project/di-type', json=body_valid)
	assert res.status == 200

	"""Patch without body"""
	res = await service_client.patch('/project/di-type')
	assert res.status == 400

	"""Patch with invalid body"""
	body_invalid = {
		'id': -1,
		'measure_id': 'abc',
		'save_timer_id': 'xyz',
		'key': 123,
		'name': 456,
		'mode': 'invalid',
		'save_algorithm': 'invalid',
		'is_deleted': 5
	}
	res = await service_client.patch('/project/di-type', json=body_invalid)
	assert res.status == 400

	"""Patch with valid body"""
	body_valid = {
		'id': 1,
		'measure_id': 1,
		'save_timer_id': 1,
		'key': 'abc123',
		'name': 'Test',
		'mode': 'readonly_flag',
		'save_algorithm': 'off',
		'is_deleted': False
	}
	res = await service_client.patch('/project/di-type', json=body_valid)
	assert res.status == 200

	"""Delete without param"""
	res = await service_client.delete('/project/di-type')
	assert res.status == 400

	"""Delete with invalid param"""
	res = await service_client.delete('/project/di-type' + '?id=abc')
	assert res.status == 400

	"""Delete with valid param"""
	res = await service_client.delete('/project/di-type' + '?id=1')
	assert res.status == 200


async def test_di_type_list(service_client):
	"""Discovery request to di_type list endpoint"""

	"""Without params"""
	res = await service_client.get('/project/di-type/list')
	assert res.status == 400

	"""With invalid params"""
	res = await service_client.get('/project/di-type/list' + '?start=-1&limit=abc&keepDeleted=x')
	assert res.status == 400

	"""With valid params"""
	res = await service_client.get('/project/di-type/list' + '?start=0&limit=5&keepDeleted=true')
	assert res.status == 200


async def test_di_plugin_param(service_client):
	"""Discovery requests to di_plugin_param endpoint"""
	res = await service_client.get('/project/di-plugin-param')
	assert res.status == 200

	res = await service_client.post('/project/di-plugin-param')
	assert res.status == 200

	res = await service_client.patch('/project/di-plugin-param')
	assert res.status == 200

	res = await service_client.delete('/project/di-plugin-param')
	assert res.status == 200


async def test_di_plugin_param_list(service_client):
	"""Discovery request to di_plugin_param list endpoint"""
	res = await service_client.get('/project/di-plugin-param/list')
	assert res.status == 200


async def test_cc_type_di_type(service_client):
	"""Discovery requests to cc_type_di_type endpoint"""
	res = await service_client.get('/project/cc-type-di-type')
	assert res.status == 200

	res = await service_client.post('/project/cc-type-di-type')
	assert res.status == 200

	res = await service_client.patch('/project/cc-type-di-type')
	assert res.status == 200

	res = await service_client.delete('/project/cc-type-di-type')
	assert res.status == 200


async def test_cc_type_di_type_list(service_client):
	"""Discovery request to cc_type_di_type list endpoint"""
	res = await service_client.get('/project/cc-type-di-type/list')
	assert res.status == 200


async def test_device_item(service_client):
	"""Discovery requests to device_item endpoint"""
	res = await service_client.get('/project/device-item')
	assert res.status == 200

	res = await service_client.post('/project/device-item')
	assert res.status == 200

	res = await service_client.patch('/project/device-item')
	assert res.status == 200

	res = await service_client.delete('/project/device-item')
	assert res.status == 200


async def test_device_item_list(service_client):
	"""Discovery request to device_item list endpoint"""
	res = await service_client.get('/project/device-item/list')
	assert res.status == 200


async def test_cc_mode_type(service_client):
	"""Discovery requests to cc_mode_type endpoint"""
	res = await service_client.get('/project/cc-mode-type')
	assert res.status == 200

	res = await service_client.post('/project/cc-mode-type')
	assert res.status == 200

	res = await service_client.patch('/project/cc-mode-type')
	assert res.status == 200

	res = await service_client.delete('/project/cc-mode-type')
	assert res.status == 200


async def test_cc_mode_type_list(service_client):
	"""Discovery request to cc_mode_type list endpoint"""
	res = await service_client.get('/project/cc-mode-type/list')
	assert res.status == 200


async def test_cc_di(service_client):
	"""Discovery requests to cc_di endpoint"""
	res = await service_client.get('/project/cc-di')
	assert res.status == 200

	res = await service_client.post('/project/cc-di')
	assert res.status == 200

	res = await service_client.patch('/project/cc-di')
	assert res.status == 200

	res = await service_client.delete('/project/cc-di')
	assert res.status == 200


async def test_cc_di_list(service_client):
	"""Discovery request to cc_di list endpoint"""
	res = await service_client.get('/project/cc-di/list')
	assert res.status == 200


async def test_cc_param(service_client):
	"""Discovery requests to cc_param endpoint"""
	res = await service_client.get('/project/cc-param')
	assert res.status == 200

	res = await service_client.post('/project/cc-param')
	assert res.status == 200

	res = await service_client.patch('/project/cc-param')
	assert res.status == 200

	res = await service_client.delete('/project/cc-param')
	assert res.status == 200


async def test_cc_param_list(service_client):
	"""Discovery request to cc_param list endpoint"""
	res = await service_client.get('/project/cc-param/list')
	assert res.status == 200


async def test_cc_status_category(service_client):
	"""Discovery requests to cc_status_category endpoint"""
	res = await service_client.get('/project/cc-status-category')
	assert res.status == 200

	res = await service_client.post('/project/cc-status-category')
	assert res.status == 200

	res = await service_client.patch('/project/cc-status-category')
	assert res.status == 200

	res = await service_client.delete('/project/cc-status-category')
	assert res.status == 200


async def test_cc_status_category_list(service_client):
	"""Discovery request to cc_status_category list endpoint"""
	res = await service_client.get('/project/cc-status-category/list')
	assert res.status == 200


async def test_cc_status_type(service_client):
	"""Discovery requests to cc_status_type endpoint"""
	res = await service_client.get('/project/cc-status-type')
	assert res.status == 200

	res = await service_client.post('/project/cc-status-type')
	assert res.status == 200

	res = await service_client.patch('/project/cc-status-type')
	assert res.status == 200

	res = await service_client.delete('/project/cc-status-type')
	assert res.status == 200


async def test_cc_status_type_list(service_client):
	"""Discovery request to cc_status_type list endpoint"""
	res = await service_client.get('/project/cc-status-type/list')
	assert res.status == 200


async def test_value_view(service_client):
	"""Discovery requests to value_view endpoint"""
	res = await service_client.get('/project/value-view')
	assert res.status == 200

	res = await service_client.post('/project/value-view')
	assert res.status == 200

	res = await service_client.patch('/project/value-view')
	assert res.status == 200

	res = await service_client.delete('/project/value-view')
	assert res.status == 200


async def test_value_view_list(service_client):
	"""Discovery request to value_view list endpoint"""
	res = await service_client.get('/project/value-view/list')
	assert res.status == 200


async def test_translation(service_client):
	"""Discovery requests to translation endpoint"""
	res = await service_client.get('/project/translation')
	assert res.status == 200

	res = await service_client.post('/project/translation')
	assert res.status == 200

	res = await service_client.patch('/project/translation')
	assert res.status == 200

	res = await service_client.delete('/project/translation')
	assert res.status == 200


async def test_translation_list(service_client):
	"""Discovery request to translation list endpoint"""
	res = await service_client.get('/project/translation/list')
	assert res.status == 200