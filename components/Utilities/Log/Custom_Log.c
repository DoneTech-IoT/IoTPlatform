#include"Custom_Log.h"
#include "string.h"
const char *TAG = "Heap log";
static Log Log2;


uint8_t FindComponentLocationInPool(Log *Log, char *Component);
uint8_t EmptyPlaceInComponentPool(Log *Log);
uint8_t FindEventInEventPool(Log *Log, int ComponentNumber, char *EventName);
void RecordStatus(Log *Log, int ComponentNumber, int EventNumber, int FistTimeFlag);
uint8_t IsEventExist(Log *Log, int ComponentNumber, char *EventName);
uint8_t IsComponentExist(Log *Log, char *ComponentName);
void RamOccupyFunction(char *Component, char *EventName);
void RamStatusFunction(char *ComponentName, char *EventName);
uint8_t NumberSavedEvent(Log *Log, char *ComponentName);
void ReportComponentRamUsed(char *ComponentName);
void ReportComponentRamStatus(char *ComponentName);
uint8_t EmptyPlaceInEventPool(Log *Log, int ComponentNumber);

void RamOccupyFunction(char *Component, char *EventName)
{
    static Log log ;
    int componentNumber;
    int eventNumber;

    printf("Initial state:\n");
    printf("Component: %s, Event: %s\n", Component, EventName);

    if (IsComponentExist(&log, Component) == false)
    {
        componentNumber = 0;
        eventNumber = 0;
        strncpy(log.Component[componentNumber].Name, Component, STRING_LEN);
        strncpy(log.Component[componentNumber].Event[eventNumber].Name,
                EventName, STRING_LEN);
        printf("State 2:\nComponentNumber: %d, EventNumber: %d, ComponentName: %s, EventName: %s\n",
               componentNumber, eventNumber, log.Component[componentNumber].Name,
               log.Component[componentNumber].Event[eventNumber].Name);
        RecordStatus(&log, componentNumber, eventNumber, LogStart);
    }
    else
    {
        componentNumber = FindComponentLocationInPool(&log, Component);
        printf("State 3:\nComponentNumber: %d, ComponentName: %s\n",
               componentNumber, log.Component[componentNumber].Name);
        if (componentNumber == ERROR_CODE)
        {
            printf("Error: there is not any space in Component pool for creating new component\n");
            return;
        }
        else
        {
            uint8_t isEventExist = IsEventExist(&log, componentNumber, EventName);
            printf("State 5:\nComponentNumber: %d, EventExist: %d\n",
                   componentNumber);
            if (isEventExist == 1)
            {
                eventNumber = FindEventInEventPool(&log, componentNumber, EventName);
                strncpy(log.Component[componentNumber].Event[eventNumber].Name,
                        EventName, STRING_LEN);
                printf("State 7:\nComponentNumber: %d, EventNumber: %d, EventName: %s\n",
                       componentNumber, eventNumber,
                       log.Component[componentNumber].Event[eventNumber].Name);
                if (log.Component[componentNumber].Event[eventNumber].Counter == 2)
                    RecordStatus(&log, componentNumber, eventNumber, LogStart); // add log
                else
                    RecordStatus(&log, componentNumber, eventNumber, LogEnd); // add log
            }
            else if (isEventExist == 0)
            {
                eventNumber = EmptyPlaceInEventPool(&log, componentNumber);
                if (eventNumber == ERROR_CODE)
                {
                    printf("Error: there is not any space in Event pool for creating new event\n");
                    return;
                }
                else
                {
                    strncpy(log.Component[componentNumber].Event[eventNumber].Name, EventName, STRING_LEN);
                    printf("State 9:\nComponentNumber: %d, EventNumber: %d, EventName: %s\n",
                           componentNumber, eventNumber,
                           log.Component[componentNumber].Event[eventNumber].Name);
                    RecordStatus(&log, componentNumber, eventNumber, LogStart); // add log
                }
            }
        }
    }
}

