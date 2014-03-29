#include <pebble.h>

static Window *window;
static TextLayer *symbol_layer;
static TextLayer *price_layer;
static char symbol[5];
static char price[10];
static bool wasFirstMsg;
static bool dataInited;
static int refreshKey;
static char *refreshSymbol;

enum {
  QUOTE_KEY_INIT = 0x0,
  QUOTE_KEY_FETCH = 0x1,
  QUOTE_KEY_SYMBOL = 0x2,
  QUOTE_KEY_PRICE = 0x03,
};

static bool send_to_phone_multi(int quote_key, char *symbol) {
  if ((quote_key == -1) && (symbol == NULL)) {
    return true;
  }
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  if (iter == NULL) {
    return false;
  }

  Tuplet tuple = (symbol == NULL)
                      ? TupletInteger(quote_key, 1)
                      : TupletCString(quote_key, symbol);
  dict_write_tuplet(iter, &tuple);
  dict_write_end(iter);

  app_message_outbox_send();
  return true;
}

static void send_to_phone(int quote_key) {
  bool queued = send_to_phone_multi(quote_key, NULL);
  if (!queued && (refreshKey == -1) && (refreshSymbol == NULL)) {
    refreshKey = quote_key;
  }
}

static void set_symbol_msg(char *symbolName) {
  bool queued = send_to_phone_multi(QUOTE_KEY_SYMBOL, symbolName);
  if (!queued) {
    refreshKey = QUOTE_KEY_SYMBOL;
    refreshSymbol = symbolName;
  }
}

static void in_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *init_tuple = dict_find(iter, QUOTE_KEY_INIT);
  Tuple *symbol_tuple = dict_find(iter, QUOTE_KEY_SYMBOL);
  Tuple *price_tuple = dict_find(iter, QUOTE_KEY_PRICE);

  if (init_tuple) {
    // only accept one initial tuple; the second may be a server reply to
    // an out-of-date action on our part
    if (dataInited) {
      return;
    } else {
      dataInited = true;
    }
  }

  if (symbol_tuple) {
    strncpy(symbol, symbol_tuple->value->cstring, 5);
    text_layer_set_text(symbol_layer, symbol);
  }

  if (price_tuple) {
    strncpy(price, price_tuple->value->cstring, 10);
    text_layer_set_text(price_layer, price);
  }
}

static void in_dropped_handler(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Dropped!");
}

static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
  if (wasFirstMsg && dataInited) {
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Failed to Send!");
  }
  wasFirstMsg = false;
}

static void app_message_init(void) {
  // Register message handlers
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_register_outbox_failed(out_failed_handler);
  // Init buffers
  app_message_open(64, 64);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(symbol_layer, "On/Off");
  send_to_phone(QUOTE_KEY_FETCH);
}

static void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(symbol_layer, symbol);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(symbol_layer, "Up");
  send_to_phone(QUOTE_KEY_SYMBOL);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(symbol_layer, "Down");
  send_to_phone(QUOTE_KEY_PRICE);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_long_click_subscribe(BUTTON_ID_SELECT, 0, select_long_click_handler, NULL);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  symbol_layer = text_layer_create((GRect) { .origin = { 0, 20 }, .size = { bounds.size.w, 50 } });
  // text_layer_set_text(symbol_layer, "PBL");
  text_layer_set_text(symbol_layer, "TV");
  text_layer_set_text_alignment(symbol_layer, GTextAlignmentCenter);
  text_layer_set_font(symbol_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(symbol_layer));

  price_layer = text_layer_create((GRect) { .origin = { 0, 75 }, .size = { bounds.size.w, 50 } });
  // text_layer_set_text(price_layer, "$0000.00");
  text_layer_set_text(price_layer, "000000");
  text_layer_set_text_alignment(price_layer, GTextAlignmentCenter);
  text_layer_set_font(price_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  layer_add_child(window_layer, text_layer_get_layer(price_layer));

  send_to_phone(QUOTE_KEY_INIT);
  wasFirstMsg = true;
}

static void window_unload(Window *window) {
  text_layer_destroy(symbol_layer);
  text_layer_destroy(price_layer);
}

static void init(void) {
  wasFirstMsg = false;
  dataInited = false;
  refreshKey = -1;
  refreshSymbol = NULL;

  app_message_init();

  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
