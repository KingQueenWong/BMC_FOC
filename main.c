#include "stm32f10x.h"
#include "stdio.h"

#define PWM_CLOCK	((u32)72000000uL)
#define DEADTIME_NS	((u16) 1800)
#define DEADTIME  (u16)((unsigned long long)PWM_CLOCK/2*(unsigned long long)DEADTIME_NS/1000000000uL)
#define PWM_PRSC ((u8)0)
#define PWM_FREQ ((u16)12500)
#define PWM_PERIOD ((u16) (PWM_CLOCK / (u32)(2 * PWM_FREQ *(PWM_PRSC+1))))
#define PWM_HalfPerMax	((u16)(PWM_PERIOD/2))
#define REP_RATE (0)

uint16_t ADC_Value[5];

int32_t Ia,Ib,Ic;
int32_t Vbus;

int32_t Ialpha;
int32_t Ibeta;

int32_t Id;
int32_t Iq;

int32_t Idr;
int32_t Iqr;

int32_t Id_error;
int32_t Iq_error;

int32_t Id_integral;
int32_t Iq_integral;

int32_t Id_last;
int32_t Iq_last;

int32_t Id_now;
int32_t Iq_now;

int32_t Id_output;
int32_t Iq_output;

int32_t Id_output_last;
int32_t Iq_output_last;

int32_t Id_output_now;
int32_t Iq_output_now;

int32_t Kp_id,Ki_id,Kd_id;
int32_t Kp_iq,Ki_iq,Kd_iq;

int32_t Sine;
int32_t Cosine;

int32_t Valpha;
int32_t Vbeta;

int32_t Vd;
int32_t Vq;

int32_t Vdr;
int32_t Vqr;

int32_t Va;
int32_t Vb;
int32_t Vc;

void ADC1_INIT(void)
{
    DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(ADC1->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_Value;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize =5;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1, ENABLE);

	ADC_InitTypeDef ADC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_DeInit(ADC1);
	ADC_StructInit(&ADC_InitStructure);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 5;
	ADC_Init(ADC1, &ADC_InitStructure);
	RCC_ADCCLKConfig(RCC_PCLK2_Div4);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_7Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_7Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_7Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_7Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_7Cycles5);
	ADC_Cmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
