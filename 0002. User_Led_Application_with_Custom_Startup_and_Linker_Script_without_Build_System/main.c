#include <stdint.h>

/*
 * ------------------------------------------------------------------------
 * Peripheral memory map
 * ------------------------------------------------------------------------
 */

/* Base address of the peripheral memory region */
#define PERIPH_BASE                  ((uint32_t)0x40000000UL)

/* Offset of the AHB1 peripheral bus from the peripheral base */
#define AHB1_PERIPH_OFFSET           ((uint32_t)0x00020000UL)

/* Base address of the AHB1 peripheral bus */
#define AHB1_PERIPH_BASE             (PERIPH_BASE + AHB1_PERIPH_OFFSET)

/* Offset of the GPIOA peripheral within the AHB1 bus */
#define GPIOA_OFFSET                 ((uint32_t)0x00000000UL)

/* Base address of the GPIOA peripheral */
#define GPIOA_BASE                   (AHB1_PERIPH_BASE + GPIOA_OFFSET)

/* Offset of the RCC (Reset and Clock Control) peripheral within the AHB1 bus */
#define RCC_OFFSET                   ((uint32_t)0x00003800UL)

/* Base address of the RCC peripheral */
#define RCC_BASE                     (AHB1_PERIPH_BASE + RCC_OFFSET)

/* Offset of the RCC_AHB1ENR register (AHB1 peripheral clock enable) */
#define RCC_AHB1ENR_OFFSET           ((uint32_t)0x00000030UL)

/* Memory-mapped RCC_AHB1ENR register: enables/disables AHB1 peripheral clocks */
#define RCC_AHB1ENR                  (*(volatile uint32_t *)(RCC_BASE + RCC_AHB1ENR_OFFSET))

/* Offset of the GPIOA_MODER register (GPIO pin mode configuration) */
#define GPIOA_MODER_OFFSET           ((uint32_t)0x00000000UL)

/* Memory-mapped GPIOA_MODER register: configures each pin as input/output/AF/analog */
#define GPIOA_MODER                  (*(volatile uint32_t *)(GPIOA_BASE + GPIOA_MODER_OFFSET))

/* Offset of the GPIOA_OTYPER register (GPIO output type configuration) */
#define GPIOA_OTYPER_OFFSET          ((uint32_t)0x00000004UL)

/* Memory-mapped GPIOA_OTYPER register: selects push-pull or open-drain output type per pin */
#define GPIOA_OTYPER                 (*(volatile uint32_t *)(GPIOA_BASE + GPIOA_OTYPER_OFFSET))

/* Offset of the GPIOA_OSPEEDR register (GPIO output speed configuration) */
#define GPIOA_OSPEEDR_OFFSET         ((uint32_t)0x00000008UL)

/* Memory-mapped GPIOA_OSPEEDR register: selects the output slew rate per pin as low/medium/fast/high speed */
#define GPIOA_OSPEEDR                (*(volatile uint32_t *)(GPIOA_BASE + GPIOA_OSPEEDR_OFFSET))

/* Offset of the GPIOA_PUPDR register (GPIO pull resistors configuration) */
#define GPIOA_PUPDR_OFFSET           ((uint32_t)0x0000000CUL)

/* Memory-mapped GPIOA_PUPDR register: selects pull-up/pull-down/no-pull per pin */
#define GPIOA_PUPDR                  (*(volatile uint32_t *)(GPIOA_BASE + GPIOA_PUPDR_OFFSET))

/* Offset of the GPIOA_ODR register (GPIO output data) */
#define GPIOA_ODR_OFFSET             ((uint32_t)0x00000014UL)

/* Memory-mapped GPIOA_ODR register: holds the output level driven on each pin */
#define GPIOA_ODR                    (*(volatile uint32_t *)(GPIOA_BASE + GPIOA_ODR_OFFSET))

/*
 * ------------------------------------------------------------------------
 * Symbolic constants and static inline functions for GPIO peripheral
 * ------------------------------------------------------------------------
 */

/* Target GPIO pin number */
#define GPIOA_PIN5                    (5U)

#define RCC_AHB1ENR_GPIOAEN_POS       (0U)
#define RCC_AHB1ENR_GPIOAEN_MASK      ((uint32_t)(0x1U) << RCC_AHB1ENR_GPIOAEN_POS)

/* MODER: 2 bits per pin (00 = input, 01 = output, 10 = alternate function, 11 = analog) */
#define GPIO_MODER_BITS_PER_PIN       (2U)

typedef enum gpio_mode
{
    GPIO_MODE_IN = 0U,
    GPIO_MODE_OUT = 1U,
    GPIO_MODE_ALT_FUNC = 2U,
    GPIO_MODE_ANALOG = 3U
} gpio_mode_t;

static inline uint32_t gpio_moder_mask(uint32_t pin)
{
    return ((uint32_t) (0x3U) << (pin * GPIO_MODER_BITS_PER_PIN));
}

static inline uint32_t gpio_moder_mode(uint32_t pin, gpio_mode_t mode)
{
    return ((uint32_t) (mode) << (pin * GPIO_MODER_BITS_PER_PIN));
}

/* OTYPER: 1 bit per pin (0 = push-pull, 1 = open-drain) */
typedef enum gpio_otype
{
    GPIO_OTYPE_PUSH_PULL = 0U,
    GPIO_OTYPE_OPEN_DRAIN = 1U
} gpio_otype_t;

static inline uint32_t gpio_otyper_mask(uint32_t pin)
{
    return ((uint32_t) (0x1U) << pin);
}

static inline uint32_t gpio_otyper_type(uint32_t pin, gpio_otype_t type)
{
    return ((uint32_t) (type) << pin);
}

