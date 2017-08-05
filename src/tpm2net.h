#ifndef TPM2NET_H
#define TPM2NET_H

#include "fw/src/mgos_app.h"
#include "mgos_mongoose.h"

// UDP specific
#define TPM2NET_SPEC "udp://:65506"

typedef void (*dmx_callback_t)(uint16_t length, char* data);
void mgos_tpm2net_init(dmx_callback_t callback);

#endif
