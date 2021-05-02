/*
 * Library for the MAX30102 SPO2 usage.
 *
 * Written by Lieselotte Verhulst
 *
 * Start 12/03/2021
 * end
 *
 */

#include <MAX30102.h>
I2C_HandleTypeDef hi2c1;



void setConfiguratie(uint8_t mode, uint8_t SampleRateSPO2,uint8_t LedPulseWidth, uint8_t LedCurrent  )
{
	/*
	 * static const uint8_t MODE_SPO2_EN = 			0x03;
	 * static const uint8_t SPO2_SR_400 = 		0x04;
	 * static const uint8_t LED_PW_400 = 		0x01;
	 * LedCurrent = 0x1F (zou default zijn???)
	 * --> nu zou adc 14 bits zijn
	 * */
	uint8_t waarde;
	uint8_t Reg_Size = 1;
	uint8_t Size = 1;

	// Set Mode
	waarde = bitmask(MAX30102_MODECONFIG, MODE_MASK,mode);
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_MODECONFIG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);

	// Set sample Rate
	waarde = bitmask(MAX30102_SPO2CONFIG, SPO2_SR_MASK,SampleRateSPO2);
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_SPO2CONFIG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);

	// Set Led Pulse Width
	waarde = bitmask(MAX30102_SPO2CONFIG, LED_PW_MASK,LedPulseWidth);
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_SPO2CONFIG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);

	// Set Led current
	waarde = LedCurrent;
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_LEDCONFIG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);

}

/* Functions for the i2c usage */

// Schrijf mode + temp_En naar MAX
void StartSensor(uint8_t mode, uint8_t temp_en)
{
	/*
	 * Schrijf naar MAX mode waarin we werken + temp_en
	 *
	 * */
	uint8_t waarde;
	uint8_t Reg_Size = 1;
	uint8_t Size = 1;

	waarde = bitmask(MAX30102_MODECONFIG, MODE_MASK,mode);
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_MODECONFIG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);

	waarde = bitmask(MAX30102_MODECONFIG, TEMP_EN_MASK,temp_en);
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_MODECONFIG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);
}

// Temperatuur uitlezen functie (deze komt in een interrupt)

float TempLezen()
{
 // lees temperatuur uit
  /* 2 registers uitlezen
   * 1. De Temp_Integer (0x16 = 8 bits) (integer)
   * 2. De Temp_fraction (0x17 = 4 bits (lsb) frac.
   * Formule voor bij elkaar Tme = Tint + T frac;
   *
   * */
	uint8_t TempINT = 0x00;
	uint8_t TempFRAC = 0x00;
	uint8_t Reg_Size = 1;
	uint8_t Size = 1;
	float TempMeting = 0;

  HAL_I2C_Mem_Read(&hi2c1, MAX_adr, MAX30102_TEMPINT, Reg_Size, &TempINT, Size, HAL_MAX_DELAY);

  HAL_I2C_Mem_Read(&hi2c1, MAX_adr, MAX30102_TEMPFRAC, Reg_Size, &TempFRAC, Size, HAL_MAX_DELAY);
  // Door de data te lezen zou interrupt gecleard moeten zijn, is dit juist?
  // Interrupt eens lezen? idk
  TempMeting = (float)TempINT + ((float)TempFRAC * 0.0625);
  return TempMeting; // return nog weghalen wegens interrupt, pointer?
}


uint8_t bitmask(uint8_t RegAdr,uint8_t mask, uint8_t waardeMeegvn){ //Naam functie nog veranderen
	uint8_t originalContent;
	uint8_t Reg_Size = 1;
	uint8_t Size = 1;

	HAL_I2C_Mem_Read(&hi2c1, MAX_adr, RegAdr, Reg_Size, &originalContent, Size, HAL_MAX_DELAY);
	// Adres van Reg moet niet met pointer!

	originalContent = originalContent & mask;

	return  originalContent | waardeMeegvn;
}

