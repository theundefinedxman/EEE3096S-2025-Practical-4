* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
   ==================================================================
   WRITTEN BY:    X. BUQWANA
   MODIFIED:      22-09-2025
   ==================================================================
**/

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "stm32f4xx.h"
#include "lcd_stm32f4.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// TODO: Add values for below variables
#define NS        128         // Number of samples in LUT
#define TIM2CLK   16000000UL  // STM Clock frequency: Hint You might want to check the ioc file
#define F_SIGNAL  125000UL    // 125 kHz (signal we want our analogue to have)
#define NUM_WAVEFORMS 6       // the number of wave forms we can produce

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2; 
TIM_HandleTypeDef htim3;
DMA_HandleTypeDef hdma_tim2_ch1;

/* USER CODE BEGIN PV */
// TODO: Add code for global variables, including LUTs
//this is the LUT for the Sine wave
uint32_t Sin_LUT[NS] = { 2047, 2147, 2248, 2347, 2446, 2545, 2641, 2737, 2831, 2922, 3012, 3100, 3185, 3267, 3346, 3422, 3495, 3564, 3630, 3692, 3749, 3803, 3853, 3898, 3939, 3975, 4006, 4033, 4055, 4072, 4085, 4092, 4095, 4092, 4085, 4072, 4055, 4033, 4006, 3975, 3939, 3898, 3853, 3803, 3749, 3692, 3630, 3564, 3495, 3422, 3346, 3267, 3185, 3100, 3012, 2922, 2831, 2737, 2641, 2545, 2446, 2347, 2248, 2147, 2047, 1947, 1846, 1747, 1648, 1549, 1453, 1357, 1263, 1172, 1082, 994, 909, 827, 748, 672, 599, 530, 464, 402, 345, 291, 241, 196, 155, 119, 88, 61, 39, 22, 9, 2, 0, 2, 9, 22, 39, 61, 88, 119, 155, 196, 241, 291, 345, 402, 464, 530, 599, 672, 748, 827, 909, 994, 1082, 1172, 1263, 1357, 1453, 1549, 1648, 1747, 1846, 1947 };

//this is the LUT for the Saw wave
uint32_t Saw_LUT[NS] = { 0, 31, 63, 95, 127, 159, 191, 223, 255, 287, 319, 351, 383, 415, 447, 479, 511, 543, 575, 607, 639, 671, 703, 735, 767, 799, 831, 863, 895, 927, 959, 991, 1023, 1055, 1087, 1119, 1151, 1183, 1215, 1247, 1279, 1311, 1343, 1375, 1407, 1439, 1471, 1503, 1535, 1567, 1599, 1631, 1663, 1695, 1727, 1759, 1791, 1823, 1855, 1887, 1919, 1951, 1983, 2015, 2047, 2079, 2111, 2143, 2175, 2207, 2239, 2271, 2303, 2335, 2367, 2399, 2431, 2463, 2495, 2527, 2559, 2591, 2623, 2655, 2687, 2719, 2751, 2783, 2815, 2847, 2879, 2911, 2943, 2975, 3007, 3039, 3071, 3103, 3135, 3167, 3199, 3231, 3263, 3295, 3327, 3359, 3391, 3423, 3455, 3487, 3519, 3551, 3583, 3615, 3647, 3679, 3711, 3743, 3775, 3807, 3839, 3871, 3903, 3935, 3967, 3999, 4031, 4063 };

//This is the LUT for the Triangle wave
uint32_t Triangle_LUT[NS] = { 0, 63, 127, 191, 255, 319, 383, 447, 511, 575, 639, 703, 767, 831, 895, 959, 1023, 1087, 1151, 1215, 1279, 1343, 1407, 1471, 1535, 1599, 1663, 1727, 1791, 1855, 1919, 1983, 2047, 2111, 2175, 2239, 2303, 2367, 2431, 2495, 2559, 2623, 2687, 2751, 2815, 2879, 2943, 3007, 3071, 3135, 3199, 3263, 3327, 3391, 3455, 3519, 3583, 3647, 3711, 3775, 3839, 3903, 3967, 4031, 4095, 4031, 3967, 3903, 3839, 3775, 3711, 3647, 3583, 3519, 3455, 3391, 3327, 3263, 3199, 3135, 3071, 3007, 2943, 2879, 2815, 2751, 2687, 2623, 2559, 2495, 2431, 2367, 2303, 2239, 2175, 2111, 2047, 1983, 1919, 1855, 1791, 1727, 1663, 1599, 1535, 1471, 1407, 1343, 1279, 1215, 1151, 1087, 1023, 959, 895, 831, 767, 703, 639, 575, 511, 447, 383, 319, 255, 191, 127, 63 };

