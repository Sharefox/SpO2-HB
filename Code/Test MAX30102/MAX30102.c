#include <MAX30102.h>
#include <MAXCalc.h>


I2C_HandleTypeDef hi2c1;
uint32_t spo2;
uint8_t validSPO2;
uint32_t heartRate;
uint8_t validHeartRate;

uint8_t waarde = 0x00;
uint8_t Reg_Size = 1;
uint8_t Size = 1;

uint8_t IR_MSB = 0x00;
uint8_t R_MSB = 0x00;
uint8_t IR_LSB = 0x00;
uint8_t R_LSB = 0x00;
uint32_t IR16[100];
uint32_t R16[100];

uint8_t bufferLength = 100;
uint32_t redBuffer[100];
uint32_t IRBuffer[100];
int dataisNotAvailable = 0;
void gnjgn()
	{
		for (int i=0; i < bufferLength; i++ )
		{
			while (dataisNotAvailable == 0)
			{
				// Blijven we in deze lus.
			}

			HAL_I2C_Mem_Read(&hi2c1, MAX_adr, MAX30102_FIFODATAREG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);
			IR_LSB = waarde;
			HAL_I2C_Mem_Read(&hi2c1, MAX_adr, MAX30102_FIFODATAREG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);
			IR_MSB = waarde;
			HAL_I2C_Mem_Read(&hi2c1, MAX_adr, MAX30102_FIFODATAREG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);
			R_LSB = waarde;
			HAL_I2C_Mem_Read(&hi2c1, MAX_adr, MAX30102_FIFODATAREG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);
			R_MSB = waarde;

			IR16[i] = IR_MSB << 8 | IR_LSB;
			R16[i]  = R_MSB  << 8 | R_LSB;
		}

		// Hier zouden we dan 100 samples in de µc moeten hebben.
		// Met dit gaan we dan berekenen hoe we aan de signaal range komen.

		// calculate with 100 samples (in het geval van arduino was dit 4 sec)
		maxim_heart_rate_and_oxygen_saturation(IR16, bufferLength, R16, &spo2, &validSPO2, &heartRate, &validHeartRate);
		// De 4 laatste parameters moeten dan eigenlijk naar de lcd worden gestuurd.
	}
