/*   Copyright (c) 2008, Swedish Institute of Computer Science
 *  All rights reserved.
 *
 *  Additional fixes for AVR contributed by:
 *
 *	Colin O'Flynn coflynn@newae.com
 *	Eric Gnoske egnoske@gmail.com
 *	Blake Leverett bleverett@gmail.com
 *	Mike Vidales mavida404@gmail.com
 *	Kevin Brown kbrown3@uccs.edu
 *	Nate Bohlmann nate@elfwerks.com
 *
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of the copyright holders nor the names of
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 *    \addtogroup atdrvr
 */

/**
 *  \file hal.h
 *  \brief AVR/AT86RF23x hardware abstraction layer header file
 *
 *   $Id: hal.h,v 1.3 2008/11/09 15:39:49 c_oflynn Exp $
*/

#ifndef HAL_AVR_H
#define HAL_AVR_H
/*============================ INCLUDE =======================================*/
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/crc16.h>
#include "contiki-conf.h"
#include "buf.h"

#define TRIG1 DDRB |= 0x04, PINB |= 0x04
#define TRIG2 DDRD |= 0x80, PIND |= 0x80

/* Define all possible revisions here */
#define RAVEN_D	    0
#define RAVENUSB_C  1
#define RCB_B	    2

#if RCB_REVISION == RCB_B
/* 1281 rcb */
#define SSPORT     B
#define SSPIN      (0x00)
#define SPIPORT    B
#define MOSIPIN    (0x02)
#define MISOPIN    (0x03)
#define SCKPIN     (0x01)
#define RSTPORT    B
#define RSTPIN     (0x05)
#define IRQPORT    D
#define IRQPIN     (0x04)
#define SLPTRPORT  B
#define SLPTRPIN   (0x04)
#define USART      1
#define USARTVECT  USART1_RX_vect
#define TICKTIMER  3
#define HAS_SPARE_TIMER

#elif RAVEN_REVISION == RAVEN_D
/* 1284 raven */
#define SSPORT     B
#define SSPIN      (0x04)
#define SPIPORT    B
#define MOSIPIN    (0x05)
#define MISOPIN    (0x06)
#define SCKPIN     (0x07)
#define RSTPORT    B
#define RSTPIN     (0x01)
#define IRQPORT    D
#define IRQPIN     (0x06)
#define SLPTRPORT  B
#define SLPTRPIN   (0x03)
#define TXCWPORT   B
#define TXCWPIN    (0x00)
#define USART      1
#define USARTVECT  USART1_RX_vect
#define TICKTIMER  3
#define HAS_CW_MODE
#define HAS_SPARE_TIMER

#elif RAVEN_REVISION == RAVENUSB_C
/* 1287USB raven */
#define SSPORT     B
#define SSPIN      (0x00)
#define SPIPORT    B
#define MOSIPIN    (0x02)
#define MISOPIN    (0x03)
#define SCKPIN     (0x01)
#define RSTPORT    B
#define RSTPIN     (0x05)
#define IRQPORT    D
#define IRQPIN     (0x04)
#define SLPTRPORT  B
#define SLPTRPIN   (0x04)
#define TXCWPORT   B
#define TXCWPIN    (0x07)
#define USART      1
#define USARTVECT  USART1_RX_vect
#define TICKTIMER  3
#define HAS_CW_MODE
#define HAS_SPARE_TIMER
#else
#error "RAVEN platform undefined in hal.h"
#endif

/**
 * \name Macros used to generate read register names from platform-specific definitions of ports.
 * \brief The various CAT macros (DDR, PORT, and PIN) are used to
 * assign port/pin/DDR names to various macro variables.  The
 * variables are assigned based on the specific connections made in
 * the hardware.  For example TCCR(TICKTIMER,A) can be used in place of TCCR0A
 * if TICKTIMER is defined as 0.
 * \{
 */
#define CAT(x, y)      x##y
#define CAT2(x, y, z)  x##y##z
#define DDR(x)         CAT(DDR,  x)
#define PORT(x)        CAT(PORT, x)
#define PIN(x)         CAT(PIN,  x)
#define UCSR(num, let) CAT2(UCSR,num,let)
#define RXEN(x)        CAT(RXEN,x)
#define TXEN(x)        CAT(TXEN,x)
#define TXC(x)         CAT(TXC,x)
#define RXC(x)         CAT(RXC,x)
#define RXCIE(x)       CAT(RXCIE,x)
#define UCSZ(x,y)      CAT2(UCSZ,x,y)
#define UBRR(x,y)      CAT2(UBRR,x,y)
#define UDRE(x)        CAT(UDRE,x)
#define UDRIE(x)       CAT(UDRIE,x)
#define UDR(x)         CAT(UDR,x)
#define TCNT(x)        CAT(TCNT,x)
#define TIMSK(x)       CAT(TIMSK,x)
#define TCCR(x,y)      CAT2(TCCR,x,y)
#define COM(x,y)       CAT2(COM,x,y)
#define OCR(x,y)       CAT2(OCR,x,y)
#define CS(x,y)        CAT2(CS,x,y)
#define WGM(x,y)       CAT2(WGM,x,y)
#define OCIE(x,y)      CAT2(OCIE,x,y)
#define COMPVECT(x)    CAT2(TIMER,x,_COMPA_vect)
#define UDREVECT(x)    CAT2(USART,x,_UDRE_vect)
#define RXVECT(x)      CAT2(USART,x,_RX_vect)

/* Pin number that corresponds to the SLP_TR pin. */
#define SLP_TR                SLPTRPIN
/*
 * Data Direction Register that corresponds to the port
 * where SLP_TR is connected.
 */
#define DDR_SLP_TR            DDR(SLPTRPORT)

/* Port (Write Access) where SLP_TR is connected. */
#define PORT_SLP_TR           PORT(SLPTRPORT)

/* Pin (Read Access) where SLP_TR is connected. */
#define PIN_SLP_TR            PIN(SLPTRPORT)

/* This macro pulls the SLP_TR pin high. */
#define hal_set_slptr_high() (PORT_SLP_TR |= (1 << SLP_TR))

/* This macro pulls the SLP_TR pin low. */
#define hal_set_slptr_low()  (PORT_SLP_TR &= ~(1 << SLP_TR))

/* Read current state of the SLP_TR pin (High/Low). */
#define hal_get_slptr() (   (PIN_SLP_TR & (1 << SLP_TR)) >> SLP_TR)

/* Pin number that corresponds to the RST pin. */
#define RST                   RSTPIN

/* Data Direction Register that corresponds to the port where RST is */
#define DDR_RST               DDR(RSTPORT)

/* Port (Write Access) where RST is connected. */
#define PORT_RST              PORT(RSTPORT)

/* Pin (Read Access) where RST is connected. */
#define PIN_RST               PIN(RSTPORT)

/* This macro pulls the RST pin high. */
#define hal_set_rst_high()   (PORT_RST |= (1 << RST))

/* This macro pulls the RST pin low. */
#define hal_set_rst_low()    (PORT_RST &= ~(1 << RST))

/* Read current state of the RST pin (High/Low). */
#define hal_get_rst()        ((PIN_RST & (1 << RST) ) >> RST)

/* The slave select pin. */
#define HAL_SS_PIN	SSPIN

/* The SPI module is located on PORTB. */
#define HAL_PORT_SPI	PORT(SPIPORT)

/* Data Direction Register for PORTB. */
#define HAL_DDR_SPI	DDR(SPIPORT)
#define HAL_DD_SS	SSPIN
#define HAL_DD_SCK	SCKPIN
#define HAL_DD_MOSI	MOSIPIN
#define HAL_DD_MISO	MISOPIN

#define HAL_SS_HIGH() (HAL_PORT_SPI |= (1 << HAL_SS_PIN)) /**< MACRO for pulling SS high. */
#define HAL_SS_LOW()  (HAL_PORT_SPI &= ~(1 << HAL_SS_PIN)) /**< MACRO for pulling SS low. */

