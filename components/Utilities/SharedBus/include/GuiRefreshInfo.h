#ifndef GUI_REFRESH_INFO_H
#define GUI_REFRESH_INFO_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    GUI_REFRESH_COFFEE_MAKER = 0,    
} GuiRefreshTypes;

typedef struct 
{    
    GuiRefreshTypes GuiRefreshTypes;    
} GuiRefreshPacket;

#ifdef __cplusplus
}
#endif

#endif //GUI_REFRESH_INFO_H