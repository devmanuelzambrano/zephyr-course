#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/autoconf.h>

//#define SLEEP_TIME_MS 1000

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = true;

    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    // 1. Evaluar qué bandera de Kconfig está activa y asignar los milisegundos
    int sleep_time_ms = 1000; // Valor por defecto (1 segundo)

    #if defined(CONFIG_LED_BLINK_SLEEP_500MS)
        sleep_time_ms = 500;
    #elif defined(CONFIG_LED_BLINK_SLEEP_1S)
        sleep_time_ms = 1000;
    #elif defined(CONFIG_LED_BLINK_SLEEP_2S)
        sleep_time_ms = 2000;
    #endif

    while (1) {
        if (gpio_pin_toggle_dt(&led) < 0) return 0;

        led_state = !led_state;
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        
        // 2. Usar la variable calculada de forma segura
        k_sleep((k_timeout_t){.ticks = sleep_time_ms});
    }
    return 0;
}