void RecordStatus(Log *Log, int ComponentNumber, int EventNumber, int FistTimeFlag)
{

    if (FistTimeFlag == 1)
    {
        Log->Component[ComponentNumber].Event[EventNumber].RAM.Psram = (esp_get_free_heap_size() / 1000);
        Log->Component[ComponentNumber].Event[EventNumber].RAM.Sram = (xPortGetFreeHeapSize() / 1000);
        print("state  3 ,SRAM: %u K bytes PSRAM K bytes occupy: %u",
              Log->Component[ComponentNumber].Event[EventNumber].RAM.Sram,
              Log->Component[ComponentNumber].Event[EventNumber].RAM.Psram);
        Log->Component[ComponentNumber].Event[EventNumber].Counter = 1;
    }
    else if (FistTimeFlag == 0)
    {
        size_t psramSize = Log->Component[ComponentNumber].Event[EventNumber].RAM.Psram;
        size_t sramSize = Log->Component[ComponentNumber].Event[EventNumber].RAM.Sram;
        psramSize = psramSize - Log->Component[ComponentNumber].Event[EventNumber].RAM.Psram;
        sramSize = sramSize - Log->Component[ComponentNumber].Event[EventNumber].RAM.Sram;
        size_t TimeFromBootUp = pdTICKS_TO_MS(xTaskGetTickCount());
        ESP_LOGE(Log->Component[ComponentNumber].Name, "Event name :%s SRAM: %u K bytes PSRAM occupy: %u K bytes occupy at %u millis",
                 Log->Component[ComponentNumber].Event[EventNumber].Name,
                 sramSize, psramSize, TimeFromBootUp);
        Log->Component[ComponentNumber].Event[EventNumber].Counter = 2;
    }
}
uint8_t IsComponentExist(Log *Log, char *ComponentName)
{
    for (int i = 0; i < LOG_MAX_COMPONENT; i++)
    {
        if (0 == (strcmp(Log->Component[i].Name, ComponentName)))
        {
            return true;
        }
    }
    return false;
}
uint8_t IsEventExist(Log *Log, int ComponentNumber, char *EventName)
{
    for (int i = 0; i < LOG_MAX_EVENT; i++)
    {
        if (0 == (strcmp(Log->Component[ComponentNumber].Event[i].Name, EventName)))
        {
            return true;
        }
    }
    return false;
}
uint8_t FindEventInEventPool(Log *Log, int ComponentNumber, char *EventName)
{
    uint8_t eventNumber = ERROR_CODE;
    for (int i = 0; i < LOG_MAX_EVENT; i++)
    {
        if (0 == (strcmp(Log->Component[ComponentNumber].Event[i].Name, EventName)))
        {
            eventNumber = i;
            break;
        }
    }
    return eventNumber;
}
uint8_t FindComponentLocationInPool(Log *Log, char *Component)
{
    uint8_t componentNumber = ERROR_CODE;
    for (int i = 0; i < LOG_MAX_COMPONENT; i++)
    {
        if (0 == (strcmp(Log->Component[i].Name, Component)))
        {
            componentNumber = i;
        }
    }
    return componentNumber;
}
uint8_t EmptyPlaceInComponentPool(Log *Log)
{
    uint8_t emptyPlace = ERROR_CODE;
    for (int i = 0; i < LOG_MAX_COMPONENT; i++)
    {
        if (strlen(Log->Component[i].Name) == 0)
        {
            emptyPlace = i;
            break;
        }
    }
    return emptyPlace;
}
uint8_t EmptyPlaceInEventPool(Log *Log, int ComponentNumber)
{
    uint8_t emptyPlace = ERROR_CODE;
    for (int i = 0; i < LOG_MAX_COMPONENT; i++)
    {
        if (strlen(Log->Component[ComponentNumber].Event[i].Name) == 0)
        {
            emptyPlace = i;
            break;
        }
    }
    return emptyPlace;
}
uint8_t NumberSavedEvent(Log *Log, char *ComponentName)
{
    uint8_t numberOfEvent = 0;
    for (int i = 0; i < LOG_MAX_EVENT; i++)
    {
        if (strlen(Log->Component[ComponentName].Event[i].Name) != 0)
            numberOfEvent++;
    }
    return numberOfEvent;
}
void RamStatusFunction(char *ComponentName, char *EventName)
{
    int componentNumber;
    int eventNumber;
    size_t psramSize;
    size_t sramSize;
    if (IsComponentExist(&Log2, ComponentName) == false)
    {
        componentNumber = 0;
        eventNumber = 0;
        psramSize = (esp_get_free_heap_size() / 1000);
        sramSize = (xPortGetFreeHeapSize() / 1000);
        size_t TimeFromBootUp = pdTICKS_TO_MS(xTaskGetTickCount());
        strncpy(Log2.Component[componentNumber].Name, ComponentName, strlen(ComponentName));
        strncpy(Log2.Component[componentNumber].Event[eventNumber].Name, EventName, strlen(EventName));
        Log2.Component[componentNumber].Event[eventNumber].RAM.Psram = psramSize;
        Log2.Component[componentNumber].Event[eventNumber].RAM.Sram = sramSize;
        Log2.Component[componentNumber].Event[eventNumber].TimeStamp = TimeFromBootUp;
        print("component:%s event:%s , SRAM: %u K bytes , PSRAM: %u K bytes at %d millis is FREE", ComponentName,
              EventName,
              sramSize,
              psramSize,
              TimeFromBootUp);
    }
    else
    {
        psramSize = (esp_get_free_heap_size() / 1000);
        sramSize = (xPortGetFreeHeapSize() / 1000);
        size_t TimeFromBootUp = pdTICKS_TO_MS(xTaskGetTickCount());
        componentNumber = FindComponentLocationInPool(&Log2, ComponentName);
        eventNumber = EmptyPlaceInEventPool(&Log2, componentNumber);
        if ( NumberSavedEvent(&Log2,ComponentName)>= (LOG_MAX_EVENT - 1))
            memset(&Log2.Component[componentNumber], 0, sizeof(Log2.Component[componentNumber]));
        strncpy(Log2.Component[componentNumber].Name, ComponentName, strlen(ComponentName));
        strncpy(Log2.Component[componentNumber].Event[eventNumber].Name, EventName, strlen(EventName));
        Log2.Component[componentNumber].Event[eventNumber].RAM.Psram = psramSize;
        Log2.Component[componentNumber].Event[eventNumber].RAM.Sram = sramSize;
        Log2.Component[componentNumber].Event[eventNumber].TimeStamp = TimeFromBootUp;
        print("component:%s event:%s , SRAM: %u K bytes , PSRAM: %u K bytes at %d millis is FREE",
              ComponentName,
              EventName,
              sramSize,
              psramSize,
              TimeFromBootUp);
    }
}
void ReportComponentRamStatus(char *ComponentName)
{
    int componentNumber;
    if (IsComponentExist(&Log2, ComponentName) == false)
    {
        print("there is not any Log for Component %s", ComponentName);
    }
    else
    {
        componentNumber = FindComponentLocationInPool(&Log2, ComponentName);
        for (int i = 0; i < LOG_MAX_EVENT; i++)
        {
            if (strlen(Log2.Component[componentNumber].Event[i].Name) != 0)
            {
                print("component:%s event:%s , SRAM: %u K bytes , PSRAM: %u K bytes at %d millis is FREE",
                      ComponentName,
                      Log2.Component[componentNumber].Event[i].Name,
                      Log2.Component[componentNumber].Event[i].RAM.Sram,
                      Log2.Component[componentNumber].Event[i].RAM.Psram,
                      Log2.Component[componentNumber].Event[i].TimeStamp);
            }
            else
                break;
        }
    }
}
void ReportComponentRamUsed(char *ComponentName)
{
    int componentNumber;
    if (IsComponentExist(&Log2, ComponentName) == false)
    {
        print("there is not any Log for Component %s", ComponentName);
    }
    else
    {
        componentNumber = FindComponentLocationInPool(&Log2, ComponentName);
        for (int i = 0; i < LOG_MAX_EVENT; i++)
        {
            if (strlen(Log2.Component[componentNumber].Event[i].Name) != 0)
            {
                print("component:%s event:%s , SRAM: %u K bytes , PSRAM: %u K bytes at %d millis is occupy",
                      ComponentName,
                      Log2.Component[componentNumber].Event[i].Name,
                      Log2.Component[componentNumber].Event[i].RAM.Sram,
                      Log2.Component[componentNumber].Event[i].RAM.Psram,
                      Log2.Component[componentNumber].Event[i].TimeStamp);
            }
            else
                break;
        }
    }
}

