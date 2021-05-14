#include <string.h>
#include <stdio.h>
#include <main.h>
// externe variabelen (globaal)
extern uint32_t IR16[100];
extern uint32_t R16[100];
extern float TempMeting;

uint8_t bitmaskReg(uint8_t ,uint8_t , uint8_t );
void ClearFifoEvery();
void ReadFifo();
void setConfiguratie(uint8_t SampleAverage, uint8_t mode,uint8_t adc_range, uint8_t SampleRate,uint8_t ledPulseWidth, uint8_t LedAmp);
void TempLezen();
void InterruptsSwitch();
void gnjgn();



static const uint8_t MAX_adr = 0b10101110; // 7 bit adres

/* Registers*/
// Status Registers (4)

static const uint8_t MAX30102_INTSTAT1 = 		0x00;
static const uint8_t MAX30102_INTSTAT2 = 		0x01;

static const uint8_t MAX30102_INTENA1 = 		0x02;
static const uint8_t MAX30102_INTENA2 = 		0x03;

// FIFO

static const uint8_t MAX30102_FIFOWRITEPTR = 	0x04;
static const uint8_t MAX30102_FIFOOVERFLOW = 	0x05;
static const uint8_t MAX30102_FIFOREADPTR = 	0x06;
static const uint8_t MAX30102_FIFODATAREG = 	0x07;

//Configuration

static const uint8_t MAX30102_FIFOCONFIG = 		0x08;
static const uint8_t MAX30102_MODECONFIG = 		0x09;
static const uint8_t MAX30102_SPO2CONFIG = 		0x0A;
static const uint8_t MAX30102_LED1_PULSAMP = 	0x0C;
static const uint8_t MAX30102_LED2_PULSAMP = 	0x0D;
static const uint8_t MAX30102_MULTILEDCONFIG1 = 0x11;
static const uint8_t MAX30102_MULTILEDCONFIG2 = 0x12;

// Die temperature
static const uint8_t MAX30102_TEMPINT = 		0x1F;
static const uint8_t MAX30102_TEMPFRAC = 		0x20;
static const uint8_t MAX30102_TEMPCONFIG = 		0x21;

// PART ID
static const uint8_t MAX30102_REVID = 			0xFE;
static const uint8_t MAX30102_PARTID = 			0xFF;

/* Writing Commands*/
// Interrupt status (register 0x00 en 0x01) (only R)
//register 0x00
static const uint8_t A_FULL_MASK = 				0b10000000;
static const uint8_t A_FULL_EN = 				0x80;
static const uint8_t A_FULL_DIS = 				0x00;

static const uint8_t PPG_RDY_MASK = 			0b01000000;
static const uint8_t PPG_RDY_EN = 				0x40;
static const uint8_t PPG_RDY_DIS = 				0x00;

static const uint8_t ALC_OVF_MASK = 			0b00100000;
static const uint8_t ALC_OVF_EN = 				0x20;
static const uint8_t ALC_OVF_DIS = 				0x00;

static const uint8_t PWR_RDY_MASK = 			0b00000001;
static const uint8_t PWR_RDY_EN = 				0x01;
static const uint8_t PWR_RDY_DIS = 				0x00;

// register 0x01
static const uint8_t TEMP_RDY_MASK = 			0b00000010;
static const uint8_t TEMP_RDY_EN = 				0x02;
static const uint8_t TEMP_RDY_DIS = 			0x00;

// Interrupt Enable register 0x03 & 0x04 R/W
// 0x03
static const uint8_t ENB_A_FULL_MASK = 			0x7F;
static const uint8_t ENB_A_FULL_EN = 			0x80;
static const uint8_t ENB_A_FULL_DIS = 			0x00;
/*
static const uint8_t PPG_RDY_MASK = 			0xBF;
static const uint8_t PPG_RDY_EN = 				0x40;
static const uint8_t PPG_RDY_DIS = 				0x00;

static const uint8_t ALC_OVF_MASK = 			0xDF;
static const uint8_t ALC_OVF_EN = 				0x20;
static const uint8_t ALC_OVF_DIS = 				0x00;*/
// 0x04
static const uint8_t TMP_RDY_MASK = 			0xFD;
static const uint8_t TMP_RDY_EN = 				0x02;
static const uint8_t TMP_RDY_DIS = 				0x00;

