//{{NO_DEPENDENCIES}}
// Microsoft Visual C++에서 생성한 포함 파일입니다.
// MapMaker.rc에서 사용되고 있습니다.
//
#define IDR_MENU1                       101
#define ID_SAVE                         40003
#define ID_LOAD                         40004



#define BLOCK_WIDTH 32
#define BLOCK_HEIGHT 32
#define MAX_WIDTH 10
#define MAX_HEIGHT 15
#define MESSAGE_STATICPAINT WM_USER + 1
#define MAX_TILE 150

struct tag_map {
	int map[MAX_HEIGHT][MAX_WIDTH];
	int stage;
};

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        102
#define _APS_NEXT_COMMAND_VALUE         40005
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif
