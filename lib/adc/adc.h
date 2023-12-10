#ifndef __ADC_H_
#define __ADC_H_
#include <avr/io.h>

/**
 * @brief Enables the ADC engine.
 * 
 */
#define adc_enable() ADCSRA |= _BV(ADEN)

/**
 * @brief Starts the ADC engine acquisition.
 * 
 */
#define adc_start() ADCSRA |= _BV(ADSC)

/**
 * @brief Checks weither the ADC engine is busy after starting the acquisition.
 * 
 */
#define adc_busy() (ADCSRA & _BV(ADSC))

#define ADC_PRESCALER_2     1
#define ADC_PRESCALER_4     2
#define ADC_PRESCALER_8     3
#define ADC_PRESCALER_16    4
#define ADC_PRESCALER_32    5
#define ADC_PRESCALER_64    6
#define ADC_PRESCALER_128   7

/**
 * @brief Determine the division factor between the system 
 *      clock frequency and the input clock to the ADC.
 * @param pres The prescaler between 0..7
 *              0 .... 2
 *              1 .... 2
 *              2 .... 4
 *              ........
 *              ........
 *              7 .... 128
 */
#define adc_prescaler(pres) ADCSRA |= (pres & 0x07U)

#define ADC_AREF_PIN                        0x0
#define ADC_AVCC_AREF_CAPACITY              0x1
#define ADC_INTERNAL1_1V_AREF_CAPACITY      0x3

/**
 * @brief Set an ADC reference
 * @param ref the Reference for the ADC Comparator
 *
 *       00b ---> AREF pin, internal VREF turned off
 *       01b ---> AVCC with external capacitor at AREF pin
 *       10b ---> Reserved
 *       11b ---> Internal 1.1V voltage reference with external capacitor at AREF pin
 */
#define adc_reference(ref) ADMUX |= ((ref << 6) & 0xC0U)

/**
 * @brief Activate an ADC Channel
 * @param channel an ADC channel number between 0 and 8.
 */
#define adc_select(channel) ADMUX |= (channel & 0x0FU)

/**
 * @brief setups and enables the ADC engine.
 * @param refernce @see adc_reference
 * @param prescaler @see adc_prescaler
 */
#define adc_setup(reference, prescaler) \
    adc_reference(reference); \
    adc_prescaler(prescaler); \
    adc_enable()

#define ADC_CHANNEL_0_PC0           0
#define ADC_CHANNEL_1_PC1           1
#define ADC_CHANNEL_2_PC2           2
#define ADC_CHANNEL_3_PC3           3
#define ADC_CHANNEL_4_PC4           4
#define ADC_CHANNEL_5_PC5           5
#define ADC_CHANNEL_6_ADC6          6
#define ADC_CHANNEL_7_ADC7          7
#define ADC_CHANNEL_8_ADC8          8

/**
 * @brief Read an ADC channel on given channel.
 * 
 * @param channel an ADC channel number between 0 and 8 (ADC8 for temperature) to read.
 * @return int the result of the ADC Channel
 */
static inline int adc_read_sync(uint8_t channel)
{

    adc_select(channel);
    adc_start();

    while(adc_busy());

    return ADC;
}


#endif