void ReadFifo(){

	uint8_t waarde = 0x00;
	uint8_t Reg_Size = 1;
	uint8_t Size = 1;

	uint8_t IR_MSB = 0x00;
	uint8_t R_MSB = 0x00;
	uint8_t IR_LSB = 0x00;
	uint8_t R_LSB = 0x00;
	uint16_t IR16[63];
	uint16_t R16[63];
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

		IR16[i] = IR_MSB << 8 | IR_LSB;
		R16[i]  = R_MSB  << 8 | R_LSB;
	}
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
void setConfiguratie(uint8_t SampleAverage, uint8_t mode,uint8_t adc_range, uint8_t SampleRate,uint8_t ledPulseWidth, uint8_t LedAmp )
{
	uint8_t waarde;
	uint8_t Reg_Size = 1;
	uint8_t Size = 1;
	uint8_t temp_en;
	uint8_t FifoRollOver = FIFO_ROLLOVER_EN_DIS;
	uint8_t FIFO_A_FULL_Value = 0x02;

	if (mode  == MODE_SPO2_EN)
	{
		temp_en = TEMP_EN_EN;
	}
	else
	{
		temp_en = TEMP_EN_DIS;
	}

	// FIFO configuratie
	waarde = bitmaskReg(MAX30102_FIFOCONFIG, SMP_AVE_MASK,SampleAverage);
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_FIFOCONFIG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);

	waarde = bitmaskReg(MAX30102_FIFOCONFIG, FIFO_ROLLOVER_EN_MASK,FifoRollOver);
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_FIFOCONFIG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);

	waarde = bitmaskReg(MAX30102_FIFOCONFIG, FIFO_A_FULL_MASK,FIFO_A_FULL_Value);
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_FIFOCONFIG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);

	//Mode configuratie
	waarde = bitmaskReg(MAX30102_MODECONFIG, MODE_MASK,mode);
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_MODECONFIG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);

	// SPO2 configuratie
	waarde = bitmaskReg(MAX30102_SPO2CONFIG, SPO2_ADC_RGE_MASK,adc_range);
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_SPO2CONFIG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);

	waarde = bitmaskReg(MAX30102_SPO2CONFIG, SPO2_SR_MASK,SampleRate);
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_SPO2CONFIG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);

	waarde = bitmaskReg(MAX30102_SPO2CONFIG, LED_PW_MASK,ledPulseWidth);
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_SPO2CONFIG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);

	// Led pulse amplitude
	//waarde = LedAmp;
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_LED1_PULSAMP, Reg_Size, &LedAmp, Size, HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_LED2_PULSAMP, Reg_Size, &LedAmp, Size, HAL_MAX_DELAY);

	// Temp enable
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_TEMPCONFIG, Reg_Size, &temp_en, Size, HAL_MAX_DELAY);
}
void StartSensor()
{
	// Dit wordt aangeboden als de Interrupt van power ready op 1 staat.
	// initialisatie van de sensor.

	 ClearFifoEvery();
	 setConfiguratie(SMP_AVE_8,MODE_SPO2_EN,SPO2_ADC_RGE_4096,SPO2_SR_400,LED_PW_118,0x7F);
}
void TempLezen()
{
	// lees temperatuur uit
	uint8_t TempINT = 0x00;
	uint8_t TempFRAC = 0x00;
	uint8_t Reg_Size = 1;
	uint8_t Size = 1;
	float TempMeting;

  HAL_I2C_Mem_Read(&hi2c1, MAX_adr, MAX30102_TEMPINT, Reg_Size, &TempINT, Size, HAL_MAX_DELAY);
  HAL_I2C_Mem_Read(&hi2c1, MAX_adr, MAX30102_TEMPFRAC, Reg_Size, &TempFRAC, Size, HAL_MAX_DELAY);

  TempMeting = (float)TempINT + ((float)TempFRAC * 0.0625);
}
void InterruptsSwitch()
{
	/* Interrupt is generated
	 * In register kijken welke interrupt.
	 */

	uint8_t maskA_Full = 0b10000000;
	uint8_t maskNewFIFO_ready = 0b01000000;
	uint8_t maskALC_OVF = 0b00100000;
	uint8_t maskTemp_ready = 0b00000010;
	uint8_t maskPwr_ready = 0b00000001;


	uint8_t WaardeIntReg1 = 0x00;
	uint8_t WaardeIntReg2 = 0x00;

	uint8_t Reg_Size = 1;
	uint8_t Size = 1;

	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_INTSTAT1, Reg_Size, &WaardeIntReg1, Size, HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_INTSTAT2, Reg_Size, &WaardeIntReg2, Size, HAL_MAX_DELAY);

	if ((WaardeIntReg1 & maskA_Full) == maskA_Full)
	{
		// Fifo almost full
		// Deze waarde wanneer deze triggered kan ik aanpassen in het FIFO_A_FULL[3:0]
		// interrupt cleared als register wordt gelezen
	}
	if ((WaardeIntReg1 & maskNewFIFO_ready) == maskNewFIFO_ready)
	{
		// New FIFO Data Ready
		// INT cleared als register INT of FIFO data wordt gelezen
		ReadFifo();
	}
	if ((WaardeIntReg1 & maskALC_OVF) == maskALC_OVF)
	{
		// Interrupt triggers when ambient light cancellation has reached its maximum
		// ambient ligth is affecting output of ADC
		// interrupt cleared door ITN register te lezen
	}
	if ((WaardeIntReg1 & maskPwr_ready) == maskPwr_ready)
	{
		// Power Ready Flag
		// module is klaar om gepowered te worden en data te collecteren
		StartSensor();

	}
	if ((WaardeIntReg2 & maskTemp_ready) == maskTemp_ready)
	{
		// interrupt zodat temperatuur kan gelezen worden
		// Wordt gecleared door TFRAC register te lezen of INT te lezen.
		TempLezen();
	}
}
uint8_t bitmaskReg(uint8_t RegAdr,uint8_t mask, uint8_t waardeMeegvn){ //Naam functie nog veranderen
	uint8_t originalContent;
	uint8_t Reg_Size = 1;
	uint8_t Size = 1;

	HAL_I2C_Mem_Read(&hi2c1, MAX_adr, RegAdr, Reg_Size, &originalContent, Size, HAL_MAX_DELAY);
	// Adres van Reg moet niet met pointer!

	originalContent = originalContent & mask;

	return  originalContent | waardeMeegvn;
}
