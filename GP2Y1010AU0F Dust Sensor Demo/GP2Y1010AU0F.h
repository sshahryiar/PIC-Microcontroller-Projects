#define ADC_CNT_MAX                    1023.0
#define ADC_VOLT_MAX                   5.0
#define ADC_CONV_FACTOR                (ADC_CNT_MAX / ADC_VOLT_MAX)

#define GP2Y1010AU0F_SAMPLE_TIME       delay_us(280)
#define GP2Y1010AU0F_HOLD_TIME         delay_us(60)
#define GP2Y1010AU0F_SLEEP_TIME        delay_us(9900)

#define GP2Y1010AU0F_MIN_LIMIT         (0.14 * ADC_CNT_MAX)
#define GP2Y1010AU0F_MAX_LIMIT         (0.71 * ADC_CNT_MAX)

#define GP2Y1010AU0F_LED_PIN           pin_B7
#define GP2Y1010AU0F_LED_ON            output_low(GP2Y1010AU0F_LED_PIN)
#define GP2Y1010AU0F_LED_OFF           output_high(GP2Y1010AU0F_LED_PIN)


void GP2Y1010AU0F_init(void);
unsigned long GP2Y1010AU0F_ADC_running_avg(unsigned long previous_avg);
unsigned long GP2Y1010AU0F_read(unsigned long value);

