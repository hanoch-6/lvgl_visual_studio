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
typedef enum btn_menu_t
{
    WEATHER_BTN,
}btn_menu;

typedef struct _lv_clock
{
    lv_obj_t* time_label; // 时间标签
    lv_obj_t* date_label; // 日期标签
    lv_obj_t* weekday_label; // 星期标签
}lv_clock_t;

struct tm
{
    int tm_sec;   // seconds after the minute - [0, 60] including leap second
    int tm_min;   // minutes after the hour - [0, 59]
    int tm_hour;  // hours since midnight - [0, 23]
    int tm_mday;  // day of the month - [1, 31]
    int tm_mon;   // months since January - [0, 11]
    int tm_year;  // years since 1900
    int tm_wday;  // days since Sunday - [0, 6]
    int tm_yday;  // days since January 1 - [0, 365]
    int tm_isdst; // daylight savings time flag
};

/**********************
*  STATIC PROTOTYPES
**********************/
static lv_obj_t* start_anim_create(lv_obj_t* parent);
static lv_obj_t* menu_create(lv_obj_t* parent);
static lv_obj_t* weather_windows_create(lv_obj_t* parent);
static void auto_step_cb(lv_timer_t* timer);
static void screen_clean_up(void* scr);
static void menu_btn_event_handler(lv_event_t* e);
static void clock_create(lv_obj_t* parent);
/**********************
*  STATIC VARIABLES
**********************/
static lv_obj_t* start_anim_obj;
static lv_obj_t* menu_obj;
static uint32_t track_id;
static lv_obj_t* label_hour;
static lv_obj_t* label_min;
static lv_obj_t* label_sec;
static lv_obj_t* label_dot;
lv_timer_t* Autoplay_timer;
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
    Autoplay_timer = lv_timer_create(auto_step_cb, 1000, NULL);
}
/**********************
*   STATIC FUNCTIONS
**********************/

static void menu_btn_event_handler(lv_event_t* e)
{
    uint8_t event_code = lv_event_get_code(e);
    uint16_t* user_data = lv_event_get_user_data(e);
    //printf("event code = %d\n" ,event_code);
    //printf("event_para = %d\n" ,user_data);

    switch (event_code)
    {
    case LV_EVENT_RELEASED:
    {
        if (user_data == 0)
        {
            lv_obj_del(backup);
            weather_windows_create(menu_obj);
        }
    }
    break;
    }
}

