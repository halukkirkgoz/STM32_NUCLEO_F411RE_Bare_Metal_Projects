#include <stdint.h>

/* Symbols defined in the linker script */
extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

/* Function prototypes */
void Default_Handler(void);
void Reset_Handler(void);
void SystemInit(void) __attribute__((weak));
void __libc_init_array(void);
int main(void);

/* Reuse one attribute form for weak interrupt aliases */
#define WEAK_DEFAULT_HANDLER __attribute__((weak, alias("Default_Handler")))

/* Exception and interrupt handlers */
void NMI_Handler(void) WEAK_DEFAULT_HANDLER;
void HardFault_Handler(void) WEAK_DEFAULT_HANDLER;
void MemManage_Handler(void) WEAK_DEFAULT_HANDLER;
void BusFault_Handler(void) WEAK_DEFAULT_HANDLER;
void UsageFault_Handler(void) WEAK_DEFAULT_HANDLER;
void SVC_Handler(void) WEAK_DEFAULT_HANDLER;
void DebugMon_Handler(void) WEAK_DEFAULT_HANDLER;
void PendSV_Handler(void) WEAK_DEFAULT_HANDLER;
void SysTick_Handler(void) WEAK_DEFAULT_HANDLER;
void WWDG_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void PVD_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void TAMP_STAMP_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void RTC_WKUP_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void FLASH_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void RCC_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void EXTI0_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void EXTI1_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void EXTI2_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void EXTI3_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void EXTI4_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void DMA1_Stream0_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void DMA1_Stream1_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void DMA1_Stream2_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void DMA1_Stream3_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void DMA1_Stream4_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void DMA1_Stream5_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void DMA1_Stream6_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void ADC_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void EXTI9_5_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void TIM1_BRK_TIM9_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void TIM1_UP_TIM10_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void TIM1_TRG_COM_TIM11_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void TIM1_CC_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void TIM2_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void TIM3_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void TIM4_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void I2C1_EV_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void I2C1_ER_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void I2C2_EV_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void I2C2_ER_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void SPI1_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void SPI2_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void USART1_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void USART2_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void EXTI15_10_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void RTC_Alarm_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void OTG_FS_WKUP_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void DMA1_Stream7_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void SDIO_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void TIM5_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void SPI3_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void DMA2_Stream0_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void DMA2_Stream1_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void DMA2_Stream2_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void DMA2_Stream3_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void DMA2_Stream4_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void OTG_FS_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void DMA2_Stream5_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void DMA2_Stream6_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void DMA2_Stream7_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void USART6_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void I2C3_EV_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void I2C3_ER_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void FPU_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void SPI4_IRQHandler(void) WEAK_DEFAULT_HANDLER;
void SPI5_IRQHandler(void) WEAK_DEFAULT_HANDLER;

