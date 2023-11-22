#ifndef __ADC_H_
#define __ADC_H_
#include <avr/io.h>


#define adc_enable() ADCSRA |= _BV(ADEN)
#define adc_prescaler(pres) ADCSRA |= (pres & 0x07U)
#define adc_start() ADCSRA |= _BV(ADSC)
#define adc_busy() (ADCSRA & _BV(ADSC))
#define adc_reference(ref) ADMUX |= ((ref << 6) & 0xC0U)
#define adc_select(channel) ADMUX |= (channel & 0x0FU)

#define adc_setup(reference, prescaler) \
    adc_reference(reference); \
    adc_prescaler(prescaler); \
    adc_enable()

static inline int adc_read_sync(uint8_t channel)
{

    adc_select(channel);
    adc_start();

    while(adc_busy());

    return ADC;
}


#endif