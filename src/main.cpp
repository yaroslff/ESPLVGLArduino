#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include "ui.h"
//#include <XPT2046_Touchscreen.h>
#include <FS.h>
#include <SPI.h>
#include <..\demos\lv_demos.h>

extern lv_img_dsc_t katya;
extern lv_img_dsc_t serdechko;
hw_timer_t *timer1 = NULL;
void lv_example_colorwheel_1(void);
extern void ui_event_Keyboard1(lv_event_t * e);
extern lv_obj_t * ui_Keyboard1;

uint16_t cccliked = 0;
//Tasks for lvgl prototype
void tasklvinc(void *pvParameters);
void tasklvtimerhendler(void *pvParameters);
////////////---------------------------------////////////////////

/*Change to your screen resolution*/
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * 10 ];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char * buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif

void IRAM_ATTR onTimer(){
    lv_tick_inc(10);
    //vTaskDelay(15);
    

}

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp );
}

/*Read the touchpad*/
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    uint16_t touchX, touchY;

    bool touched = tft.getTouch( &touchX, &touchY, 600 );

    if( !touched )
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = 320 - touchY;
        data->point.y = touchX;

        //Serial.print( "Data x " );
        //Serial.println( touchX );

        //Serial.print( "Data y " );
        //Serial.println( touchY );
    }
}
void backligt(void *pvParameters);

void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */
    timer1 = timerBegin(1, 80, true);
    timerAttachInterrupt(timer1, onTimer, true);
    timerAlarmWrite(timer1, 10000, true);
    timerAlarmEnable(timer1);
    



    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.println( LVGL_Arduino );
    Serial.println( "I am LVGL_Arduino" );

    lv_init();

    ledcSetup(0,5000, 8);
    ledcAttachPin(33,0);
    ledcWrite(0,255);

    



    tft.begin();          /* TFT init */
    tft.setRotation( 3 ); 
    
    /* Landscape orientation, flipped */

    /*Set the touchscreen calibration data,
     the actual data for your display can be acquired using
     the Generic -> Touch_calibrate example from the TFT_eSPI library*/
    uint16_t calData[5] = {303, 3376, 1406, 2422, 4};
    tft.setTouch( calData );

    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register( &indev_drv );

    
    


    Serial.println( "Setup done" );
    ui_init();
    xTaskCreateUniversal(tasklvinc, "tasklvinc", 2048, NULL, 5, NULL,1); 
    xTaskCreateUniversal(backligt, "backligt", 2048, NULL, 5, NULL,1); 
    
}

void loop()
{
    lv_timer_handler();
    vTaskDelay(15);
    

    //lv_tick_inc(1); /* let the GUI do its work */
    
}

void lv_example_colorwheel_1(void)
{
    lv_obj_t * cw;

    cw = lv_colorwheel_create(lv_scr_act(), true);
    lv_obj_set_size(cw, 200, 200);
    lv_obj_center(cw);
}

void backligt(void *pvParameters){
    int ledpwm = 100;
    char text[5];
    while (1)
    {
        
        
        

        vTaskDelay(1000);
        
    }
    
}

void tasklvinc(void *pvParameters){
    while(1){
        //lv_tick_inc(15);
        vTaskDelay(500);
        //lv_img_set_src(ui_Image1, &katya);
        //vTaskDelay(3500);
       // Serial.println(cccliked);
        //lv_img_set_src(ui_Image1, &serdechko);

    }
}

void ui_event_Keyboard1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_SHORT_CLICKED) {
       // keyboardcall(e);
       cccliked++;
       
       Serial.println("cccliked");
       Serial.println(lv_keyboard_get_selected_btn(ui_Keyboard1));
       if(39 == lv_keyboard_get_selected_btn(ui_Keyboard1));
       Serial.println(lv_textarea_get_text(ui_TextArea1));
       ///

    }
    

}    
    