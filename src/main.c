#include "mgos.h"
#include "artnet.h"
#include "mgos_bitbang.h"
#include "mgos_gpio.h"
#include "fw/src/mgos_timers.h"

#define PIN 23
#define LEDS 240

// Total number of channels you want to receive (1 led = 4 channels)
#define NUMBER_OF_CHANNELS LEDS * 3

uint8_t leds[LEDS * 3];

const int startUniverse = 1; // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as 0.

// Check if we got all universes
#define MAX_UNIVERSES NUMBER_OF_CHANNELS / 480 + ((NUMBER_OF_CHANNELS % 480) ? 1 : 0)
bool sendFrame = 1;
int previousDataLength = 0;
bool universesReceived[MAX_UNIVERSES];

void show()
{
  mgos_gpio_write(PIN, false);
  mgos_usleep(60);
  mgos_bitbang_write_bits(PIN, MGOS_DELAY_100NSEC, 3, 8, 7, 6, leds, LEDS * 3);
  mgos_gpio_write(PIN, false);
  mgos_usleep(60);
  mgos_gpio_write(PIN, true);
}

void on_dmx_frame(uint16_t universe, uint16_t length, uint8_t sequence, char *data)
{
  sendFrame = 1;
  // set brightness of the whole strip
  if (universe == 15)
  {
    // leds.setBrightness(data[0]);
    show();
  }

  // Store which universe has got in
  if ((universe - startUniverse) < MAX_UNIVERSES)
  {
    universesReceived[universe - startUniverse] = 1;
  }

  for (int i = 0; i < MAX_UNIVERSES; i++)
  {
    if (universesReceived[i] == 0)
    {
      //Serial.println("Broke");
      sendFrame = 0;
      break;
    }
  }

  // read universe and put into the right part of the display buffer
  for (int i = 0; i < length / 3; i++)
  {
    int led = i + (universe - startUniverse) * (previousDataLength / 3);
    if (led < LEDS)
    {
      leds[led * 3] = (uint8_t)data[i * 3];
      leds[led * 3 + 1] = (uint8_t)data[i * 3 + 1];
      leds[led * 3 + 2] = (uint8_t)data[i * 3 + 2];
    }
  }

  previousDataLength = length;

  if (sendFrame)
  {
    show();

    // Reset universeReceived to 0
    memset(universesReceived, 0, MAX_UNIVERSES);
  }
}

enum mgos_app_init_result mgos_app_init(void)
{
  mgos_gpio_set_mode(PIN, MGOS_GPIO_MODE_OUTPUT);
  mgos_artnet_init(on_dmx_frame);
  return MGOS_APP_INIT_SUCCESS;
}
