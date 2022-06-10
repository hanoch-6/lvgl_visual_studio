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

#include <time.h>
/*********************
*      DEFINES
*********************/
#define HOR_STEP 40
#define VER_STEP 40

#define MAX_WEATHER_CODE 38；
LV_IMG_DECLARE(img_sunny);
LV_IMG_DECLARE(img_clear);
LV_IMG_DECLARE(img_fair_day);
LV_IMG_DECLARE(img_fair_night);
LV_IMG_DECLARE(img_cloudy);
LV_IMG_DECLARE(img_partly_cloudy_day);
LV_IMG_DECLARE(img_partly_cloudy_night);
LV_IMG_DECLARE(img_mostly_cloudy_day);
LV_IMG_DECLARE(img_mostly_cloudy_night);
LV_IMG_DECLARE(img_overcast);
LV_IMG_DECLARE(img_shower);
LV_IMG_DECLARE(img_thundershower);
LV_IMG_DECLARE(img_thundershower_with_hail);
LV_IMG_DECLARE(img_light_rain);
LV_IMG_DECLARE(img_moderate_rain);
LV_IMG_DECLARE(img_heavy_rain);
LV_IMG_DECLARE(img_storm);
LV_IMG_DECLARE(img_heavy_storm);
LV_IMG_DECLARE(img_severe_storm);
LV_IMG_DECLARE(img_ice_rain);
LV_IMG_DECLARE(img_sleet);
LV_IMG_DECLARE(img_snow_flurry);
LV_IMG_DECLARE(img_light_snow);
LV_IMG_DECLARE(img_moderate_snow);
LV_IMG_DECLARE(img_heavy_snow);
LV_IMG_DECLARE(img_snowstorm);
LV_IMG_DECLARE(img_dust);
LV_IMG_DECLARE(img_sand);
LV_IMG_DECLARE(img_duststorm);
LV_IMG_DECLARE(img_sandstorm);
LV_IMG_DECLARE(img_foggy);
LV_IMG_DECLARE(img_haze);
LV_IMG_DECLARE(img_windy);
LV_IMG_DECLARE(img_blustery);
LV_IMG_DECLARE(img_hurricance);
LV_IMG_DECLARE(img_tropical_storm);
LV_IMG_DECLARE(img_tornado);
LV_IMG_DECLARE(img_cold);
LV_IMG_DECLARE(img_hot);
LV_IMG_DECLARE(img_unknown);
/**********************
*      TYPEDEFS
**********************/
typedef enum btn_menu_t
{
    WEATHER_BTN,
}btn_menu;

static void* weather_table[] = {
    &img_sunny,                   // 0 晴（国内城市白天晴）
    &img_clear,                   // 1 晴（国内城市夜晚晴）
    &img_fair_day,                // 2 晴（国外城市白天晴）
    &img_fair_night,              // 3 晴（国外城市夜晚晴）
    &img_cloudy,                  // 4 多云
    &img_partly_cloudy_day,       // 5 晴间多云
    &img_partly_cloudy_night,     // 6 晴间多云
    &img_mostly_cloudy_day,       // 7 大部多云
    &img_mostly_cloudy_night,     // 8 大部多云
    &img_overcast,                // 9 阴
    &img_shower,                  // 10 阵雨
    &img_thundershower,           // 11 雷阵雨
    &img_thundershower_with_hail, // 12 雷阵雨伴有冰雹
    &img_light_rain,              // 13 小雨
    &img_moderate_rain,           // 14 中雨
    &img_heavy_rain,              // 15 大雨
    &img_storm,                   // 16 暴雨
    &img_heavy_storm,             // 17 大暴雨
    &img_severe_storm,            // 18 特大暴雨
    &img_ice_rain,                // 19 冻雨
    &img_sleet,                   // 20 雨夹雪
    &img_snow_flurry,             // 21 阵雪
    &img_light_snow,              // 22 小雪
    &img_moderate_snow,           // 23 中雪
    &img_heavy_snow,              // 24 大雪
    &img_snowstorm,               // 25 暴雪
    &img_dust,                    // 26 浮尘
    &img_sand,                    // 27 扬沙
    &img_duststorm,               // 28 沙尘暴
    &img_sandstorm,               // 29 强沙尘暴
    &img_foggy,                   // 30 雾
    &img_haze,                    // 31 霾
    &img_windy,                   // 32 风
    &img_blustery,                // 33 大风
    &img_hurricance,              // 34 飓风
    &img_tropical_storm,          // 35 热带风暴
    &img_tornado,                 // 36 龙卷风
    &img_cold,                    // 37 冷
    &img_hot,                     // 38 热
    &img_unknown,                 // 99 N/A
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

static lv_font_t* font_normal;
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
    font_normal = &lv_font_montserrat_14;
    lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK, font_normal);
    //lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x343247), 0);
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
    static struct tm last_date;
    time_t now_time;
    struct tm *now_date;

    static uint8_t toggle;

    time(&now_time);

    now_date = localtime(&now_time);

    if (now_date->tm_hour != last_date.tm_hour)
    {
        lv_label_set_text_fmt(label_hour, "%2d", now_date->tm_hour);
    }

    if (now_date->tm_min != last_date.tm_min)
    {
        if (now_date->tm_min != last_date.tm_min)
        {
            lv_label_set_text_fmt(label_min, "%02d", now_date->tm_min);
        }
    }
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
    lv_obj_t* weather_condition = lv_gif_create(cont);
    lv_obj_set_size(weather_condition, 120, 120);
    lv_obj_set_grid_cell(weather_condition, LV_GRID_ALIGN_START, 1, 1,
                                            LV_GRID_ALIGN_START, 0, 1);
    //lv_obj_t* weather_condition_gif = lv_gif_create(weather_condition);
    lv_img_set_src(weather_condition, weather_table[20]);

    //TODO 时间参数传入。
    //clock_create(cont);
    lv_obj_t* current_time = lv_obj_create(cont);
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
