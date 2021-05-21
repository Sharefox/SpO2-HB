#include <MAX30102.h>
#include <MAXCalc.h>

I2C_HandleTypeDef hi2c1;
uint8_t Reg_Size = 1;
uint8_t Size = 1;
uint8_t L;

uint32_t SamplesTaken;
uint8_t R_1 = 0x00;
uint8_t R_2 = 0x00;
uint8_t R_3 = 0x00;
uint8_t IR_1 = 0x00;
uint8_t IR_2 = 0x00;
uint8_t IR_3 = 0x00;
uint32_t IR16[200];
uint32_t R16[200];

uint32_t spo2;
uint8_t validSPO2;
uint8_t validHeartRate;

uint8_t bufferLength = 200;

uint8_t flag = 1;

void LeesData(void)
{
	/* 1. We kijken hoeveel samples er te lezen zijn. De Write pointer en de Read pointer lezen we in
	* Deze  gaan we dan van elkaar aftrekken en zo weten we hoeveel samples we hebben.
	*/
	uint8_t i = 0;
	uint8_t waarde[7] = {0,0,0,0,0,0,0};
	uint8_t zero = 0x00;
	uint8_t ReadCommand = 0x07;


	HAL_I2C_Mem_Read(&hi2c1, MAX_adr, MAX30102_FIFOWRITEPTR, Reg_Size, waarde, Size, 1000);
	uint8_t FIFO_Write = waarde[0];
	HAL_I2C_Mem_Read(&hi2c1, MAX_adr, MAX30102_FIFOREADPTR, Reg_Size, waarde, Size, 1000);
	uint8_t FIFO_Read = waarde[0];
	uint8_t AvailableSamples = FIFO_Write - FIFO_Read;

	if(L + SamplesToRead >= 200-1){
		L = 0;
	}
	if(AvailableSamples > 0){
		while (i <= SamplesToRead)
		{
			HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_FIFOREADPTR, Reg_Size, &i, 1, 1000);

			HAL_I2C_Mem_Read(&hi2c1, MAX_adr, MAX30102_FIFOREADPTR, Reg_Size, waarde, 1, 1000);


			HAL_I2C_Mem_Read(&hi2c1, MAX_adr, MAX30102_FIFODATAREG, Reg_Size, waarde, 7, 1000);
			R_1 = waarde[0];
			R_2 = waarde[1];
			R_3 = waarde[2];
			IR_1 = waarde[3];
			IR_2 = waarde[4];
			IR_3 = waarde[5];

			IR16[L + i] = (IR_1 << 14) | (IR_2 << 6) | (IR_3 >> 2);
			R16[L + i++]  = (R_1 << 14) | (R_2 << 6) | (R_3 >> 2);
			SamplesTaken += 1;
		}
	}


	i = 0;
	L += i;
	AvailableSamples = 0;
}

void BeginBerekening()
{
	if (SamplesTaken >= 200)
	{
		// Begin berekening uitvoeren --> Vormt gemiddelde van alles --> Duurt 4 seconden voor we hier zijn
		maxim_heart_rate_and_oxygen_saturation(IR16, bufferLength, R16, &spo2, &validSPO2, &heartRate, &validHeartRate);
	}


	else{
		return;
	}
}

void BerekeningTussendoor()
{

	  HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
	for (uint8_t i=50; i<200; i++)
	{
		IR16[i-50] = IR16[i];
		R16[i-50] = R16[i];
	}

	for (L = 150; L>149; L++)
	{
		LeesData();
	}
	maxim_heart_rate_and_oxygen_saturation(IR16, bufferLength, R16, &spo2, &validSPO2, &heartRate, &validHeartRate);
}

void ClearFifoEvery(){
	// Moet gebeuren ook bij het begin van van het programma best
	uint8_t waarde = 0x00;

	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_FIFOWRITEPTR, Reg_Size, &waarde, Size, HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_FIFOOVERFLOW, Reg_Size, &waarde, Size, HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_FIFOREADPTR, Reg_Size, &waarde, Size, HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_FIFODATAREG, Reg_Size, &waarde, Size, HAL_MAX_DELAY);
}
void setConfiguratie(uint8_t SampleAverage, uint8_t mode,uint8_t adc_range, uint8_t SampleRate,uint8_t ledPulseWidth, uint8_t LedAmp )
{
	uint8_t waarde;

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

	uint8_t waardeIntEn = 0b10100000;
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_INTENA1, Reg_Size, &waardeIntEn, Size, HAL_MAX_DELAY);
	waardeIntEn = 0b00000010;
	HAL_I2C_Mem_Write(&hi2c1, MAX_adr, MAX30102_INTENA2, Reg_Size, &waardeIntEn, Size, HAL_MAX_DELAY);

	 ClearFifoEvery();
	 setConfiguratie(SMP_AVE_8,MODE_SPO2_EN,SPO2_ADC_RGE_4096,SPO2_SR_400,LED_PW_118,0x7F);
	 L = 0;
}
void TempLezen()
{
	// lees temperatuur uit
	uint8_t TempINT = 0x00;
	uint8_t TempFRAC = 0x00;
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

	HAL_I2C_Mem_Read(&hi2c1, MAX_adr, MAX30102_INTSTAT1, Reg_Size, &WaardeIntReg1, Size, HAL_MAX_DELAY);
	HAL_I2C_Mem_Read(&hi2c1, MAX_adr, MAX30102_INTSTAT2, Reg_Size, &WaardeIntReg2, Size, HAL_MAX_DELAY);


	if ((WaardeIntReg2 & maskTemp_ready) == maskTemp_ready)
	{
		// interrupt zodat temperatuur kan gelezen worden
		// Wordt gecleared door TFRAC register te lezen of INT te lezen.
		TempLezen();
	}
	if ((WaardeIntReg1 & maskA_Full) == maskA_Full)
	{
		// Fifo almost full
		// Deze waarde wanneer deze triggered kan ik aanpassen in het FIFO_A_FULL[3:0]
		// interrupt cleared als register wordt gelezen
		LeesData();
	}
	if ((WaardeIntReg1 & maskNewFIFO_ready) == maskNewFIFO_ready)
	{
		// New FIFO Data Ready
		// INT cleared als register INT of FIFO data wordt gelezen
		LeesData();
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
	if(flag){
		  //BeginBerekening();
		  flag = 0;
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
