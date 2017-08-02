#include "artnet.h"

static art_dmx_callback_t dmx_callback;

void handler(struct mg_connection *nc, int ev, void *ev_data,
                     void *user_data)
{
  struct mbuf *io = &nc->recv_mbuf;

  switch (ev)
  {
  case MG_EV_RECV:
    parse(io->buf, io->len);
    break;
  default:
    break;
  }
}

void mgos_artnet_init(art_dmx_callback_t callback)
{
  dmx_callback = callback;
  mgos_bind(ART_NET_SPEC, handler, NULL);
}

uint16_t parse(char *packet, size_t len)
{
  for (byte i = 0; i < 8; i++)
  {
    if (packet[i] != ART_NET_ID[i])
    {
      return 0;
    }
  }

  uint16_t opcode = packet[8] | packet[9] << 8;

  if (opcode == ART_DMX)
  {
    uint16_t sequence = packet[12];
    uint16_t incomingUniverse = packet[14] | packet[15] << 8;
    uint16_t dmxDataLength = packet[17] | packet[16] << 8;

    (*dmx_callback)(incomingUniverse, dmxDataLength, sequence, packet + ART_DMX_START);

    return ART_DMX;
  }

  if (opcode == ART_POLL)
  {
    return ART_POLL;
  }

  return 0;
}