/* OSPEEDR: 2 bits per pin (00 = low, 01 = medium, 10 = fast, 11 = high speed) */
#define GPIO_OSPEED_BITS_PER_PIN      (2U)

typedef enum gpio_ospeed
{
    GPIO_OSPEED_LOW = 0U,
    GPIO_OSPEED_MEDIUM = 1U,
    GPIO_OSPEED_FAST = 2U,
    GPIO_OSPEED_HIGH = 3U
} gpio_ospeed_t;

static inline uint32_t gpio_ospeedr_mask(uint32_t pin)
{
    return ((uint32_t) (0x3U) << (pin * GPIO_OSPEED_BITS_PER_PIN));
}

static inline uint32_t gpio_ospeedr_speed(uint32_t pin, gpio_ospeed_t speed)
{
    return ((uint32_t) (speed) << (pin * GPIO_OSPEED_BITS_PER_PIN));
}

/* PUPDR: 2 bits per pin (00 = no pull, 01 = pull-up, 10 = pull-down) */
#define GPIO_PUPD_BITS_PER_PIN        (2U)

typedef enum gpio_pupdr
{
    GPIO_PUPD_NONE = 0U,
    GPIO_PUPD_PU = 1U,
    GPIO_PUPD_PD = 2U
} gpio_pupdr_t;

static inline uint32_t gpio_pupdr_mask(uint32_t pin)
{
    return ((uint32_t) (0x3U) << (pin * GPIO_PUPD_BITS_PER_PIN));
}

static inline uint32_t gpio_pupdr_pupd(uint32_t pin, gpio_pupdr_t pupd)
{
    return ((uint32_t) (pupd) << (pin * GPIO_PUPD_BITS_PER_PIN));
}

/*
 * ------------------------------------------------------------------------
 * LED operating mode (build-time configuration)
 * ------------------------------------------------------------------------
 * Two mutually exclusive behaviors are supported and selected at compile
 * time via the LED_MODE macro:
 *   - LED_MODE_TOGGLE:   the LED blinks (toggles) with a fixed busy-wait
 *                        delay between each transition.
 *   - LED_MODE_CONST_ON: the LED is turned on once and stays on forever.
 *
 * If LED_MODE is not defined externally, LED_MODE_TOGGLE is used.
 */
#define LED_MODE_CONST_ON             (0U)
#define LED_MODE_TOGGLE               (1U)

#ifndef LED_MODE
#define LED_MODE LED_MODE_TOGGLE
#endif

#if (LED_MODE != LED_MODE_CONST_ON) && (LED_MODE != LED_MODE_TOGGLE)
#error "LED_MODE must be defined as either LED_MODE_CONST_ON or LED_MODE_TOGGLE"
#endif

/* Number of empty loop iterations used as a crude busy-wait delay between LED toggles */
#define LED_TOGGLE_DELAY_LOOPS        (1000000UL)

/*
 * ------------------------------------------------------------------------
 * Entry point of the program
 * ------------------------------------------------------------------------
 */
int main(void)
{
    /* Enable the peripheral clock for GPIOA before touching any of its registers */
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN_MASK;

    /*
     * Best practice / errata note: on several Cortex-M based MCUs (including
     * STM32), a peripheral's registers must not be accessed immediately
     * after its clock is enabled, because the enable write may still be
     * propagating through the clock tree. Reading the same register back
     * once forces the write to complete before we continue, avoiding a
     * class of intermittent, hard-to-reproduce bugs.
     */
    (void) RCC_AHB1ENR;

    /* Port mode: general purpose output */
    GPIOA_MODER &= ~gpio_moder_mask(GPIOA_PIN5);
    GPIOA_MODER |= gpio_moder_mode(GPIOA_PIN5, GPIO_MODE_OUT);

    /* Output type: push-pull */
    GPIOA_OTYPER &= ~gpio_otyper_mask(GPIOA_PIN5);
    GPIOA_OTYPER |= gpio_otyper_type(GPIOA_PIN5, GPIO_OTYPE_PUSH_PULL);

    /* Output speed: low speed is sufficient for a simple led output operations */
    GPIOA_OSPEEDR &= ~gpio_ospeedr_mask(GPIOA_PIN5);
    GPIOA_OSPEEDR |= gpio_ospeedr_speed(GPIOA_PIN5, GPIO_OSPEED_LOW);

    /* Pull-up/pull-down: none */
    GPIOA_PUPDR &= ~gpio_pupdr_mask(GPIOA_PIN5);
    GPIOA_PUPDR |= gpio_pupdr_pupd(GPIOA_PIN5, GPIO_PUPD_NONE);

    /* Ensure the LED starts in a known OFF state before entering the main loop */
    GPIOA_ODR &= ~((uint32_t) (0x1U) << GPIOA_PIN5);

#if (LED_MODE == LED_MODE_CONST_ON)

    /* Turn the LED on once; it then stays on for the remaining lifetime of the program */
    GPIOA_ODR |= ((uint32_t) (0x1U) << GPIOA_PIN5);

    /* Empty main loop: keep the MCU busy indefinitely */
    for (;;)
    {
    }

#else /* LED_MODE == LED_MODE_TOGGLE */

    /* Main loop: toggle the LED and keep the MCU busy indefinitely */
    for (;;)
    {
        /* Toggle the LED */
        GPIOA_ODR ^= ((uint32_t) (0x1U) << GPIOA_PIN5);

        /* Crude busy-wait delay between toggles */
        for (volatile uint32_t i = 0U; i < LED_TOGGLE_DELAY_LOOPS; i++)
        {
        }
    }

#endif /* LED_MODE */
}