void ReadFifo(){
	//MAX30102_FIFODATAREG = 0x05; is het register om uit te lezen
	uint8_t waarde = 0x00;
	uint8_t Reg_Size = 1;
	uint8_t Size = 1;

	uint8_t IR_MSB = 0x00;
	uint8_t R_MSB = 0x00;
	uint8_t IR_LSB = 0x00;
	uint8_t R_LSB = 0x00;

	uint16_t IR16;
	uint16_t R16;

	HAL_I2C_Mem_Read(&hi2c1, MAX_adr, MAX30102_FIFOWRITEPTR, Reg_Size, &waarde, Size, HAL_MAX_DELAY);
	uint8_t FIFO_Write = waarde;
	HAL_I2C_Mem_Read(&hi2c1, MAX_adr, MAX30102_FIFOREADPTR, Reg_Size, &waarde, Size, HAL_MAX_DELAY);

	uint8_t FIFO_Read = waarde;
	uint8_t AvailableSamples = FIFO_Write - FIFO_Read;
// Data uitlezen
	HAL_I2C_Mem_Read(&hi2c1, MAX_adr, MAX30102_FIFODATAREG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);

	for ( int i = 0; i < AvailableSamples; i++)
	{
		HAL_I2C_Mem_Read(&hi2c1, MAX_adr, MAX30102_FIFODATAREG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);
		IR_LSB = waarde;
		HAL_I2C_Mem_Read(&hi2c1, MAX_adr, MAX30102_FIFODATAREG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);
		IR_MSB = waarde;
		HAL_I2C_Mem_Read(&hi2c1, MAX_adr, MAX30102_FIFODATAREG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);
		R_LSB = waarde;
		HAL_I2C_Mem_Read(&hi2c1, MAX_adr, MAX30102_FIFODATAREG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);
		R_MSB = waarde;
	}

	IR16 = IR_MSB << 8 | IR_LSB;
	R16  = R_MSB  << 8 | R_LSB;
// nog pointer maken van deze 2 waarden (deze waarde is nodig in SPO2CALC.C)

}

void ClearFifoEvery(){
	// Moet gebeuren ook bij het begin van van het programma best
	uint8_t waarde = 0x00;
	uint8_t Reg_Size = 1;
	uint8_t Size = 1;

	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_FIFOWRITEPTR, Reg_Size, &waarde, Size, HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_FIFOOVERFLOW, Reg_Size, &waarde, Size, HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_FIFOREADPTR, Reg_Size, &waarde, Size, HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_FIFODATAREG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);
}

void InterruptsSwitch()
{
	// Interupt is gegenereerd
	/*
	 * Hier moeten we dan kijken welke interrupt, m.a.w. register van interrupt uitlezen.
	 * en dan met switchke zien welke waarden
	 *
	 * Interrupt is active low. Dus We moeten naar de 0 zoeken
	 */
	uint8_t mask = 0b11110001;
	uint8_t WaardeIntReg = 0x00;
	uint8_t Reg_Size = 1;
	uint8_t Size = 1;

	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_INTSTAT, Reg_Size, &WaardeIntReg, Size, HAL_MAX_DELAY);

	// We moeten uit deze waarde nog de default pinnen halen, die altijd op 0 staan
	// mss nog masken?

	WaardeIntReg = WaardeIntReg & mask;

	switch(WaardeIntReg)
	    {
				case 0b10000000 :

					// Fifo Almost full, FIFO wr ptr = FIFO read ptr -1
					// Lees FIFO direct
					ReadFifo(); // de waarden hier uit moeten nog ergens komen

					break;
				case 0b01000000 :
					// Temperature ready  --> Lees temperatuur register uit
					TempLezen(); // De waarde dat hier uitkomt moet ook nog ergens naartoe
					break;
				case 0b00100000:
					// Heart rate data ready
					// Na elke data sample, heart rate sample is 1 IR data punt
					// Dit wordt sowieso gecleared als de FIFO wordt gelezen
					// Kan het dan zijn dat deze ook aan staat met anderen interrupt???
					break;
				case 0b00010000:
					// SPO2 data ready
					// Na elke data sample = 1 IR en 1 RED data punt
					break;
				case 0b00000001:
					// Power ready flag
					// om aan te geven dat de IC powered is en kan data collecteren.
					// Ik zou hier de start lus beginnen.
					break;
				case 0b00000000:
					// niks doen, geen enkele interrupt hoog
					break;
	    }
}



