#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"

#include "lwip/pbuf.h"
#include "lwip/udp.h"

#define UDP_PORT 4444
#define BEACON_MSG_LEN_MAX 127
#define BEACON_TARGET "255.255.255.255"

#define SAMPLE_TIME 10000

void broadcastData(int msg) {
    struct udp_pcb* pcb = udp_new();

    ip_addr_t addr;
    ipaddr_aton(BEACON_TARGET, &addr);

    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, BEACON_MSG_LEN_MAX+1, PBUF_RAM);
    char *req = (char *)p->payload;
    memset(req, 0, BEACON_MSG_LEN_MAX+1);
    snprintf(req, BEACON_MSG_LEN_MAX, "%d\n", msg);
    err_t er = udp_sendto(pcb, p, &addr, UDP_PORT);
    pbuf_free(p);
    if (er != ERR_OK) {
        printf("Failed to send UDP packet! error=%d", er);
    } else {
        printf("Sent packet : %d\n", msg);
    }
}



int main()
{
    stdio_init_all();

    // Initialise the Wi-Fi chip
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }

    // Enable wifi station
    cyw43_arch_enable_sta_mode();

    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms("Router? I hardly know her", "AylaIsDeGrootsteSchat", CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        return 1;
    } else {
        printf("Connected.\n");
        // Read the ip address in a human readable way
        uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
        printf("IP address %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
    }

    //put define pin
    const int MOIST_PIN = 27;

    //init some things ai told me to init
    adc_init();
    adc_gpio_init(MOIST_PIN);
    adc_select_input(1);

    while (true) {
        uint16_t  volts = adc_read();
        int data = volts;
        broadcastData(data);
        sleep_ms(SAMPLE_TIME);
    }
}
