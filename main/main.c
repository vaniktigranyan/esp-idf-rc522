#include <esp_log.h>
#include <inttypes.h>
#include "rc522.h"

static const char* TAG = "rc522-demo";
static rc522_handle_t scanner;
/*
RC 522          ESP
--------------------
SDA             D22
SCK             D19
MOSI            D23
MISO            D25
RST             +3.3V
*/
uint32_t reverse_bytes(uint32_t bytes)
{
    uint32_t aux = 0;
    uint8_t byte;
    int i;
    for(i = 0; i < 32; i+=8){
        byte = (bytes >> i) & 0xff;
        aux |= byte << (32 - 8 - i);
    }
    return aux;
}
static void rc522_handler(void* arg, esp_event_base_t base, int32_t event_id, void* event_data)
{
    rc522_event_data_t* data = (rc522_event_data_t*) event_data;
    switch(event_id) {
        case RC522_EVENT_TAG_SCANNED: {
                rc522_tag_t* tag = (rc522_tag_t*) data->ptr;
                ESP_LOGI(TAG, "Tag scanned (sn: %" PRIu64 ")", tag->serial_number);
                uint64_t t = tag->serial_number&0xFFFFFFFF;
                uint64_t rev = reverse_bytes(t);
                
                printf("HEX ==>       %llx\n",t);
                printf("REV HEX ==>   %llx\n",rev);
                printf("DEC ==>       %lld\n",t);
                printf("REV DEC ==>   %lld\n",rev);
            }
            break;
    }
}
void app_main()
{
    rc522_config_t config = {
    .spi.host = VSPI_HOST,
    .spi.miso_gpio = 25,
    .spi.mosi_gpio = 23,
    .spi.sck_gpio = 19,
    .spi.sda_gpio = 22,
};

    rc522_create(&config, &scanner);
    rc522_register_events(scanner, RC522_EVENT_ANY, rc522_handler, NULL);
    rc522_start(scanner);

}
