import pytest

h = {}
json = {}

# run by make test-project or make run-project
async def test_details(service_client):
	"""Discovery request to details endpoint"""
	res = await service_client.post('/project/details')
	assert res.status == 200


async def test_project(service_client):
	"""Discovery requests to project endpoint"""
	"""Get by id (uuid)"""
	res = await service_client.get('/project')
	assert res.status == 200

	"""Get by key"""
	res = await service_client.get('/project')
	assert res.status == 200

	res = await service_client.post('/project')
	assert res.status == 200

	res = await service_client.patch('/project')
	assert res.status == 200

	res = await service_client.delete('/project')
	assert res.status == 200


async def test_project_list(service_client):
	"""Discovery request to project list endpoint"""
	res = await service_client.get('/project/list')
	assert res.status == 200


async def test_param_type(service_client):
	"""Discovery requests to param_type endpoint"""
	res = await service_client.get('/project/param-type')
	assert res.status == 200

	res = await service_client.post('/project/param-type')
	assert res.status == 200

	res = await service_client.patch('/project/param-type')
	assert res.status == 200

	res = await service_client.delete('/project/param-type')
	assert res.status == 200


async def test_param_type_list(service_client):
	"""Discovery request to param_type list endpoint"""
	res = await service_client.get('/project/param-type/list')
	assert res.status == 200


async def test_project_param(service_client):
	"""Discovery requests to project_param endpoint"""
	res = await service_client.get('/project/project-param')
	assert res.status == 200

	res = await service_client.post('/project/project-param')
	assert res.status == 200

	res = await service_client.patch('/project/project-param')
	assert res.status == 200

	res = await service_client.delete('/project/project-param')
	assert res.status == 200


async def test_project_param_list(service_client):
	"""Discovery request to project_param list endpoint"""
	res = await service_client.get('/project/project-param/list')
	assert res.status == 200


async def test_section(service_client):
	"""Discovery requests to section endpoint"""
	res = await service_client.get('/project/section')
	assert res.status == 200

	res = await service_client.post('/project/section')
	assert res.status == 200

	res = await service_client.patch('/project/section')
	assert res.status == 200

	res = await service_client.delete('/project/section')
	assert res.status == 200


async def test_section_list(service_client):
	"""Discovery request to section list endpoint"""
	res = await service_client.get('/project/section/list')
	assert res.status == 200


async def test_section_param(service_client):
	"""Discovery requests to section_param endpoint"""
	res = await service_client.get('/project/section-param')
	assert res.status == 200

	res = await service_client.post('/project/section-param')
	assert res.status == 200

	res = await service_client.patch('/project/section-param')
	assert res.status == 200

	res = await service_client.delete('/project/section-param')
	assert res.status == 200


async def test_section_param_list(service_client):
	"""Discovery request to section_param list endpoint"""
	res = await service_client.get('/project/section-param/list')
	assert res.status == 200


async def test_cc_type(service_client):
	"""Discovery requests to cc_type endpoint"""
	res = await service_client.get('/project/cc-type')
	assert res.status == 200

	res = await service_client.post('/project/cc-type')
	assert res.status == 200

	res = await service_client.patch('/project/cc-type')
	assert res.status == 200

	res = await service_client.delete('/project/cc-type')
	assert res.status == 200


async def test_cc_type_list(service_client):
	"""Discovery request to cc_type list endpoint"""
	res = await service_client.get('/project/cc-type/list')
	assert res.status == 200


async def test_control_circuit(service_client):
	"""Discovery requests to control_circuit endpoint"""
	res = await service_client.get('/project/control-circuit')
	assert res.status == 200

	res = await service_client.post('/project/control-circuit')
	assert res.status == 200

	res = await service_client.patch('/project/control-circuit')
	assert res.status == 200

	res = await service_client.delete('/project/control-circuit')
	assert res.status == 200


async def test_control_circuit_list(service_client):
	"""Discovery request to control_circuit list endpoint"""
	res = await service_client.get('/project/control-circuit/list')
	assert res.status == 200


