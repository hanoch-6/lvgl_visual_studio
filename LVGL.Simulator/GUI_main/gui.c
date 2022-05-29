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
#define HOR_STEP 40
#define VER_STEP 40
/**********************
*      TYPEDEFS
**********************/

/**********************
*  STATIC PROTOTYPES
**********************/
static void start_gif_create();
static void scroll_creatorInfo_create();
static void weather_sys_create();

static void startup_timer(lv_timer_t* timer);
static void screen_clean_up(void* scr);
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
    lv_timer_t* timer = lv_timer_create(startup_timer, 1000, NULL);
    lv_timer_set_repeat_count(timer, 5);
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
static void scroll_creatorInfo_create()
{
    lv_obj_t* Info = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(Info, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_width(Info, 150);
    lv_label_set_text(Info, "Creator:Hanoch&小新, 三年2班\nLVGL version:8.3");
    lv_obj_align(Info, LV_ALIGN_BOTTOM_MID, 0, -HOR_STEP);
    //lv_async_call(screen_clean_up, Info);

}
static void weather_sys_create(void)
{
    LV_IMG_DECLARE(weather);
    lv_obj_t* weather_obj = lv_img_create(lv_scr_act());
    lv_img_set_src(weather_obj, &weather);
    lv_obj_align(weather_obj, LV_ALIGN_CENTER, 0, 0);
}
static void startup_timer(lv_timer_t* timer)
{
    printf("user_data = %d\n", timer->user_data);
    start_gif_create();
    scroll_creatorInfo_create();
}
static void screen_clean_up(void* scr)
{
    printf("cleanUP");
    lv_obj_del(scr);
    //lv_obj_del_async();
}

