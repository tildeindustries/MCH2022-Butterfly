#include <esp_log.h>
#include "main.h"
#include "rp2040.h"

static pax_buf_t buf;
xQueueHandle buttonQueue;

static const char *TAG = "Tilde addon";

// Updates the screen with the latest buffer.
void disp_flush() {
    ili9341_write(get_ili9341(), buf.buf);
}

// Exits the app, returning to the launcher.
void exit_to_launcher() {
    REG_WRITE(RTC_CNTL_STORE0_REG, 0);
    esp_restart();
}

void sao_demo() {
    RP2040* rp2040 = get_rp2040();
    
    for (uint8_t j = 0; j < 127; j++) {
        for (uint8_t i = 0; i < 5; i++) {
            rp2040_set_ws2812_data(rp2040, i, j << 16);
        }
        rp2040_ws2812_trigger(rp2040);
        vTaskDelay(pdMS_TO_TICKS(5));
    }
    for (uint8_t j = 127; j > 0; j--) {
        for (uint8_t i = 0; i < 5; i++) {
            rp2040_set_ws2812_data(rp2040, i, j << 16);
        }
        rp2040_ws2812_trigger(rp2040);
        vTaskDelay(pdMS_TO_TICKS(5));
    }
    for (uint8_t j = 0; j < 127; j++) {
        for (uint8_t i = 0; i < 5; i++) {
            rp2040_set_ws2812_data(rp2040, i, j << 24);
        }
        rp2040_ws2812_trigger(rp2040);
        vTaskDelay(pdMS_TO_TICKS(5));
    }
    for (uint8_t j = 127; j > 0; j--) {
        for (uint8_t i = 0; i < 5; i++) {
            rp2040_set_ws2812_data(rp2040, i, j << 24);
        }
        rp2040_ws2812_trigger(rp2040);
        vTaskDelay(pdMS_TO_TICKS(5));
    }
    for (uint8_t j = 0; j < 127; j++) {
        for (uint8_t i = 0; i < 5; i++) {
            rp2040_set_ws2812_data(rp2040, i, j << 8);
        }
        rp2040_ws2812_trigger(rp2040);
        vTaskDelay(pdMS_TO_TICKS(5));
    }
    for (uint8_t j = 127; j > 0; j--) {
        for (uint8_t i = 0; i < 5; i++) {
            rp2040_set_ws2812_data(rp2040, i, j << 8);
        }
        rp2040_ws2812_trigger(rp2040);
        vTaskDelay(pdMS_TO_TICKS(5));
    }
    for (uint8_t i = 0; i < 5; i++) {
        for (uint8_t j = 0; j < 127; j++) {
            rp2040_set_ws2812_data(rp2040, i, j << 24);
            rp2040_ws2812_trigger(rp2040);
            vTaskDelay(pdMS_TO_TICKS(5));
        }
    }
    for (uint8_t i = 0; i < 5; i++) {
        for (uint8_t j = 127; j > 0; j--) {
            rp2040_set_ws2812_data(rp2040, i, j << 24);
            rp2040_ws2812_trigger(rp2040);
            vTaskDelay(pdMS_TO_TICKS(5));
        }
    }
    for (uint8_t i = 0; i < 5; i++) {
        for (uint8_t j = 0; j < 127; j++) {
            rp2040_set_ws2812_data(rp2040, i, j << 16);
            rp2040_ws2812_trigger(rp2040);
            vTaskDelay(pdMS_TO_TICKS(5));
        }
    }
    for (uint8_t i = 0; i < 5; i++) {
        for (uint8_t j = 127; j > 0; j--) {
            rp2040_set_ws2812_data(rp2040, i, j << 16);
            rp2040_ws2812_trigger(rp2040);
            vTaskDelay(pdMS_TO_TICKS(5));
        }
    }
    for (uint8_t i = 0; i < 5; i++) {
        for (uint8_t j = 0; j < 127; j++) {
            rp2040_set_ws2812_data(rp2040, i, j << 8);
            rp2040_ws2812_trigger(rp2040);
            vTaskDelay(pdMS_TO_TICKS(5));
        }
    }
    for (uint8_t i = 0; i < 5; i++) {
        for (uint8_t j = 127; j > 0; j--) {
            rp2040_set_ws2812_data(rp2040, i, j << 8);
            rp2040_ws2812_trigger(rp2040);
            vTaskDelay(pdMS_TO_TICKS(5));
        }
    }
}

void app_main() {
  
    
    ESP_LOGI(TAG, "Welcome to the template app!");

    // Initialize the screen, the I2C and the SPI busses.
    bsp_init();

    // Initialize the RP2040 (responsible for buttons, etc).
    bsp_rp2040_init();
    
    // This queue is used to receive button presses.
    buttonQueue = get_rp2040()->queue;
    
    // Initialize graphics for the screen.
    pax_buf_init(&buf, NULL, 320, 240, PAX_BUF_16_565RGB);
    
    // Initialize NVS.
    nvs_flash_init();
    
    // Initialize WiFi. This doesn't connect to Wifi yet.
    wifi_init();
    
    // Tilde SAO init
    RP2040* rp2040 = get_rp2040();
    rp2040_set_gpio_dir(rp2040, 0, true); // Set SAO GPIO 1 to output
    rp2040_set_ws2812_mode(rp2040, 1); // Set WS2812 mode to enabled, 24-bit (RGB) mode
    rp2040_set_ws2812_length(rp2040, 5); // 5 leds
    
    while (1) {
        // Pick a random background color.
        int hue = esp_random() & 255;
        pax_col_t col = pax_col_hsv(hue, 255 /*saturation*/, 255 /*brighness*/);
        
        // Greet the World in front of a random background color! 
        // Fill the background with the random color.
        pax_background(&buf, col);
        
        // This text is shown on screen.
        char             *text = "[~]";
        
        // Pick the font (Saira is the only one that looks nice in this size).
        const pax_font_t *font = pax_font_saira_condensed;

        // Determine how the text dimensions so we can display it centered on
        // screen.
        pax_vec1_t        dims = pax_text_size(font, font->default_size, text);

        // Draw the centered text.
        pax_draw_text(
            &buf, // Buffer to draw to.
            0xFFFFFFFF, // color
            font, font->default_size, // Font and size to use.
            // Position (top left corner) of the app.
            (buf.width  - dims.x) / 2.0,
            (buf.height - dims.y) / 2.0,
            // The text to be rendered.
            text
        );

        // Draws the entire graphics buffer to the screen.
        disp_flush();
        
        sao_demo(col);
        
        // Wait for button presses and do another cycle.
        
        // Structure used to receive data.
        rp2040_input_message_t message;
        
        // Wait forever for a button press (because of portMAX_DELAY)
        xQueueReceive(buttonQueue, &message, portMAX_DELAY);
        
        // Which button is currently pressed?
        if (message.input == RP2040_INPUT_BUTTON_HOME && message.state) {
            // If home is pressed, exit to launcher.
            exit_to_launcher();
        }
    }
}