/** \brief Macros defined for HAL_TIMER1.
 *
 *  These macros are used to define the correct setupt of the AVR's Timer1, and
 *  to ensure that the hal_get_system_time function returns the system time in
 *  symbols (16 us ticks).
 */

#if (F_CPU == 16000000UL)
#define HAL_TCCR1B_CONFIG ((1 << ICES1) | (1 << CS12))
#define HAL_US_PER_SYMBOL (1)
#define HAL_SYMBOL_MASK   (0xFFFFffff)

#elif (F_CPU == 8000000UL)
#define HAL_TCCR1B_CONFIG ((1 << ICES1) | (1 << CS11) | (1 << CS10))
#define HAL_US_PER_SYMBOL (2)
#define HAL_SYMBOL_MASK   (0x7FFFffff)

#elif (F_CPU == 4000000UL)
#define HAL_TCCR1B_CONFIG ((1 << ICES1) | (1 << CS11) | (1 << CS10))
#define HAL_US_PER_SYMBOL (1)
#define HAL_SYMBOL_MASK   (0xFFFFffff)

#elif (F_CPU == 1000000UL)
#define HAL_TCCR1B_CONFIG ((1 << ICES1) | (1 << CS11))
#define HAL_US_PER_SYMBOL (2)
#define HAL_SYMBOL_MASK   (0x7FFFffff)

#else
#error "Clock speed not supported."
#endif


#define RADIO_VECT TIMER1_CAPT_vect
#define HAL_ENABLE_RADIO_INTERRUPT()	(TIMSK1 |= (1 << ICIE1))
#define HAL_DISABLE_RADIO_INTERRUPT()	(TIMSK1 &= ~(1 << ICIE1))

#define HAL_ENABLE_OVERFLOW_INTERRUPT()	(TIMSK1 |= (1 << TOIE1))
#define HAL_DISABLE_OVERFLOW_INTERRUPT() (TIMSK1 &= ~(1 << TOIE1))

/* Enable the interrupt from the radio transceiver */
#define hal_enable_trx_interrupt() HAL_ENABLE_RADIO_INTERRUPT()

/*
 * \brief  Disable the interrupt from the radio transceiver.
 *
 *  \retval 0 if the pin is low, 1 if the pin is high.
 */
#define hal_disable_trx_interrupt() HAL_DISABLE_RADIO_INTERRUPT()

#define HAL_BAT_LOW_MASK	(0x80) /* Mask for the BAT_LOW */
#define HAL_TRX_UR_MASK		(0x40) /* Mask for the TRX_UR */
#define HAL_TRX_END_MASK	(0x08) /* Mask for the TRX_END */
#define HAL_RX_START_MASK	(0x04) /* Mask for the RX_START */
#define HAL_PLL_UNLOCK_MASK	(0x02) /* Mask for the PLL_UNLOCK */
#define HAL_PLL_LOCK_MASK	(0x01) /* Mask for the PLL_LOCK */

#define HAL_MIN_FRAME_LENGTH	(0x03) /* A frame should be at least 3 bytes */
#define HAL_MAX_FRAME_LENGTH	(0x7F) /* A frame should no more than 127 bytes */

typedef enum
{
	TRAC_SUCCESS                = 0,
	TRAC_SUCCESS_DATA_PENDING   = 1,
	TRAC_SUCCESS_WAIT_FOR_ACK   = 2,
	TRAC_CHANNEL_ACCESS_FAILURE = 3,
	TRAC_NO_ACK                 = 5,
	TRAC_INVALID                = 7
} trac_status_t;

void hal_init(void);
U8 hal_get_pll_lock_flag(void);
void hal_clear_pll_lock_flag(void);
U8 hal_register_read(U8 address);
void hal_register_write(U8 address, U8 value);
U8 hal_subregister_read(U8 address, U8 mask, U8 position);
void hal_subregister_write(U8 address, U8 mask, U8 position, U8 value);
void hal_frame_read(buffer_t *buf);
void hal_frame_write(U8 *data, U8 len);
void hal_sram_read(U8 address, U8 length, U8 *data);
void hal_sram_write(U8 address, U8 length, U8 *data);

#endif