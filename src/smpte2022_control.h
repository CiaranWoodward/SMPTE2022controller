/*
 * smpte2022_control.h
 *
 *  Created on: 8 Nov 2016
 *      Author: cw15g13
 */

#ifndef SRC_SMPTE2022_CONTROL_H_
#define SRC_SMPTE2022_CONTROL_H_

void smpte2022_init(uint32_t baseAddr);

void smpte2022_reset(void);

//Channel semaphore must be taken before any operation on a channel
//isSecondary enables specific configuration for matching data (0 = primary channel) all other data is shared between streams on same channel
void smpte2022_channel_getSemaphore(uint8_t channelID, uint8_t isSecondary);

//Matching is per channel and per stream
void smpte2022_channel_matchDestIp(uint8_t isEnabled, uint32_t ip4Addr);
void smpte2022_channel_matchSrcIp(uint8_t isEnabled, uint32_t ip4Addr);
void smpte2022_channel_matchDestPort(uint8_t isEnabled, uint16_t port);
void smpte2022_channel_matchSrcPort(uint8_t isEnabled, uint16_t port);
void smpte2022_channel_matchVLAN(uint8_t isEnabled, uint16_t VLAN_TCI);

//All config below here is per channel, but shared between streams
void smpte2022_channel_setEnabled(uint8_t isEnabled);
void smpte2022_channel_setBypassRtpTimestamp(uint8_t isBypassEnabled);
void smpte2022_channel_setPlayoutDelay(uint32_t playoutDelay_27Mhz);

//Length of individual RTP packets is 1396 bytes so "endAddr = baseAddr + (numPackets * 1396)"
void smpte2022_channel_setDDR3Params(uint32_t baseAddr, uint16_t numPackets);

//Channel semaphore must be released in order to commit changes to a particular channel
void smpte2022_channel_releaseSemaphore(void);

#endif /* SRC_SMPTE2022_CONTROL_H_ */
