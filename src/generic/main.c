#include "gpio.h"
#include "morse.h"
#include "ledborg.h"
#include "mailbox.h"

/* Use some free memory in the area below the kernel/stack, 16-byte aligned */
#define BUFFER_ADDRESS 0x1000

#define RR_REQUEST 0x00000000
#define RR_RESPONSE_OK 0x80000000
#define RR_RESPONSE_ERROR 0x80000001

#define SLOT_OVERALL_LENGTH 0
#define SLOT_RR 1
#define SLOT_TAGSTART 2

#define SLOT_TAG_ID 0
#define SLOT_TAG_BUFLEN 1
#define SLOT_TAG_DATALEN 2
#define SLOT_TAG_DATA 3

#define MBOX_HEADER_LENGTH 2
#define TAG_HEADER_LENGTH 3

#define MBX_DEVICE_SDCARD 0x00000000
#define MBX_DEVICE_UART0 0x00000001
#define MBX_DEVICE_UART1 0x00000002
#define MBX_DEVICE_USBHCD 0x00000003
#define MBX_DEVICE_I2C0 0x00000004
#define MBX_DEVICE_I2C1 0x00000005
#define MBX_DEVICE_I2C2 0x00000006
#define MBX_DEVICE_SPI 0x00000007
#define MBX_DEVICE_CCP2TX 0x00000008

#define MBX_TAG_GET_FIRMWARE 0x00000001 /* in 0, out 4 */
#define MBX_TAG_GET_BOARD_MODEL 0x00010001 /* in 0, out 4 */
#define MBX_TAG_GET_BOARD_REVISION 0x00010002 /* in 0, out 4 */
#define MBX_TAG_GET_MAC_ADDRESS 0x00010003 /* in 0, out 6 */
#define MBX_TAG_GET_BOARD_SERIAL 0x00010004 /* in 0, out 8 */
#define MBX_TAG_GET_ARM_MEMORY 0x00010005 /* in 0, out 8 (4 -> base addr, 4 -> len in bytes) */
#define MBX_TAG_GET_VC_MEMORY 0x00010006 /* in 0, out 8 (4 -> base addr, 4 -> len in bytes) */
#define MBX_TAG_GET_COMMANDLINE 0x00050001 /* in 0, out variable */
#define MBX_TAG_GET_DMA_CHANNELS 0x00060001 /* in 0, out 4 */

#define MBX_TAG_GET_POWER_STATE 0x00020001 /* in 4 -> dev id, out 8 (4 -> device, 4 -> status) */
#define MBX_TAG_GET_TIMING 0x00020002 /* in 0, out 4 */
#define MBX_TAG_GET_FIRMWARE 0x00000001 /* in 0, out 4 */

// there are a load more, see https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface

// TODO support more than one tag in buffer
void add_mailbox_tag(volatile uint32_t* buffer, uint32_t tag, uint32_t buflen, uint32_t len, uint32_t* data) {
  volatile uint32_t* start = buffer + SLOT_TAGSTART;
  start[SLOT_TAG_ID] = tag;
  start[SLOT_TAG_BUFLEN] = buflen;
  start[SLOT_TAG_DATALEN] = len & 0x7FFFFFFF;

  uint32_t bufwords = buflen >> 2;

  if (0 == data) {
    for (int i = 0; i < bufwords; ++i) {
      start[SLOT_TAG_DATA + i] = 0;
    }
  } else {
    for (int i = 0; i < bufwords; ++i) {
      start[SLOT_TAG_DATA + i] = data[i];
    }
  }

  start[SLOT_TAG_DATA+bufwords] = 0; // end of tags, unless overwritten later
}

// TODO support more than one tag in buffer
void build_mailbox_request(volatile uint32_t* buffer) {
  uint32_t tag_length = buffer[MBOX_HEADER_LENGTH + SLOT_TAG_BUFLEN];
  uint32_t end = (MBOX_HEADER_LENGTH*4) + (TAG_HEADER_LENGTH*4) + tag_length;
  uint32_t overall_length = end + 4;
  buffer[SLOT_OVERALL_LENGTH] = overall_length;
  buffer[SLOT_RR] = RR_REQUEST;
}

volatile uint32_t *mailbuffer = (uint32_t *) BUFFER_ADDRESS;

void dump_parameter(const char* name, uint32_t value) {
  raspi_mini_uart_send_string(" ");
  raspi_mini_uart_send_string(name);
  raspi_mini_uart_send_string(": ");
  raspi_mini_uart_send_hex(value);
  raspi_mini_uart_send_newline();
}

void dump_mailbox_to_uart() {
  raspi_mini_uart_send_string("Mailbox[");
  raspi_mini_uart_send_newline();
  dump_parameter("mailbox length", mailbuffer[0]);
  dump_parameter("mailbox rr", mailbuffer[1]);
  int i = 2;
  while (mailbuffer[i] > 0) {
    dump_parameter("tag id", mailbuffer[i++]);
    uint32_t buflen = mailbuffer[i++];
    dump_parameter("tag buffez", buflen);
    dump_parameter("tag len", mailbuffer[i++]);

    dump_parameter("tag data(w0)", mailbuffer[i]);
    i += buflen >>2;
  }
  raspi_mini_uart_send_string("]");
  raspi_mini_uart_send_newline();
}

void warmup(void) {
  for (int i = 3; i > 0; --i) {
    morse_char('a');
    raspi_mini_uart_send_char('0'+i);
  }
  raspi_mini_uart_send_newline();
}

void dump_response(const char* name, int nwords) {
  raspi_mini_uart_send_string(name);
  raspi_mini_uart_send_string(": ");
  for (int i = 0; i < nwords; ++i) {
    uint32_t value = mailbuffer[MBOX_HEADER_LENGTH + TAG_HEADER_LENGTH + i];
    raspi_mini_uart_send_hex(value);
    raspi_mini_uart_send_char(' ');
  }
  raspi_mini_uart_send_newline();
}

void print_parameter(const char* name, uint32_t tag, int nwords) {
  add_mailbox_tag(mailbuffer, tag, nwords * 4, 0, 0);
  build_mailbox_request(mailbuffer);

//  raspi_mini_uart_send_string("before:");
//  raspi_mini_uart_send_newline();
//  dump_mailbox_to_uart();

  writemailbox(8, (uint32_t)mailbuffer);
  readmailbox(8);

  /* Valid response in data structure */
  if(mailbuffer[1] != 0x80000000) {
    raspi_mini_uart_send_string("error:");
    raspi_mini_uart_send_newline();
    dump_mailbox_to_uart();
  } else {
    dump_response(name, nwords);
  }
}

int main(void) {
//  rapi_okled_init();
//  raspi_mini_uart_init();
//  warmup();
//
//  print_parameter("firmware", MBX_TAG_GET_FIRMWARE, 1);
//  print_parameter("board model", MBX_TAG_GET_BOARD_MODEL, 1);
//  print_parameter("board rev", MBX_TAG_GET_BOARD_REVISION, 1);
//  print_parameter("mac address", MBX_TAG_GET_MAC_ADDRESS, 2);
//  print_parameter("board serial", MBX_TAG_GET_BOARD_SERIAL, 2);
//  print_parameter("arm mem", MBX_TAG_GET_ARM_MEMORY, 2);
//  print_parameter("vc mem", MBX_TAG_GET_VC_MEMORY, 2);

  ledborg_init();
  morse_set_switch(&ledborg_set_all);
  halt("ledborg ");
}
