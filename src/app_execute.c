/**
  ******************************************************************************
  * @file    STM32F3xx_IAP/src/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    02-October-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */   


/* Includes ------------------------------------------------------------------*/
#include "menu.h"
#include "crc.h"
#include "main.h"

/** @addtogroup STM32F3xx_IAP
  * @{
  */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern pFunction JumpToApplication;
extern uint32_t JumpAddress;
unsigned long __attribute__((section (".myBufSection"))) bootloader_flag;

/* Private function prototypes -----------------------------------------------*/
static void IAP_Init(void);
void LED_Boot_Init(void);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void app_execute(void)
{


	/*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f37x.s/startup_stm32f30x.s) before to branch to 
       application main. To reconfigure the default setting of SystemInit() 
       function, refer to system_stm32f37x.c/system_stm32f30x.c files.
     */ 
  
	if ((CheckCRC() == FAILED) ||		//CRC della flash fallito, entro in bootloader
			(((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) != 0x20000000)	|| /* Test if user code is programmed starting from address "APPLICATION_ADDRESS" */
			(bootloader_flag == 0xB000)		// software reset, chiamato dall'applicativo
			)

	{
		// reset software reset flag
		RCC->CSR |= RCC_CSR_RMVF;
		// reset bootloader request flag
		bootloader_flag=0;


		/* If Key is pressed, execute the IAP driver in order to re-program the Flash */
		IAP_Init();

		/* Display main menu */
		Main_Menu ();
	}

	/* Keep the user application running */
	else
	{

		/* Jump to user application */
		JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
		JumpToApplication = (pFunction) JumpAddress;

		// invert led position
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET); // led verde acceso
		HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET); // led blu spento

		/* Initialize user application's Stack Pointer */
		__set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);


		/* Jump to application */
		JumpToApplication();

	}

	/* Infinite loop */
	while (1)
	{
	}
}



// rapido lampeggio del led fail
void LED_Boot_Init(void)
{
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET); // led verde spento
	HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET); // led blu acceso
	
}




/**
  * @brief  Initialize the IAP: Configure RCC, USART, CRC, Flash and GPIOs.
  * @param  None
  * @retval None
  */
void IAP_Init(void)
{
  

 /* Unlock the Flash Program Erase controller */
  FLASH_If_Init();
  
  LED_Boot_Init();

#ifdef USE_CRC_PERIPH
  /* DeInit CRC peripheral */
  CRC_DeInit();
  /* Init the INIT register */
  CRC_SetInitRegister(0);
  /* Select 16-bit pol size */
  CRC_PolynomialSizeSelect(CRC_PolSize_16);
  /* Set the polynomial coefficients: x16 + x12 + x5 + 1 used in ymodem file transfer protocol */
  CRC_SetPolynomial(0x1021);
#endif /* USE_CRC_PERIPH */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
