// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.3
// LVGL version: 8.3.6
// Project name: timergui

#include "../ui.h"

void ui_i2cui_screen_init(void)
{
ui_i2cui = lv_obj_create(NULL);
lv_obj_clear_flag( ui_i2cui, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_i2cheader = lv_obj_create(ui_i2cui);
lv_obj_set_width( ui_i2cheader, lv_pct(100));
lv_obj_set_height( ui_i2cheader, lv_pct(20));
lv_obj_set_align( ui_i2cheader, LV_ALIGN_TOP_MID );
lv_obj_set_flex_flow(ui_i2cheader,LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(ui_i2cheader, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
lv_obj_clear_flag( ui_i2cheader, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_i2cheader, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_i2cheader, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_i2cheader, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_i2cheader, lv_color_hex(0xB4B4B4), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_i2cheader, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_i2cheader, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_side(ui_i2cheader, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_i2cheader, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_i2cheader, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_i2cheader, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_i2cheader, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_color(ui_i2cheader, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_i2cheader, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_i2cheader, &ui_font_misans16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_i2ctitle = lv_label_create(ui_i2cheader);
lv_obj_set_width( ui_i2ctitle, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_i2ctitle, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_i2ctitle, 30 );
lv_obj_set_y( ui_i2ctitle, -2 );
lv_obj_set_align( ui_i2ctitle, LV_ALIGN_BOTTOM_LEFT );
lv_label_set_text(ui_i2ctitle,"I2C实验");

ui_i2cmasterpanel = lv_obj_create(ui_i2cui);
lv_obj_set_width( ui_i2cmasterpanel, lv_pct(104));
lv_obj_set_height( ui_i2cmasterpanel, lv_pct(20));
lv_obj_set_x( ui_i2cmasterpanel, lv_pct(0) );
lv_obj_set_y( ui_i2cmasterpanel, lv_pct(20) );
lv_obj_set_align( ui_i2cmasterpanel, LV_ALIGN_TOP_MID );
lv_obj_set_flex_flow(ui_i2cmasterpanel,LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(ui_i2cmasterpanel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
lv_obj_clear_flag( ui_i2cmasterpanel, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_i2cmasterlabel = lv_label_create(ui_i2cmasterpanel);
lv_obj_set_width( ui_i2cmasterlabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_i2cmasterlabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_i2cmasterlabel, -87 );
lv_obj_set_y( ui_i2cmasterlabel, -33 );
lv_obj_set_align( ui_i2cmasterlabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_i2cmasterlabel,"TX");

ui_i2cmastertext = lv_textarea_create(ui_i2cmasterpanel);
lv_obj_set_width( ui_i2cmastertext, lv_pct(93));
lv_obj_set_height( ui_i2cmastertext, lv_pct(210));
lv_obj_set_align( ui_i2cmastertext, LV_ALIGN_CENTER );
lv_textarea_set_placeholder_text(ui_i2cmastertext,"Placeholder...");



ui_i2csenorpanel = lv_obj_create(ui_i2cui);
lv_obj_set_width( ui_i2csenorpanel, lv_pct(104));
lv_obj_set_height( ui_i2csenorpanel, lv_pct(20));
lv_obj_set_x( ui_i2csenorpanel, lv_pct(0) );
lv_obj_set_y( ui_i2csenorpanel, lv_pct(40) );
lv_obj_set_align( ui_i2csenorpanel, LV_ALIGN_TOP_MID );
lv_obj_set_flex_flow(ui_i2csenorpanel,LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(ui_i2csenorpanel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
lv_obj_clear_flag( ui_i2csenorpanel, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_i2csenolabel = lv_label_create(ui_i2csenorpanel);
lv_obj_set_width( ui_i2csenolabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_i2csenolabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_i2csenolabel, -87 );
lv_obj_set_y( ui_i2csenolabel, -33 );
lv_obj_set_align( ui_i2csenolabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_i2csenolabel,"RX");

ui_i2csenortext = lv_textarea_create(ui_i2csenorpanel);
lv_obj_set_width( ui_i2csenortext, lv_pct(93));
lv_obj_set_height( ui_i2csenortext, lv_pct(210));
lv_obj_set_align( ui_i2csenortext, LV_ALIGN_CENTER );
lv_textarea_set_placeholder_text(ui_i2csenortext,"Placeholder...");



ui_i2ckeyboard = lv_keyboard_create(ui_i2cui);
lv_obj_set_width( ui_i2ckeyboard, lv_pct(100));
lv_obj_set_height( ui_i2ckeyboard, lv_pct(41));
lv_obj_set_x( ui_i2ckeyboard, lv_pct(0) );
lv_obj_set_y( ui_i2ckeyboard, lv_pct(60) );
lv_obj_set_align( ui_i2ckeyboard, LV_ALIGN_TOP_MID );

lv_obj_add_event_cb(ui_i2cmastertext, ui_event_i2cmastertext, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_i2csenortext, ui_event_i2csenortext, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_i2ckeyboard, ui_event_i2ckeyboard, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_i2cui, ui_event_i2cui, LV_EVENT_ALL, NULL);

}