/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/


// #include <stdio.h>
// #include <string.h>
// #include <stdint.h>
// #include <stdbool.h>

// #define LOG_MAX_EVENT 40
// #define LOG_MAX_COMPONENT 3
// #define STRING_LEN 20
// #define ERROR_CODE 100

// typedef enum
// {
//   LogEnd = 0,
//   LogStart
// } LogFlag;

// typedef struct
// {
//   size_t Psram;
//   size_t Sram;
// } Ram_srt;

// typedef struct
// {
//   Ram_srt RAM;
//   char Name[STRING_LEN];
//   size_t TimeStamp;
//   uint8_t Counter;
// } Event_str;

// typedef struct
// {
//   Event_str Event[LOG_MAX_EVENT];
//   char Name[STRING_LEN];

// } Component_str;

// typedef struct
// {
//   Component_str Component[LOG_MAX_COMPONENT];
// } Log;
// static Log Log2;
// // Mock functions for the ESP-IDF specific functions
// size_t
// esp_get_free_heap_size ()
// {
//   return 5000;
// }

// size_t
// xPortGetFreeHeapSize ()
// {
//   return 4000;
// }

// size_t
// pdTICKS_TO_MS (size_t ticks)
// {
//   return ticks * 10;
// }								// Assume each tick is 10ms

