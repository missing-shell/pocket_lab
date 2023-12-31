// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.3
// LVGL version: 8.3.6
// Project name: timergui

#include "../ui.h"

void ui_isrui_screen_init(void)
{
ui_isrui = lv_obj_create(NULL);
lv_obj_clear_flag( ui_isrui, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_isrheader = lv_obj_create(ui_isrui);
lv_obj_set_width( ui_isrheader, lv_pct(100));
lv_obj_set_height( ui_isrheader, lv_pct(20));
lv_obj_set_align( ui_isrheader, LV_ALIGN_TOP_MID );
lv_obj_set_flex_flow(ui_isrheader,LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(ui_isrheader, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
lv_obj_clear_flag( ui_isrheader, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_isrheader, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_isrheader, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_isrheader, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_isrheader, lv_color_hex(0xB4B4B4), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_isrheader, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_isrheader, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_side(ui_isrheader, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_isrheader, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_isrheader, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_isrheader, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_isrheader, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_color(ui_isrheader, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_isrheader, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_isrheader, &ui_font_misans16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_isrtitle = lv_label_create(ui_isrheader);
lv_obj_set_width( ui_isrtitle, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_isrtitle, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_isrtitle, 30 );
lv_obj_set_y( ui_isrtitle, -2 );
lv_obj_set_align( ui_isrtitle, LV_ALIGN_BOTTOM_LEFT );
lv_label_set_text(ui_isrtitle,"中断实验");

ui_isrpanel = lv_obj_create(ui_isrui);
lv_obj_set_width( ui_isrpanel, lv_pct(100));
lv_obj_set_height( ui_isrpanel, lv_pct(80));
lv_obj_set_x( ui_isrpanel, lv_pct(0) );
lv_obj_set_y( ui_isrpanel, lv_pct(20) );
lv_obj_set_align( ui_isrpanel, LV_ALIGN_TOP_MID );
lv_obj_clear_flag( ui_isrpanel, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_isrpanel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_isrpanel, lv_color_hex(0xE6E6E6), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_isrpanel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_isrpanel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_isrbtn1 = lv_btn_create(ui_isrpanel);
lv_obj_set_width( ui_isrbtn1, lv_pct(30));
lv_obj_set_height( ui_isrbtn1, lv_pct(30));
lv_obj_set_flex_flow(ui_isrbtn1,LV_FLEX_FLOW_COLUMN_WRAP);
lv_obj_set_flex_align(ui_isrbtn1, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
lv_obj_add_flag( ui_isrbtn1, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_isrbtn1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_isrlabel1 = lv_label_create(ui_isrbtn1);
lv_obj_set_width( ui_isrlabel1, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_isrlabel1, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_isrlabel1, 154 );
lv_obj_set_y( ui_isrlabel1, 55 );
lv_obj_set_align( ui_isrlabel1, LV_ALIGN_CENTER );
lv_label_set_text(ui_isrlabel1,"下降沿");

ui_isrbtn2 = lv_btn_create(ui_isrpanel);
lv_obj_set_width( ui_isrbtn2, lv_pct(30));
lv_obj_set_height( ui_isrbtn2, lv_pct(30));
lv_obj_set_align( ui_isrbtn2, LV_ALIGN_BOTTOM_LEFT );
lv_obj_set_flex_flow(ui_isrbtn2,LV_FLEX_FLOW_COLUMN_WRAP);
lv_obj_set_flex_align(ui_isrbtn2, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
lv_obj_add_flag( ui_isrbtn2, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_isrbtn2, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_isrlabel2 = lv_label_create(ui_isrbtn2);
lv_obj_set_width( ui_isrlabel2, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_isrlabel2, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_isrlabel2, 154 );
lv_obj_set_y( ui_isrlabel2, 55 );
lv_obj_set_align( ui_isrlabel2, LV_ALIGN_CENTER );
lv_label_set_text(ui_isrlabel2,"任意沿");

lv_obj_add_event_cb(ui_isrbtn1, ui_event_isrbtn1, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_isrbtn2, ui_event_isrbtn2, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_isrui, ui_event_isrui, LV_EVENT_ALL, NULL);

}
