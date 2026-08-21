#include <stdint.h>

/*
 * ------------------------------------------------------------------------
 * Peripheral access qualifiers
 * ------------------------------------------------------------------------
 * __IO: read/write register field
 * __I : read-only register field
 * __O : write-only register field
 * __R : reserved register field
 */
#ifndef __IO
#define __IO volatile
#endif
#ifndef __I
#define __I volatile const
#endif
#ifndef __O
#define __O volatile
#endif
#ifndef __R
#define __R  volatile const
#endif

/*
 * ------------------------------------------------------------------------
 * GPIO peripheral register map matching the memory layout of one GPIO port instance
 * ------------------------------------------------------------------------
 */
typedef struct
{
    __IO uint32_t MODER;   /* Offset: 0x00 */
    __IO uint32_t OTYPER;  /* Offset: 0x04 */
    __IO uint32_t OSPEEDR; /* Offset: 0x08 */
    __IO uint32_t PUPDR;   /* Offset: 0x0C */
    __I uint32_t IDR;      /* Offset: 0x10 */
    __IO uint32_t ODR;     /* Offset: 0x14 */
    __O uint32_t BSRR;     /* Offset: 0x18 */
    __IO uint32_t LCKR;    /* Offset: 0x1C */
    __IO uint32_t AFRL;    /* Offset: 0x20 */
    __IO uint32_t AFRH;    /* Offset: 0x24 */
} GPIO_Regs_t;

/*
 * ------------------------------------------------------------------------
 * RCC peripheral register map matching the memory layout of the reset and clock control block
 * ------------------------------------------------------------------------
 */
typedef struct
{
    __IO uint32_t CR;           /* Offset: 0x00 */
    __IO uint32_t PLLCFGR;      /* Offset: 0x04 */
    __IO uint32_t CFGR;         /* Offset: 0x08 */
    __IO uint32_t CIR;          /* Offset: 0x0C */
    __IO uint32_t AHB1RSTR;     /* Offset: 0x10 */
    __IO uint32_t AHB2RSTR;     /* Offset: 0x14 */
    __R uint32_t RESERVED1[2];  /* Offset: 0x18 and 0x1C */
    __IO uint32_t APB1RSTR;     /* Offset: 0x20 */
    __IO uint32_t APB2RSTR;     /* Offset: 0x24 */
    __R uint32_t RESERVED2[2];  /* Offset: 0x28 and 0x2C */
    __IO uint32_t AHB1ENR;      /* Offset: 0x30 */
    __IO uint32_t AHB2ENR;      /* Offset: 0x34 */
    __R uint32_t RESERVED3[2];  /* Offset: 0x38 and 0x3C */
    __IO uint32_t APB1ENR;      /* Offset: 0x40 */
    __IO uint32_t APB2ENR;      /* Offset: 0x44 */
    __R uint32_t RESERVED4[2];  /* Offset: 0x48 and 0x4C */
    __IO uint32_t AHB1LPENR;    /* Offset: 0x50 */
    __IO uint32_t AHB2LPENR;    /* Offset: 0x54 */
    __R uint32_t RESERVED5[2];  /* Offset: 0x58 and 0x5C */
    __IO uint32_t APB1LPENR;    /* Offset: 0x60 */
    __IO uint32_t APB2LPENR;    /* Offset: 0x64 */
    __R uint32_t RESERVED6[2];  /* Offset: 0x68 and 0x6C */
    __IO uint32_t BDCR;         /* Offset: 0x70 */
    __IO uint32_t CSR;          /* Offset: 0x74 */
    __R uint32_t RESERVED7[2];  /* Offset: 0x78 and 0x7C */
    __IO uint32_t SSCGR;        /* Offset: 0x80 */
    __IO uint32_t PLLI2SCFGR;   /* Offset: 0x84 */
    __R uint32_t RESERVED8;     /* Offset: 0x88 */
    __IO uint32_t DCKCFGR;      /* Offset: 0x8C */
} RCC_Regs_t;