// size_t
// xTaskGetTickCount ()
// {
//   return 100;
// }

// uint8_t FindComponentLocationInPool (Log * Log, char *Component);
// uint8_t EmptyPlaceInComponentPool (Log * Log);
// uint8_t FindEventInEventPool (Log * Log, int ComponentNumber,
// 							  char *EventName);
// void RecordStatus (Log * Log, int ComponentNumber, int EventNumber,
// 				   int FirstTimeFlag);
// uint8_t IsEventExist (Log * Log, int ComponentNumber, char *EventName);
// uint8_t IsComponentExist (Log * Log, char *ComponentName);
// uint8_t EmptyPlaceInEventPool (Log * Log, int ComponentNumber);
// void
// RamOccupyFunction (char *Component, char *EventName)
// {
//   static Log log = { 0 };		// Ensure the Log is initialized once
//   int componentNumber;
//   int eventNumber;

// //   printf ("Initial state:\n");
// //   printf ("Component: %s, Event: %s\n", Component, EventName);

//   if (IsComponentExist (&log, Component) == false)
// 	{
// 	  componentNumber = 0;
// 	  eventNumber = 0;
// 	  strncpy (log.Component[componentNumber].Name, Component, STRING_LEN);
// 	  strncpy (log.Component[componentNumber].Event[eventNumber].Name,
// 			   EventName, STRING_LEN);
// // 	  printf
// // 		("State 2:\nComponentNumber: %d, EventNumber: %d, ComponentName: %s, EventName: %s\n",
// // 		 componentNumber, eventNumber, log.Component[componentNumber].Name,
// // 		 log.Component[componentNumber].Event[eventNumber].Name);
// 	  RecordStatus (&log, componentNumber, eventNumber, LogStart);
// 	}
//   else
// 	{
// 	  componentNumber = FindComponentLocationInPool (&log, Component);
// // 	  printf ("State 3:\nComponentNumber: %d, ComponentName: %s\n",
// // 			  componentNumber, log.Component[componentNumber].Name);
// 	  if (componentNumber == ERROR_CODE)
// 		{
// 		  printf
// 			("Error: there is not any space in Component pool for creating new component\n");
// 		  return;
// 		}
// 	  else
// 		{
// 		  uint8_t isEventExist =
// 			IsEventExist (&log, componentNumber, EventName);
// 		  //printf ("State 5:\nComponentNumber: %d, EventExist: %d\n",
// 				//   componentNumber);
// 		  if (isEventExist == 1)
// 			{
// 			  eventNumber =
// 				FindEventInEventPool (&log, componentNumber, EventName);
// 			  strncpy (log.Component[componentNumber].Event[eventNumber].Name,
// 					   EventName, STRING_LEN);
// 			 // printf
// 				// ("State 7:\nComponentNumber: %d, EventNumber: %d, EventName: %s\n",
// 				//  componentNumber, eventNumber,
// 				//  log.Component[componentNumber].Event[eventNumber].Name);
// 			  if (log.Component[componentNumber].Event[eventNumber].Counter ==
// 				  2)
// 				RecordStatus (&log, componentNumber, eventNumber, LogStart);	// add log
// 			  else
// 				RecordStatus (&log, componentNumber, eventNumber, LogEnd);	// add log
// 			}
// 		  else if (isEventExist == 0)
// 			{
// 			  eventNumber = EmptyPlaceInEventPool (&log, componentNumber);
// 			  if (eventNumber == ERROR_CODE)
// 				{
// 				  printf
// 					("Error: there is not any space in Event pool for creating new event\n");
// 				  return;
// 				}
// 			  else
// 				{
// 				  strncpy (log.Component[componentNumber].
// 						   Event[eventNumber].Name, EventName, STRING_LEN);
// 				//   printf
// 				// 	("State 9:\nComponentNumber: %d, EventNumber: %d, EventName: %s\n",
// 				// 	 componentNumber, eventNumber,
// 				// 	 log.Component[componentNumber].Event[eventNumber].Name);
// 				  RecordStatus (&log, componentNumber, eventNumber, LogStart);	// add log
// 				}
// 			}
// 		}
// 	}
// }

