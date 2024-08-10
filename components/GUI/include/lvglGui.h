/*
 * a component for create indipendent test for gui
 */
#ifndef LVGL_GUI_H_
#define LVGL_GUI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include"LVGLInterface.h"
#include"lvglTimer.h"

#define IMAGE_ON LV_EVENT_CLICKED
#define IMAGE_OFF LV_EVENT_PRESSED
/**
 * @brief Initializes the GUI task.
 * This function initializes the GUI task by allocating memory and creating the task.
 * @param GuiTaskHandler Pointer to the variable that will hold the GUI task handler.
 * @param TaskPriority Priority of the GUI task.
 * @param TaskStack Size of the GUI task stack.
 * @return void
 */
void GUI_TaskInit(TaskHandle_t *GuiTaskHandler, UBaseType_t TaskPriority, uint32_t TaskStack);

/**
 * @brief Updates the LVGL screen with Spotify information.
 * This function updates various elements on the LVGL screen with Spotify information
 * such as artist name, song title, album name, playback progress, and cover photo.
 * @param songUpdated Flag indicating whether the song information has been updated.
 * @param Artist Artist name.
 * @param Song Title of the song.
 * @param Album Album name.
 * @param DurationMS Duration of the song in milliseconds.
 * @param ProgressMS Current playback progress in milliseconds.
 * @param coverPhoto Pointer to the cover photo image data.
 * @return void
 */
void GUI_UpdateSpotifyScreen(bool songUpdated, char *Artist, char *Song, char *Album, int DurationMS, int ProgressMS, uint8_t *coverPhoto);

/**
 * @brief Handles the event of Matter network connection.
 * This function is called when the Matter network is connected. It sends an event
 * to update the Matter logo on the LVGL UI.
 * @return void
 */
void MatterNetworkConnected();

/**
 * @brief Kills the GUI task and frees allocated memory.
 * This function deletes the GUI task and frees the memory allocated for LVGL components.
 * @param GUITaskHandler Handler of the GUI task to be killed.
 * @return void
 */
void GUI_TaskKill(TaskHandle_t *TaskHandler);

/**
 * @brief Updates the timer display on the GUI with the given time in milliseconds.
 * 
 * @param milliseconds Duration in milliseconds to be converted and displayed as minutes and seconds.
 */
void CoffeeMakerTimer(int milliseconds);


/**
 * @brief Sends the current count of cups to the GUI.
 * 
 * @param cup The number of cups to be displayed.
 */
void CountOfCup(int cup);

/**
 * @brief Controls the visibility of the Scop image on the GUI.
 * 
 * @param flag If true, the Scop image is shown; otherwise, it is hidden.
 */
void ScopImage(int flag);

/**
 * @brief Controls the visibility of the Tea image on the GUI.
 * 
 * @param flag If true, the Tea image is shown; otherwise, it is hidden.
 */
void TeaImage(int flag);

/**
 * @brief Controls the visibility of the SmallGrind image on the GUI.
 * 
 * @param flag If true, the SmallGrind image is shown; otherwise, it is hidden.
 */
void SmallGrindImage(int flag);

/**
 * @brief Controls the visibility of the MediumGrind image on the GUI.
 * 
 * @param flag If true, the MediumGrind image is shown; otherwise, it is hidden.
 */
void MediumGrindImage(int flag);

/**
 * @brief Controls the visibility of the LongGrind image on the GUI.
 * 
 * @param flag If true, the LongGrind image is shown; otherwise, it is hidden.
 */
void LongGrindImage(int flag);

/**
 * @brief Runs a sequence of GUI tests, controlling various images on the display.
 * 
 * This function toggles the visibility of different images on the GUI in a sequence,
 * with delays in between each toggle.
 */
void GUItest();

#ifdef __cplusplus
}
#endif

#endif /* LVGL_GUI_H_ */
