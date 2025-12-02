/**
*******************************************************************************
* @file           : pressure_mgr.h
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
#include <stdint.h>
#include <stddef.h>

#include "pressure_mgr.h"
/******************************************************************************
    Defines and constants
******************************************************************************/

/******************************************************************************
    Data types
******************************************************************************/

/******************************************************************************
    Local variables
******************************************************************************/
static pressure_subject_t *pressure_subjet = NULL;
/******************************************************************************
    Local function prototypes
******************************************************************************/
static float convert_raw_to_pressure(uint16_t raw_pressure);
static void pressure_notify(pressure_subject_t *p);
/******************************************************************************
    Local function definitions
******************************************************************************/
static float convert_raw_to_pressure(uint16_t raw_pressure)
{
	return raw_pressure * 0.56; // example.
}

static void pressure_notify(pressure_subject_t *p)
{
	for (uint8_t i = 0; i < p->observer_count; i++)
	{
		p->observers[i].callback(p->current_pressure, p->observers[i].user_ctx);
	}
}
/******************************************************************************
    Public function definitions
******************************************************************************/
pressure_ret_t pressure_mgr_init(pressure_subject_t *p)
{
	
	p->current_pressure = 0.0;
	p->observer_count = 0;
	
	return PRESS_MGR_OK;
}

pressure_ret_t pressure_mgr_attach(pressure_subject_t *p, const pressure_observer_cb_t observer_cb, void *user_ctx)
{
	if (p->observer_count >= MAX_OBS)
	{
		return PRESS_MGR_ERR_NOT_SPACE;
	}
	
	p->observers[p->observer_count].callback = observer_cb;
	p->observers[p->observer_count].user_ctx = user_ctx;
	p->observer_count++;
	
	return PRESS_MGR_OK;
}

pressure_ret_t pressure_mgr_dettach(pressure_subject_t *p, const pressure_observer_cb_t observer_cb)
{
	
	for (uint8_t i = 0; i < p->observer_count; i++)
	{
		if (p->observers[i].callback == observer_cb)
		{
			p->observers[i].callback = p->observers[p->observer_count - 1].callback;
			p->observer_count--;
			return PRESS_MGR_OK;
		}
	}
	
	return PRESS_MGR_ERR_NOT_FOUND;
}

void pressure_mgr_update(pressure_subject_t *p, uint16_t raw_pressure)
{
	float pressure = convert_raw_to_pressure(raw_pressure);
	p->current_pressure = pressure;
	pressure_notify(p);
}