/* Vector table */
const uint32_t vector_tbl[] __attribute__((used, section(".isr_vector"))) = {
    (uint32_t)&_estack,
    (uint32_t)&Reset_Handler,
    (uint32_t)&NMI_Handler,
    (uint32_t)&HardFault_Handler,
    (uint32_t)&MemManage_Handler,
    (uint32_t)&BusFault_Handler,
    (uint32_t)&UsageFault_Handler,
    0,
    0,
    0,
    0,
    (uint32_t)&SVC_Handler,
    (uint32_t)&DebugMon_Handler,
    0,
    (uint32_t)&PendSV_Handler,
    (uint32_t)&SysTick_Handler,
    (uint32_t)&WWDG_IRQHandler,         /* Window watchdog interrupt */
    (uint32_t)&PVD_IRQHandler,          /* PVD through EXTI line detection interrupt */
    (uint32_t)&TAMP_STAMP_IRQHandler,   /* Tamper and timestamp interrupt */
    (uint32_t)&RTC_WKUP_IRQHandler,     /* RTC wakeup through EXTI line interrupt */
    (uint32_t)&FLASH_IRQHandler,        /* FLASH global interrupt */
    (uint32_t)&RCC_IRQHandler,          /* RCC global interrupt */
    (uint32_t)&EXTI0_IRQHandler,        /* EXTI line 0 interrupt */
    (uint32_t)&EXTI1_IRQHandler,        /* EXTI line 1 interrupt */
    (uint32_t)&EXTI2_IRQHandler,        /* EXTI line 2 interrupt */
    (uint32_t)&EXTI3_IRQHandler,        /* EXTI line 3 interrupt */
    (uint32_t)&EXTI4_IRQHandler,        /* EXTI line 4 interrupt */
    (uint32_t)&DMA1_Stream0_IRQHandler, /* DMA1 Stream0 global interrupt */
    (uint32_t)&DMA1_Stream1_IRQHandler, /* DMA1 Stream1 global interrupt */
    (uint32_t)&DMA1_Stream2_IRQHandler, /* DMA1 Stream2 global interrupt */
    (uint32_t)&DMA1_Stream3_IRQHandler, /* DMA1 Stream3 global interrupt */
    (uint32_t)&DMA1_Stream4_IRQHandler, /* DMA1 Stream4 global interrupt */
    (uint32_t)&DMA1_Stream5_IRQHandler, /* DMA1 Stream5 global interrupt */
    (uint32_t)&DMA1_Stream6_IRQHandler, /* DMA1 Stream6 global interrupt */
    (uint32_t)&ADC_IRQHandler,          /* ADC1 global interrupt */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    (uint32_t)&EXTI9_5_IRQHandler,      /* EXTI lines 5 to 9 interrupts */
    (uint32_t)&TIM1_BRK_TIM9_IRQHandler, /* TIM1 break and TIM9 global interrupt */
    (uint32_t)&TIM1_UP_TIM10_IRQHandler, /* TIM1 update and TIM10 global interrupt */
    (uint32_t)&TIM1_TRG_COM_TIM11_IRQHandler, /* TIM1 trigger/commutation and TIM11 global interrupt */
    (uint32_t)&TIM1_CC_IRQHandler,      /* TIM1 capture compare interrupt */
    (uint32_t)&TIM2_IRQHandler,         /* TIM2 global interrupt */
    (uint32_t)&TIM3_IRQHandler,         /* TIM3 global interrupt */
    (uint32_t)&TIM4_IRQHandler,         /* TIM4 global interrupt */
    (uint32_t)&I2C1_EV_IRQHandler,      /* I2C1 event interrupt */
    (uint32_t)&I2C1_ER_IRQHandler,      /* I2C1 error interrupt */
    (uint32_t)&I2C2_EV_IRQHandler,      /* I2C2 event interrupt */
    (uint32_t)&I2C2_ER_IRQHandler,      /* I2C2 error interrupt */
    (uint32_t)&SPI1_IRQHandler,         /* SPI1 global interrupt */
    (uint32_t)&SPI2_IRQHandler,         /* SPI2 global interrupt */
    (uint32_t)&USART1_IRQHandler,       /* USART1 global interrupt */
    (uint32_t)&USART2_IRQHandler,       /* USART2 global interrupt */
    0,                                  /* Reserved */
    (uint32_t)&EXTI15_10_IRQHandler,    /* EXTI lines 10 to 15 interrupts */
    (uint32_t)&RTC_Alarm_IRQHandler,    /* RTC alarm through EXTI line interrupt */
    (uint32_t)&OTG_FS_WKUP_IRQHandler,  /* USB OTG FS wakeup through EXTI line interrupt */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    (uint32_t)&DMA1_Stream7_IRQHandler, /* DMA1 stream 7 global interrupt */
    0,                                  /* Reserved */
    (uint32_t)&SDIO_IRQHandler,         /* SDIO global interrupt */
    (uint32_t)&TIM5_IRQHandler,         /* TIM5 global interrupt */
    (uint32_t)&SPI3_IRQHandler,         /* SPI3 global interrupt */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    (uint32_t)&DMA2_Stream0_IRQHandler, /* DMA2 stream 0 global interrupt */
    (uint32_t)&DMA2_Stream1_IRQHandler, /* DMA2 stream 1 global interrupt */
    (uint32_t)&DMA2_Stream2_IRQHandler, /* DMA2 stream 2 global interrupt */
    (uint32_t)&DMA2_Stream3_IRQHandler, /* DMA2 stream 3 global interrupt */
    (uint32_t)&DMA2_Stream4_IRQHandler, /* DMA2 stream 4 global interrupt */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    (uint32_t)&OTG_FS_IRQHandler,       /* USB OTG FS global interrupt */
    (uint32_t)&DMA2_Stream5_IRQHandler, /* DMA2 stream 5 global interrupt */
    (uint32_t)&DMA2_Stream6_IRQHandler, /* DMA2 stream 6 global interrupt */
    (uint32_t)&DMA2_Stream7_IRQHandler, /* DMA2 stream 7 global interrupt */
    (uint32_t)&USART6_IRQHandler,       /* USART6 global interrupt */
    (uint32_t)&I2C3_EV_IRQHandler,      /* I2C3 event interrupt */
    (uint32_t)&I2C3_ER_IRQHandler,      /* I2C3 error interrupt */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    (uint32_t)&FPU_IRQHandler,          /* FPU global interrupt */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    (uint32_t)&SPI4_IRQHandler,         /* SPI4 global interrupt */
    (uint32_t)&SPI5_IRQHandler          /* SPI5 global interrupt */

};

/* Default handler that enters an infinite loop */
void Default_Handler(void)
{
    while (1) {
    }
}

/* Reset handler */
void Reset_Handler(void)
{
    /* Perform low-level system initialization before memory setup */
    SystemInit();

    /* Calculate section sizes in bytes */
    uint32_t data_mem_size = (uint32_t)&_edata - (uint32_t)&_sdata;
    uint32_t bss_mem_size = (uint32_t)&_ebss - (uint32_t)&_sbss;

    /* Convert byte sizes to 32-bit word counts to match uint32_t* accesses */
    data_mem_size /= 4;
    bss_mem_size /= 4;

    /* Copy initialized data from its FLASH load address into SRAM */
    uint32_t *p_src_mem = (uint32_t *)&_sidata;
    uint32_t *p_dest_mem = (uint32_t *)&_sdata;

    /* Copy .data section from FLASH to SRAM */
    for (uint32_t i = 0; i < data_mem_size; i++) {
        *p_dest_mem++ = *p_src_mem++;
    }

    /* Zero initialize the .bss section in SRAM */
    p_dest_mem = (uint32_t *)&_sbss;

    for (uint32_t i = 0; i < bss_mem_size; i++) {
        /* Set .bss section words to zero */
        *p_dest_mem++ = 0;
    }

    /* Run static constructors before entering main */
    __libc_init_array();

    /* Call the application's entry point */
    main();

    while (1) {
    }
}