static lv_obj_t* start_anim_create(lv_obj_t* parent)
{
    LV_IMG_DECLARE(xiaoxin);
    printf("start anim create\n");
    lv_obj_t* cont= lv_obj_create(parent);
    //TODO 设置上下滑动
    //lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);


    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_align(cont, LV_FLEX_ALIGN_CENTER);

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
    lv_obj_set_align(cont, LV_FLEX_ALIGN_CENTER);

    //创建weather bar
    lv_obj_t* weather_obj = lv_gif_create(cont);
    lv_obj_set_height(weather_obj, LV_SIZE_CONTENT);

    lv_obj_set_flex_flow(weather_obj, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_align(weather_obj, LV_FLEX_ALIGN_START);

    lv_obj_set_align(weather_obj, LV_FLEX_ALIGN_CENTER);

    lv_obj_set_align(weather_obj, LV_FLEX_ALIGN_CENTER);
    lv_gif_set_src(weather_obj, &weather);
    lv_obj_align(weather_obj, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t* weather_btn = lv_btn_create(cont);
    lv_obj_add_event_cb(weather_btn, menu_btn_event_handler, LV_EVENT_RELEASED, WEATHER_BTN);
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

static void clock_create(lv_obj_t* parent)
{
    time_t now_time;
    struct tm* now_date;

    time(&now_time);
    now_date = localtime(&now_time);

    label_hour = lv_label_create(parent);
    lv_obj_set_style_text_font(label_hour, &lv_font_montserrat_48, 0);
    lv_obj_align(label_hour, LV_ALIGN_CENTER, -70, 0);
    lv_label_set_text_fmt(label_hour, "%2d", now_date->tm_hour);

    label_dot = lv_label_create(parent);
    lv_obj_set_style_text_font(label_dot, &lv_font_montserrat_48, 0);
    lv_obj_align(label_dot, LV_ALIGN_CENTER, -25, -3);
    lv_label_set_text(label_dot, ":");

    label_min = lv_label_create(parent);
    lv_obj_set_style_text_font(label_min, &lv_font_montserrat_48, 0);
    lv_obj_align(label_min, LV_ALIGN_CENTER, 20, 0);
    lv_label_set_text_fmt(label_min, "%02d", now_date->tm_min);

    label_sec = lv_label_create(parent);
    lv_obj_set_style_text_font(label_sec, &lv_font_montserrat_24, 0);
    lv_obj_align(label_sec, LV_ALIGN_CENTER, 75, 8);
    lv_label_set_text_fmt(label_sec, "%02d", now_date->tm_sec);
}

/**
 * @brief data 传进帧数据
 * @return 
*/
static lv_obj_t* weather_windows_create(lv_obj_t* parent)
{
    lv_obj_t* cont = lv_obj_create(parent);
    lv_obj_set_style_bg_color(cont, lv_color_hex(0x00BFFF), 0);

    lv_obj_t* astronaut_img = lv_obj_create(cont);
    lv_obj_t* air_quality = lv_obj_create(cont);

    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES);
    lv_obj_center(cont);
    static const lv_coord_t grid_cols[] = { 80,80,80, LV_GRID_TEMPLATE_LAST };
    static lv_coord_t grid_rows[] = { 80,80,80,LV_GRID_TEMPLATE_LAST };

    for (uint8_t i = 0; i < lv_obj_get_child_cnt(cont); i++)
    {
        lv_obj_t* child = lv_obj_get_child(cont, i);
        lv_obj_remove_style_all(child);
    }
    lv_obj_set_grid_dsc_array(cont, grid_cols, grid_rows);
    lv_obj_set_style_grid_row_align(cont, LV_GRID_ALIGN_SPACE_BETWEEN, 0);

    //TODO 城市信息传入，字体大小更改
    lv_obj_t* location = lv_label_create(cont);
    lv_obj_set_size(location, 80, 80);
    lv_obj_set_grid_cell(location, LV_GRID_ALIGN_STRETCH, 0, 1,
                                   LV_GRID_ALIGN_STRETCH, 0, 1);
    //lv_obj_t* label_city = lv_label_create(location);
    lv_label_set_text(location, "重庆");
    

    //TODO 天气数据传入。选择rain,sunny,big_sunny,clouds
    //TODO 修改天气图片大小，如果有需要可以创建一个容器，但是容器可能回留有缝隙，不美观
    LV_IMG_DECLARE(rain);
    lv_obj_t* weather_condition = lv_gif_create(cont);
    lv_obj_set_size(weather_condition, 120, 120);
    lv_obj_set_grid_cell(weather_condition, LV_GRID_ALIGN_START, 1, 1,
                                            LV_GRID_ALIGN_START, 0, 1);
    //lv_obj_t* weather_condition_gif = lv_gif_create(weather_condition);
    lv_gif_set_src(weather_condition, &rain);

    //TODO 时间参数传入。
    clock_create(cont);
    lv_obj_set_size(current_time, 80, 80);
    lv_obj_set_grid_cell(current_time, LV_GRID_ALIGN_STRETCH, 1, 2,
                                       LV_GRID_ALIGN_STRETCH, 1, 1);
    //TODO 创建时钟
    lv_label_set_text(current_time, "23:15");


    //lv_obj_t* label = lv_label_create(weather_condition);
    //lv_obj_t* temperature = lv_label_create(cont);
    //lv_label_set_long_mode(temperature, LV_LABEL_LONG_WRAP);
    //lv_label_set_recolor(temperature, true);
    //lv_label_set_text(temperature, "25度");
    //lv_obj_set_grid_cell(temperature, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, 4, 1);
    //lv_obj_align(temperature, LV_ALIGN_CENTER, 0, -40);


    
    //lv_obj_set_grid_cell(current_date, LV_GRID_ALIGN_STRETCH, 0, 1, LV_ALIGN_CENTER, 6, 1);
    //lv_obj_set_grid_cell(air_quality, LV_GRID_ALIGN_STRETCH, 0, 1, LV_ALIGN_CENTER, 8, 1);
    return cont;
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
            backup = menu_create(menu_obj);
        }
        case 10:
        {

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
    lv_obj_del_delayed(scr,500);
    //menu_create();
}
