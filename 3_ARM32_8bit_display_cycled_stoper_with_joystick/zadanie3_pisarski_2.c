/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

#define LED_1 GPIO_PIN_6
#define LED_2 GPIO_PIN_7
#define LED_3 GPIO_PIN_8
#define LED_4 GPIO_PIN_9
#define LED_5 GPIO_PIN_4
#define LED_6 GPIO_PIN_3
#define LED_7 GPIO_PIN_5
#define LED_8 GPIO_PIN_6

#define LED_RED GPIO_PIN_13
#define LED_BLUE GPIO_PIN_12
#define LED_GREEN GPIO_PIN_8

#define LCD_DP GPIO_PIN_9
#define LCD_DA GPIO_PIN_0
#define LCD_DB GPIO_PIN_1
#define LCD_DC GPIO_PIN_2
#define LCD_DD GPIO_PIN_3
#define LCD_DE GPIO_PIN_4
#define LCD_DF GPIO_PIN_5
#define LCD_DG GPIO_PIN_6

#define DIG1 GPIO_PIN_5
#define DIG2 GPIO_PIN_4

#define DELAY 125   //1000


#define NOT_CLICKED -1
#define DOWN 0
#define UP 1
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void led_show_green(void);
static void led_show_red(void);
static void led_show_blue(void);

static void joystick_right(void);
static void joystick_handleIO(void);

static uint16_t DISPLAY8BIT(int);


/* USER CODE BEGIN PFP */

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

	//int counter = 0;
	//int NumberLED = 0;
//	uint16_t diodes[8] = {LED_1, LED_2, LED_3, LED_4, LED_5, LED_6, LED_7, LED_8};
	//GPIO_TypeDef* ports[8] = {GPIOC, GPIOC, GPIOC, GPIOC, GPIOE, GPIOD, GPIOE, GPIOE};


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
   led_show_green();
   led_show_red();
   led_show_blue();
   joystick_right();
   joystick_handleIO();


  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

   GPIO_PinState click;
     int state = -1;
     int digit = 0;
     int i = 0;
     int startPress = 0;
     int endPress = 0;


     HAL_GPIO_WritePin(GPIOB, DIG1 , GPIO_PIN_RESET);
  while (1)
  {

	  if(click == GPIO_PIN_SET && endPress == 1){
	 		  endPress = 0;
	 	  }

	 	 click = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15);

	 	 //USTALANIE KIERUNKU RUCHU
	 	 if(click == GPIO_PIN_RESET && endPress == 0 && startPress == 0){
	 		 HAL_GPIO_WritePin(GPIOB, DIG1 , GPIO_PIN_SET);
	 		 startPress = 1;
	 		 endPress = 1;
	 		 if(state == NOT_CLICKED){ //jesli wyłączony
	 			 state = UP; //włącz w górę
	 		 } else { // jeżeli włączony
	 			 if(state == UP){ //jezeli leci w góre
	 				 state = DOWN; // włącz w dół
	 			 } else{ // jeżeli leci w dół
	 				 state = UP; // włącz w górę
	 			 }
	 		 }


	 	 }

	 	 //WYSWIETLANIE LICZBY
	 	 if(i > DELAY){
	 		  HAL_GPIO_WritePin(GPIOG, DISPLAY8BIT(digit) , GPIO_PIN_RESET);

	 		  if(state == 1){ //w górę
	 			  digit++;
	 		  }
	 		  if(state == 0){ //w dół
	 			  digit--;
	 		  }
	 		  if(digit >= 9){ // jezeli sufit, lec w dol
	 			  state = DOWN;
	 			  //digit = 0;
	 		  }
	 		  if(digit < 1){ // jezeli dół, jeć w górę
	 			  state = UP;
	 			  //digit = 9;
	 		  }
	 		  i = 0;
	 		  startPress = 0;
	 	 } else {
	 		  HAL_GPIO_WritePin(GPIOG, DISPLAY8BIT(digit) , GPIO_PIN_SET);
	 	 }


	 	 i++;
	 	  //HAL_Delay(1);
	     /* USER CODE END WHILE */
	 	joystick_handleIO();



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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
   GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  // __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin : PC6 */
  // GPIO_InitStruct.Pin = GPIO_PIN_6;
  // GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  // HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
   __HAL_RCC_GPIOG_CLK_ENABLE();
	GPIO_InitStruct.Pin = LCD_DA | LCD_DB | LCD_DC | LCD_DD | LCD_DE | LCD_DF | LCD_DG;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitStruct.Pin = DIG1 | DIG2;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


	HAL_PWREx_EnableVddIO2();

}
void led_show_blue(void){
		__HAL_RCC_GPIOD_CLK_ENABLE();
		GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /* GPIO Ports Clock Enable */


	  /*Configure GPIO pin : PC6 */
	  GPIO_InitStruct.Pin = LED_BLUE;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}
