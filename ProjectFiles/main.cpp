#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <stdio.h>
#include "pico/stdlib.h"

#include "MathClass.h"

MathClass *pMath;

bool state = true;

const uint LED_PIN = PICO_DEFAULT_LED_PIN;
const uint TEST_PIN = 10; 

TickType_t pindelay, leddelay;
bool fast = false;
TimerHandle_t timerHandle;

void led_task(void*)
{   
    while (true)
    {
        gpio_put(LED_PIN, true);
        vTaskDelay(pdMS_TO_TICKS(leddelay));
        gpio_put(LED_PIN, false);
        vTaskDelay(pdMS_TO_TICKS(leddelay));
    }
}

void timerExpired(TimerHandle_t xTimer)
{
    // set pin
    gpio_put(TEST_PIN, state);
    
    // toggle state
    state = !state;
}

int main()
{


    BaseType_t result;
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(TEST_PIN);
    gpio_set_dir(TEST_PIN, GPIO_OUT);

    pindelay = 20;
    leddelay = 250;

    
    pMath = new MathClass(1);

    timerHandle = xTimerCreate("Timer", pdMS_TO_TICKS(pindelay), pdTRUE, (void*)0, timerExpired);

    result = xTimerStart(timerHandle, 0);

    xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL);
    vTaskStartScheduler();

    
    // should never get here
    while(1){};
}
