#ifndef ZEPHYR_INCLUDE_DRIVERS_LED_SENSOR_H_
#define ZEPHYR_INCLUDE_DRIVERS_LED_SENSOR_H_

#include <zephyr/device.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Función de extensión personalizada para cambiar un parámetro
 *        en la estructura de datos dinámica del driver.
 */
int led_sensor_set_custom_param(const struct device *dev, int32_t new_value);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_DRIVERS_LED_SENSOR_H_ */