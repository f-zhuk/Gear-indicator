/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "CO_app_STM32.h"
#include "OD.h"
#include "ST7735.h"
#include "display.h"
#include "fonts.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
enum DIRECTION_ENUM // арифметическая операция
{
  DIRECTION_UNKNOWN,
  DIRECTION_UP,
  DIRECTION_DOWN
};

typedef struct {
  OD_entry_t* CAN_entry;
  uint8_t direction;
  //uint8_t gear;
  uint8_t gear_total;
  uint8_t position[16];
  uint8_t log[32];
} shifter_descriptor;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim4;

/* USER CODE BEGIN PV */
shifter_descriptor shifter[2];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* Timer interrupt function executes every 1 ms */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) 
{
  static uint16_t overflow_counter = 0; //15:12 - reserved, 11:7 - 32 samples, 6 - shifter interleave, 5:1 64ms delay
  int16_t shift = 0; //Needed for easier operations with boudaries
  uint8_t sh_num;
  if (htim == canopenNodeSTM32->timerHandle) 
  {
    canopen_app_interrupt();
  }
  //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
  overflow_counter ++;
  if ((overflow_counter&0x3F) == 0x3F) // Triggered every 64 ms
  {
    sh_num = (overflow_counter>>6)&1;
    OD_get_u8(shifter[sh_num].CAN_entry, 0x01+sh_num, (uint8_t*)&shift, false); //get shifter value

    if (shift==0) // Reserved default value meaning absence of real data
      return;
    else if (shifter[sh_num].gear_total == 0) // Triggered only at startup to get initial valid position
    {
      //shifter[sh_num].gear = 1;
      shifter[sh_num].gear_total = 1;
      shifter[sh_num].position[1] = shift;
    }

    if((shifter[sh_num].gear_total != 0) && (shifter[sh_num].direction == DIRECTION_UNKNOWN)) // Only for first shift detection
    {
      if (shift-5 >= (shifter[sh_num].position[ 1 ]))
        shifter[sh_num].direction = DIRECTION_UP;
      else if (shift+5 < (shifter[sh_num].position[ 1 ]))
        shifter[sh_num].direction = DIRECTION_DOWN;
    }
      
    shifter[sh_num].log[(overflow_counter>>7)&0x0F] = shift; // Storing 16*128ms of samples

      if (((overflow_counter>>7)&0x0F) == 0x0F) // When we got all 16 samples
      {
        int16_t min = 255;
        int16_t max = 0;
        for(uint8_t i = 0; i<16; i++) //searching for lowest and highhest values
        {
          if (shifter[sh_num].log[i] < min)
            min = shifter[sh_num].log[i];
          if (shifter[sh_num].log[i] > max)
            max = shifter[sh_num].log[i];
        }

        if ((max-min)>5) // discard all if movement not finished or deviation is too high
          return;

        if (shifter[sh_num].gear_total >= 15) // discard all if max gear count per shipter reached
          return;

        if (shifter[sh_num].direction == DIRECTION_UNKNOWN) // discard all if no direction was defined yet
          return;

        if (shifter[sh_num].direction == DIRECTION_UP)
        {
          if ((min-5) >= shifter[sh_num].position[ shifter[sh_num].gear_total ])
          {
            //shifter[sh_num].gear = gear_total;
            shifter[sh_num].gear_total += 1;
            shifter[sh_num].position[shifter[sh_num].gear_total] = (max+min)/2;
          }
          else if ((max+5) <= (shifter[sh_num].position[ 1 ]))
          {
            //shifter[sh_num].gear =1;
            shifter[sh_num].gear_total += 1;
            for(uint8_t i = shifter[sh_num].gear_total; i>1; i--) // moving all positions up to add one gear to the bottom 
            {
              shifter[sh_num].position[i] = shifter[sh_num].position[i-1];
            }
            shifter[sh_num].position[ 1 ] = (max+min)/2;
          }
        }

        if (shifter[sh_num].direction == DIRECTION_DOWN)
        {
          if ((min-5) >= shifter[sh_num].position[ 1 ])
          {
            //shifter[sh_num].gear = gear_total;
            shifter[sh_num].gear_total += 1;
            for(uint8_t i = shifter[sh_num].gear_total; i>1; i--) // moving all positions up to add one gear to the bottom 
            {
              shifter[sh_num].position[i] = shifter[sh_num].position[i-1];
            }
            shifter[sh_num].position[ 1 ] = (max+min)/2;
          }
          else if ((max+5) <= (shifter[sh_num].position[ shifter[sh_num].gear_total ]))
          {
            //shifter[sh_num].gear =1;
            shifter[sh_num].gear_total += 1;
            shifter[sh_num].position[shifter[sh_num].gear_total] = (max+min)/2;
          }
        }
      }
  }
}
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
  for(uint8_t i = 0; i<2; i++)
  {
    shifter[i].CAN_entry = OD_find(OD,0x6001); //Same Index, different subindices
    shifter[i].direction = DIRECTION_UNKNOWN;
    shifter[i].gear_total = 0;
    for(uint8_t j = 0; j<16; j++)
    {
      shifter[i].position[j]=0;
    }
    for(uint8_t j = 0; j<32; j++)
    {
      shifter[i].log[j]=0;
    }
  }
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN_Init();
  MX_SPI1_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */

  CANopenNodeSTM32 canOpenNodeSTM32;
  canOpenNodeSTM32.CANHandle = &hcan;
  canOpenNodeSTM32.HWInitFunction = MX_CAN_Init;
  canOpenNodeSTM32.timerHandle = &htim4;
  canOpenNodeSTM32.desiredNodeID = 3;
  canOpenNodeSTM32.baudrate = 125;
  canopen_app_init(&canOpenNodeSTM32);

  initR(&hspi1);
  displayInit(getBufferPointer(), ST7735_WIDTH, ST7735_HEIGHT);
  enableDisplay(false);
  enableInvert(true);
  setCursor(0,0);
  fillRectangle(80,160,0x00);

  redraw();
  enableDisplay(true);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint8_t i=0;
  uint8_t trigger=0;
  uint8_t shift=0;
  //CO_NMT_t state;

  setCursor(2,2);
  while (1)
  {
    canopen_app_process();

    setCursor(0,0);
    fillRectangle(80,160,0x00);
    OD_get_u8(OD_find(OD,0x6001), 0x01, &shift, false);

    setCursor(0,60);
    putNumber_u8(shift, lv_font_unscii_8, RGBI_LT_GREEN);
    setCursor(0,70);
    putNumber_u8(shifter[0].gear_total, lv_font_unscii_8, RGBI_LT_GREEN);
    setCursor(0,80);
    if(shifter[0].direction==DIRECTION_UNKNOWN)
      putChar('?', lv_font_unscii_8, RGBI_WHITE);
    else if(shifter[0].direction==DIRECTION_UP)
      putChar('U', lv_font_unscii_8, RGBI_WHITE);
    else if(shifter[0].direction==DIRECTION_DOWN)
      putChar('D', lv_font_unscii_8, RGBI_WHITE);

    for(uint8_t j=0; j<shifter[0].gear_total; j++)
    {
      setCursor(0,90+j*10);
      putNumber_u8(shifter[0].position[j+1], lv_font_unscii_8, RGBI_LT_GREEN);
    }
    /*setCursor(-shift,36);
    putChar('1', lv_font_montserrat_40, 0);
    putChar('2', lv_font_montserrat_40, 0);
    putChar('1', lv_font_montserrat_20, 0);
    putChar('2', lv_font_montserrat_20, 0);
    setCursor(-shift,60);
    putString("CHECK: ", lv_font_unscii_8, RGBI_LT_GREEN);
    setCursor(-shift,70);
    putString("WARNING: ", lv_font_unscii_8, 14);
    setCursor(-shift,80);
    putString("ERROR: ", lv_font_unscii_8, 12);
    setCursor(70,80);
    putChar('8', lv_font_unscii_8, 0xF);
    putChar('1', lv_font_unscii_8, 0x1);
    putChar('2', lv_font_unscii_8, 0x3);
    putChar('3', lv_font_unscii_8, 0x5);
    putChar('4', lv_font_unscii_8, 0x7);
    putChar('5', lv_font_unscii_8, 0x9);
    putChar('6', lv_font_unscii_8, 0xB);
    putChar('7', lv_font_unscii_8, 0xD);
    putChar('8', lv_font_unscii_8, 0xF);*/
    
    //setCursor(0,55);
    //fillRectangle(80,25,0x00);
    //setCursor(10,70);
    //putChar(0x30+shifter[0].gear_total);
    //setCursor(30,70);
    //putChar(0x30+shifter[1].gear_total);
    
    //setCursor(0,0);
    //fillRectangle(80,10,i);
    //setCursor(0,10);
    //fillRectangle(80,10,0x01);
    //setCursor(0,20);
    //fillRectangle(80,10,0x02);
    //setCursor(0,30);
    //fillRectangle(80,10,0x03);
    
/*
    setCursor(0,40);
    fillRectangle(80,10,0x04);
    setCursor(0,50);
    fillRectangle(80,10,0x05);
    setCursor(0,60);
    fillRectangle(80,10,0x06);
    setCursor(0,70);
    fillRectangle(80,10,0x07);
    setCursor(0,80);
    fillRectangle(80,10,0x08);
    setCursor(0,90);
    fillRectangle(80,10,0x09);
    setCursor(0,100);
    fillRectangle(80,10,0x0A);
    setCursor(0,110);
    fillRectangle(80,10,0x0B);
    setCursor(0,120);
    fillRectangle(80,10,0x0C);
    setCursor(0,130);
    fillRectangle(80,10,0x0D);
    setCursor(0,140);
    fillRectangle(80,10,0x0E);
    setCursor(0,150);
    fillRectangle(80,10,0x0F);s*/

    //fillRectangle(20,2,i);
    i++;
    
    if (!redraw_busy)
    {
      if (trigger)
        redraw_partial(0, 0, ST7735_WIDTH, 44, (uint16_t*)&st7735_palette);
      else
        redraw_partial(0, 50, ST7735_WIDTH, 110, (uint16_t*)&st7735_palette_rgbi);
      trigger ^= 0x01;
    }
      //redraw_partial(0, 0, ST7735_WIDTH, 44, (uint16_t*)&st7735_palette);
      //redraw_partial(0, i&0x1F, ST7735_WIDTH, 80, (uint16_t*)&st7735_palette_rgbi);
    //redraw_partial(0, 80, ST7735_WIDTH, 160, (uint16_t*)&st7735_palette);
    //redraw_partial(0, 0, ST7735_WIDTH, 80, (uint16_t*)&st7735_palette_red);

    HAL_Delay(10);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 12;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = ENABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */

  /* USER CODE END CAN_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 23;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 1000;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SPI1_RES_Pin|SPI1_CS_Pin|SPI_DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : SPI1_RES_Pin SPI1_CS_Pin SPI_DC_Pin */
  GPIO_InitStruct.Pin = SPI1_RES_Pin|SPI1_CS_Pin|SPI_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