/* Base address of the GPIOA peripheral */
#define GPIOA_BASE ((uint32_t)0x40020000UL)

/* Base address of the RCC peripheral */
#define RCC_BASE  ((uint32_t)0x40023800UL)

/* Typed-casted peripheral instances derived from their corresponding base addresses */
#define GPIOA ((GPIO_Regs_t *)(GPIOA_BASE))
#define RCC   ((RCC_Regs_t *)(RCC_BASE))

/*
 * ------------------------------------------------------------------------
 * Symbolic constants and static inline functions for GPIO peripheral
 * ------------------------------------------------------------------------
 */

/* Target GPIO pin number */
#define GPIOA_PIN5 (5U)

#define RCC_AHB1ENR_GPIOAEN_POS (0U)
#define RCC_AHB1ENR_GPIOAEN_MASK ((uint32_t)(0x1U) << RCC_AHB1ENR_GPIOAEN_POS)

/* MODER: 2 bits per pin (00 = input, 01 = output, 10 = alternate function, 11 = analog) */
#define GPIO_MODER_BITS_PER_PIN (2U)

typedef enum gpio_mode
{
    GPIO_MODE_IN = 0U,
    GPIO_MODE_OUT = 1U,
    GPIO_MODE_ALT_FUNC = 2U,
    GPIO_MODE_ANALOG = 3U
} gpio_mode_t;

static inline uint32_t gpio_moder_mask(uint32_t pin)
{
    return ((uint32_t)(0x3U) << (pin * GPIO_MODER_BITS_PER_PIN));
}

static inline uint32_t gpio_moder_mode(uint32_t pin, gpio_mode_t mode)
{
    return ((uint32_t)(mode) << (pin * GPIO_MODER_BITS_PER_PIN));
}

/* OTYPER: 1 bit per pin (0 = push-pull, 1 = open-drain) */
typedef enum gpio_otype
{
    GPIO_OTYPE_PUSH_PULL = 0U,
    GPIO_OTYPE_OPEN_DRAIN = 1U
} gpio_otype_t;

static inline uint32_t gpio_otyper_mask(uint32_t pin)
{
    return ((uint32_t)(0x1U) << pin);
}

static inline uint32_t gpio_otyper_type(uint32_t pin, gpio_otype_t type)
{
    return ((uint32_t)(type) << pin);
}

/* OSPEEDR: 2 bits per pin (00 = low, 01 = medium, 10 = fast, 11 = high speed) */
#define GPIO_OSPEED_BITS_PER_PIN (2U)

typedef enum gpio_ospeed
{
    GPIO_OSPEED_LOW = 0U,
    GPIO_OSPEED_MEDIUM = 1U,
    GPIO_OSPEED_FAST = 2U,
    GPIO_OSPEED_HIGH = 3U
} gpio_ospeed_t;

static inline uint32_t gpio_ospeedr_mask(uint32_t pin)
{
    return ((uint32_t)(0x3U) << (pin * GPIO_OSPEED_BITS_PER_PIN));
}

static inline uint32_t gpio_ospeedr_speed(uint32_t pin, gpio_ospeed_t speed)
{
    return ((uint32_t)(speed) << (pin * GPIO_OSPEED_BITS_PER_PIN));
}

/* PUPDR: 2 bits per pin (00 = no pull, 01 = pull-up, 10 = pull-down) */
#define GPIO_PUPD_BITS_PER_PIN (2U)

typedef enum gpio_pupdr
{
    GPIO_PUPD_NONE = 0U,
    GPIO_PUPD_PU = 1U,
    GPIO_PUPD_PD = 2U
} gpio_pupdr_t;

static inline uint32_t gpio_pupdr_mask(uint32_t pin)
{
    return ((uint32_t)(0x3U) << (pin * GPIO_PUPD_BITS_PER_PIN));
}

