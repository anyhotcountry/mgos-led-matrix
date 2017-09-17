#include "tpm2net.h"
#define UNUSED(x) (void)(x)

static dmx_callback_t dmx_callback;

void parse(char *packet, uint16_t length)
{
  if (packet && length >= 6 && packet[0] == 0x9C)
  {
    uint8_t blocktype = packet[1];
    uint16_t framelength = ((uint16_t)packet[2] << 8) | (uint16_t)packet[3];
    if (blocktype == 0xDA)
    {
      if (length >= framelength + 7 && packet[6 + framelength] == 0x36)
      {
        char *frame = &packet[6];
        (*dmx_callback)(framelength, frame);
      }
    }
  }
}

void handler(struct mg_connection *nc, int ev, void *ev_data,
             void *user_data)
{
  struct mbuf *io = &nc->recv_mbuf;

  switch (ev)
  {
  case MG_EV_RECV:
    parse(io->buf, (uint16_t)io->len);
    break;
  default:
    break;
  }

  UNUSED(ev_data);
  UNUSED(user_data);
}

void mgos_tpm2net_init(dmx_callback_t callback)
{
  dmx_callback = callback;
  mgos_bind(TPM2NET_SPEC, handler, NULL);
}