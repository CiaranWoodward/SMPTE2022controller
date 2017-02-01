#include "xil_io.h"
#include "eth_init.h"

static uint32_t ETH_BASE_ADDR;

void ethernet_init(uint32_t baseAddr){ //init 10G ethernet
	ETH_BASE_ADDR = baseAddr;

	/*
	//STARTUP
	Xil_Out32(ETH_BASE_ADDR + CONFIG_MANAGEMENT_ADDR, 0x45);
	//RESET_MAC_RX
	Xil_Out32(ETH_BASE_ADDR + RECEIVER_ADDR, 0x90000000);
	//RESET_MAC_TX
	Xil_Out32(ETH_BASE_ADDR + TRANSMITTER_ADDR, 0x90000000);
	*/

	Xil_Out32(ETH_BASE_ADDR + 0x400, 0xAABBCCDD); //Lower bits of mac address
	uint32_t rec2 = 0xEEFF; //Upper bits of mac address
	rec2 |= 1 << 28; //enable receiver
	Xil_Out32(ETH_BASE_ADDR + 0x404, rec2); //Lower bits of mac address

	//MDIO_ADDR_1
	//Xil_Out32(ETH_BASE_ADDR + MDIO_CONTROL, 0x2000);
	//MDIO_WR
	//MDIO_RD_1
	//Xil_Out32(ETH_BASE_ADDR + MDIO_CONTROL, 0x20);
	//MDIO_ADDR
	//Xil_Out32(ETH_BASE_ADDR + MDIO_CONTROL, 0x20);
	//MDIO_RD
	//MDIO_POLL_CHECK
	//MDIO_CHECK_DATA
	//Xil_Out32(ETH_BASE_ADDR + MDIO_RX_DATA, 0x20);

}