static inline uint32_t gpio_pupdr_pupd(uint32_t pin, gpio_pupdr_t pupd)
{
    return ((uint32_t)(pupd) << (pin * GPIO_PUPD_BITS_PER_PIN));
}

/* BSRR: 1 bit to set, 1 bit to reset per pin */
static inline uint32_t gpio_bsrr_set(uint32_t pin)
{
    return ((uint32_t)(0x1U) << pin);
}

static inline uint32_t gpio_bsrr_reset(uint32_t pin)
{
    return ((uint32_t)(0x1U) << (pin + 16U));
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
#define LED_MODE_CONST_ON (0U)
#define LED_MODE_TOGGLE (1U)

#ifndef LED_MODE
#define LED_MODE LED_MODE_TOGGLE
#endif

#if (LED_MODE != LED_MODE_CONST_ON) && (LED_MODE != LED_MODE_TOGGLE)
#error "LED_MODE must be defined as either LED_MODE_CONST_ON or LED_MODE_TOGGLE"
#endif

/* Number of empty loop iterations used as a crude busy-wait delay between LED toggles */
#define LED_TOGGLE_DELAY_LOOPS (100000UL)

/*
 * ------------------------------------------------------------------------
 * Entry point of the program
 * ------------------------------------------------------------------------
 */
int main(void)
{
    /* Enable the peripheral clock for GPIOA before touching any of its registers */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN_MASK;

    /*
     * Best practice / errata note: on several Cortex-M based MCUs (including
     * STM32), a peripheral's registers must not be accessed immediately
     * after its clock is enabled, because the enable write may still be
     * propagating through the clock tree. Reading the same register back
     * once forces the write to complete before we continue, avoiding a
     * class of intermittent, hard-to-reproduce bugs.
     */
    (void) RCC->AHB1ENR;

    /* Port mode: general purpose output */
    GPIOA->MODER &= ~gpio_moder_mask(GPIOA_PIN5);
    GPIOA->MODER |= gpio_moder_mode(GPIOA_PIN5, GPIO_MODE_OUT);

    /* Output type: push-pull */
    GPIOA->OTYPER &= ~gpio_otyper_mask(GPIOA_PIN5);
    GPIOA->OTYPER |= gpio_otyper_type(GPIOA_PIN5, GPIO_OTYPE_PUSH_PULL);

    /* Output speed: low speed is sufficient for a simple led output operations */
    GPIOA->OSPEEDR &= ~gpio_ospeedr_mask(GPIOA_PIN5);
    GPIOA->OSPEEDR |= gpio_ospeedr_speed(GPIOA_PIN5, GPIO_OSPEED_LOW);

    /* Pull-up/pull-down: none */
    GPIOA->PUPDR &= ~gpio_pupdr_mask(GPIOA_PIN5);
    GPIOA->PUPDR |= gpio_pupdr_pupd(GPIOA_PIN5, GPIO_PUPD_NONE);

    /* Ensure the LED starts in a known OFF state before entering the main loop */
    GPIOA->BSRR = gpio_bsrr_reset(GPIOA_PIN5);

#if (LED_MODE == LED_MODE_CONST_ON)

    /* Turn the LED on once; it then stays on for the remaining lifetime of the program */
    GPIOA->BSRR = gpio_bsrr_set(GPIOA_PIN5);

    /* Empty main loop: keep the MCU busy indefinitely */
    for (;;) {
    }

#else /* LED_MODE == LED_MODE_TOGGLE */

    /* Main loop: toggle the LED and keep the MCU busy indefinitely */
    for (;;)
    {
        /* Toggle the LED */
        GPIOA->ODR ^= ((uint32_t)(0x1U) << GPIOA_PIN5);

        /* Crude busy-wait delay between toggles */
        for (volatile uint32_t i = 0U; i < LED_TOGGLE_DELAY_LOOPS; i++)
        {
        }
    }

#endif /* LED_MODE */
}