// void
// RecordStatus (Log * Log, int ComponentNumber, int EventNumber,
// 			  int FistTimeFlag)
// {

//   if (FistTimeFlag == 1)
// 	{
// 	  Log->Component[ComponentNumber].Event[EventNumber].RAM.Psram =
// 		(esp_get_free_heap_size () / 1000);
// 	  Log->Component[ComponentNumber].Event[EventNumber].RAM.Sram =
// 		(xPortGetFreeHeapSize () / 1000);
// 	  Log->Component[ComponentNumber].Event[EventNumber].Counter = 1;
// 	}
//   else if (FistTimeFlag == 0)
// 	{
// 	  size_t psramSize =
// 		Log->Component[ComponentNumber].Event[EventNumber].RAM.Psram;
// 	  size_t sramSize =
// 		Log->Component[ComponentNumber].Event[EventNumber].RAM.Sram;
// 	  psramSize =
// 		psramSize -
// 		Log->Component[ComponentNumber].Event[EventNumber].RAM.Psram;
// 	  sramSize =
// 		sramSize -
// 		Log->Component[ComponentNumber].Event[EventNumber].RAM.Sram;
// 	  size_t TimeFromBootUp = pdTICKS_TO_MS (xTaskGetTickCount ());
// 	  printf
// 		("\nEvent name :%s SRAM: %u K bytes PSRAM occupy: %u K bytes occupy at %u millis",
// 		 Log->Component[ComponentNumber].Event[EventNumber].Name, sramSize,
// 		 psramSize, TimeFromBootUp);
// 	  Log->Component[ComponentNumber].Event[EventNumber].Counter = 2;
// 	}
// }

// uint8_t
// IsComponentExist (Log * Log, char *ComponentName)
// {
//   for (int i = 0; i < LOG_MAX_COMPONENT; i++)
// 	{
// 	  if (0 == (strcmp (Log->Component[i].Name, ComponentName)))
// 		{
// 		  return true;
// 		}
// 	}
//   return false;
// }

// uint8_t
// IsEventExist (Log * Log, int ComponentNumber, char *EventName)
// {
//   for (int i = 0; i < LOG_MAX_EVENT; i++)
// 	{
// 	  if (0 ==
// 		  (strcmp (Log->Component[ComponentNumber].Event[i].Name, EventName)))
// 		{
// 		  return true;
// 		}
// 	}
//   return false;
// }

