#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"

#include "lwip/pbuf.h"
#include "lwip/udp.h"

#define UDP_PORT 4444
#define BEACON_MSG_LEN_MAX 5
#define BEACON_TARGET "255.255.255.255"

#define WIFI_SSID "Your Wifi SSID"
#define WIFI_PASSWORD "Your Wifi Password"

// define how long in ms the pico waits before measuring & broadcasting again, in this case 15 minutes
#define SAMPLE_TIME 900000

//the function for broadcasting the data gathered
void broadcastData(int data, struct udp_pcb* pcb, ip_addr_t addr) {

    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, BEACON_MSG_LEN_MAX+1, PBUF_RAM);
    char *req = (char *)p->payload;
    memset(req, 0, BEACON_MSG_LEN_MAX+1);
    snprintf(req, BEACON_MSG_LEN_MAX, "%d\n", data);
    err_t er = udp_sendto(pcb, p, &addr, UDP_PORT);
    pbuf_free(p);
    if (er != ERR_OK) {
        printf("Failed to send UDP packet! error=%d\n", er);
        printf("Failed Value : %d\n", data);
    } else {
        printf("Sent packet : %d\n", data);
    }
    #if PICO_CYW43_ARCH_POLL
        // if you are using pico_cyw43_arch_poll, then you must poll periodically from your
        // main loop (not from a timer) to check for Wi-Fi driver or lwIP work that needs to be done.
        cyw43_arch_poll();
        sleep_ms(1);
#else
        // if you are not using pico_cyw43_arch_poll, then WiFI driver and lwIP work
        // is done via interrupt in the background. This sleep is just an example of some (blocking)
        // work you might be doing.
        sleep_ms(1);
#endif
}

//the main loop, gathering data and broadcasting it
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
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        return 1;
    } else {
        printf("Connected.\n");
        // Read the ip address in a human readable way
        uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
        printf("IP address %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
    }

    // defenition of the (adc) pin connected to the moisture sensor
    const int MOIST_PIN = 27;

    // initiallisation of the liberaries and functions used to get the data from the moisture sensor
    adc_init();
    adc_gpio_init(MOIST_PIN);
    adc_select_input(1);

    struct udp_pcb* pcb = udp_new();

    ip_addr_t addr;
    ipaddr_aton(BEACON_TARGET, &addr);

    //udp_set_flags(pcb, UDP_FLAGS_BROADCAST);

    // the loop keeping the pico running
    while (true) {
        // reads the data and converts it into a intiger
        uint16_t  volts = adc_read();
        int data = volts;

        // broadcasts the data into the open
        broadcastData(data, pcb, addr);

        //let that pico rest
        sleep_ms(SAMPLE_TIME);
    }
}