// FIFO
static const uint8_t FIFO_WR_PTR_MASK = 		0b00001111;
static const uint8_t FIFO_OVF_COUNTER_MASK = 	0b00001111;
static const uint8_t FIFO_RD_PTR = 				0b00001111;

// FIFO configuratie

static const uint8_t SMP_AVE_MASK = 0b00011111;
static const uint8_t SMP_AVE_1 = 0x00;
static const uint8_t SMP_AVE_2 = 0x20;
static const uint8_t SMP_AVE_4 = 0x40;
static const uint8_t SMP_AVE_8 = 0x60;
static const uint8_t SMP_AVE_16 = 0x80;
static const uint8_t SMP_AVE_32 = 0xA0;

static const uint8_t FIFO_ROLLOVER_EN_MASK = 0b11101111; // dit is een write
static const uint8_t FIFO_ROLLOVER_EN_EN = 0b00010000;
static const uint8_t FIFO_ROLLOVER_EN_DIS = 0x00;

static const uint8_t FIFO_A_FULL_MASK = 0b11110000; // zet hoeveel data samples er in fifo mogen zitten wnr INT wordt gegenereerd
// Deze zetten in de code zelf, gaan we geen const van maken


//Configuratie
// Mode Configuration (register 0x09) R/W
static const uint8_t SHDN_MASK = 				0x7F;
static const uint8_t SHDN_EN = 					0x80; // Zet in power-save mode, all registers retain value, interrupts are cleared to zero.
static const uint8_t SHDN_DIS = 				0x00;

static const uint8_t RESET_MASK = 				0xBF;
static const uint8_t RESET_EN = 				0x40;
static const uint8_t RESET_DIS = 				0x00;

static const uint8_t MODE_MASK = 				0xF8;
static const uint8_t MODE_HR_EN = 				0x02;
static const uint8_t MODE_SPO2_EN = 			0x03;

// SPO2 configuratie
// register 0x0A
static const uint8_t SPO2_ADC_RGE_MASK = 		0b10011111;
static const uint8_t SPO2_ADC_RGE_2048 = 		0x00;
static const uint8_t SPO2_ADC_RGE_4096 = 		0x20;
static const uint8_t SPO2_ADC_RGE_8192 = 		0x40;
static const uint8_t SPO2_ADC_RGE_16384 = 		0x60;

static const uint8_t SPO2_SR_MASK = 	0xE3; // #samples/sec. Nog eens kijken in de datasheet
static const uint8_t SPO2_SR_50 = 		0x00;
static const uint8_t SPO2_SR_100 = 		0x01;
static const uint8_t SPO2_SR_200 = 		0x02;
static const uint8_t SPO2_SR_400 = 		0x03;
static const uint8_t SPO2_SR_800 = 		0x04;
static const uint8_t SPO2_SR_1000 = 	0x05;
static const uint8_t SPO2_SR_1600= 		0x06;
static const uint8_t SPO2_SR_3200 = 	0x07;

static const uint8_t LED_PW_MASK = 		0xFC; // #samples/sec. Nog eens kijken in de datasheet
static const uint8_t LED_PW_69 = 		0x00;
static const uint8_t LED_PW_118 = 		0x01;
static const uint8_t LED_PW_215 = 		0x02;
static const uint8_t LED_PW_411 = 		0x03;

// Temperatuur FRAC
static const uint8_t TEMP_FRAC_MASK = 	0b00001111;

// temperatuur enable
static const uint8_t TEMP_EN_MASK = 	0x01;
static const uint8_t TEMP_EN_EN = 	0x01;// Zet  zich automatisch op 0 als temp wordt gelezen
static const uint8_t TEMP_EN_DIS = 	0x00;
// Als we deze zetten, dan lezen we 1x temperatuur, lezen we de temp uit register, cleared deze bit

// MULTILED nog inzetten.
//Multi-LED Mode configuration (pg 22)
static const uint8_t MAX30105_SLOT1_MASK = 		0xF8;
static const uint8_t MAX30105_SLOT2_MASK = 		0x8F;
static const uint8_t MAX30105_SLOT3_MASK = 		0xF8;
static const uint8_t MAX30105_SLOT4_MASK = 		0x8F;








