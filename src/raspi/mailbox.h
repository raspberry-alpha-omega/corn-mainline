#ifndef MAILBOX_H
#define MAILBOX_H

#include <stdint.h>

extern uint32_t readmailbox(uint32_t channel);
extern void writemailbox(uint32_t channel, uint32_t data);

#endif
