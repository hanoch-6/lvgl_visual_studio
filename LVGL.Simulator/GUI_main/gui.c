/**
* @file gui.c
*
*/

/*********************
*      INCLUDES
*********************/
#include "gui.h"

#include "lvgl/lvgl.h"
#include "lv_conf.h"

/*********************
*      DEFINES
*********************/

/**********************
*      TYPEDEFS
**********************/

/**********************
*  STATIC PROTOTYPES
**********************/
static void start_gif_create(void);
/**********************
*  STATIC VARIABLES
**********************/

/**********************
*      MACROS
**********************/

/**********************
*   GLOBAL FUNCTIONS
**********************/

void gui_start(void)
{
    start_gif_create();
}
/**********************
*   STATIC FUNCTIONS
**********************/
static void start_gif_create(void)
{
    LV_IMG_DECLARE(xiaoxin);

    lv_obj_t* start_gif = lv_gif_create(lv_scr_act());
    lv_gif_set_src(start_gif, &xiaoxin);
    lv_obj_align(start_gif, LV_ALIGN_CENTER, 0, 0);
}
