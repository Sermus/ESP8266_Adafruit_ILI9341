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
#include "cube.h"

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
Adafruit_ILI9341 tft;

LOCAL double degree = -180.0;
LOCAL double scale = 2.0;
LOCAL double scale_inc = 0.01;
int16_t current[AMOUNT_NODE][3];
int16_t previous[AMOUNT_NODE][3];

ICACHE_FLASH_ATTR static void updateScreen(void)
{
	REG_SET_BIT(0x3ff00014, BIT(0));
	os_update_cpu_frequency(160);

	/*
	if (degree >= 180.0) degree = -180.0;
	if ((scale < 0.5) && (scale_inc < 0)) scale_inc = -scale_inc;
	if ((scale > 1.5) && (scale_inc > 0)) scale_inc = -scale_inc;
	cube_calculate(current, degree, degree, degree, scale, 0, 0, 0);
	degree += 2.5;
//	scale += scale_inc;
	cube_draw(previous, 0);
	cube_draw(current, 0xFFFF);
    memcpy(previous, current, sizeof (previous));
	 */
	uint32_t begin  = system_get_rtc_time();
	for (int i = 0; i < 100; i++) tft.drawLine(0,0,240,240,0xFFFF);
	uint32_t end  = system_get_rtc_time();
	ets_uart_printf("\r\nbenchmark:%d\r\n", end - begin);
}

ICACHE_FLASH_ATTR void sendMsgToHandler(void *arg)
{
	system_os_post(USER_TASK_PRIO_0, UPDATE_SCREEN, 'a');
}

ICACHE_FLASH_ATTR void handler_task (os_event_t *e)
{
	switch (e->sig)
	{
		case UPDATE_SCREEN: updateScreen(); break;
		default: break;
	}
}

extern "C" ICACHE_FLASH_ATTR void user_init(void)
{
	// Configure the UART
	uart_init(BIT_RATE_115200,BIT_RATE_115200);
	ets_uart_printf("\r\nSystem init...\r\n");
	do_global_ctors();
	ets_uart_printf("\r\nGlobal constructors invoked\r\n");
	os_event_t *handlerQueue;

	// Initialize TFT
	tft.begin();
	tft.fillScreen(0);
	cube_draw_init();
	ets_uart_printf("\r\ntft initialized\r\n");

	// Set up a timer to send the message to handler
	os_timer_disarm(&timerHandler);
	os_timer_setfn(&timerHandler, (os_timer_func_t *)sendMsgToHandler, (void *)0);
	os_timer_arm(&timerHandler, 20, 1);

	// Set up a timerHandler to send the message to handler
	handlerQueue = (os_event_t *)os_malloc(sizeof(os_event_t)*TEST_QUEUE_LEN);
	system_os_task(handler_task, USER_TASK_PRIO_0, handlerQueue, TEST_QUEUE_LEN);

	ets_uart_printf("System init done \r\n");
}
