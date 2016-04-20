#include <pebble.h>
// Meh App
 
static Window *s_main_window;

static TextLayer *s_title_layer;
static TextLayer *s_cost_layer;


static AppSync s_sync;
static uint8_t s_sync_buffer[1024];

enum MehKey {
  
  MEH_TITLE_KEY = 0x0,  // TUPLE_CSTRING
  MEH_COST_KEY = 0x1,   // TUPLE_CSTRING
  
};


static void appmsg_in_dropped(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "In dropped: %i", reason);
}


static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
  
}



static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
  switch (key) {
    
    case MEH_TITLE_KEY:
      // App Sync keeps new_tuple in s_sync_buffer, so we may use it directly
      text_layer_set_text(s_title_layer, new_tuple->value->cstring);
      break;

    case MEH_COST_KEY:
      text_layer_set_text(s_cost_layer, new_tuple->value->cstring);
      break;
    
     
  }
}

static void request_meh(void) {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  if (!iter) {
    // Error creating outbound message
    return;
  }

  int value = 1;
  dict_write_int(iter, 1, &value, sizeof(int), true);
  dict_write_end(iter);

  app_message_outbox_send();
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  

  s_title_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));
  text_layer_set_text_color(s_title_layer, GColorBlack);
  text_layer_set_background_color(s_title_layer, GColorClear);
  text_layer_set_font(s_title_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_title_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_title_layer));

  s_cost_layer = text_layer_create(GRect(0, 120, bounds.size.w, bounds.size.h));
  text_layer_set_text_color(s_cost_layer, GColorBlack);
  text_layer_set_background_color(s_cost_layer, GColorClear);
  text_layer_set_font(s_cost_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_cost_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_cost_layer));
  
  

  Tuplet initial_values[] = {
   
    TupletCString(MEH_TITLE_KEY, "Today's Deal"),
    TupletCString(MEH_COST_KEY, "$0.00"),
    
  };

  app_sync_init(&s_sync, s_sync_buffer, sizeof(s_sync_buffer), 
      initial_values, ARRAY_LENGTH(initial_values),
      sync_tuple_changed_callback, sync_error_callback, NULL
  );

  request_meh();
}

static void window_unload(Window *window) {
  
  text_layer_destroy(s_cost_layer);
  text_layer_destroy(s_title_layer);
  
  
}

static void init(void) {
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorLavenderIndigo);
  //window_set_fullscreen(s_main_window, true);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  window_stack_push(s_main_window, true);

  app_message_register_inbox_dropped(appmsg_in_dropped);
  
  app_message_open(64, 64);
  
}

static void deinit(void) {
  window_destroy(s_main_window);

  app_sync_deinit(&s_sync);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}