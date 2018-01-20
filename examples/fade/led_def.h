#ifndef __LED_DEF_H__
#define __LED_DEF_H__

#ifdef LED_BUILTIN

#define LED_PIN LED_BUILTIN

#else // LED_BUILTIN

#ifdef ARDUINO_AVR_DIGISPARK
#define LED_PIN 1
#endif // ARDUINO_AVR_DIGISPARK

#endif // LED_BUILTIN

#endif // __LED_DEF_H__