void led_show_red(void){
		__HAL_RCC_GPIOD_CLK_ENABLE();
		GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /* GPIO Ports Clock Enable */


	  /*Configure GPIO pin : PC6 */
	  GPIO_InitStruct.Pin = LED_RED;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}
void led_show_green(void){
		__HAL_RCC_GPIOB_CLK_ENABLE();
		GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /* GPIO Ports Clock Enable */


	  /*Configure GPIO pin : PC6 */

	  GPIO_InitStruct.Pin = LED_GREEN;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}
void joystick_right(void){
		__HAL_RCC_GPIOE_CLK_ENABLE();
		GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /* GPIO Ports Clock Enable */


	  /*Configure GPIO pin : PC6 */



	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);


	  GPIO_InitStruct.Pin = GPIO_PIN_1;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);


	  GPIO_InitStruct.Pin = GPIO_PIN_2;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);


	  GPIO_InitStruct.Pin = GPIO_PIN_15;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}
joystick_handleIO(){

	GPIO_PinState isBlue;
	GPIO_PinState isRed;
	GPIO_PinState isGreen;
	GPIO_PinState isAll;


	   HAL_Delay(1); //milisekunda

	isBlue = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0);
	if(isBlue == GPIO_PIN_RESET){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	}
	else if(isBlue == GPIO_PIN_SET){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	}

	   HAL_Delay(1);

	isGreen = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1);
	if(isGreen == GPIO_PIN_RESET){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
	}
	else if(isGreen == GPIO_PIN_SET){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
	}
	  HAL_Delay(1);

	isAll = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15);
	if(isAll == GPIO_PIN_RESET){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);

	}
	else if(isAll == GPIO_PIN_SET){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
	}

	   HAL_Delay(1);

	isRed = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2);
	if(isRed == GPIO_PIN_RESET){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	}
	else if(isRed == GPIO_PIN_SET){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);

	}
}
uint16_t DISPLAY8BIT(int i){



	uint16_t digits[10] = {LCD_DA | LCD_DB | LCD_DC | LCD_DD | LCD_DE | LCD_DF,
			LCD_DB | LCD_DC,
			LCD_DA | LCD_DB | LCD_DG | LCD_DE | LCD_DD,
			LCD_DA | LCD_DB | LCD_DG | LCD_DC | LCD_DD,
			LCD_DF | LCD_DG | LCD_DB | LCD_DC,
			LCD_DA | LCD_DF | LCD_DG | LCD_DC | LCD_DD,
			LCD_DA | LCD_DF | LCD_DG | LCD_DC | LCD_DD | LCD_DE,
			LCD_DA | LCD_DB | LCD_DC,
			LCD_DA | LCD_DB | LCD_DC | LCD_DD | LCD_DE | LCD_DF | LCD_DG,
			LCD_DA | LCD_DB | LCD_DG | LCD_DF | LCD_DC,
	};
	//HAL_GPIO_WritePin(GPIOG, digits[i] , GPIO_PIN_SET);
	//HAL_Delay(1000);

	//HAL_GPIO_WritePin(GPIOG, digits[i] , GPIO_PIN_RESET);
	return digits[i];

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

#ifdef  USE_FULL_ASSERT
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
