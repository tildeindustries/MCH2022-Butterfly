#include <esp_log.h>
#include "main.h"
#include "rp2040.h"

#include <driver/gpio.h>
#include <driver/i2c.h>
#include <sdkconfig.h>


#define White	0x00FFFFFF







	char testu[] = { '2', '3', '4' };	







#define WRITE_BIT     I2C_MASTER_WRITE /* I2C master write */
#define READ_BIT      I2C_MASTER_READ  /* I2C master read */
#define ACK_CHECK_EN  0x1              /* I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0              /* I2C master will not check ack from slave */
#define ACK_VAL       0x0              /* I2C ack value */
#define NACK_VAL      0x1              /* I2C nack value */

uint8_t addressbytelenght = 1;
char testuturu = '1';

long unsigned int Butterfly_ID = 0x00000000;
long unsigned int Colour1 = 0x00000000;
long unsigned int Colour2 = 0x00000000;
long unsigned int Colour3 = 0x00000000;
long unsigned int Colour4 = 0x00000000;
long unsigned int Colour5 = 0x00000000;


esp_err_t eeprom_init(EEPROM* device) { return ESP_OK; }

void test_eeprom_read(uint8_t addr, uint16_t address, uint8_t* data, size_t length) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    esp_err_t        res = i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, address >> 8, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, address & 0xFF, ACK_CHECK_EN);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (addr << 1) | READ_BIT, ACK_CHECK_EN);
    if (length > 1) {
        i2c_master_read(cmd, data, length - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, &data[length - 1], NACK_VAL);
    i2c_master_stop(cmd);
    i2c_cmd_link_delete(cmd);
}

esp_err_t _eeprom_read(int bus, uint8_t addr, uint16_t address, uint8_t* data, size_t length) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    esp_err_t        res = i2c_master_start(cmd);
    if (res != ESP_OK) {
        i2c_cmd_link_delete(cmd);
        return res;
    }
    res = i2c_master_write_byte(cmd, (addr << 1) | WRITE_BIT, ACK_CHECK_EN);
    if (res != ESP_OK) {
        i2c_cmd_link_delete(cmd);
        return res;
    }
   /* res = i2c_master_write_byte(cmd, address >> 8, ACK_CHECK_EN);
    if (res != ESP_OK) {
        i2c_cmd_link_delete(cmd);
        return res;
    }*/
    res = i2c_master_write_byte(cmd, address & 0xFF, ACK_CHECK_EN);
    if (res != ESP_OK) {
        i2c_cmd_link_delete(cmd);
        return res;
    }
    res = i2c_master_start(cmd);
    if (res != ESP_OK) {
        i2c_cmd_link_delete(cmd);
        return res;
    }
    res = i2c_master_write_byte(cmd, (addr << 1) | READ_BIT, ACK_CHECK_EN);
    if (res != ESP_OK) {
        i2c_cmd_link_delete(cmd);
        return res;
    }
    if (length > 1) {
        res = i2c_master_read(cmd, data, length - 1, ACK_VAL);
        if (res != ESP_OK) {
            i2c_cmd_link_delete(cmd);
            return res;
        }
    }
    res = i2c_master_read_byte(cmd, &data[length - 1], NACK_VAL);
    if (res != ESP_OK) {
        i2c_cmd_link_delete(cmd);
        return res;
    }
    res = i2c_master_stop(cmd);
    if (res != ESP_OK) {
        i2c_cmd_link_delete(cmd);
        return res;
    }

    res = i2c_master_cmd_begin(bus, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return res;
}

esp_err_t _eeprom_write(int bus, uint8_t addr, uint16_t address, uint8_t* data, size_t length) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    esp_err_t        res = i2c_master_start(cmd);
    if (res != ESP_OK) {
        i2c_cmd_link_delete(cmd);
        return res;
    }
    res = i2c_master_write_byte(cmd, (addr << 1) | WRITE_BIT, ACK_CHECK_EN);
    if (res != ESP_OK) {
        i2c_cmd_link_delete(cmd);
        return res;
    }
    res = i2c_master_write_byte(cmd, address >> 8, ACK_CHECK_EN);
    if (res != ESP_OK) {
        i2c_cmd_link_delete(cmd);
        return res;
    }
    res = i2c_master_write_byte(cmd, address & 0xFF, ACK_CHECK_EN);
    if (res != ESP_OK) {
        i2c_cmd_link_delete(cmd);
        return res;
    }
    for (size_t i = 0; i < length; i++) {
        res = i2c_master_write_byte(cmd, data[i], ACK_CHECK_EN);
        if (res != ESP_OK) {
            i2c_cmd_link_delete(cmd);
            return res;
        }
    }
    res = i2c_master_stop(cmd);
    if (res != ESP_OK) {
        i2c_cmd_link_delete(cmd);
        return res;
    }

    res = i2c_master_cmd_begin(bus, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return res;
}

