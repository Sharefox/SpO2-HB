#include <string.h>
#include <stdio.h>
#include <main.h>

void StartSensor(uint8_t, uint8_t );
uint8_t bitmask(uint8_t ,uint8_t , uint8_t );
void ClearFifoEvery();
void ReadFifo();
void setConfiguratie(uint8_t, uint8_t,uint8_t, uint8_t );


static const uint8_t MAX_adr = 0b10101110;


/* Writing the registers*/
// Status registers
static const uint8_t MAX30102_INTSTAT = 		0x00;
static const uint8_t MAX30102_INTENA = 			0x01;

// FIFO
static const uint8_t MAX30102_FIFOWRITEPTR = 	0x02;
static const uint8_t MAX30102_FIFOOVERFLOW = 	0x03;
static const uint8_t MAX30102_FIFOREADPTR = 	0x04;
static const uint8_t MAX30102_FIFODATAREG = 	0x05;

// Configuration
static const uint8_t MAX30102_MODECONFIG = 		0x06;
static const uint8_t MAX30102_SPO2CONFIG = 		0x07;
static const uint8_t MAX30102_LEDCONFIG = 		0x09;

// Temperature
static const uint8_t MAX30102_TEMPINT = 		0x16;
static const uint8_t MAX30102_TEMPFRAC = 		0x17;

// PART ID
static const uint8_t MAX30102_REVID = 			0xFE;
static const uint8_t MAX30102_PARTID = 			0xFF;

/* Writing Commands */
// Interrupt status Register commando's (register 0x00) R
static const uint8_t A_FULL_MASK = 				0b10000000;
static const uint8_t A_FULL_EN = 				0x80;
static const uint8_t A_FULL_DIS = 				0x00;

static const uint8_t TEMP_RDY_MASK = 			0b01000000;
static const uint8_t TEMP_RDY_EN = 				0x40;
static const uint8_t TEMP_RDY_DIS = 			0x00;

static const uint8_t HR_RDY_MASK = 				0b00100000;
static const uint8_t HR_RDY_EN = 				0x20;
static const uint8_t HR_RDY_DIS = 				0x00;

static const uint8_t SPO2_RDY_MASK = 			0b00010000;
static const uint8_t SPO2_RDY_EN = 				0x10;
static const uint8_t SPO2_RDY_DIS = 			0x00;

static const uint8_t PWR_RDY_MASK = 			0b00000001;
static const uint8_t PWR_RDY_EN = 				0x01;
static const uint8_t PWR_RDY_DIS = 				0x00;

// Interrupt Enable Register commando's (register 0x01) R/W
static const uint8_t ENB_A_FULL_MASK = 			0x7F;
static const uint8_t ENB_A_FULL_EN = 			0x80;
static const uint8_t ENB_A_FULL_DIS = 			0x00;

static const uint8_t ENB_TEMP_RDY_MASK = 		0xBF;
static const uint8_t ENB_TEMP_RDY_EN = 			0x40;
static const uint8_t ENB_TEMP_RDY_DIS = 		0x00;

static const uint8_t ENB_HR_RDY_MASK = 			0xDF;
static const uint8_t ENB_HR_RDY_EN = 			0x20;
static const uint8_t ENB_HR_RDY_DIS = 			0x00;

static const uint8_t ENB_SPO2_RDY_MASK = 		0xEF;
static const uint8_t ENB_SPO2_RDY_EN = 			0x10;
static const uint8_t ENB_SPO2_RDY_DIS = 		0x00;

//FIFO
// Alle 3 de registers leeg maken bij nieuw begin van lezen
static const uint8_t FIFO_WR_PTR_MASK = 		0b00001111;
static const uint8_t FIFO_OVF_COUNTER_MASK = 	0b00001111;
static const uint8_t FIFO_RD_PTR = 				0b00001111;

//Configuratie
// Mode Configuration (register 0x06) R/W
static const uint8_t SHDN_MASK = 				0x7F;
static const uint8_t SHDN_EN = 					0x80; // Zet in power-save mode, all registers retain value, interrupts are cleared to zero.
static const uint8_t SHDN_DIS = 				0x00;

static const uint8_t RESET_MASK = 				0xBF;
static const uint8_t RESET_EN = 				0x40;
static const uint8_t RESET_DIS = 				0x00;

static const uint8_t TEMP_EN_MASK = 			0xF7;
static const uint8_t TEMP_EN_EN = 				0x08; // zet zich normaal automatisch terug op 0.
static const uint8_t TEMP_EN_DIS = 				0x00;

static const uint8_t MODE_MASK = 				0xF8;
static const uint8_t MODE_HR_EN = 				0x02;
static const uint8_t MODE_SPO2_EN = 			0x03;

// SPO2 configuration (register 0x07) R/W
static const uint8_t SPO2_HI_RES_EN_MASK = 		0xBF;
static const uint8_t SPO2_HI_RES_EN_EN = 		0x40;
static const uint8_t SPO2_HI_RES_EN_DIS = 		0x00;

static const uint8_t SPO2_SR_MASK = 	0xE3; // #samples/sec. Nog eens kijken in de datasheet
static const uint8_t SPO2_SR_50 = 		0x00;
static const uint8_t SPO2_SR_100 = 		0x01;
static const uint8_t SPO2_SR_167 = 		0x02;
static const uint8_t SPO2_SR_200 = 		0x03;
static const uint8_t SPO2_SR_400 = 		0x04;
static const uint8_t SPO2_SR_600 = 		0x05;
static const uint8_t SPO2_SR_800 = 		0x06;
static const uint8_t SPO2_SR_1000 = 	0x07;

static const uint8_t LED_PW_MASK = 		0xFC; // #samples/sec. Nog eens kijken in de datasheet
static const uint8_t LED_PW_200 = 		0x00;
static const uint8_t LED_PW_400 = 		0x01;
static const uint8_t LED_PW_800 = 		0x02;
static const uint8_t LED_PW_1600 = 		0x03;

// LED configuration (register 0x09) R/W

static const uint8_t RED_PA_MASK = 		0x0F;
static const uint8_t IR_PA_MASK = 		0xF0;
// hier nog LED current control zetten?

// temperatuur INT  Configuration (register 0x16) R/W
// Hier zijn geen masks nodig.

// Temperatuur FRAC config (register 0x17) R/W

static const uint8_t TEMP_FRAC_MASK = 	0b00001111;

