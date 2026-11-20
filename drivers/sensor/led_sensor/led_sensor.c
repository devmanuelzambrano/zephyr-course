#define DT_DRV_COMPAT custom_led_sensor

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(led_sensor, LOG_LEVEL_INF);

struct led_sensor_config {
	struct gpio_dt_spec led;
};

struct led_sensor_data {
	bool state;
};

static int led_sensor_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
	const struct led_sensor_config *cfg = dev->config;
	struct led_sensor_data *data = dev->data;
	int ret;

	ret = gpio_pin_set_dt(&cfg->led, 1);
	if (ret < 0) {
		return ret;
	}

	data->state = true;
	return 0;
}

static int led_sensor_channel_get(const struct device *dev,
				  enum sensor_channel chan,
				  struct sensor_value *val)
{
	const struct led_sensor_config *cfg = dev->config;
	struct led_sensor_data *data = dev->data;
	int ret;

	ret = gpio_pin_set_dt(&cfg->led, 0);
	if (ret < 0) {
		return ret;
	}

	data->state = false;
	val->val1 = 0;
	val->val2 = 0;
	return 0;
}

static int led_sensor_init(const struct device *dev)
{
	const struct led_sensor_config *cfg = dev->config;

	if (!gpio_is_ready_dt(&cfg->led)) {
		LOG_ERR("GPIO device not ready");
		return -ENODEV;
	}

	return gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE);
}

static DEVICE_API(sensor, led_sensor_api) = {
	.sample_fetch = led_sensor_sample_fetch,
	.channel_get = led_sensor_channel_get,
};

#define LED_SENSOR_DEFINE(inst)								\
	static struct led_sensor_data led_sensor_data_##inst;				\
	static const struct led_sensor_config led_sensor_cfg_##inst = {			\
		.led = GPIO_DT_SPEC_INST_GET(inst, gpios),				\
	};										\
	SENSOR_DEVICE_DT_INST_DEFINE(inst, led_sensor_init, NULL,			\
				     &led_sensor_data_##inst, &led_sensor_cfg_##inst,	\
				     POST_KERNEL, 90,		\
				     &led_sensor_api);

DT_INST_FOREACH_STATUS_OKAY(LED_SENSOR_DEFINE)
