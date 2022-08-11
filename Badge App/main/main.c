#include <esp_log.h>
#include "main.h"
#include "rp2040.h"

#define Colour1 0x00058240
#define Colour2 0x0087C159
#define Colour3 0x00ADCACB
#define Colour4 0x00FEE3A2
#define Colour5 0x00F3C301
#define White   0x00FFFFFF

static pax_buf_t buf;
xQueueHandle buttonQueue;

static const char *TAG = "Tilde addon";

const static uint8_t GammaTable[] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

void delay_(unsigned long time_delay)
{
    vTaskDelay(pdMS_TO_TICKS(time_delay));
}

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
    
    void DisplayCorrectedHexColour(int Position, long unsigned int Colour, long unsigned int brightness)
    {
        long unsigned int RemappedColour = 0;
        RemappedColour = RemappedColour + (GammaTable[(((Colour & 0x00FF0000) >> 16) * brightness / 255)] << 16); // Red
        RemappedColour = RemappedColour + (GammaTable[(((Colour & 0x0000FF00) >> 8) * brightness / 255)] << 24); // Green
        RemappedColour = RemappedColour + (GammaTable[((Colour & 0x000000FF) * brightness / 255)] << 8); // Blue
        rp2040_set_ws2812_data(rp2040, Position, RemappedColour);
        rp2040_ws2812_trigger(rp2040);
    }

    for (int i = 0; i < 180; i++)
    {
        for (int j = 0; j < NUMPIXELS; j++) {
            DisplayCorrectedHexColour(j, White, i);
        }
        delay_(3 + i / 16);
    }
    delay_(500);
    for (int i = 180; i < 255; i++)
    {
        for (int j = 0; j < NUMPIXELS; j++) {
            DisplayCorrectedHexColour(j, White, i);
        }
        delay_(2);
    }
    for (int i = 255; i > 0; i--)
    {
        for (int j = 0; j < NUMPIXELS; j++) {
            DisplayCorrectedHexColour(j, White, i);
        }
        delay_(2);
    }
    delay_(500);

    for (int i = 0; i < 255; i++)
    {
        DisplayCorrectedHexColour(0, Colour1, i);
        DisplayCorrectedHexColour(1, Colour2, i);
        DisplayCorrectedHexColour(2, Colour3, i);
        DisplayCorrectedHexColour(3, Colour4, i);
        DisplayCorrectedHexColour(4, Colour5, i);
        delay_(3);
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
