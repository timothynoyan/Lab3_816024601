/* STUDENT ID#: 816024601 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"

#include "esp_log.h"
#include "esp_system.h"

/* Including semaphore header file */
#include "freertos/semphr.h"

/* Defining pin for LED */
#define GPIO_OUTPUT 2

unsigned int value;
static void task1(void *pvParam);
static void task2(void *pvParam);
static void task3(void *pvParam);

static const char *TAG = "main";

/* Declaring variable type of SemaphoreHandle_t */
SemaphoreHandle_t mutex_v = NULL;

TaskHandle_t task_handle = NULL;

void app_main(void){	
	/* Struct for configuring pins */
	gpio_config_t io_conf;
    /*disable interrupt*/
    io_conf.intr_type = GPIO_INTR_DISABLE;
    /*set as output mode*/
    io_conf.mode = GPIO_MODE_OUTPUT;
    /*bit mask of the pins that you want to set*/
    io_conf.pin_bit_mask = (1ULL << GPIO_OUTPUT);
    /*disable pull-down mode*/
    io_conf.pull_down_en = 0;
    /*disable pull-up mode*/
    io_conf.pull_up_en = 0;
    /*configure GPIO2 with the given settings*/
    gpio_config(&io_conf);
	
	value = 0;
	
	/* Creating mutex */
	mutex_v = xSemaphoreCreateMutex();
	
	/* Give back Mutex */
	xSemaphoreGive(mutex_v);
	
	/* UNIT TEST: Status Message Function */
	/* Stub for turning LED On */
	gpio_set_level(GPIO_OUTPUT, 1);
	/* Task to call status message function */
	xTaskCreate(task3, "task3", 2048, NULL, 3, &task_handle);
	/* Using task handler to delete running task */
	if (task_handle != NULL){
		vTaskDelete(task_handle);
	}
	gpio_set_level(GPIO_OUTPUT, 0);
	ESP_LOGI(TAG, "UNIT TEST: task3 was successful\n");
}
	
void task1(void *pvParam){
	while (1){
		while (1){
			if (xSemaphoreTake(mutex_v, (TickType_t) 15) == pdTRUE) {
				/* Turning LED on */
				ESP_LOGI(TAG, "LED is turning on...\n");
				gpio_set_level(GPIO_OUTPUT, 1);
				break;
			}
			else{
				continue;
			}
		}
		
		/* Actively wait for 0.5s */
		while (value != 2650000){
		value++;
		}
		
		ESP_LOGI(TAG, "Task 1 is complete.\n");
		
		value = 0;
		
		/* Give back Mutex */
		xSemaphoreGive(mutex_v);
		
		/* Task-delay for 1 second */
		vTaskDelay(1000 / portTICK_RATE_MS);
	}
}

void task2(void *pvParam){
	while (1){
		while (1){
			if (xSemaphoreTake(mutex_v, (TickType_t) 15) == pdTRUE) {
				/* Turning LED off */
				ESP_LOGI(TAG, "LED is turning off...\n");
				gpio_set_level(GPIO_OUTPUT, 0);
				break;
			}
			else{
				continue;
			}
		}
		
		/* Actively wait for 0.5s */
		while (value != 2650000){
		value++;
		}
		
		ESP_LOGI(TAG, "Task 2 is complete.\n");
		
		value = 0;
		
		/* Give back Mutex */
		xSemaphoreGive(mutex_v);
		
		/* Task-delay for 1 second */
		vTaskDelay(1000 / portTICK_RATE_MS);
	}
}

void task3(void *pvParam){
	while (1){
		ESP_LOGI(TAG, "Status of GPIO2: %d\n", gpio_get_level(GPIO_OUTPUT));
		ESP_LOGI(TAG, "Task 3 is complete.\n");
		
		/* Task-delay for 1 second */
		vTaskDelay(1000 / portTICK_RATE_MS);
		}
}
