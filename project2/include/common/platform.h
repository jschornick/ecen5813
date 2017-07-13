/**
 * @file platform.h
 * @brief Platform indpendence layer
 *
 * Combined with compile-time flags, creates a platform independent interface layer
 * to various platform-specific lower-level functions.
 *
 * @author Jeff Schornick
 * @date 2017/07/12
 **/

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#ifdef KL25Z
  #include "led.h"
  #include "uart.h"
  #include "system_MKL25Z4.h"
#else
  #include <stdio.h>
  #define SystemInit()
  #if DEBUG_LED
    #define led_setup() printf("led_setup()\n")
    #define led_on(x) printf("%s on\n", #x)
    #define led_off(x) printf("%s off\n", #x)
    #define led_toggle(x) printf("%s toggle\n", #x)
  #else
    #define led_setup()
    #define led_on(x)
    #define led_off(x)
    #define led_toggle(x)
  #endif
  #if DEBUG_UART
    #define UART_configure() printf("UART_configure()\n")
    #define UART_send(x) printf("UART_send(%d)\n", *x)
    #define UART_receive(x) printf("UART_receive(%d)\n", *x)
    #define UART_send_n(x,y) printf("UART_send_n(%s,%d)\n", x, y)
    #define UART_receive_n(x,y) printf("UART_receive_n(%s,%d)\n", x,y)
  #else
    #define UART_configure()
    #define UART_send(x)
    #define UART_receive(x)
    #define UART_send_n(x,y)
    #define UART_receive_n(x,y)
  #endif
#endif

#endif /* __PLATFORM_H__ */
