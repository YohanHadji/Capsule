///////////////////////////////////////////////////////////////////////////////////////
//
//                                                                                                                                                            
//	RRRRRRRRRRRRRRRRR   FFFFFFFFFFFFFFFFFFFFFFBBBBBBBBBBBBBBBBB           GGGGGGGGGGGGG
//	R::::::::::::::::R  F::::::::::::::::::::FB::::::::::::::::B       GGG::::::::::::G
//	R::::::RRRRRR:::::R F::::::::::::::::::::FB::::::BBBBBB:::::B    GG:::::::::::::::G
//	RR:::::R     R:::::RFF::::::FFFFFFFFF::::FBB:::::B     B:::::B  G:::::GGGGGGGG::::G
//	  R::::R     R:::::R  F:::::F       FFFFFF  B::::B     B:::::B G:::::G       GGGGGG
//	  R::::R     R:::::R  F:::::F               B::::B     B:::::BG:::::G              
//	  R::::RRRRRR:::::R   F::::::FFFFFFFFFF     B::::BBBBBB:::::B G:::::G              
//	  R:::::::::::::RR    F:::::::::::::::F     B:::::::::::::BB  G:::::G    GGGGGGGGGG
//	  R::::RRRRRR:::::R   F:::::::::::::::F     B::::BBBBBB:::::B G:::::G    G::::::::G
//	  R::::R     R:::::R  F::::::FFFFFFFFFF     B::::B     B:::::BG:::::G    GGGGG::::G
//	  R::::R     R:::::R  F:::::F               B::::B     B:::::BG:::::G        G::::G
//	  R::::R     R:::::R  F:::::F               B::::B     B:::::B G:::::G       G::::G
//	RR:::::R     R:::::RFF:::::::FF           BB:::::BBBBBB::::::B  G:::::GGGGGGGG::::G
//	R::::::R     R:::::RF::::::::FF           B:::::::::::::::::B    GG:::::::::::::::G
//	R::::::R     R:::::RF::::::::FF           B::::::::::::::::B       GGG::::::GGG:::G
//	RRRRRRRR     RRRRRRRFFFFFFFFFFF           BBBBBBBBBBBBBBBBB           GGGGGG   GGGG
//                                                                                                                                                                   
//  Interface header file for RF communication protocol 
//
//  EPFL Rocket Team - Nordend Project 2023
//
//  Charlotte Heibig & Lionel Isoz 
//  25.03.2023 
///////////////////////////////////////////////////////////////////////////////////////
#ifndef RADIO_PACKET_H
#define RADIO_PACKET_H

#include <stdint.h>

#define RF_PREFIX 					'Y' //0b01011001
#define IGNITION_CODE 				0X434C //CL
#define CMD_ACTIVE 					0b1111
#define CMD_INACTIVE 				0b0000

///////////////////////////////////////////////////////////////////////////////////////

// TODO: clean everything once AV data to send are defined

// Test: text written from CLion from ERT2023GS repo toto
// I think it is working !
// And this message is written from STM32Cube IDE !!

typedef struct __attribute__((__packed__)) {
	// TODO: @Avioncis update for Nordend 2023 Mission
	uint32_t prefix;
	uint32_t timestamp;
	int32_t acc_z;
	int32_t baro_press;
	int16_t baro_temp;
	int32_t kalman_z;
	int32_t kalman_v;
//	int32_t kalman_a;
	int32_t kalman_sigma_z;
//	float	gnss_hdop;
	float	gnss_lon;
	float	gnss_lat;
	int32_t	gnss_alt;
	uint8_t av_state;
    uint32_t packet_nbr;
    int32_t baro_alt;
    //RF_cmd engine_state;
    
} PacketAV_downlink;
const uint32_t packetAV_downlink_size = sizeof(PacketAV_downlink);

///////////////////////////////////////////////////////////////////////////////////////

//must be 32 bits
typedef struct __attribute__((__packed__)) {
	unsigned int ventN20 : 4;
	unsigned int ventEthanol : 4;
	unsigned int servoN20 : 4;
	unsigned int servoEthanol : 4;
	unsigned int pressurization : 4;
	unsigned int abort : 4;
	unsigned int error : 4;
	unsigned int other : 4;
} RF_cmd;
const uint32_t RF_cmd_size = sizeof(RF_cmd);


typedef struct __attribute__((__packed__)) {
	uint8_t prefix;
	uint8_t cmd_counter;
	RF_cmd cmd;
	uint16_t cmd_ignition;
} PacketAV_uplink;
const uint32_t packetAV_uplink_size = sizeof(PacketAV_uplink);



///////////////////////////////////////////////////////////////////////////////////////

// old packet, it will be deleted soon
typedef struct __attribute__((__packed__)) radio_packet { 
	uint32_t prefix;
	uint32_t timestamp;
	int32_t acc_z;
	int32_t baro_press;
	int16_t baro_temp;
	int32_t kalman_z;
	int32_t kalman_v;
//	int32_t kalman_a;
	int32_t kalman_sigma_z;
//	float	gnss_hdop;
	float	gnss_lon;
	float	gnss_lat;
	int32_t	gnss_alt;
	uint8_t av_state;
    uint32_t packet_nbr;
    int32_t baro_alt;
} radio_packet_t;
const uint32_t radio_packet_size = sizeof(radio_packet_t);


#endif /* RADIO_PACKET_H */