// uint8_t
// FindEventInEventPool (Log * Log, int ComponentNumber, char *EventName)
// {
//   uint8_t eventNumber = ERROR_CODE;
//   for (int i = 0; i < LOG_MAX_EVENT; i++)
// 	{
// 	  if (0 ==
// 		  (strcmp (Log->Component[ComponentNumber].Event[i].Name, EventName)))
// 		{
// 		  eventNumber = i;
// 		  break;
// 		}
// 	}
//   return eventNumber;
// }

// uint8_t
// FindComponentLocationInPool (Log * Log, char *Component)
// {
//   uint8_t componentNumber = ERROR_CODE;
//   for (int i = 0; i < LOG_MAX_COMPONENT; i++)
// 	{
// 	  if (0 == (strcmp (Log->Component[i].Name, Component)))
// 		{
// 		  componentNumber = i;
// 		}
// 	}
//   return componentNumber;
// }

// uint8_t
// EmptyPlaceInComponentPool (Log * Log)
// {
//   uint8_t emptyPlace = ERROR_CODE;
//   for (int i = 0; i < LOG_MAX_COMPONENT; i++)
// 	{
// 	  if (strlen (Log->Component[i].Name) == 0)
// 		{
// 		  emptyPlace = i;
// 		  break;
// 		}
// 	}
//   return emptyPlace;
// }

// uint8_t
// EmptyPlaceInEventPool (Log * Log, int ComponentNumber)
// {
//   uint8_t emptyPlace = ERROR_CODE;
//   for (int i = 0; i < LOG_MAX_EVENT; i++)
// 	{
// 	  if (strlen (Log->Component[ComponentNumber].Event[i].Name) == 0)
// 		{
// 		  emptyPlace = i;
// 		  break;
// 		}
// 	}
//   return emptyPlace;
// }


// uint8_t
// NumberSavedEvent (Log * Log, int ComponentNumber)
// {
//   uint8_t numberOfEvent = 0;
//   for (int i = 0; i < LOG_MAX_EVENT; i++)
// 	{
// 	  if (strlen (Log->Component[ComponentNumber].Event[i].Name) != 0)
// 		numberOfEvent++;
// 	}
// 	return numberOfEvent;
// }

// void
// RamStatusFunction (char *ComponentName, char *EventName)
// {
//   int componentNumber;
//   int eventNumber;
//   size_t psramSize;
//   size_t sramSize;
//   if (IsComponentExist (&Log2, ComponentName) == false)
// 	{
// 	  componentNumber = 0;
// 	  eventNumber = 0;
// 	  psramSize = (esp_get_free_heap_size () / 1000);
// 	  sramSize = (xPortGetFreeHeapSize () / 1000);
// 	  size_t TimeFromBootUp = pdTICKS_TO_MS (xTaskGetTickCount ());
// 	  strncpy (Log2.Component[componentNumber].Name, ComponentName,
// 			   strlen (ComponentName));
// 	  strncpy (Log2.Component[componentNumber].Event[eventNumber].Name,
// 			   EventName, strlen (EventName));
// 	  Log2.Component[componentNumber].Event[eventNumber].RAM.Psram =
// 		psramSize;
// 	  Log2.Component[componentNumber].Event[eventNumber].RAM.Sram = sramSize;
// 	  Log2.Component[componentNumber].Event[eventNumber].TimeStamp =
// 		TimeFromBootUp;
// 	  printf
// 		("component:%s event:%s , SRAM: %u K bytes , PSRAM: %u K bytes at %d millis is FREE",
// 		 ComponentName, EventName, sramSize, psramSize, TimeFromBootUp);
// 	}
//   else
// 	{
// 	  psramSize = (esp_get_free_heap_size () / 1000);
// 	  sramSize = (xPortGetFreeHeapSize () / 1000);
// 	  size_t TimeFromBootUp = pdTICKS_TO_MS (xTaskGetTickCount ());
// 	  componentNumber = FindComponentLocationInPool (&Log2, ComponentName);
// 	  eventNumber = EmptyPlaceInEventPool (&Log2, componentNumber);
// 	  if (NumberSavedEvent (&Log2, componentNumber) >= (LOG_MAX_EVENT - 1))
// 		memset (&Log2.Component[componentNumber], 0,
// 				sizeof (Log2.Component[componentNumber]));
// 	  strncpy (Log2.Component[componentNumber].Name, ComponentName,
// 			   strlen (ComponentName));
// 	  strncpy (Log2.Component[componentNumber].Event[eventNumber].Name,
// 			   EventName, strlen (EventName));
// 	  Log2.Component[componentNumber].Event[eventNumber].RAM.Psram =
// 		psramSize;
// 	  Log2.Component[componentNumber].Event[eventNumber].RAM.Sram = sramSize;
// 	  Log2.Component[componentNumber].Event[eventNumber].TimeStamp =
// 		TimeFromBootUp;
// 	  printf
// 		("component:%s event:%s , SRAM: %u K bytes , PSRAM: %u K bytes at %d millis is FREE",
// 		 ComponentName, EventName, sramSize, psramSize, TimeFromBootUp);
// 	}
// }