//This is the LUT for the Piano wave
uint32_t Piano_LUT[NS] = { 1901, 2046, 1834, 1955, 1916, 1944, 1890, 2020, 1954, 2015, 1940, 1908, 1867, 2207, 1913, 1598, 1879, 1899, 1818, 1909, 1918, 1904, 1403, 1874, 2208, 1897, 2798, 1521, 1592, 1682, 2110, 2059, 1347, 2025, 1817, 1915, 1837, 1945, 1564, 1799, 1779, 1959, 2120, 1768, 2121, 1775, 2510, 1882, 1954, 1918, 1835, 1918, 1876, 1890, 2727, 1790, 2271, 1708, 1850, 1795, 1285, 1755, 1519, 1887, 1613, 1925, 2003, 1900, 1866, 1873, 1548, 2014, 1745, 1942, 1629, 2339, 2065, 1885, 2236, 2038, 1879, 1897, 1855, 1927, 1886, 1894, 2051, 1907, 1946, 1773, 1682, 1886, 1862, 2092, 1860, 1964, 2222, 1906, 1932, 1891, 1926, 1875, 1587, 1766, 1842, 1846, 2024, 1941, 1889, 1849, 1675, 2011, 1856, 1935, 1847, 1927, 1884, 1915, 1788, 1755, 1426, 1615, 1932, 1910, 1852, 1783, 1951, 1901 };

//This is the LUT for the Guitar
uint32_t Guitar_LUT[NS] = { 1970, 2297, 2295, 2048, 2021, 1777, 1748, 1936, 1437, 2016, 2004, 1975, 2011, 1999, 1810, 1959, 1966, 1949, 1615, 2441, 1834, 1813, 2405, 2161, 1687, 1975, 1936, 1985, 1958, 1914, 1883, 1976, 1975, 2022, 1771, 2075, 1774, 2053, 1924, 1661, 2180, 1946, 1979, 1974, 1987, 1852, 2071, 1963, 1792, 1927, 2111, 1930, 2119, 2070, 1808, 1926, 2072, 2065, 2019, 1742, 2091, 2022, 2711, 2160, 2031, 2160, 2200, 2177, 2065, 2044, 1496, 1595, 1988, 1962, 2003, 1986, 2199, 1966, 1954, 1979, 2092, 2108, 1714, 2055, 2003, 1648, 2051, 1797, 2011, 2026, 1968, 1955, 1879, 1806, 2040, 1968, 2221, 1890, 2174, 1904, 1952, 1926, 2461, 2361, 1932, 1974, 1956, 1964, 2042, 1854, 1967, 1974, 1850, 1949, 2084, 1812, 2230, 1863, 2142, 1561, 2030, 1769, 2321, 1823, 1548, 2562, 1852, 1970 };

//This is the LUT for the Drum
uint32_t Drum_LUT [NS] = { 2047, 3396, 1864, 2110, 2063, 3400, 1893, 1933, 2086, 2144, 2036, 2030, 2039, 1949, 1414, 1848, 2091, 2032, 2044, 2059, 2048, 2093, 2044, 2020, 2094, 1803, 2072, 2045, 2173, 2017, 3235, 1276, 1671, 2193, 2436, 2104, 1931, 2062, 2028, 2032, 4095, 1922, 2035, 2042, 2502, 2372, 1820, 1996, 2284, 2072, 2018, 2051, 671, 2425, 2075, 2069, 17, 2239, 2042, 2023, 2192, 2052, 2902, 2386, 4073, 2009, 2285, 2038, 2518, 2711, 2048, 2096, 1629, 2001, 2086, 2043, 2549, 1894, 2061, 2076, 2001, 2055, 2043, 2049, 2099, 2047, 2065, 2047, 2923, 2071, 2017, 2080, 2018, 2049, 1831, 2014, 837, 2295, 1978, 2040, 1980, 2036, 2106, 2042, 1643, 2055, 2066, 2032, 2050, 2442, 2145, 2218, 2127, 2032, 2038, 1781, 2154, 2441, 2132, 4095, 2150, 2012, 2036, 1615, 1981, 323, 2525, 2047 };

// TODO: Equation to calculate TIM2_Ticks
uint32_t TIM2_Ticks = 0; // How often to write new LUT value
uint32_t DestAddress = (uint32_t) &(TIM3->CCR3); // Write LUT TO TIM3->CCR3 to modify PWM duty cycle


