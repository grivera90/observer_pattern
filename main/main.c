/**
*******************************************************************************
* @file           : main_app.c
* @brief          : Description of header file
* @author         : Gonzalo Rivera
* @date           : 01/12/2025
*******************************************************************************
* @attention
*
* Copyright (c) <date> grivera. All rights reserved.
*
*/
/******************************************************************************
    Includes
******************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "pressure_mgr.h"
#include "pressure_sensor_drv.h"
/******************************************************************************
    Defines and constants
******************************************************************************/

/******************************************************************************
    Data types
******************************************************************************/
typedef struct
{
    float last_pressure;
    int screen;
    
} ui_ctx_t;

typedef struct
{
    float low_threshold;
    float high_threshold;
    int alarm_low_active;
    int alarm_high_active;
    
} alarm_ctx_t;
/******************************************************************************
    Local variables
******************************************************************************/
static pressure_subject_t pressure;
static ui_ctx_t ui;
static alarm_ctx_t alarm_ctx;
/******************************************************************************
    Local function prototypes
******************************************************************************/
// UI Observer.
static void ui_pressure_update_cb(float pressure, const void *ctx);
// Alarm Observer.
static void alarm_pressure_cb(float pressure, const void *ctx);
// Pressure calc task.
static void pressure_task(void *arg);
/******************************************************************************
    Local function definitions
******************************************************************************/
static void ui_pressure_update_cb(float pressure, const void *ctx)
{
    ui_ctx_t *ui_ctx = (ui_ctx_t *) ctx;
    
    if (ui_ctx->screen == 1)
    {
		printf("[UI] Pressure: %f", pressure);	
	}
}

static void alarm_pressure_cb(float pressure, const void *ctx)
{
    alarm_ctx_t *alarm = (alarm_ctx_t *)ctx;

    if (pressure < alarm->low_threshold) 
    {
        printf("[ALAMR] Low pressure\n");
    } 
    else if (pressure > alarm->high_threshold) 
    {
        printf("[ALAMR] High pressure\n");
    } 
    else 
    {
		if (alarm->alarm_low_active || alarm->alarm_high_active) 
		{
		    alarm->alarm_low_active = 0;
		    alarm->alarm_high_active = 0;
		}
    }	
}

static void pressure_task(void *arg)
{

    for (;;)
    {
        uint16_t raw = pressure_sensor_read();
        pressure_mgr_update(&pressure, raw);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
/******************************************************************************
    Main app
******************************************************************************/
void app_main(void)
{
	pressure_sensor_init();
	pressure_mgr_init(&pressure);
	
	pressure_mgr_attach(&pressure, ui_pressure_update_cb, &ui);
	pressure_mgr_attach(&pressure, alarm_pressure_cb, &alarm_ctx);
	
	xTaskCreate(pressure_task, "pressure", 2048, NULL, 5, NULL);
    
    while (true) 
    {
        printf("Hello from app_main!\n");
        sleep(5);
    }
}


