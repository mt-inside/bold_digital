#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define NOT_USED(x) (void)(x)

#define MY_UUID { 0xED, 0x70, 0x06, 0x28, 0x49, 0xBF, 0x4B, 0x7F, 0x80, 0x5E, 0x6C, 0x88, 0x22, 0xC1, 0xC5, 0xA4 }
PBL_APP_INFO(MY_UUID,
             "Bold Digital", "Matt Turner",
             0, 1, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

static Window window;
#define COLOR_BACK GColorBlack
#define COLOR_FORE GColorWhite

/* Bold Digital's business logic */
static void redraw( PblTm* t )
{
  fuzzy_time_to_words(t->tm_hour, t->tm_min, s_data.buffer, BUFFER_SIZE);
  text_layer_set_text(&s_data.label, s_data.buffer);
}

static void handle_init( AppContextRef ctx )
{
    NOT_USED(ctx);

    window_init( &window, "Bold Digital Watchface" );
    window_stack_push( &window, true /* Animated */ );
    window_set_background_color( &window, COLOR_BACK );


    text_layer_init( &layer_text_hour, window.layer.frame );
    text_layer_set_text_color( &layer_text_hour, COLOR_FORE );
    text_layer_set_background_color( &layer_text_hour, COLOR_BACK );
    layer_set_frame( &layer_text_hour.layer, GRect( 8, 68, 144-8, 168-68 ) );
    text_layer_set_font( &layer_text_hour, fonts_get_system_font( FONT_KEY_GOTHAM_42_BOLD ) );
    layer_add_child( &window.layer, &layer_text_hour.layer );

    text_layer_init( &layer_text_min, window.layer.frame );
    text_layer_set_text_color( &layer_text_min, COLOR_FORE );
    text_layer_set_background_color( &layer_text_min, COLOR_BACK );
    layer_set_frame( &layer_text_min.layer, GRect( 8, 68, 144-8, 168-68 ) );
    text_layer_set_font( &layer_text_min, fonts_get_system_font( FONT_KEY_GOTHAM_42_BOLD ) );
    layer_add_child( &window.layer, &layer_text_min.layer );


    /* Initial draw */
    PblTm t;
    get_time( &t );
    redraw( &t );
}

static void handle_minute_tick( AppContextRef app_ctx,
                                PebbleTickEvent* e )
{
    redraw( e->tick_time );
}

static PebbleAppHandlers handlers =
{
    .init_handler = &handle_init
    .tick_info =
    {
        .tick_handler = &handle_minute_tick,
        .tick_units = MINUTE_UNIT
    }
};

void pbl_main( void *params )
{
    app_event_loop( params, &handlers );
}
