/*
 * smpte2022_control.c
 *
 *  Created on: 8 Nov 2016
 *      Author: cw15g13
 */

#include "smpte2022_control.h"
#include "smpte2022_regdef.h"
#include "xil_io.h"
#include "vplat.h"

static uint32_t smpte2022_baseAddr;
static uint8_t smpte2022_sem = 0;
static uint32_t smpte2022_matchField = 0;
static uint8_t smpte2022_matchSelDirty = 0;
static uint8_t smpte2022_sChanEnabled;

void smpte2022_init(uint32_t baseAddr){
	smpte2022_baseAddr = baseAddr;
	smpte2022_sem = 0;
}

void smpte2022_reset(void){
	u32 out = Xil_In32(smpte2022_baseAddr + SMPTE2022_RESET) | 0x01;
	Xil_Out32(smpte2022_baseAddr + SMPTE2022_RESET, out);
	smpte2022_sem = 0;
}

//Channel semaphore must be taken before any operation on a channel
//isSecondary enables specific configuration for matching data (0 = primary channel) all other data is shared between streams on same channel
void smpte2022_channel_getSemaphore(uint8_t channelID, uint8_t isSecondary){
	u32 val = Xil_In32(smpte2022_baseAddr + SMPTE2022_CONTROL);

	vplat_assert(!smpte2022_sem); //Assert semaphore not already held

	val &= ~(0x02);
	Xil_Out32(smpte2022_baseAddr + SMPTE2022_CONTROL, val);

	//Set channel number and stream
	val = Xil_In32(smpte2022_baseAddr + SMPTE2022_CHANNEL_ACCESS);
	val &= ~(0xFF | (1<<31));
	val |= channelID;
	val |= isSecondary ? (1<<31) : 0;
	Xil_Out32(smpte2022_baseAddr + SMPTE2022_CHANNEL_ACCESS, val);

	//for match_sel logic
	smpte2022_matchSelDirty = 0;

	smpte2022_sem = 1;
}

static void matchSelDirtyHandle(){
	//Todo: Follow the steps on p30 of the xilinx doc to correctly manage this for dual link SMPTE
	if(!smpte2022_matchSelDirty){
		smpte2022_matchSelDirty = 1;
		smpte2022_matchField = Xil_In32(smpte2022_baseAddr + SMPTE2022_MATCH_SEL);
		Xil_Out32(smpte2022_baseAddr + SMPTE2022_MATCH_SEL, 0); //Clear match select

		u32 chanen = Xil_In32(smpte2022_baseAddr + SMPTE2022_CHAN_EN);
		smpte2022_sChanEnabled = chanen & 0x01;
		chanen &= ~(0x01);
		Xil_Out32(smpte2022_baseAddr + SMPTE2022_CHAN_EN, chanen);
	}
}

//Matching is per channel and per stream
void smpte2022_channel_matchDestIp(uint8_t isEnabled, uint32_t ip4Addr){
	vplat_assert(smpte2022_sem); //Assert semaphore is actually held
	matchSelDirtyHandle();

	if(isEnabled){
		Xil_Out32(smpte2022_baseAddr + SMPTE2022_MATCH_DEST_IP_ADDR, ip4Addr);
		smpte2022_matchField |= (1<<2);
	}
	else{
		smpte2022_matchField &= ~(1<<2);
	}
}

void smpte2022_channel_matchSrcIp(uint8_t isEnabled, uint32_t ip4Addr){
	vplat_assert(smpte2022_sem); //Assert semaphore is actually held
	matchSelDirtyHandle();

	if(isEnabled){
		Xil_Out32(smpte2022_baseAddr + SMPTE2022_MATCH_SRC_IP_ADDR, ip4Addr);
		smpte2022_matchField |= (1<<1);
	}
	else{
		smpte2022_matchField &= ~(1<<1);
	}
}

void smpte2022_channel_matchDestPort(uint8_t isEnabled, uint16_t port){
	vplat_assert(smpte2022_sem); //Assert semaphore is actually held
	matchSelDirtyHandle();

	if(isEnabled){
		u32 out = Xil_In32(smpte2022_baseAddr + SMPTE2022_MATCH_DEST_PORT);
		out &= 0xFF00;
		out |= port;
		Xil_Out32(smpte2022_baseAddr + SMPTE2022_MATCH_DEST_PORT, out);
		smpte2022_matchField |= (1<<4);
	}
	else{
		smpte2022_matchField &= ~(1<<4);
	}
}

void smpte2022_channel_matchSrcPort(uint8_t isEnabled, uint16_t port){
	vplat_assert(smpte2022_sem); //Assert semaphore is actually held
	matchSelDirtyHandle();

	if(isEnabled){
		u32 out = Xil_In32(smpte2022_baseAddr + SMPTE2022_MATCH_SRC_PORT);
		out &= 0xFF00;
		out |= port;
		Xil_Out32(smpte2022_baseAddr + SMPTE2022_MATCH_SRC_PORT, out);
		smpte2022_matchField |= (1<<3);
	}
	else{
		smpte2022_matchField &= ~(1<<3);
	}
}

void smpte2022_channel_matchVLAN(uint8_t isEnabled, uint16_t VLAN_TCI){
	vplat_assert(smpte2022_sem); //Assert semaphore is actually held
	matchSelDirtyHandle();

	if(isEnabled){
		u32 out = Xil_In32(smpte2022_baseAddr + SMPTE2022_MATCH_VLAN);
		out &= 0xFF00;
		out |= (1<<31) | VLAN_TCI;
		Xil_Out32(smpte2022_baseAddr + SMPTE2022_MATCH_VLAN, out);
		smpte2022_matchField |= (0x01);
	}
	else{
		u32 out = Xil_In32(smpte2022_baseAddr + SMPTE2022_MATCH_VLAN);
		out &= 0x7F00;
		Xil_Out32(smpte2022_baseAddr + SMPTE2022_MATCH_VLAN, out);
		smpte2022_matchField &= ~(0x01);
	}
}

//All config below here is per channel, but shared between streams
void smpte2022_channel_setEnabled(uint8_t isEnabled);
void smpte2022_channel_setBypassRtpTimestamp(uint8_t isBypassEnabled);
void smpte2022_channel_setPlayoutDelay(uint32_t playoutDelay_27Mhz);

//Length of individual RTP packets is 1396 bytes so "endAddr = baseAddr + (numPackets * 1396)"
void smpte2022_channel_setDDR3Params(uint32_t baseAddr, uint16_t numPackets);

//Channel semaphore must be released in order to commit changes to a particular channel
void smpte2022_channel_releaseSemaphore(void){
	vplat_assert(smpte2022_sem); //Assert semaphore is actually held

	if(smpte2022_matchSelDirty){
		//update the match list
		Xil_Out32(smpte2022_baseAddr + SMPTE2022_MATCH_SEL, smpte2022_matchField);

		//reenable channel if necessary
		if(smpte2022_sChanEnabled){
			u32 chanen = Xil_In32(smpte2022_baseAddr + SMPTE2022_CHAN_EN);
			chanen |= 0x01;
			Xil_Out32(smpte2022_baseAddr + SMPTE2022_CHAN_EN, chanen);
		}
	}

	//Release the semaphore
	u32 val = Xil_In32(smpte2022_baseAddr + SMPTE2022_CONTROL);
	val |= (0x02);
	Xil_Out32(smpte2022_baseAddr + SMPTE2022_CONTROL, val);

	smpte2022_sem = 0;
}
