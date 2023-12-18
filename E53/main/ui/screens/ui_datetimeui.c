// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.3
// LVGL version: 8.3.6
// Project name: timergui

#include "../ui.h"

void ui_datetimeui_screen_init(void)
{
ui_datetimeui = lv_obj_create(NULL);
lv_obj_clear_flag( ui_datetimeui, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_datetimeui, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_datetimeui, lv_color_hex(0xE6E6E6), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_datetimeui, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_datetimeui, &ui_font_misans16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_datetimeheader = lv_obj_create(ui_datetimeui);
lv_obj_set_width( ui_datetimeheader, lv_pct(100));
lv_obj_set_height( ui_datetimeheader, lv_pct(20));
lv_obj_set_align( ui_datetimeheader, LV_ALIGN_TOP_MID );
lv_obj_set_flex_flow(ui_datetimeheader,LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(ui_datetimeheader, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
lv_obj_clear_flag( ui_datetimeheader, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_datetimeheader, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_datetimeheader, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_datetimeheader, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_datetimeheader, lv_color_hex(0xB4B4B4), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_datetimeheader, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_datetimeheader, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_side(ui_datetimeheader, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_datetimeheader, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_datetimeheader, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_datetimeheader, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_datetimeheader, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_color(ui_datetimeheader, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_datetimeheader, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_datetimeheader, &ui_font_misans16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_datetimetitle = lv_label_create(ui_datetimeheader);
lv_obj_set_width( ui_datetimetitle, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_datetimetitle, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_datetimetitle, 0 );
lv_obj_set_y( ui_datetimetitle, -2 );
lv_obj_set_align( ui_datetimetitle, LV_ALIGN_BOTTOM_MID );
lv_label_set_text(ui_datetimetitle,"日期和时间");

ui_datetimebody = lv_obj_create(ui_datetimeui);
lv_obj_set_width( ui_datetimebody, lv_pct(100));
lv_obj_set_height( ui_datetimebody, lv_pct(80));
lv_obj_set_x( ui_datetimebody, lv_pct(0) );
lv_obj_set_y( ui_datetimebody, lv_pct(20) );
lv_obj_set_align( ui_datetimebody, LV_ALIGN_TOP_MID );
lv_obj_set_flex_flow(ui_datetimebody,LV_FLEX_FLOW_COLUMN);
lv_obj_set_flex_align(ui_datetimebody, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
lv_obj_set_scrollbar_mode(ui_datetimebody, LV_SCROLLBAR_MODE_OFF);
lv_obj_set_style_radius(ui_datetimebody, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_datetimebody, lv_color_hex(0xE6E6E6), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_datetimebody, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_datetimebody, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_datetimebody, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_datetimebody, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_datetimebody, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_datetimebody, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_row(ui_datetimebody, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_column(ui_datetimebody, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_datetimepanel = lv_obj_create(ui_datetimebody);
lv_obj_set_width( ui_datetimepanel, lv_pct(100));
lv_obj_set_height( ui_datetimepanel, lv_pct(20));
lv_obj_set_align( ui_datetimepanel, LV_ALIGN_TOP_MID );
lv_obj_clear_flag( ui_datetimepanel, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_datetimepanel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_datetimepanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_datetimepanel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_datetimepanel, lv_color_hex(0xB4B4B4), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_datetimepanel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_datetimepanel, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_side(ui_datetimepanel, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_autodatetimelabel1 = lv_label_create(ui_datetimepanel);
lv_obj_set_width( ui_autodatetimelabel1, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_autodatetimelabel1, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_autodatetimelabel1, LV_ALIGN_LEFT_MID );
lv_label_set_text(ui_autodatetimelabel1,"24小时制");

ui_autodatetimeswitch1 = lv_switch_create(ui_datetimepanel);
lv_obj_set_width( ui_autodatetimeswitch1, 45);
lv_obj_set_height( ui_autodatetimeswitch1, 24);
lv_obj_set_align( ui_autodatetimeswitch1, LV_ALIGN_RIGHT_MID );
lv_obj_add_state( ui_autodatetimeswitch1, LV_STATE_CHECKED );     /// States

lv_obj_set_style_bg_color(ui_autodatetimeswitch1, lv_color_hex(0xAAAAAA), LV_PART_INDICATOR | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_autodatetimeswitch1, 255, LV_PART_INDICATOR| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_autodatetimeswitch1, lv_color_hex(0x00D200), LV_PART_INDICATOR | LV_STATE_CHECKED );
lv_obj_set_style_bg_opa(ui_autodatetimeswitch1, 255, LV_PART_INDICATOR| LV_STATE_CHECKED);

ui_datetimepanel1 = lv_obj_create(ui_datetimebody);
lv_obj_set_width( ui_datetimepanel1, lv_pct(100));
lv_obj_set_height( ui_datetimepanel1, lv_pct(20));
lv_obj_set_align( ui_datetimepanel1, LV_ALIGN_TOP_MID );
lv_obj_clear_flag( ui_datetimepanel1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_datetimepanel1, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_datetimepanel1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_datetimepanel1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_datetimepanel1, lv_color_hex(0xB4B4B4), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_datetimepanel1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_datetimepanel1, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_side(ui_datetimepanel1, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_autodatetimelabel = lv_label_create(ui_datetimepanel1);
lv_obj_set_width( ui_autodatetimelabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_autodatetimelabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_autodatetimelabel, LV_ALIGN_LEFT_MID );
lv_label_set_text(ui_autodatetimelabel,"自动设置时间");

ui_autodatetimeswitch = lv_switch_create(ui_datetimepanel1);
lv_obj_set_width( ui_autodatetimeswitch, 45);
lv_obj_set_height( ui_autodatetimeswitch, 24);
lv_obj_set_align( ui_autodatetimeswitch, LV_ALIGN_RIGHT_MID );
lv_obj_add_state( ui_autodatetimeswitch, LV_STATE_CHECKED );     /// States

lv_obj_set_style_bg_color(ui_autodatetimeswitch, lv_color_hex(0xAAAAAA), LV_PART_INDICATOR | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_autodatetimeswitch, 255, LV_PART_INDICATOR| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_autodatetimeswitch, lv_color_hex(0x00D200), LV_PART_INDICATOR | LV_STATE_CHECKED );
lv_obj_set_style_bg_opa(ui_autodatetimeswitch, 255, LV_PART_INDICATOR| LV_STATE_CHECKED);

ui_setdate = lv_obj_create(ui_datetimebody);
lv_obj_set_width( ui_setdate, lv_pct(100));
lv_obj_set_height( ui_setdate, lv_pct(20));
lv_obj_set_align( ui_setdate, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_setdate, LV_OBJ_FLAG_HIDDEN );   /// Flags
lv_obj_clear_flag( ui_setdate, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_setdate, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_setdate, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_setdate, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_setdate, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_side(ui_setdate, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_date = lv_label_create(ui_setdate);
lv_obj_set_width( ui_date, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_date, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_date, LV_ALIGN_LEFT_MID );
lv_label_set_text(ui_date,"日期  2023年3月21日");

ui_dateright = lv_label_create(ui_setdate);
lv_obj_set_width( ui_dateright, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_dateright, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_dateright, LV_ALIGN_RIGHT_MID );
lv_label_set_text(ui_dateright,"");
lv_obj_set_style_text_color(ui_dateright, lv_color_hex(0x787878), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_dateright, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_dateright, &ui_font_symbols16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_settime = lv_obj_create(ui_datetimebody);
lv_obj_set_width( ui_settime, lv_pct(100));
lv_obj_set_height( ui_settime, lv_pct(20));
lv_obj_set_align( ui_settime, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_settime, LV_OBJ_FLAG_HIDDEN );   /// Flags
lv_obj_clear_flag( ui_settime, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_settime, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_settime, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_settime, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_settime, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_side(ui_settime, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_time1 = lv_label_create(ui_settime);
lv_obj_set_width( ui_time1, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_time1, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_time1, LV_ALIGN_LEFT_MID );
lv_label_set_text(ui_time1,"时间  12:12");

ui_timeright = lv_label_create(ui_settime);
lv_obj_set_width( ui_timeright, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_timeright, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_timeright, LV_ALIGN_RIGHT_MID );
lv_label_set_text(ui_timeright,"");
lv_obj_set_style_text_color(ui_timeright, lv_color_hex(0x787878), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_timeright, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_timeright, &ui_font_symbols16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_datetimecountry = lv_obj_create(ui_datetimebody);
lv_obj_set_width( ui_datetimecountry, lv_pct(100));
lv_obj_set_height( ui_datetimecountry, lv_pct(20));
lv_obj_set_align( ui_datetimecountry, LV_ALIGN_TOP_MID );
lv_obj_clear_flag( ui_datetimecountry, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_datetimecountry, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_datetimecountry, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_datetimecountry, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_datetimecountry, lv_color_hex(0xB4B4B4), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_datetimecountry, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_datetimecountry, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_side(ui_datetimecountry, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_datetimecountrylabel = lv_label_create(ui_datetimecountry);
lv_obj_set_width( ui_datetimecountrylabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_datetimecountrylabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_datetimecountrylabel, LV_ALIGN_LEFT_MID );
lv_label_set_text(ui_datetimecountrylabel,"时区");

ui_datetimelabel1 = lv_label_create(ui_datetimecountry);
lv_obj_set_width( ui_datetimelabel1, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_datetimelabel1, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_datetimelabel1, LV_ALIGN_RIGHT_MID );
lv_label_set_text(ui_datetimelabel1,"北京");
lv_obj_set_style_text_color(ui_datetimelabel1, lv_color_hex(0x787878), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_datetimelabel1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_calendar = lv_calendar_create(ui_datetimebody);
lv_calendar_set_today_date(ui_calendar,2023,1,1);
lv_calendar_set_showed_date(ui_calendar,2023,1);
lv_obj_t *ui_calendar_header = lv_calendar_header_arrow_create(ui_calendar);
lv_obj_set_width( ui_calendar, lv_pct(100));
lv_obj_set_height( ui_calendar, lv_pct(125));
lv_obj_set_align( ui_calendar, LV_ALIGN_CENTER );
lv_obj_set_style_radius(ui_calendar, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_add_event_cb(ui_autodatetimeswitch, ui_event_autodatetimeswitch, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_setdate, ui_event_setdate, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_calendar, ui_event_calendar, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_datetimeui, ui_event_datetimeui, LV_EVENT_ALL, NULL);

}