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
#include "time.h"
// declare lib methods
extern int ets_uart_printf(const char *fmt, ...);
void ets_timer_disarm(ETSTimer *ptimer);
void ets_timer_setfn(ETSTimer *ptimer, ETSTimerFunc *pfunction, void *parg);
}

LOCAL os_timer_t timerHandler;
LOCAL Adafruit_ILI9341 tft;

static void updateScreen(void)
{
	REG_SET_BIT(0x3ff00014, BIT(0));
	os_update_cpu_frequency(160);

	uint32_t begin  = system_get_rtc_time();
	for(int i = 0; i < 100; i++)
		tft.fillRect(0, 0, 100, 100, rand());
	uint32_t end  = system_get_rtc_time();
	ets_uart_printf("\r\nbenchmark:%d\r\n", end - begin);
}

void sendMsgToHandler(void *arg)
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
	// Configure the UART
	uart_init(BIT_RATE_115200,BIT_RATE_115200);
	ets_uart_printf("\r\nSystem init...\r\n");
	do_global_ctors();
	ets_uart_printf("\r\nGlobal constructors invoked\r\n");
	os_event_t *handlerQueue;

	// Initialize TFT
	tft.begin();
	ets_uart_printf("\r\ntft initialized\r\n");

	// Set up a timer to send the message to handler
	os_timer_disarm(&timerHandler);
	os_timer_setfn(&timerHandler, (os_timer_func_t *)sendMsgToHandler, (void *)0);
	os_timer_arm(&timerHandler, DELAY_TIMER, 1);

	// Set up a timerHandler to send the message to handler
	handlerQueue = (os_event_t *)os_malloc(sizeof(os_event_t)*TEST_QUEUE_LEN);
	system_os_task(handler_task, USER_TASK_PRIO_0, handlerQueue, TEST_QUEUE_LEN);

	ets_uart_printf("System init done \r\n");
}
