
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <drivers/led_sensor.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
	const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(my_led_sensor));
	struct sensor_value val;
    int32_t counter = 10;

	if (!device_is_ready(dev)) {
		LOG_ERR("Sensor device not ready");
		return 0;
	}

	while (1) {
        /* Llamada a la función de extensión personalizada */
		led_sensor_set_custom_param(dev, counter++);

		sensor_sample_fetch(dev);
		LOG_INF("Fetch ejecutado: LED ON");
		k_sleep(K_MSEC(500));

		sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
		LOG_INF("Get ejecutado: LED OFF");
		k_sleep(K_MSEC(500));
	}

	return 0;
}
