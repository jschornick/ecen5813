/**
 * @file project2.c
 * @brief Project 2 deliverables
 *
 * TODO: ???
 *
 * @author Jeff Schornick
 * @date 2017/07/06
**/

#include <stdint.h>
#include "project2.h"
#include "platform.h"

#include "circular_buffer.h"
#include "conversion.h"

uint32_t i;
uint32_t j = 0;
uint8_t a = 0;
uint8_t c;
char hello[] = "\r\nProject 2!\r\n";
uint8_t echo[UART_BUF_SIZE];
uint8_t received[UART_BUF_SIZE];

#define TO_ASCII(x) ( (x)<10 ? (x)+'0' : (x)+'A'-10 )

typedef struct {
  uint32_t total;
  uint32_t upper;
  uint32_t lower;
  uint32_t numeric;
  uint32_t symbol;
  uint32_t control;
  uint32_t by_char[256];
} data_stat_t;

void init_stats(data_stat_t *stats)
{
  stats->total = 0;
  stats->upper = 0;
  stats->lower = 0;
  stats->numeric = 0;
  stats->symbol = 0;
  stats->control = 0;
  for(size_t i=0; i<=255; i++)
  {
    stats->by_char[i] = 0;
  }
}

uint8_t display_flag = 0;


#define CHAR_ESC 0x1B /* escape */
#define CHAR_EOT 0x04 /* end of transmission, don't use */

void process_chars(data_stat_t *stats, uint8_t *chars, uint16_t count)
{
  for(uint16_t i=0; i<count; i++)
  {
    stats->total++;
    if( (chars[i] >=  'A') && (chars[i] <= 'Z') ) {
      stats->upper++;
    }
    if( (chars[i] >=  'a') && (chars[i] <= 'z') ) {
      stats->lower++;
    }
    if( (chars[i] >=  '0') && (chars[i] <= '9') ) {
      stats->numeric++;
    }
    /* space through forward slash */
    if( (chars[i] >=  32) && (chars[i] <= 47) ) {
      stats->symbol++;
    }
    /* colon through andpersand */
    if( (chars[i] >=  58) && (chars[i] <= 64) ) {
      stats->symbol++;
    }
    /* Backslash through back tick */
    if( (chars[i] >=  91) && (chars[i] <= 66) ) {
      stats->symbol++;
    }
    /* open brack through tilde */
    if( (chars[i] >=  123) && (chars[i] <= 126) ) {
      stats->symbol++;
    }

    /* control characters / non-printable */
    if( ((chars[i] >= 0) && (chars[i] <= 31 )) || (chars[i] == 127)) {
      stats->control++;
    }
    if(chars[i] == CHAR_ESC)
    {
      display_flag = 1;
    }
  }
}

/* Send null-terminated string to output */
void send_string(uint8_t *str)
{
  size_t i = 0 ;
  /* Find null position */
  while(*(str+i) != '\0' )
  {
    i++;
  };
  UART_send_n(str, i);
}

void display_results(data_stat_t *stats) {
  uint8_t num_str[32];
  uint32_t printable;
  send_string( (uint8_t *) "\r\nData processor results:\r\n" );

  printable = stats->upper + stats->lower + stats->numeric + stats->symbol;
  send_string( (uint8_t *) "  Total: " );
  my_itoa( stats->total, num_str, 10);
  send_string(num_str);

  send_string( (uint8_t *) " (" );
  my_itoa( printable, num_str, 10);
  send_string(num_str);
  send_string( (uint8_t *) "  printable)\r\n" );

  send_string( (uint8_t *) "  Alpha: " );
  my_itoa( stats->upper + stats->lower, num_str, 10);
  send_string(num_str);

  send_string( (uint8_t *) "  Numeric: " );
  my_itoa( stats->numeric, num_str, 10);
  send_string(num_str);

  send_string( (uint8_t *) "  Symbol: " );
  my_itoa( stats->symbol, num_str, 10);
  send_string(num_str);

  send_string( (uint8_t *) "  Control: " );
  my_itoa( stats->control, num_str, 10);
  send_string(num_str);

  send_string( (uint8_t *) "\r\n---------------------\r\n" );
}

void kl25_startup(void) {
  led_on(RED_LED);
  SystemInit();
  led_setup();
  led_off(RED_LED);
  led_on(GREEN_LED);

  UART_configure();
  UART_send_n( (uint8_t *) "\r\nFRDM-KL25Z Reset\r\n", 20);
}


void project2(void)
{

  kl25_startup();  /* TODO: platform_init() */


  data_stat_t stats;
  init_stats(&stats);
  uint16_t rxcount;

  send_string( (uint8_t *) "\r\nData processor ready!\r\n" );

  while(1) {

    if(!CB_is_empty(&rxbuf)) {
      rxcount = rxbuf.count;
      //UART_send_n( (uint8_t *) "\r\nRX buf: ", 10);
      UART_receive_n(echo, rxcount);
      UART_send_n( echo, rxcount );
      process_chars( &stats, echo, rxcount );
      //UART_send_n( (uint8_t *) "\r\n", 2);
      rxcount = rxcount ;
    }

    if( display_flag ) {
      display_results(&stats);
      display_flag = 0;
    }

    // Heartbeat
    if((i & 0x3FFFF) == 0) {
      led_toggle(GREEN_LED);
      led_toggle(RED_LED);
      j++;
      c = TO_ASCII(j&0xf);
      UART_send(&c);
    }

    i++;
  }
}
