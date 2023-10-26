#include "mylvgl.h"

#include "lvgl.h"
#include "lv_conf.h"

#define DMX_Controller_CH 64

static void slider_event_cb(lv_event_t *e);
static void btn_event_cb(lv_event_t *e);
static void all_btn_event_cb(lv_event_t *e);
static void BRI_change_cb(lv_event_t *e);

void SysTemp_refresh_cb(lv_timer_t *lv_timer);
void VBAT_refresh_cb(lv_timer_t *lv_timer);
void SysTime_refresh_cb(lv_timer_t *lv_timer);

lv_obj_t *tab;
lv_obj_t *DMX_Ctrl;
lv_obj_t *panel1;
lv_obj_t *panel2;

void create_all_ctrl(void);
void HOME_tab(lv_obj_t *parent);
/*创建DMX控台*/
void mylv_DMX_Controller()
{
    tab = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 40);
    lv_obj_clear_flag(lv_tabview_get_content(tab), LV_OBJ_FLAG_SCROLLABLE); // 禁止滚动切换

    lv_obj_t *home = lv_tabview_add_tab(tab, "HOME");
    HOME_tab(home);

    DMX_Ctrl = lv_tabview_add_tab(tab, "DMX Controller");

    lv_obj_t *RS485_Ctrl = lv_tabview_add_tab(tab, "RS485 Controller");

    panel1 = lv_obj_create(DMX_Ctrl);
    // lv_obj_set_height(panel1, LV_SIZE_CONTENT);
    // lv_obj_set_width(panel1, LV_SIZE_CONTENT);
    lv_obj_set_size(panel1, lv_pct(100), lv_pct(80));
    lv_obj_align(panel1, LV_ALIGN_TOP_MID, 0, 0);
    /*弹性布局*/
    lv_obj_set_layout(panel1, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(panel1, LV_FLEX_FLOW_ROW); // 子项成行且不换行

    panel2 = lv_obj_create(DMX_Ctrl);
    lv_obj_set_size(panel2, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_align(panel2, LV_ALIGN_BOTTOM_MID, 0, 0);
    /*弹性布局*/
    lv_obj_set_layout(panel2, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(panel2, LV_FLEX_FLOW_ROW); // 子项成行且不换行

    /*弹性布局*/
    lv_obj_set_layout(panel1, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(panel1, LV_FLEX_FLOW_ROW); // 子项成行且不换行

    create_all_ctrl();

    lv_obj_t *all_btn = lv_btn_create(panel2), *all_lab = lv_label_create(all_btn);
    lv_obj_set_size(all_btn, 100, 50);
    lv_obj_center(all_lab);
    lv_obj_add_event_cb(all_btn, all_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_label_set_text_static(all_lab, "ALL");

    for (uint8_t i = 0; i < 17; i++)
    {
        lv_obj_t *bottom = lv_btn_create(panel2), *label = lv_label_create(bottom);
        lv_obj_set_size(bottom, 100, 50);
        lv_obj_center(label);
        lv_obj_add_event_cb(bottom, btn_event_cb, LV_EVENT_CLICKED, (void *)i);
        lv_label_set_text_fmt(label, "%d~%d", i * 15 + 1, i * 15 + 15);
    }

    for (uint16_t i = 0; i < DMX_Controller_CH; i++)
    {
        lv_obj_t *cont = lv_obj_create(panel1);
        lv_obj_set_height(cont, 350);
        lv_obj_set_width(cont, 55);
        lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
        // lv_obj_set_style_opa(cont, 0, LV_PART_ANY);
        // lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
        // lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN); // 子项成列且不换列
        // lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

        lv_obj_t *channel_value = lv_label_create(cont);
        lv_obj_add_flag(channel_value, LV_OBJ_FLAG_IGNORE_LAYOUT);
        lv_obj_align(channel_value, LV_ALIGN_TOP_MID, 0, 0);
        lv_label_set_text_fmt(channel_value, "%d%%", 0);

        lv_obj_t *slider = lv_slider_create(cont);
        lv_obj_align(slider, LV_ALIGN_CENTER, 0, 0);
        lv_slider_set_range(slider, 0, 255);
        lv_slider_set_value(slider, 0, LV_ANIM_OFF);
        lv_obj_set_size(slider, 12, 250);
        lv_obj_set_scrollbar_mode(slider, LV_SCROLLBAR_MODE_OFF);
        lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

        lv_obj_t *channel_id = lv_label_create(cont);
        lv_obj_add_flag(channel_id, LV_OBJ_FLAG_IGNORE_LAYOUT);
        lv_obj_align(channel_id, LV_ALIGN_BOTTOM_MID, 0, 0);
        lv_label_set_text_fmt(channel_id, "%d", i + 1);
    }
}

/*创建总控台*/
void create_all_ctrl()
{
    lv_obj_t *cont = lv_obj_create(panel1);
    lv_obj_set_height(cont, 350);
    lv_obj_set_width(cont, 55);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *channel_value = lv_label_create(cont);
    lv_obj_add_flag(channel_value, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_align(channel_value, LV_ALIGN_TOP_MID, 0, 0);
    lv_label_set_text_fmt(channel_value, "%d%%", 0);

    lv_obj_t *slider = lv_slider_create(cont);
    lv_obj_align(slider, LV_ALIGN_CENTER, 0, 0);
    lv_slider_set_range(slider, 0, 255);
    lv_slider_set_value(slider, 0, LV_ANIM_OFF);
    lv_obj_set_size(slider, 12, 250);
    lv_obj_set_scrollbar_mode(slider, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t *channel_id = lv_label_create(cont);
    lv_obj_add_flag(channel_id, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_align(channel_id, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_label_set_text_static(channel_id, "ALL");
}

#include "DMX.h"
#include "stdlib.h"
static void slider_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    lv_label_set_text_fmt(lv_obj_get_child(lv_obj_get_parent(slider), 0), "%d%%", lv_slider_get_value(slider) * 100 / 255);
    uint16_t channel = atoi(lv_label_get_text(lv_obj_get_child(lv_obj_get_parent(slider), -1)));
    if (channel == 0)
    {
        for (uint16_t i = 0; i < DMX_Controller_CH; i++)
        {
            lv_obj_t *cont = lv_obj_get_child(lv_obj_get_parent(lv_obj_get_parent(slider)), i + 1);
            lv_label_set_text_fmt(lv_obj_get_child(cont, 0), "%d%%", lv_slider_get_value(slider) * 100 / 255);
            lv_slider_set_value(lv_obj_get_child(cont, 1), lv_slider_get_value(slider), LV_ANIM_OFF);
            DMX_DATA_CHANGE(i + 1, lv_slider_get_value(slider));
        }
    }
    else
        DMX_DATA_CHANGE(channel, lv_slider_get_value(slider));
}

static void btn_event_cb(lv_event_t *e)
{
    uint16_t botton_id = (uint16_t)lv_event_get_user_data(e);
    lv_obj_scroll_to_x(panel1, lv_obj_get_x(lv_obj_get_child(panel1, botton_id * 15 + 1)), LV_ANIM_ON); // 移动
}

static void all_btn_event_cb(lv_event_t *e)
{
    lv_obj_scroll_to_x(panel1, lv_obj_get_x(lv_obj_get_child(panel1, 0)), LV_ANIM_ON); // 移动
}

#include "SysTemp.h"
#include "RTC_SD3078.h"
/*创建HOME页*/
void HOME_tab(lv_obj_t *home_tab)
{
    /*温度显示容器*/
    lv_obj_t *temp_panel = lv_obj_create(home_tab);
    lv_obj_set_size(temp_panel, lv_pct(20), lv_pct(28));
    lv_obj_align(temp_panel, LV_ALIGN_TOP_LEFT, 0, 0);

    lv_obj_t *temp_label = lv_label_create(temp_panel);
    lv_obj_align(temp_label, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_label_set_text_static(temp_label, "System Temp: ");

    lv_obj_t *temp = lv_label_create(temp_panel);

    static lv_style_t temp_sytle;
    lv_style_init(&temp_sytle);
    lv_style_set_text_font(&temp_sytle, &lv_font_montserrat_30);
    lv_style_set_text_align(&temp_sytle, LV_TEXT_ALIGN_RIGHT);
    lv_style_set_text_color(&temp_sytle, lv_color_hex(0x0000FF));

    lv_obj_add_style(temp, &temp_sytle, LV_STATE_DEFAULT);
    // lv_obj_set_style_text_color(temp,lv_color_hex(0xFF0000),LV_STATE_DEFAULT);
    lv_obj_align_to(temp, temp_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_obj_set_size(temp, lv_pct(100), LV_SIZE_CONTENT);

    float tempv = SysTemp_GET();
    lv_label_set_text_fmt(temp, "%.2f °C   ", tempv);
    int temp_u8 = ((tempv + 0.5 + 40) / (125 - (-40)) * 0xFF);
    lv_obj_set_style_text_color(temp, lv_color_hex((uint8_t)temp_u8 << 16 | (255 - (uint8_t)temp_u8)), LV_STATE_DEFAULT);

    lv_timer_create(SysTemp_refresh_cb, 1000, temp);
    /*电池电压电量显示容器*/
    lv_obj_t *VBAT_panel = lv_obj_create(home_tab), *VBAT_label = lv_label_create(VBAT_panel), *VBAT = lv_label_create(VBAT_panel);
    lv_obj_set_size(VBAT_panel, lv_pct(20), lv_pct(32));
    lv_obj_align_to(VBAT_panel, temp_panel, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);

    lv_obj_align(VBAT_label, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_label_set_text_static(VBAT_label, "Battery Voltage: ");

    lv_obj_add_style(VBAT, &temp_sytle, LV_STATE_DEFAULT);
    lv_obj_align_to(VBAT, VBAT_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_label_set_text_static(VBAT, "3.70 V   \r100%   ");
    lv_obj_set_size(VBAT, lv_pct(100), LV_SIZE_CONTENT);

    lv_timer_create(VBAT_refresh_cb, 2000, VBAT);

    /*亮度滑条*/
    lv_obj_t *BRI_SLID = lv_slider_create(home_tab);
    lv_obj_set_size(BRI_SLID, lv_pct(7), lv_pct(80));
    lv_obj_align(BRI_SLID, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_slider_set_range(BRI_SLID, 18, 0xFF);
    lv_slider_set_value(BRI_SLID, 0x3F, LV_ANIM_OFF);
    static lv_style_t BRI_SLID_KNOB_STYLE, BRI_SLID_MAIN_STYLE;
    lv_style_init(&BRI_SLID_KNOB_STYLE);
    lv_style_init(&BRI_SLID_MAIN_STYLE);
    lv_style_set_bg_opa(&BRI_SLID_KNOB_STYLE, 0); // 隐藏滑块
    lv_obj_add_style(BRI_SLID, &BRI_SLID_KNOB_STYLE, LV_PART_KNOB);
    lv_style_set_radius(&BRI_SLID_MAIN_STYLE, 15); // 设置滑条主题圆角
    lv_obj_add_style(BRI_SLID, &BRI_SLID_MAIN_STYLE, LV_PART_MAIN);
    lv_obj_add_style(BRI_SLID, &BRI_SLID_MAIN_STYLE, LV_PART_INDICATOR);

    lv_obj_add_event_cb(BRI_SLID, BRI_change_cb, LV_EVENT_VALUE_CHANGED, NULL);

    /*系统时间*/
    lv_obj_t *SysTime = lv_label_create(home_tab);
    lv_obj_set_size(SysTime, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_text_font(SysTime, &lv_font_montserrat_30, LV_PART_MAIN);
    lv_obj_set_style_text_align(SysTime, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
    lv_obj_align(SysTime, LV_ALIGN_TOP_MID, 0, 0);
    SD3078_Read_Time(&SD3078_Time);
    lv_label_set_text_fmt(SysTime, "System Time   %02X:%02X:%02X ", SD3078_Time.hou, SD3078_Time.min, SD3078_Time.sec);

    lv_timer_create(SysTime_refresh_cb, 1000, SysTime);
}
#include "stdio.h"
#include "RTC_SD3078.h"
void SysTemp_refresh_cb(lv_timer_t *lv_timer)
{

    float tempv = SysTemp_GET();
    lv_label_set_text_fmt(lv_timer->user_data, "%.2f °C   ", tempv);
    int temp_u8 = ((tempv + 0.5 + 40) / (125 - (-40)) * 0xFF);

    lv_obj_set_style_text_color(lv_timer->user_data, lv_color_hex((uint8_t)temp_u8 << 16 | (255 - (uint8_t)temp_u8)), LV_STATE_DEFAULT);
}

void VBAT_refresh_cb(lv_timer_t *lv_timer)
{
    lv_label_set_text_fmt(lv_timer->user_data, "%0.2f V   \r%3d%%   ", VBAT_GET(), VBAT_GET_PCT());
}
#include "tim.h"
static void BRI_change_cb(lv_event_t *e)
{
    BL_change(lv_slider_get_value(e->target));
}

void SysTime_refresh_cb(lv_timer_t *lv_timer)
{
    SD3078_Read_Time(&SD3078_Time);
    lv_label_set_text_fmt(lv_timer->user_data, "System Time   %02X:%02X:%02X ", SD3078_Time.hou, SD3078_Time.min, SD3078_Time.sec);
}