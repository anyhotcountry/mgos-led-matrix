#ifndef ARTNET_H
#define ARTNET_H

#include "fw/src/mgos_app.h"
#include "mgos_mongoose.h"

typedef uint8_t boolean;
typedef uint8_t byte;

// UDP specific
#define ART_NET_SPEC "udp://:6454"

// Opcodes
#define ART_POLL 0x2000
#define ART_DMX 0x5000

// Buffers
#define MAX_BUFFER_ARTNET 530

// Packet
#define ART_NET_ID "Art-Net\0"
#define ART_DMX_START 18

typedef void (*art_dmx_callback_t)(uint16_t universe, uint16_t length, uint8_t sequence, char* data); 
void mgos_artnet_init(art_dmx_callback_t callback);
void handler(struct mg_connection *nc, int ev, void *ev_data, void *user_data);
uint16_t parse(char *packet, size_t len);

#endif