async def test_plugin(service_client):
	"""Discovery requests to plugin endpoint"""
	res = await service_client.get('/project/plugin')
	assert res.status == 200

	res = await service_client.post('/project/plugin')
	assert res.status == 200

	res = await service_client.patch('/project/plugin')
	assert res.status == 200

	res = await service_client.delete('/project/plugin')
	assert res.status == 200


async def test_plugin_list(service_client):
	"""Discovery request to plugin list endpoint"""
	res = await service_client.get('/project/plugin/list')
	assert res.status == 200


async def test_device(service_client):
	"""Discovery requests to device endpoint"""
	res = await service_client.get('/project/device')
	assert res.status == 200

	res = await service_client.post('/project/device')
	assert res.status == 200

	res = await service_client.patch('/project/device')
	assert res.status == 200

	res = await service_client.delete('/project/device')
	assert res.status == 200


async def test_device_list(service_client):
	"""Discovery request to device list endpoint"""
	res = await service_client.get('/project/device/list')
	assert res.status == 200


async def test_device_plugin_param(service_client):
	"""Discovery requests to device_plugin_param endpoint"""
	res = await service_client.get('/project/device-plugin-param')
	assert res.status == 200

	res = await service_client.post('/project/device-plugin-param')
	assert res.status == 200

	res = await service_client.patch('/project/device-plugin-param')
	assert res.status == 200

	res = await service_client.delete('/project/device-plugin-param')
	assert res.status == 200


async def test_device_plugin_param_list(service_client):
	"""Discovery request to device_plugin_param list endpoint"""
	res = await service_client.get('/project/device-plugin-param/list')
	assert res.status == 200


async def test_code(service_client):
	"""Discovery requests to code endpoint"""
	res = await service_client.get('/project/code')
	assert res.status == 200

	res = await service_client.post('/project/code')
	assert res.status == 200

	res = await service_client.patch('/project/code')
	assert res.status == 200

	res = await service_client.delete('/project/code')
	assert res.status == 200


async def test_code_list(service_client):
	"""Discovery request to code list endpoint"""
	res = await service_client.get('/project/code/list')
	assert res.status == 200


async def test_measure(service_client):
	"""Discovery requests to measure endpoint"""
	res = await service_client.get('/project/measure')
	assert res.status == 200

	res = await service_client.post('/project/measure')
	assert res.status == 200

	res = await service_client.patch('/project/measure')
	assert res.status == 200

	res = await service_client.delete('/project/measure')
	assert res.status == 200


async def test_measure_list(service_client):
	"""Discovery request to measure list endpoint"""
	res = await service_client.get('/project/measure/list')
	assert res.status == 200


async def test_save_timer(service_client):
	"""Discovery requests to save_timer endpoint"""
	res = await service_client.get('/project/save-timer')
	assert res.status == 200

	res = await service_client.post('/project/save-timer')
	assert res.status == 200

	res = await service_client.patch('/project/save-timer')
	assert res.status == 200

	res = await service_client.delete('/project/save-timer')
	assert res.status == 200


async def test_save_timer_list(service_client):
	"""Discovery request to save_timer list endpoint"""
	res = await service_client.get('/project/save-timer/list')
	assert res.status == 200


async def test_cc_type_param(service_client):
	"""Discovery requests to cc_type_param endpoint"""
	res = await service_client.get('/project/cc-type-param')
	assert res.status == 200

	res = await service_client.post('/project/cc-type-param')
	assert res.status == 200

	res = await service_client.patch('/project/cc-type-param')
	assert res.status == 200

	res = await service_client.delete('/project/cc-type-param')
	assert res.status == 200


async def test_cc_type_param_list(service_client):
	"""Discovery request to cc_type_param list endpoint"""
	res = await service_client.get('/project/cc-type-param/list')
	assert res.status == 200


async def test_di_type(service_client):
	"""Discovery requests to di_type endpoint"""
	res = await service_client.get('/project/di-type')
	assert res.status == 200

	res = await service_client.post('/project/di-type')
	assert res.status == 200

	res = await service_client.patch('/project/di-type')
	assert res.status == 200

	res = await service_client.delete('/project/di-type')
	assert res.status == 200


async def test_di_type_list(service_client):
	"""Discovery request to di_type list endpoint"""
	res = await service_client.get('/project/di-type/list')
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