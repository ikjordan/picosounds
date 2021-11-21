#include "pwm_channel.h"

// Initialise the pwm
void pwmChannelInit(pwm_data* data, uint gpio_num)
{
    pwm_config config = pwm_get_default_config();

    gpio_set_function(gpio_num, GPIO_FUNC_PWM);
    data->pin_slice = pwm_gpio_to_slice_num(gpio_num);
    data->gpio_number = gpio_num;

    pwm_init(data->pin_slice, &config, false);
}

void pwmChannelReconfigure(pwm_data* data, float clk_div, uint16_t wrap)
{
    pwm_config config = pwm_get_default_config();

    pwm_config_set_clkdiv(&config, clk_div); 
    pwm_config_set_wrap(&config, wrap); 

    pwm_init(data->pin_slice, &config, false);

}

// Set the first value inthe pwm
void pwmChannelSetFirstValue(pwm_data* data, uint16_t value)
{
    pwm_set_chan_level(data->pin_slice, pwm_gpio_to_channel(data->gpio_number), value);
}

