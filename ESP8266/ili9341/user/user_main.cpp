/*
 *  Example of working ESP8266 and 2.2 Inch SPI TFT LCD Serial Port Module Display ILI9341
 *  Specification on ILI9341 - http://www.newhavendisplay.com/app_notes/ILI9341.pdf
 *  Specification on ESP8266 - http://www.esp8266.com/
 *
 *  Original source https://github.com/Perfer/esp8266_ili9341
 *
 *  Author: Semen Sachkov
 *
 */

#include "Adafruit_ILI9341_fast_as.h"

// =============================================================================================
// C includes and declarations
// =============================================================================================
#include "cpp_routines/routines.h"
extern "C"
{
#include <osapi.h>
#include "user_config.h"
#include "espmissingincludes.h"
#include "driver/uart.h"
// declare lib methods
extern int ets_uart_printf(const char *fmt, ...);
void ets_timer_disarm(ETSTimer *ptimer);
void ets_timer_setfn(ETSTimer *ptimer, ETSTimerFunc *pfunction, void *parg);
}

LOCAL os_timer_t timerHandler;
LOCAL Adafruit_ILI9341 tft;

static void updateScreen(void)
{
	for(int i = 0; i < 100; i++)
		tft.fillRect(0, 0, tft.width(), tft.height(), ILI9341_BLACK);
}

LOCAL void ICACHE_FLASH_ATTR sendMsgToHandler(void *arg)
{
	system_os_post(USER_TASK_PRIO_0, UPDATE_SCREEN, 'a');
}

void handler_task (os_event_t *e)
{
	switch (e->sig)
	{
		case UPDATE_SCREEN: updateScreen(); break;
		default: break;
	}
}

extern "C" void user_init(void)
{
	do_global_ctors();
	os_event_t *handlerQueue;
	// Configure the UART
	uart_init(BIT_RATE_115200,BIT_RATE_115200);
	ets_uart_printf("\r\nSystem init...\r\n");

	// Initialize TFT
	tft.begin();

	// Set up a timer to send the message to handler
	os_timer_disarm(&timerHandler);
	os_timer_setfn(&timerHandler, (os_timer_func_t *)sendMsgToHandler, (void *)0);
	os_timer_arm(&timerHandler, DELAY_TIMER, 1);

	// Set up a timerHandler to send the message to handler
	handlerQueue = (os_event_t *)os_malloc(sizeof(os_event_t)*TEST_QUEUE_LEN);
	system_os_task(handler_task, USER_TASK_PRIO_0, handlerQueue, TEST_QUEUE_LEN);

	ets_uart_printf("System init done \r\n");
}
