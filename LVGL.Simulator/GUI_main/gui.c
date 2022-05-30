/**
* @file gui.c
*
*/

/*********************
*      INCLUDES
*********************/
#include "gui.h"
#include "stdio.h"
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
static lv_obj_t* start_anim_create(lv_obj_t* parent);
static lv_obj_t* menu_create(lv_obj_t* parent);
static void auto_step_cb(lv_timer_t* timer);
static void screen_clean_up(void* scr);
/**********************
*  STATIC VARIABLES
**********************/
lv_obj_t* start_ui;
/**********************
*      MACROS
**********************/

/**********************
*   GLOBAL FUNCTIONS
**********************/

void gui_start(void)
{
    lv_obj_t* backup = start_anim_create(lv_scr_act());
    lv_timer_t* timer = lv_timer_create(auto_step_cb, 1000, NULL);
    lv_timer_set_repeat_count(timer, 5);
}
/**********************
*   STATIC FUNCTIONS
**********************/
static lv_obj_t* start_anim_create(lv_obj_t* parent)
{
    LV_IMG_DECLARE(xiaoxin);

    lv_obj_t* cont= lv_obj_create(parent);
    lv_obj_set_height(cont, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t* gif_xiaoxin = lv_gif_create(cont);
    lv_gif_set_src(gif_xiaoxin, &xiaoxin);
    lv_obj_align(gif_xiaoxin, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t* scroll_Info = lv_label_create(cont);
    lv_label_set_long_mode(scroll_Info, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_width(scroll_Info, 180);
    lv_label_set_text(scroll_Info, "Hanoch&小新, 三年2班\nLVGL version:8.3");
    lv_obj_align(scroll_Info, LV_ALIGN_BOTTOM_MID, 0, -HOR_STEP);
    
    return cont;
}
static lv_obj_t* menu_create(lv_obj_t* parent)
{
    LV_IMG_DECLARE(sunny);
    printf("menu create\n");
    lv_obj_t* weather_obj = lv_img_create(lv_scr_act());
    lv_img_set_src(weather_obj, &sunny);
    lv_obj_align(weather_obj, LV_ALIGN_CENTER, 0, 0);
}
static void auto_step_cb(lv_timer_t* timer)
{
    static uint32_t state = 0;
    printf("state = %d\n", state);

    switch (state)
    {
        case 5:
        {

        }
        break;
        case 6:
        {

        }
        break;
        default :
        break;
    }
}
static void screen_clean_up(void* scr)
{
    printf("clean up the screen\n");
    lv_obj_clean(scr);
    //menu_create();
}
