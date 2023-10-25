#include "stm32f4xx.h"

// Define the servo motor control parameters
#define SERVO_TIM TIM4
#define SERVO_CHANNEL TIM_CHANNEL_1
#define SERVO_PWM_PERIOD 20000  // Pulse period in microseconds (20ms)
#define SERVO_MIN_PULSE_WIDTH 1000  // Minimum pulse width for servo's minimum angle
#define SERVO_MAX_PULSE_WIDTH 2000  // Maximum pulse width for servo's maximum angle

// Function to initialize the servo motor
void Servo_Init() {
  // Enable the peripheral clock for TIM4
  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

  // Configure the timer to generate PWM
  SERVO_TIM->PSC = SystemCoreClock / 1000000 - 1;  // 1 MHz
  SERVO_TIM->ARR = SERVO_PWM_PERIOD - 1;

  // Configure the output compare mode and polarity
  SERVO_TIM->CCR1 = SERVO_MIN_PULSE_WIDTH;
  SERVO_TIM->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
  SERVO_TIM->CCER |= TIM_CCER_CC1E;

  // Enable the timer
  SERVO_TIM->CR1 |= TIM_CR1_CEN;
}

// Function to move the servo to a specific angle
void Servo_MoveToAngle(float angle) {
  if (angle < 0.0f) {
    angle = 0.0f;
  }
  if (angle > 180.0f) {
    angle = 180.0f;
  }

  // Convert the desired angle to a corresponding pulse width
  uint32_t pulseWidth = SERVO_MIN_PULSE_WIDTH +
                       (uint32_t)((SERVO_MAX_PULSE_WIDTH - SERVO_MIN_PULSE_WIDTH) * (angle / 180.0f));

  SERVO_TIM->CCR1 = pulseWidth;
}

int main(void) {
  // Configure the system clock
  SystemInit();

  // Initialize the servo motor
  Servo_Init();

  while (1) {
    // Move the servo to a specific angle (e.g., 90 degrees)
    Servo_MoveToAngle(90.0f);
    HAL_Delay(1000);  // Delay for 1 second
    Servo_MoveToAngle(0.0f);
    HAL_Delay(1000);  // Delay for 1 second
    Servo_MoveToAngle(180.0f);
    HAL_Delay(1000);  // Delay for 1 second
  }
}