uint8_t waveform_index = 0;
char *waveform_names[NUM_WAVEFORMS] = {
    "Sine", "Sawtooth", "Triangle", "Piano", "Guitar", "Drum"
};
uint32_t *waveform_LUTs[NUM_WAVEFORMS] = {
    Sin_LUT, Saw_LUT, Triangle_LUT, Piano_LUT, Guitar_LUT, Drum_LUT
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */
void EXTI0_IRQHandler(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  init_LCD();
  /* USER CODE BEGIN 2 */
  // TODO: Start TIM3 in PWM mode on channel 3
  if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3) != HAL_OK)
  {
      Error_Handler(); // Just in case something goes wrong
  }
  // TODO: Start TIM2 in Output Compare (OC) mode on channel 1
  if (HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_1) != HAL_OK)
  {
      Error_Handler(); // Just in case something goes wrong
  }
  // TODO: Start DMA in IT mode on TIM2->CH1. Source is LUT and Dest is TIM3->CCR3; start with Sine LUT
  if (HAL_DMA_Start_IT(&hdma_tim2_ch1,(uint32_t)&Sin_LUT[0],(uint32_t)(&htim3.Instance->CCR3),(uint32_t)NS) != HAL_OK)
  {
      Error_Handler();
  }

  // TODO: Write current waveform to LCD(Sine is the first waveform)
  lcd_command(CLEAR);
  lcd_command(CURSOR_HOME);
  lcd_putstring("Sine");
  // TODO: Enable DMA (start transfer from LUT to CCR)
  __HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_CC1);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
  /* TIM2_CH1 DMA Init */
  __HAL_RCC_DMA1_CLK_ENABLE();

  hdma_tim2_ch1.Instance = DMA1_Stream5;
  hdma_tim2_ch1.Init.Channel = DMA_CHANNEL_3;         // TIM2_CH1 is on channel 3
  hdma_tim2_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH; // Memory -> TIM3->CCR3
  hdma_tim2_ch1.Init.PeriphInc = DMA_PINC_DISABLE;    // Peripheral address fixed
  hdma_tim2_ch1.Init.MemInc = DMA_MINC_ENABLE;        // Memory address increments
  hdma_tim2_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_tim2_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma_tim2_ch1.Init.Mode = DMA_CIRCULAR;            // Repeat LUT automatically
  hdma_tim2_ch1.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_tim2_ch1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

  if (HAL_DMA_Init(&hdma_tim2_ch1) != HAL_OK)
  {
      Error_Handler();
  }

  /* Link DMA handle to TIM2 handle */
  __HAL_LINKDMA(&htim2, hdma[TIM_DMA_ID_CC1], hdma_tim2_ch1);
  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  // -------------------------------
  // LCD pins configuration
  // -------------------------------
  // Configure PC14 (RS) and PC15 (E) as output push-pull
  GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // Configure PB8 (D4) and PB9 (D5) as output push-pull
  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // Configure PA12 (D6) and PA15 (D7) as output push-pull
  GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Set all LCD pins LOW initially
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12 | GPIO_PIN_15, GPIO_PIN_RESET);


  // -------------------------------
  // Button0 configuration (PA0)
  // -------------------------------
  GPIO_InitStruct.Pin = Button0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING; // Interrupt on rising edge
  GPIO_InitStruct.Pull = GPIO_PULLUP;         // Use pull-up resistor
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Enable and set EXTI line 0 interrupt priority
  HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void EXTI0_IRQHandler(void){
	static uint8_t waveform_index = 0;
	uint32_t end = 0;
	uint32_t start = HAL_GetTick();

	// TODO: Debounce using HAL_GetTick()
	if( start-end > 200){
		    start = end;
			HAL_DMA_Abort_IT(&hdma_tim2_ch1); // Disable current DMA


			waveform_index = (waveform_index + 1) % 6;  // 6 waveforms
	// TODO: Disable DMA transfer and abort IT, then start DMA in IT mode with new LUT and re-enable transfer
	// HINT: Consider using C's "switch" function to handle LUT changes
			switch (waveform_index)
			        {
			            case 0:
			                lcd_command(CLEAR);
			                lcd_command(CURSOR_HOME);
			                lcd_putstring("Sine");
			                HAL_DMA_Start_IT(&hdma_tim2_ch1,(uint32_t)&Sin_LUT[0],(uint32_t)&(htim3.Instance->CCR3),NS);
			                break;

			            case 1:
			                lcd_command(CLEAR);
			                lcd_command(CURSOR_HOME);
			                lcd_putstring("Sawtooth");
			                HAL_DMA_Start_IT(&hdma_tim2_ch1,(uint32_t)&Saw_LUT[0],(uint32_t)&(htim3.Instance->CCR3),NS);
			                break;

			            case 2:
			                lcd_command(CLEAR);
			                lcd_command(CURSOR_HOME);
			                lcd_putstring("Triangle");
			                HAL_DMA_Start_IT(&hdma_tim2_ch1,(uint32_t)&Triangle_LUT[0],(uint32_t)&(htim3.Instance->CCR3),NS);
			                break;

			            case 3:
			                lcd_command(CLEAR);
			                lcd_command(CURSOR_HOME);
			                lcd_putstring("Piano");
			                HAL_DMA_Start_IT(&hdma_tim2_ch1,(uint32_t)&Piano_LUT[0],(uint32_t)&(htim3.Instance->CCR3),NS);
			                break;

			            case 4:
			                lcd_command(CLEAR);
			                lcd_command(CURSOR_HOME);
			                lcd_putstring("Guitar");
			                HAL_DMA_Start_IT(&hdma_tim2_ch1,(uint32_t)&Guitar_LUT[0],(uint32_t)&(htim3.Instance->CCR3),NS);
			                break;

			            case 5:
			            	lcd_command(CLEAR);
			            	lcd_command(CURSOR_HOME);
			            	lcd_putstring("Drum");
			                HAL_DMA_Start_IT(&hdma_tim2_ch1,(uint32_t)&Drum_LUT[0],(uint32_t)&(htim3.Instance->CCR3),NS);
			                break;
			        }
			 __HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_CC1);

	}

	HAL_GPIO_EXTI_IRQHandler(Button0_Pin); // Clear interrupt flags
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */



