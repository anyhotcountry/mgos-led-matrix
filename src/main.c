#include "mgos.h"
#include "tpm2net.h"
#include "mgos_bitbang.h"
#include "mgos_gpio.h"
#include "fw/src/mgos_timers.h"

#define PIN 14

void show(uint16_t length, uint8_t *leds)
{
  mgos_gpio_write(PIN, false);
  mgos_usleep(60);
  mgos_bitbang_write_bits(PIN, MGOS_DELAY_100NSEC, 3, 8, 7, 6, leds, length);
  mgos_gpio_write(PIN, false);
  mgos_usleep(60);
  mgos_gpio_write(PIN, true);
}

void on_dmx_frame(uint16_t length, char *data)
{
  show(length, (uint8_t *)data);
}

enum mgos_app_init_result mgos_app_init(void)
{
  mgos_gpio_set_mode(PIN, MGOS_GPIO_MODE_OUTPUT);
  mgos_tpm2net_init(on_dmx_frame);
  return MGOS_APP_INIT_SUCCESS;
}