// void
// ReportComponentRamStatus (char *ComponentName)
// {
//   int componentNumber;
//   if (IsComponentExist (&Log2, ComponentName) == false)
// 	{
// 	  printf ("there is not any Log for Component %s", ComponentName);
// 	}
//   else
// 	{
// 	  componentNumber = FindComponentLocationInPool (&Log2, ComponentName);
// 	  for (int i = 0; i < LOG_MAX_EVENT; i++)
// 		{
// 		  if (strlen (Log2.Component[componentNumber].Event[i].Name) != 0)
// 			{
// 			  printf
// 				("in the report \ncomponent:%s event:%s , SRAM: %u K bytes , PSRAM: %u K bytes at %d millis is FREE",
// 				 ComponentName, Log2.Component[componentNumber].Event[i].Name,
// 				 Log2.Component[componentNumber].Event[i].RAM.Sram,
// 				 Log2.Component[componentNumber].Event[i].RAM.Psram,
// 				 Log2.Component[componentNumber].Event[i].TimeStamp);
// 			}
// 		  else
// 			break;
// 		}
// 	}
// }

// void
// ReportComponentRamUsed (char *ComponentName)
// {
//   int componentNumber;
//   if (IsComponentExist (&Log2, ComponentName) == false)
// 	{
// 	  printf ("there is not any Log for Component %s", ComponentName);
// 	}
//   else
// 	{
// 	  componentNumber = FindComponentLocationInPool (&Log2, ComponentName);
// 	  for (int i = 0; i < LOG_MAX_EVENT; i++)
// 		{
// 		  if (strlen (Log2.Component[componentNumber].Event[i].Name) != 0)
// 			{
// 			  printf
// 				("\ncomponent:%s event:%s , SRAM: %u K bytes , PSRAM: %u K bytes at %d millis is occupy",
// 				 ComponentName, Log2.Component[componentNumber].Event[i].Name,
// 				 Log2.Component[componentNumber].Event[i].RAM.Sram,
// 				 Log2.Component[componentNumber].Event[i].RAM.Psram,
// 				 Log2.Component[componentNumber].Event[i].TimeStamp);
// 			}
// 		  else
// 			break;
// 		}
// 	}
// }

// void
// test_RamStatusFunction ()
// {

//   // Test 5: Handle full event pool
//   for (int i = 1; i < LOG_MAX_EVENT; i++)
// 	{
// 	  char eventName[STRING_LEN];
// 	  snprintf (eventName, STRING_LEN, "Event%d", i);
// 	  RamStatusFunction ("Component1", eventName);
// 	}
// }

// int
// main ()
// {
//   // test_RamStatusFunction();
//   RamOccupyFunction ("main", "event1");
//   RamOccupyFunction ("main", "event2");
//   RamOccupyFunction ("main", "event1");
//   RamOccupyFunction ("main", "event2");
//   RamOccupyFunction ("main", "event2");
//   RamOccupyFunction ("main", "event2");
// //   ReportComponentRamStatus ("Component1");
//   return 0;
// }
