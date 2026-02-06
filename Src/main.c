#include "main.h"


#include <stdio.h>
#include <unistd.h> // Required for _write() syscall function

UART_HandleTypeDef hlpuart1;

void SystemClock_Config(void);
static void MX_GPIO_LPUART1_Init(void);
static void MX_LPUART1_UART_Init(void);
void BspCOM_Init(void);

/* Retarget printf/puts to USART2 (USB VCP) */
int _write(int file, char *ptr, int len)
{
  // HAL_UART_Transmit(&hlpuart1, (uint8_t*)ptr, (uint16_t)len, HAL_MAX_DELAY);
    if (file == STDOUT_FILENO || file == STDERR_FILENO) {
        (void)HAL_UART_Transmit(&hlpuart1, (uint8_t*)ptr, (uint16_t)len, HAL_MAX_DELAY);
        return len;
    }
    return -1;
}


int main(void){
    //Reset of all peripherals, initialize the flash interface and Systick
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();
    BspCOM_Init();
    // Make printf unbuffered so logs appear immediately
    setvbuf(stdout, NULL, _IONBF, 0);

    printf("Hello World :)");

    while(1){

    }
}

/**
  * @brief Retargets the C library printf function to the LPUART1 peripheral (VCP).
  * This is the standard syscall for ARM-GCC.
  * @param file The file descriptor.
  * @param ptr A pointer to the data to send.
  * @param len The number of bytes to send.
  * @return The number of bytes sent, or -1 on error.
  */
/* Bring up the board "COM" (USB VCP) so printf uses the on-board USB port */
void BspCOM_Init(void)
{
    MX_GPIO_LPUART1_Init();
    MX_LPUART1_UART_Init();

    /* Optional: make stdio unbuffered so logs flush immediately */
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}


/* LPUART on PG7 (TX) / PG8 (RX) for ST-LINK VCP (same USB used to flash) */
static void MX_GPIO_LPUART1_Init(void)
{
    HAL_PWREx_EnableVddIO2();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin       = GPIO_PIN_7 | GPIO_PIN_8; /* PG7 TX, PG8 RX */
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

static void MX_LPUART1_UART_Init(void)
{
  __HAL_RCC_LPUART1_CLK_ENABLE();
  
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 9600;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  hlpuart1.FifoMode = UART_FIFOMODE_DISABLE;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&hlpuart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&hlpuart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void)
{
    //Loop forever
    while(1){

    }
}
