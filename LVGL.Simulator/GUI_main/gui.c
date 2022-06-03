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
static lv_obj_t* start_anim_obj;
static lv_obj_t* menu_obj;
static uint32_t track_id;
lv_timer_t* timer;
lv_obj_t* backup;
/**********************
*      MACROS
**********************/

/**********************
*   GLOBAL FUNCTIONS
**********************/

void gui_start(void)
{
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x343247), 0);

    start_anim_obj = lv_scr_act();
    menu_obj = lv_scr_act();
    timer = lv_timer_create(auto_step_cb, 1000, NULL);
}
/**********************
*   STATIC FUNCTIONS
**********************/
static lv_obj_t* start_anim_create(lv_obj_t* parent)
{
    LV_IMG_DECLARE(xiaoxin);

    printf("start anim create\n");
    lv_obj_t* cont= lv_obj_create(parent);
    //TODO 设置上下滑动
    //lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);


    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t* gif_xiaoxin = lv_gif_create(cont);
    lv_gif_set_src(gif_xiaoxin, &xiaoxin);
    lv_obj_align(gif_xiaoxin, LV_ALIGN_CENTER, 0, -HOR_STEP);

    lv_obj_t* scroll_Info = lv_label_create(cont);
    lv_label_set_long_mode(scroll_Info, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_width(scroll_Info, 150);
    lv_label_set_text(scroll_Info, "黄妙之&小新, 三年2班\nLVGL version:8.3");
    lv_obj_align(scroll_Info, LV_ALIGN_BOTTOM_MID, 0, -HOR_STEP);
    
    return cont;
}
static lv_obj_t* menu_create(lv_obj_t* parent)
{
    LV_IMG_DECLARE(weather);
    printf("menu create\n");

    lv_obj_t* cont = lv_obj_create(parent);
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    //创建weather bar
    lv_obj_t* weather_obj = lv_gif_create(cont);
    lv_obj_set_height(weather_obj, LV_SIZE_CONTENT);
    lv_obj_set_align(weather_obj, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_gif_set_src(weather_obj, &weather);
    lv_obj_align(weather_obj, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t* weather_btn = lv_btn_create(cont);
    //lv_obj_add_event_cb(weather_btn, weather_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_remove_style_all(weather_btn);
    lv_obj_align(weather_btn, LV_ALIGN_BOTTOM_MID, 0, -VER_STEP);
    lv_obj_t* weather_label = lv_label_create(weather_btn);
    lv_label_set_text(weather_label, "weather");
    lv_obj_center(weather_label);

    //TODO创建media播放器
    
    //TODO创建setting设置
    //lv_obj_t* setting_obj = lv_obj_create(cont);
    //lv_obj_set_height(setting_obj, LV_SIZE_CONTENT);
    //lv_obj_set_flex_flow(setting_obj, LV_FLEX_FLOW_COLUMN);
    //lv_obj_set_align(setting_obj, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    //lv_obj_align(setting_obj, LV_ALIGN_CENTER, 0, 0);

    //TODO创建idea时刻

    //TODO创建photo个人照片
    return cont;
}
/**
 * @brief data 传进帧数据
 * @return 
*/
static lv_obj_t* weather_windows_create()
{
    lv_obj_t* cont;
    lv_obj_t* weather_condition;
    lv_obj_t* location;
    lv_obj_t* temperature;
    lv_obj_t* current_date;
    lv_obj_t* astronaut_img;
    lv_obj_t* air_quality;

    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_set_grid_dsc_array(cont, grid_cols, grid_rows);
    lv_obj_set_style_grid_row_align(cont, LV_GRID_ALIGN_SPACE_BETWEEN, 0);
    lv_obj_set_grid_cell(spectrum_obj, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_set_grid_cell(title_box, LV_GRID_ALIGN_STRETCH, 0, 1, LV_ALIGN_CENTER, 2, 1);
    lv_obj_set_grid_cell(icon_box, LV_GRID_ALIGN_STRETCH, 0, 1, LV_ALIGN_CENTER, 4, 1);
    lv_obj_set_grid_cell(ctrl_box, LV_GRID_ALIGN_STRETCH, 0, 1, LV_ALIGN_CENTER, 6, 1);
    lv_obj_set_grid_cell(handle_box, LV_GRID_ALIGN_STRETCH, 0, 1, LV_ALIGN_CENTER, 8, 1);
}
static void auto_step_cb(lv_timer_t* timer)
{
    static uint32_t state = 0;
    printf("state = %d\n", state);

    switch (state)
    {
        case 0:
        {
            backup = start_anim_create(start_anim_obj);
        }
        break;
        case 5:
        {
            printf("删除gif\n");
            lv_obj_t* child = lv_obj_get_child(start_anim_obj, 0);
            lv_obj_del(child);
        }
        case 10:
        {
            menu_create(start_anim_obj);
        }
        break;
        case 12:
        {

        }
        default :
        break;
    }
    state++;
}
static void screen_clean_up(void* scr)
{
    printf("");
    printf("clean up the screen\n");
    lv_obj_clean(scr);
    //menu_create();
}
