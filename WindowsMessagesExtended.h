#pragma once
#include <Windows.h>

/* Since MSDN doesn't provide a comprehensive list of Windows Messages, we can refer to multiple sources that collectively provide extensive information, such as:
WineHQ Wiki with its extensive list of Windows messages with both hexadecimal and decimal values, which includes many system messages not found in standard MSDN documentation?.
Various GitHub repositories that provide lists of windows messages and helper functions that return a message name by its identifier.
fined by the system headers? (CodeProject)?.*/

#define WM_UAHDESTROYWINDOW 0x0090
#define WM_UAHDRAWMENU 0x0091
#define WM_UAHDRAWMENUITEM 0x0092
#define WM_UAHINITMENU 0x0093
#define WM_UAHMEASUREMENUITEM 0x0094
#define WM_UAHNCPAINTMENUPOPUP 0x0095