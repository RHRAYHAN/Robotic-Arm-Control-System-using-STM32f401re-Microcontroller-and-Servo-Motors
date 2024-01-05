#include "stm32f4xx.h"

#define UI_ADC_PIN      GPIO_PIN_0
#define UI_ADC_PORT     GPIOA
#define SERVO_PIN       GPIO_PIN_1
#define SERVO_PORT      GPIOA

// Function prototypes
void SystemClock_Config(void);
void GPIO_Init(void);
void ADC_Init(void);
void Servo_Init(void);
void UpdateServoPosition(uint16_t flexSensorValue);

int main(void) {
    // MCU Configuration
    SystemClock_Config();

    // Initialize peripherals
    GPIO_Init();
    ADC_Init();
    Servo_Init();

    while (1) {
        // Read flex sensor value from ADC
        ADC1->CR2 |= ADC_CR2_SWSTART;  // Start ADC conversion
        while (!(ADC1->SR & ADC_SR_EOC)) {}  // Wait for conversion to complete
        uint16_t flexSensorValue = ADC1->DR;

        // Update servo position based on flex sensor value
        UpdateServoPosition(flexSensorValue);

        // Add a delay or use interrupts depending on your requirements
        for (volatile int i = 0; i < 1000000; ++i);
    }
}

void SystemClock_Config(void) {
    // Configure the system clock
    // Add your clock configuration code here
}

void GPIO_Init(void) {
    // Initialize GPIO pins for flex sensor (analog input) and servo control (PWM)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // Enable GPIOA clock

    // Flex sensor pin (Analog input)
    GPIOA->MODER |= GPIO_MODER_MODER0;  // Analog mode

    // Servo motor control pin (PWM)
    GPIOA->MODER |= GPIO_MODER_MODER1_1;  // Alternate function mode
    GPIOA->AFR[0] |= 0x01 << (4 * 1);     // AF1 for TIM2_CH2
}

void ADC_Init(void) {
    // Initialize ADC for reading flex sensor values
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;  // Enable ADC1 clock

    ADC1->CR1 = 0;  // Clear CR1 register
    ADC1->CR2 = 0;  // Clear CR2 register

    // Set ADC prescaler to yield a clock frequency within the allowed range (check reference manual)
    RCC->CFGR &= ~RCC_CFGR_ADCPRE;  // Clear ADCPRE bits
    RCC->CFGR |= RCC_CFGR_ADCPRE_DIV4;  // ADCPRE = 4 (divide by 4)

    // Configure ADC for a single conversion in continuous mode
    ADC1->CR2 |= ADC_CR2_CONT;  // Continuous conversion mode
    ADC1->SQR1 |= 0;  // Single conversion (reset value)

    // Set sampling time for the flex sensor channel (adjust based on your requirements)
    ADC1->SMPR2 |= ADC_SMPR2_SMP0;  // 15 cycles for channel 0

    // Configure the flex sensor pin (channel 0)
    ADC1->SQR3 = 0;  // Channel 0

    // Enable ADC
    ADC1->CR2 |= ADC_CR2_ADON;
}

void Servo_Init(void) {
    // Initialize Timer 2 for PWM (adjust as needed)
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;  // Enable TIM2 clock

    TIM2->CR1 = 0;  // Clear control register 1
    TIM2->CR2 = 0;  // Clear control register 2
    TIM2->CCMR1 = 0;  // Clear capture/compare mode register 1
    TIM2->CCER = 0;  // Clear capture/compare enable register
    TIM2->ARR = 20000;  // Set auto-reload register (adjust based on your servo's specifications)
    TIM2->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;  // PWM mode 1
    TIM2->CCER |= TIM_CCER_CC2E;  // Enable capture/compare 2 output
    TIM2->PSC = 83;  // Set prescaler (adjust for your desired PWM frequency)
    TIM2->CR1 |= TIM_CR1_CEN;  // Enable TIM2
}

void UpdateServoPosition(uint16_t flexSensorValue) {
    // Implement servo position update based on flex sensor value
    // Add your servo control code here
    // You may need to map the flex sensor value to a suitable servo position
    // and set the corresponding PWM duty cycle using TIM2->CCR2.
}