esp_err_t eeprom_read(EEPROM* device, uint8_t address, uint8_t* data, size_t length) {
    uint8_t position = 0;
    while (length - position > 0) {
        uint8_t transaction_length = length - position;
        if (transaction_length > 64) transaction_length = 64;
        esp_err_t res = _eeprom_read(device->i2c_bus, device->i2c_address, address + position, &data[position], transaction_length);
        if (res != ESP_OK) {
            //ESP_LOGE(TAG, "EEPROM read failed (%d)", res);
            return res;
        }
        position += transaction_length;
    }
    return ESP_OK;
}

esp_err_t eeprom_write(EEPROM* device, uint16_t address, uint8_t* data, size_t length) {
    uint16_t position = 0;
    while (length - position > 0) {
        uint8_t transaction_length = length - position;
        if (transaction_length > 64) transaction_length = 64;
        esp_err_t res = _eeprom_write(device->i2c_bus, device->i2c_address, address + position, &data[position], transaction_length);
        if (res != ESP_OK) {
            //ESP_LOGE(TAG, "EEPROM write failed (%d)", res);
            return res;
        }
        position += transaction_length;
    }
    return ESP_OK;
}













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
/*
    pixels.setPixelColor(Position, pixels.Color(
        GammaTable[(((Colour & 0x00FF0000) >> 16) * brightness / 255)],
        GammaTable[((Colour & 0x0000FF00) >> 8) * brightness / 255],
        GammaTable[(Colour & 0x000000FF) * brightness / 255]));*/
}

    for (int i = 0; i < 180; i++)
    {
        for (int j = 0; j < 5; j++) {
            DisplayCorrectedHexColour(j, White, i);
        }
        delay_(3 + i / 16);
    }
    delay_(500);
    for (int i = 180; i < 255; i++)
    {
        for (int j = 0; j < 5; j++) {
            DisplayCorrectedHexColour(j, White, i);
        }
        delay_(2);
    }
    for (int i = 255; i > 0; i--)
    {
        for (int j = 0; j < 5; j++) {
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
        

    


EEPROM sao_eeprom = {.i2c_bus = 0, .i2c_address = 0x50};
    uint8_t header[8];
    eeprom_read(&sao_eeprom, 0, header, sizeof(header));
    
    //uint8_t header[8];
//test_eeprom_read(0x50, 0, header, sizeof(header));
//if (header[4] == 0x0C) testu[0] = 'A';
//if (header[5] == 0x0F) testu[1] = 'B';
//if (header[6] == 0x12) testu[2] = 'C';

uint8_t device_name [header[4]];
uint8_t buffer_data [header[6]];
bool device_name_check = 1;

    eeprom_read(&sao_eeprom, 8 , device_name, header[4]);
if (device_name[0] != 'M') device_name_check = 0;
if (device_name[1] != 'C') device_name_check = 0;
if (device_name[2] != 'H') device_name_check = 0;
if (device_name[3] != 'b') device_name_check = 0;
if (device_name[4] != 'u') device_name_check = 0;
if (device_name[5] != 't') device_name_check = 0;
if (device_name[6] != 't') device_name_check = 0;
if (device_name[7] != 'e') device_name_check = 0;
if (device_name[8] != 'r') device_name_check = 0;
if (device_name[9] != 'f') device_name_check = 0;
if (device_name[10] != 'l') device_name_check = 0;
if (device_name[11] != 'y') device_name_check = 0;

 Colour1 = 0x00000000;
 Colour2 = 0x00000000;
 Colour3 = 0x00000000;
 Colour4 = 0x00000000;
 Colour5 = 0x00000000;
Butterfly_ID = 0x00000000;

    eeprom_read(&sao_eeprom, 8 + header[4] + header[5], buffer_data, header[6]);

Butterfly_ID = Butterfly_ID + (buffer_data[0] << 16);
Butterfly_ID = Butterfly_ID + (buffer_data[1] << 8);
Butterfly_ID = Butterfly_ID + (buffer_data[2]);

Colour1 = Colour1 + (buffer_data[3] << 16);
Colour1 = Colour1 + (buffer_data[4] << 8);
Colour1 = Colour1 + (buffer_data[5]);

Colour2 = Colour2 + (buffer_data[6] << 16);
Colour2 = Colour2 + (buffer_data[7] << 8);
Colour2 = Colour2 + (buffer_data[8]);

Colour3 = Colour3 + (buffer_data[9] << 16);
Colour3 = Colour3 + (buffer_data[10] << 8);
Colour3 = Colour3 + (buffer_data[11]);

Colour4 = Colour4 + (buffer_data[12] << 16);
Colour4 = Colour4 + (buffer_data[13] << 8);
Colour4 = Colour4 + (buffer_data[14]);

Colour5 = Colour5 + (buffer_data[15] << 16);
Colour5 = Colour5 + (buffer_data[16] << 8);
Colour5 = Colour5 + (buffer_data[17]);

	
        // This text is shown on screen.
	//char testsq[] = { (char) header[4], (char) header[1], (char) header[2], (char) header[3]};	
	char testsq[] = { '2', '3', '4' };
	
        if (device_name_check == 1) testsq[1] = '1';
        if (device_name_check == 0) testsq[1] = '0';
        char* text = testsq;

        

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
