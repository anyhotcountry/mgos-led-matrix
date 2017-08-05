#include "mgos.h"
#include "tpm2net.h"
#include "mgos_bitbang.h"
#include "mgos_gpio.h"
#include "fw/src/mgos_timers.h"

#define PIN 23
#define LEDS 240

// Total number of channels you want to receive (1 led = 3 channels)
#define NUMBER_OF_CHANNELS LEDS * 3

uint8_t leds[NUMBER_OF_CHANNELS];

void show()
{
  mgos_gpio_write(PIN, false);
  mgos_usleep(60);
  mgos_bitbang_write_bits(PIN, MGOS_DELAY_100NSEC, 3, 8, 7, 6, leds, LEDS * 3);
  mgos_gpio_write(PIN, false);
  mgos_usleep(60);
  mgos_gpio_write(PIN, true);
}

void on_dmx_frame(uint16_t length, char *data)
{
  for (int i = 0; i < length && i < NUMBER_OF_CHANNELS; i++)
  {
    leds[i] = (uint8_t)data[i];
  }

  show();
}

enum mgos_app_init_result mgos_app_init(void)
{
  mgos_gpio_set_mode(PIN, MGOS_GPIO_MODE_OUTPUT);
  mgos_tpm2net_init(on_dmx_frame);
  return MGOS_APP_INIT_SUCCESS;
}
