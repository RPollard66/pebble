#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer, *s_date_layer;
static GFont s_time_font, s_date_font;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static void update_time() {
  
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);
  
  text_layer_set_text(s_time_layer, s_buffer);
  
  // date stuff
  static char date_buffer[16];
  strftime(date_buffer, sizeof(date_buffer), "%a %b %d", tick_time);
  text_layer_set_text(s_date_layer, date_buffer);
  
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  // Create GBitmap
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SOUTHPARK);

  // Create BitmapLayer to display the GBitmap
  s_background_layer = bitmap_layer_create(bounds);

  // Set the bitmap onto the layer and add to the window
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));

  // Time layer
  s_time_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  
  
  // Date layer
  // placement: (x, y, width, height) origin at top left
  // take size of font into consideration
  s_date_layer = text_layer_create(GRect(0, 140, 144, 30));
  text_layer_set_text_color(s_date_layer, GColorBlack);
  text_layer_set_background_color(s_date_layer, GColorClear);
    
  
  // Time font
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SOUTHPARK_30));
  
  //Date font
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SOUTHPARK_24));


  // Apply fonts to TextLayers
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_font(s_date_layer, s_date_font);
  
  // Time alignment
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);
  
  // Date alignment
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  
  // Add time layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
  // Add date layer
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
  
  
}


static void main_window_unload(Window *window) {

  // Destroy time layer & font
  text_layer_destroy(s_time_layer);
  fonts_unload_custom_font(s_time_font);
  
  // destroy date stuff
  text_layer_destroy(s_date_layer);
  fonts_unload_custom_font(s_date_font);
  
  // Destroy GBitmap stuff
  gbitmap_destroy(s_background_bitmap);
  bitmap_layer_destroy(s_background_layer);
  
}


static void init() {
  
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Make sure the time is displayed from the start
  update_time();
  
  window_set_background_color(s_main_window, GColorBlack);
  
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}


