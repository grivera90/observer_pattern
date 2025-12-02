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
#ifndef __PRESSURE_MGR_H__
#define __PRESSURE_MGR_H__
/******************************************************************************
        Includes
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/******************************************************************************
        Constants
******************************************************************************/
#define MAX_OBS				3 // defined by desing
/******************************************************************************
        Data types
******************************************************************************/
typedef enum
{
	PRESS_MGR_OK = 0,
	PRESS_MGR_ERR_NOT_SPACE = -1,
	PRESS_MGR_ERR_NOT_FOUND = -2
	
} pressure_ret_t;

typedef void (*pressure_observer_cb_t)(float pressure, const void *user_ctx);

typedef struct 
{
    pressure_observer_cb_t callback;
    void *user_ctx;

} pressure_observer_t;

typedef struct 
{
	float current_pressure;
    pressure_observer_t observers[MAX_OBS];
	uint8_t observer_count;
	
} pressure_subject_t;
/******************************************************************************
        Public function prototypes
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

pressure_ret_t pressure_mgr_init(pressure_subject_t *p);
pressure_ret_t pressure_mgr_attach(pressure_subject_t *p, const pressure_observer_cb_t observer_cb, void *user_ctx);
pressure_ret_t pressure_mgr_dettach(pressure_subject_t *p, const pressure_observer_cb_t observer_cb);
void pressure_mgr_update(pressure_subject_t *p, uint16_t raw_pressure);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // EOF __PRESSURE_MGR_H__

