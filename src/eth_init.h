#define eth_addr = 0x44A10000

#define CONFIG_MANAGEMENT_ADDR 0x500
#define RECEIVER_ADDR 0x404
#define TRANSMITTER_ADDR 0x408
#define MDIO_CONTROL 0x504

void ethernet_init(uint32_t baseAddr);