void TIM3_INIT(void)
{
	TIM_TimeBaseInitTypeDef TIM1_TimeBaseStructure;
	TIM_OCInitTypeDef TIM1_OCInitStructure;
	TIM_BDTRInitTypeDef TIM1_BDTRInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM1_TimeBaseStructure.TIM_Prescaler=0;
	TIM1_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1 ;
	TIM1_TimeBaseStructure.TIM_Period = PWM_PERIOD;
	TIM1_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM1_TimeBaseStructure.TIM_RepetitionCounter = REP_RATE;
	TIM_TimeBaseInit(TIM3, &TIM1_TimeBaseStructure);

	TIM1_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM1_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM1_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;

	TIM1_OCInitStructure.TIM_Pulse =1000;
	TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM1_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM1_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM1_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM3, &TIM1_OCInitStructure);

	TIM1_OCInitStructure.TIM_Pulse = 1000;
	TIM_OC3Init(TIM3, &TIM1_OCInitStructure);

	TIM1_OCInitStructure.TIM_Pulse = 1000;
	TIM_OC4Init(TIM3, &TIM1_OCInitStructure);

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM1_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
	TIM1_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
	TIM1_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;

	TIM1_BDTRInitStructure.TIM_DeadTime = DEADTIME;//
	TIM1_BDTRInitStructure.TIM_Break =TIM_Break_Enable;
	TIM1_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;
	TIM1_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;

	TIM_BDTRConfig(TIM3, &TIM1_BDTRInitStructure);

	TIM_ClearITPendingBit(TIM3, TIM_IT_Break);
	TIM_ITConfig(TIM3,TIM_IT_Break ,ENABLE);

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update ,ENABLE);

	TIM_Cmd(TIM3, ENABLE);
	TIM_CtrlPWMOutputs(TIM3, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void GPIO_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//adc
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//pwm
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//hall
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void SYSTICKT_INIT(void)
{
	if (SysTick_Config(SystemCoreClock /100))   //10ms
	{
		while (1);
	}
	NVIC_SetPriority(SysTick_IRQn, 0x0);
}

void CURRENT_CALIBRATION(void)
{

}

int32_t   ele_angle;
void Hall_Angle_Calculate(void)
{

}
const int16_t Sin_Cos_Table[256]={\
0x0000,0x00C9,0x0192,0x025B,0x0324,0x03ED,0x04B6,0x057F,\
0x0648,0x0711,0x07D9,0x08A2,0x096A,0x0A33,0x0AFB,0x0BC4,\
0x0C8C,0x0D54,0x0E1C,0x0EE3,0x0FAB,0x1072,0x113A,0x1201,\
0x12C8,0x138F,0x1455,0x151C,0x15E2,0x16A8,0x176E,0x1833,\
0x18F9,0x19BE,0x1A82,0x1B47,0x1C0B,0x1CCF,0x1D93,0x1E57,\
0x1F1A,0x1FDD,0x209F,0x2161,0x2223,0x22E5,0x23A6,0x2467,\
0x2528,0x25E8,0x26A8,0x2767,0x2826,0x28E5,0x29A3,0x2A61,\
0x2B1F,0x2BDC,0x2C99,0x2D55,0x2E11,0x2ECC,0x2F87,0x3041,\
0x30FB,0x31B5,0x326E,0x3326,0x33DF,0x3496,0x354D,0x3604,\
0x36BA,0x376F,0x3824,0x38D9,0x398C,0x3A40,0x3AF2,0x3BA5,\
0x3C56,0x3D07,0x3DB8,0x3E68,0x3F17,0x3FC5,0x4073,0x4121,\
0x41CE,0x427A,0x4325,0x43D0,0x447A,0x4524,0x45CD,0x4675,\
0x471C,0x47C3,0x4869,0x490F,0x49B4,0x4A58,0x4AFB,0x4B9D,\
0x4C3F,0x4CE0,0x4D81,0x4E20,0x4EBF,0x4F5D,0x4FFB,0x5097,\
0x5133,0x51CE,0x5268,0x5302,0x539B,0x5432,0x54C9,0x5560,\
0x55F5,0x568A,0x571D,0x57B0,0x5842,0x58D3,0x5964,0x59F3,\
0x5A82,0x5B0F,0x5B9C,0x5C28,0x5CB3,0x5D3E,0x5DC7,0x5E4F,\
0x5ED7,0x5F5D,0x5FE3,0x6068,0x60EB,0x616E,0x61F0,0x6271,\
0x62F1,0x6370,0x63EE,0x646C,0x64E8,0x6563,0x65DD,0x6656,\
0x66CF,0x6746,0x67BC,0x6832,0x68A6,0x6919,0x698B,0x69FD,\
0x6A6D,0x6ADC,0x6B4A,0x6BB7,0x6C23,0x6C8E,0x6CF8,0x6D61,\
0x6DC9,0x6E30,0x6E96,0x6EFB,0x6F5E,0x6FC1,0x7022,0x7083,\
0x70E2,0x7140,0x719D,0x71F9,0x7254,0x72AE,0x7307,0x735E,\
0x73B5,0x740A,0x745F,0x74B2,0x7504,0x7555,0x75A5,0x75F3,\
0x7641,0x768D,0x76D8,0x7722,0x776B,0x77B3,0x77FA,0x783F,\
0x7884,0x78C7,0x7909,0x794A,0x7989,0x79C8,0x7A05,0x7A41,\
0x7A7C,0x7AB6,0x7AEE,0x7B26,0x7B5C,0x7B91,0x7BC5,0x7BF8,\
0x7C29,0x7C59,0x7C88,0x7CB6,0x7CE3,0x7D0E,0x7D39,0x7D62,\
0x7D89,0x7DB0,0x7DD5,0x7DFA,0x7E1D,0x7E3E,0x7E5F,0x7E7E,\
0x7E9C,0x7EB9,0x7ED5,0x7EEF,0x7F09,0x7F21,0x7F37,0x7F4D,\
0x7F61,0x7F74,0x7F86,0x7F97,0x7FA6,0x7FB4,0x7FC1,0x7FCD,\
0x7FD8,0x7FE1,0x7FE9,0x7FF0,0x7FF5,0x7FF9,0x7FFD,0x7FFE};

#define SIN_IDX     0x0300
#define R0_90       0x0000
#define R90_180     0x0100
#define R180_270    0x0200
#define R270_360    0x0300
void Sin_Cos_Calculate(void)
{
	uint16_t  hindex;
	hindex = (uint16_t) ele_angle;
	hindex >>=6;
	switch (hindex & SIN_IDX)
	{
		case R0_90:
			Sine = Sin_Cos_Table[(uint8_t)(hindex)];
			Cosine = Sin_Cos_Table[(uint8_t)(0xFF-(uint8_t)(hindex))];
			break;
		case R90_180:
			Sine = Sin_Cos_Table[(uint8_t)(0xFF-(uint8_t)(hindex))];
			Cosine = -Sin_Cos_Table[(uint8_t)(hindex)];
			break;
		case R180_270:
			Sine = -Sin_Cos_Table[(uint8_t)(hindex)];
			Cosine = -Sin_Cos_Table[(uint8_t)(0xFF-(uint8_t)(hindex))];
			break;
		case R270_360:
			Sine = -Sin_Cos_Table[(uint8_t)(0xFF-(uint8_t)(hindex))];
			Cosine = Sin_Cos_Table[(uint8_t)(hindex)];
			break;
		default:
			break;
	}
}
void PARAMETERS_INIT(void)
{
}
void TIM3_IRQHandler(void)
{
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);

	Hall_Angle_Calculate();

	Ia = ADC_Value[2];
	Ib = ADC_Value[3];
	Vbus = ADC_Value[4];

	//Ialpha = Ia
	//Ibeta = ((¡Ì3/3*2^15)*Ia + 2*(¡Ì3/3*2^15)*Ib)/(2^15)
	Ialpha = Ia;
	Ibeta = (int32_t)(((Ia + (int32_t)(Ib<<1))*18918)>>15);

	Sin_Cos_Calculate();

	//Id = Ialpha*Cosine + Ibeta*Sine
	//Iq = Ibeta*Cosine - Ialpha*Sine
	Id = (int32_t)((Ialpha*Cosine)>>15) + (int32_t)((Ibeta*Sine)>>15);
	Iq = (int32_t)((Ibeta*Cosine)>>15) - (int32_t)((Ialpha*Sine)>>15);

	Idr=0;
	Iqr=0;

	Id_error = Idr - Id;
	Id_error = Iqr - Iq;

	Vd=0;
	Vq=800;

	//Valpha = Vd*Cosine - Vq*Sine
	//Vbeta = Vq*Cosine + Vd*Sine
	Valpha = (int32_t)((Vd*Cosine)>>15) - (int32_t)((Vq*Sine)>>15);
	Vbeta = (int32_t)((Vq*Cosine)>>15) + (int32_t)((Vd*Sine)>>15);

	//Va = Valpha
	//Vb = ((1/2*2^15)*Valpha + (¡Ì3/2*2^15)*Vbeta)/(2^15)
	//Vc = ((1/2*2^15)*Valpha - (¡Ì3/2*2^15)*Vbeta)/(2^15)
	Va = Valpha;
	Vb = (int32_t)(Valpha>>1) + (int32_t)((Vbeta*28377)>>15);
	Vc = (int32_t)(Valpha>>1) - (int32_t)((Vbeta*28377)>>15);

	//sinusoidal
	Va = (int32_t)((Va*28377)>>15) + 16384;
	Vb = (int32_t)((Vb*28377)>>15) + 16384;
	Vc = (int32_t)((Vc*28377)>>15) + 16384;

	TIM3->CCR3 = (int32_t)((Va*PWM_HalfPerMax)>>15) + PWM_HalfPerMax;
	TIM3->CCR4 = (int32_t)((Vb*PWM_HalfPerMax)>>15) + PWM_HalfPerMax;
	TIM3->CCR1 = (int32_t)((Vc*PWM_HalfPerMax)>>15) + PWM_HalfPerMax;
}

int main(void)
{
	SYSTICKT_INIT();
	GPIO_INIT();
	ADC1_INIT();
	TIM3_INIT();
	CURRENT_CALIBRATION();
	PARAMETERS_INIT();

	GPIO_SetBits(GPIOA, GPIO_Pin_5);	//enable phase a
	GPIO_SetBits(GPIOA, GPIO_Pin_7);	//enable phase b
	GPIO_SetBits(GPIOB, GPIO_Pin_2);	//enable phase c
	while (1)
	{

	}
}

