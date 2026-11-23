
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>
#include <drivers/led_sensor.h>

static const struct device *const sensor_dev = DEVICE_DT_GET(DT_NODELABEL(my_led_sensor));

static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	if (!device_is_ready(sensor_dev)) {
		shell_error(sh, "Sensor device is not ready");
		return -ENODEV;
	}

	int ret = sensor_sample_fetch(sensor_dev);
	if (ret < 0) {
		shell_error(sh, "Failed to fetch sample (err %d)", ret);
		return ret;
	}

	shell_print(sh, "Sample fetched (LED ON)");
	return 0;
}

static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);
	struct sensor_value val;

	if (!device_is_ready(sensor_dev)) {
		shell_error(sh, "Sensor device is not ready");
		return -ENODEV;
	}

	int ret = sensor_channel_get(sensor_dev, SENSOR_CHAN_ALL, &val);
	if (ret < 0) {
		shell_error(sh, "Failed to get channel data (err %d)", ret);
		return ret;
	}

	shell_print(sh, "Channel read (LED OFF) -> val1: %d, val2: %d", val.val1, val.val2);
	return 0;
}

static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	bool ready = device_is_ready(sensor_dev);
	shell_print(sh, "Device name: %s", sensor_dev->name);
	shell_print(sh, "Ready state: %s", ready ? "READY" : "NOT READY");
	return 0;
}

/* Nuevo subcomando: sensor set <value> (rango válido: 0 a 100) */
static int cmd_sensor_set(const struct shell *sh, size_t argc, char **argv)
{
	if (!device_is_ready(sensor_dev)) {
		shell_error(sh, "Sensor device is not ready");
		return -ENODEV;
	}

	char *endptr;
	long val = strtol(argv[1], &endptr, 10);

	/* Validar que sea un número entero válido y esté dentro del rango [0, 100] */
	if (*endptr != '\0' || val < 0 || val > 100) {
		shell_error(sh, "Invalid value '%s'. Must be an integer between 0 and 100.", argv[1]);
		return -EINVAL;
	}

	int ret = led_sensor_set_custom_param(sensor_dev, (int32_t)val);
	if (ret < 0) {
		shell_error(sh, "Failed to set custom parameter (err %d)", ret);
		return ret;
	}

	shell_print(sh, "Custom parameter updated to: %ld", val);
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
	SHELL_CMD(fetch, NULL, "Fetch sensor sample (turns LED ON)", cmd_sensor_fetch),
	SHELL_CMD(read, NULL, "Read sensor channel (turns LED OFF)", cmd_sensor_read),
	SHELL_CMD(info, NULL, "Print sensor device name and ready state", cmd_sensor_info),
	SHELL_CMD_ARG(set, NULL, "Set custom parameter (0-100)", cmd_sensor_set, 2, 0),
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sub_sensor, "LED Sensor control commands", NULL);

int main(void)
{
	return 0;
}
