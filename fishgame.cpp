// ContraGame.cpp : 定义应用程序的入口点。
//

#include "FishGame.h"

#include <algorithm>

using namespace std;
// 函数声明（解决编译错误）
void CheckPlayerEnemyCollision();
void ShowLevelCompleteButtons(HWND hWnd);
void HideLevelCompleteButtons();
void ShowLevelFailedButtons(HWND hWnd);
void HideLevelFailedButtons();
void DrawFourBitmaps(HDC hdcDest, HDC hdcSrc, int startX, int startY,
	HBITMAP bmp1, HBITMAP bmp2, HBITMAP bmp3, HBITMAP bmp4);

#pragma region 全局变量

#define MAX_LOADSTRING			100		

// 全局变量: 
HINSTANCE hInst; // 当前窗体实例
WCHAR szTitle[MAX_LOADSTRING]; // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING]; // 主窗口类名

// 关卡选择相关
HBITMAP bmp_SelectBackground = NULL;
HBITMAP bmp_Level1Button = NULL;
HBITMAP bmp_Level2Button = NULL;
HBITMAP bmp_Level3Button = NULL;
int selectedLevel = 1;  // 默认第一关
int gameState = GAME_RUNNING;            // 游戏状态初始化
HBITMAP bmp_NextLevel = NULL;            // 下一关按钮图像
HBITMAP bmp_SelectLevel = NULL;          // 选择关卡按钮图像
HBITMAP bmp_RetryLevel = NULL;           // 重玩本关按钮图像

HBITMAP bmp_start_bckground = NULL;	    //游戏开始前背景图像资源
HBITMAP bmp_game_bckground = NULL; //进入游戏后背景图像资源
HBITMAP bmp_StartButton = NULL;	//开始按钮图像资源
HBITMAP bmp_Unit_Fish1 = NULL;		//小鱼1图像资源
HBITMAP bmp_Unit_Fish2 = NULL;		//小鱼2图像资源

HBITMAP bmp_Start_Background = NULL;		//生成的背景图像

HBITMAP bmp_Stage_Background = NULL;		//生成的背景图像


Stage* currentStage = NULL; //当前场景状态
std::vector<Unit*> units;		//单位
std::vector<Button*> buttons;	//按钮

int mouseX = 0;
int mouseY = 0;
bool mouseDown = false;
bool keyUpDown = false;
bool keyDownDown = false;
bool keyLeftDown = false;
bool keyRightDown = false;
int progressValue = 0;//进度条值初始化
extern std::vector<Unit*> smallFishes;
extern int smallFishCount;
int smallFishBmpWidth = 0;
int smallFishBmpHeight = 0;

//帧
int FRAMES_HOLD[] = { 0 };
int FRAMES_HOLD_COUNT = 1;
int FRAMES_WALK[] = { 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2 };
int FRAMES_WALK_COUNT = 16;
int FRAMES_ATTACK[] = { 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 };
int FRAMES_ATTACK_COUNT = 24;



// TODO: 在此添加其它全局变量

// 音频相关全局变量
bool g_bStartMenuMusicPlaying = false;   // 开始界面音乐播放状态
bool g_bEnableAudio = true;              // 全局音频开关（默认开启）
bool g_bLevel2MusicPlaying = false;      // 关卡2音乐播放状态
bool g_bLevel2MusicPaused = false;       // 关卡2音乐暂停状态
MCIDEVICEID g_wLevel2MusicDeviceID = 0;    // MCI设备ID
DWORD g_dwLevel2MusicPosition = 0;         // 记录暂停时的播放位置
bool g_bLevel2MusicInitialized = false;    // MCI设备是否已初始化

// 关卡1音频相关全局变量
bool g_bLevel1MusicPlaying = false;      // 关卡1音乐播放状态
bool g_bLevel1MusicPaused = false;       // 关卡1音乐暂停状态
MCIDEVICEID g_wLevel1MusicDeviceID = 0;    // 关卡1 MCI设备ID
DWORD g_dwLevel1MusicPosition = 0;         // 关卡1音乐暂停时的播放位置
bool g_bLevel1MusicInitialized = false;    // 关卡1 MCI设备是否已初始化

// 关卡3音频相关全局变量
bool g_bLevel3MusicPlaying = false;      // 关卡3音乐播放状态
bool g_bLevel3MusicPaused = false;       // 关卡3音乐暂停状态
MCIDEVICEID g_wLevel3MusicDeviceID = 0;    // 关卡3 MCI设备ID
DWORD g_dwLevel3MusicPosition = 0;         // 关卡3音乐暂停时的播放位置
bool g_bLevel3MusicInitialized = false;    // 关卡3 MCI设备是否已初始化

// 小鱼相关全局变量
vector<Unit*> smallFishes;  
int smallFishCount = 0;     
HBITMAP bmp_SmallFish;
int MAX_SMALL_FISHES = 15;        // 初始化为默认值

int MAX_SMALL_ENEMY_FISHES = 6;        // size=2敌人鱼最大数量（第一关）
int smallEnemyFishCount = 0;           // 当前size=2敌人鱼数量（第一关）


// 鱼雷相关全局变量
HBITMAP bmp_Torpedo = NULL;           // 鱼雷位图
std::vector<Unit*> torpedoes;              // 鱼雷数组
int torpedoCount = 0;                 // 当前鱼雷数量

//1123添加1，此处开始
static HBITMAP bmp_PauseButton = NULL;  // 暂停按钮图像资源
static HBITMAP bmp_ResumeButton = NULL; // 继续按钮图像资源
//1123添加1，此处结束

//1125添加6，此处开始
HBITMAP bmp_HelpButton = NULL;        // 帮助按钮图像资源
//1125添加6，此处结束


HBITMAP bmp_tiao = NULL;      // "挑"字位图
HBITMAP bmp_zhan = NULL;      // "战"字位图
HBITMAP bmp_cheng = NULL;     // "成"字位图
HBITMAP bmp_gong = NULL;      // "功"字位图
HBITMAP bmp_ren = NULL;                  // "人"字位图
HBITMAP bmp_wu = NULL;                   // "物"字位图
HBITMAP bmp_shi = NULL;                  // "失"字位图
HBITMAP bmp_bai = NULL;                  // "败"字位图

double const PI = acos(double(-1));

StageConfig stageConfigs[MAX_STAGES];
int currentStageLevel = 1;  // 当前关卡ID（从1开始）

// 等级缩放比例（索引0不使用，1-5对应等级）
float sizeScales[6] = { 0.0f, 0.8f, 1.0f, 1.4f, 1.8f, 2.2f };

// 升级所需经验表（索引为当前等级）
int expRequired[6] = { 0, 0, EXP_FOR_LEVEL_2_TO_3, EXP_FOR_LEVEL_3_TO_4, EXP_FOR_LEVEL_4_TO_5, 0 };

// 吃不同size鱼的经验值（索引为鱼的size）
int expValues[6] = { 0, EXP_SMALL_FISH, EXP_SMALL_ENEMY, EXP_MEDIUM_ENEMY, EXP_LARGE_ENEMY, EXP_GIANT_ENEMY };

// 鱼位图数组
std::vector<HBITMAP> fishBitmaps;

// 系统配置
struct SystemConfig {
	bool useNewCollision = false;      // 是否使用新碰撞系统
	bool useNewExperience = false;     // 是否使用新经验系统
	bool useNewSpawning = false;       // 是否使用新生成系统
	bool showDebugInfo = true;         // 是否显示调试信息
	bool enableSound = true;           // 是否启用音效（默认开启）
	bool enableMusic = true;           // 是否启用音乐（默认开启）
};

SystemConfig systemConfig;  // 系统配置实例

// 关卡解锁状态 - 索引1-3对应关卡1-3，索引0不使用
bool levelUnlocked[4] = { false, true, false, false };  // 初始：第一关解锁

#pragma endregion


#pragma region Win32程序框架



// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);




int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CONTRAGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CONTRAGAME));

	MSG msg;

	// 主消息循环: 
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CONTRAGAME);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	HWND hWnd = CreateWindow(szWindowClass, szTitle,
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, // 设置窗口样式，不可改变大小，不可最大化,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT + WINDOW_TITLEBARHEIGHT,
		nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		// 初始化游戏窗体
		OutputDebugString(L"=== WM_CREATE ===\n");
		InitGame(hWnd, wParam, lParam);
		// 强制立即重绘
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
		break;
	case WM_KEYDOWN:
		// 键盘按下事件
		KeyDown(hWnd, wParam, lParam);
		break;
	case WM_KEYUP:
		// 键盘松开事件
		KeyUp(hWnd, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		// 鼠标移动事件
		MouseMove(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		// 鼠标左键按下事件
		LButtonDown(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		// 鼠标左键松开事件
		LButtonUp(hWnd, wParam, lParam);
		break;
	case MM_MCINOTIFY:
		if (wParam == MCI_NOTIFY_SUCCESSFUL)
		{
			// 关卡1音乐播放完成，重新开始播放（实现循环）
			if (currentStageLevel == 1 && g_bLevel1MusicPlaying && !g_bLevel1MusicPaused)
			{
				// 使用定时器延迟重新开始，避免递归
				SetTimer(hWnd, 102, 100, NULL); // 使用ID=102的临时定时器
			}
			// 关卡2音乐播放完成，重新开始播放（实现循环）
			else if (currentStageLevel == 2 && g_bLevel2MusicPlaying && !g_bLevel2MusicPaused)
			{
				// 使用定时器延迟重新开始，避免递归
				SetTimer(hWnd, 100, 100, NULL); // 使用ID=100的临时定时器
			}
			else if (currentStageLevel == 3 && g_bLevel3MusicPlaying && !g_bLevel3MusicPaused)
			{
				// 使用定时器延迟重新开始，避免递归
				SetTimer(hWnd, 101, 100, NULL); // 使用ID=101的临时定时器
			}
		}
		break;
	case WM_TIMER:
		// 定时器事件
		if (currentStage != NULL && currentStage->timerOn) TimerUpdate(hWnd, wParam, lParam);
		break;
	case WM_PAINT:
		// 绘图
		Paint(hWnd);
		break;
	case WM_DESTROY:
		// === 新增：清理游戏资源 ===
		CleanupGameResources();
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


#pragma endregion


#pragma region 事件处理函数


HBITMAP CreateSimpleColorButton(COLORREF color, int width, int height, const wchar_t* text);


// 初始化游戏窗体函数
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// 初始化系统配置
	systemConfig.useNewCollision = true;      // 默认开启新碰撞系统
	systemConfig.useNewExperience = true;     // 启用新经验系统
	systemConfig.useNewSpawning = true;       // 启用新生成系统
	systemConfig.showDebugInfo = true;        // 显示调试信息

	// ====== 新增：音频系统初始化 ======
	systemConfig.enableSound = true;
	systemConfig.enableMusic = true;
	g_bStartMenuMusicPlaying = false;


	// 检查音频文件是否存在（仅调试模式）
#ifdef _DEBUG

OutputDebugString(L"[音频] 初始化音频系统\n");
#endif
	// ====== 音频初始化结束 ======

	//加载图像资源
	bmp_start_bckground = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_START_BG));
	bmp_game_bckground = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_STAGE_BG));
	bmp_StartButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_START));
	// ！【贴图互换】为了让玩家鱼使用原鱼2的贴图，敌人鱼使用原鱼1的贴图
	bmp_Unit_Fish1 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_Fish2));
	bmp_Unit_Fish2 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP_Fish1));
	//添加按钮
	
	if (bmp_start_bckground == NULL) {
		OutputDebugString(L"严重错误: bmp_start_bckground 加载失败\n");
		// 创建简单背景
		HDC hdc = GetDC(hWnd);
		bmp_start_bckground = CreateCompatibleBitmap(hdc, 100, 100);
		ReleaseDC(hWnd, hdc);
	}


	//1123添加7，此处开始
	bmp_PauseButton = (HBITMAP)LoadImageW(
		NULL,                           // 不指定实例
		L"images\\PAUSE.bmp",           // 图片文件路径
		IMAGE_BITMAP,                   // 加载为位图
		0, 0,                           // 使用图片原始尺寸
		LR_LOADFROMFILE | LR_CREATEDIBSECTION // 从文件加载
	);

	bmp_ResumeButton = (HBITMAP)LoadImageW(
		NULL,
		L"images\\RESUME.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);
	// 如果加载失败，使用备用方案
	if (bmp_PauseButton == NULL) {
		bmp_PauseButton = CreateSimpleColorButton(RGB(255, 0, 0), 80, 40, L"暂停");
	}
	if (bmp_ResumeButton == NULL) {
		bmp_ResumeButton = CreateSimpleColorButton(RGB(0, 255, 0), 80, 40, L"继续");
	}
	//1123添加7，此处结束

	//1125添加6.1，此处开始
	// 加载帮助按钮图像
	bmp_HelpButton = (HBITMAP)LoadImageW(
		NULL,
		L"images\\gamehelp.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);

	// 如果加载失败，使用备用方案
	//1125debug2，此处开始
	if (bmp_HelpButton == NULL) {
		OutputDebugString(L"警告: 帮助按钮图像加载失败，使用备用按钮\n");
		bmp_HelpButton = CreateSimpleColorButton(RGB(0, 0, 255), BUTTON_HELP_WIDTH, BUTTON_HELP_HEIGHT, L"帮助");
	}
	//1125debug2，此处结束
	//1125添加6.1，此处结束

	// 加载选择关卡背景
	bmp_SelectBackground = (HBITMAP)LoadImageW(
		NULL,
		L"images\\levelselect.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);
	if (bmp_SelectBackground == NULL) {
		OutputDebugString(L"警告：关卡选择背景加载失败，使用备用背景\n");
		bmp_SelectBackground = CreateSimpleColorButton(RGB(50, 50, 120), WINDOW_WIDTH, WINDOW_HEIGHT, L"");
	}

	// 加载关卡1按钮
	bmp_Level1Button = (HBITMAP)LoadImageW(
		NULL,
		L"images\\level1_button.bmp",  // 请确保此文件存在
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);
	if (bmp_Level1Button == NULL) {
		OutputDebugString(L"警告：关卡1按钮加载失败，使用备用按钮\n");
		bmp_Level1Button = CreateSimpleColorButton(RGB(0, 150, 0), BUTTON_LEVEL_WIDTH, BUTTON_LEVEL_HEIGHT, L"关卡 1");
	}

	// 加载关卡2按钮
	bmp_Level2Button = (HBITMAP)LoadImageW(
		NULL,
		L"images\\level2_button.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);
	if (bmp_Level2Button == NULL) {
		OutputDebugString(L"警告：关卡2按钮加载失败，使用备用按钮\n");
		bmp_Level2Button = CreateSimpleColorButton(RGB(200, 150, 0), BUTTON_LEVEL_WIDTH, BUTTON_LEVEL_HEIGHT, L"关卡 2");
	}

	// 加载关卡3按钮
	bmp_Level3Button = (HBITMAP)LoadImageW(
		NULL,
		L"images\\level3_button.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);
	if (bmp_Level3Button == NULL) {
		OutputDebugString(L"警告：关卡3按钮加载失败，使用备用按钮\n");
		bmp_Level3Button = CreateSimpleColorButton(RGB(200, 0, 0), BUTTON_LEVEL_WIDTH, BUTTON_LEVEL_HEIGHT, L"关卡 3");
	}


	Button* startButton = CreateButton(BUTTON_STARTGAME, bmp_StartButton, BUTTON_STARTGAME_WIDTH, BUTTON_STARTGAME_HEIGHT,
		(WINDOW_WIDTH - BUTTON_STARTGAME_WIDTH) / 2, (WINDOW_WIDTH - BUTTON_STARTGAME_HEIGHT) / 2);
	buttons.push_back(startButton);

	//1123添加3，此处开始
	Button* pauseButton = CreateButton(BUTTON_PAUSE, bmp_PauseButton,
		BUTTON_PAUSEGAME_WIDTH, BUTTON_PAUSEGAME_HEIGHT,
		20, 20);  // 放在左上角
	buttons.push_back(pauseButton);
	Button* resumeButton = CreateButton(BUTTON_RESUME, bmp_ResumeButton,
		BUTTON_PAUSEGAME_WIDTH, BUTTON_PAUSEGAME_HEIGHT,
		20, 20);  // 同样位置，与暂停按钮重叠
	buttons.push_back(resumeButton);
	//1123添加3，此处结束

	//1125添加6.2，此处开始
	Button* helpButton = CreateButton(BUTTON_HELP, bmp_HelpButton,
		BUTTON_HELP_WIDTH, BUTTON_HELP_HEIGHT,
		WINDOW_WIDTH - BUTTON_HELP_WIDTH - 40, 20);  // 放在右上角
	buttons.push_back(helpButton);
	//1125添加6.2，此处结束

	// 加载小鱼专用贴图
	bmp_SmallFish = (HBITMAP)LoadImageW(
		NULL,
		L"images\\smallfish.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);
	if (bmp_SmallFish != NULL) {
		// 获取位图的实际尺寸
		BITMAP bm;
		GetObject(bmp_SmallFish, sizeof(BITMAP), &bm);
		smallFishBmpWidth = bm.bmWidth;
		smallFishBmpHeight = bm.bmHeight;

		wchar_t debugMsg[100];
		wsprintf(debugMsg, L"小鱼位图加载成功，尺寸: %dx%d\n", smallFishBmpWidth, smallFishBmpHeight);
		OutputDebugString(debugMsg);
	}
	else {
		OutputDebugString(L"小鱼贴图加载失败，创建简单颜色按钮\n");
		// 创建一个测试贴图，设置默认尺寸
		bmp_SmallFish = CreateSimpleColorButton(RGB(255, 0, 0), 32, 32, L"小鱼");
		smallFishBmpWidth = 32;
		smallFishBmpHeight = 32;
	}

	// 加载鱼雷贴图
	bmp_Torpedo = (HBITMAP)LoadImageW(
		NULL,
		L"images\\boom.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);
	if (bmp_Torpedo == NULL) {
		OutputDebugString(L"鱼雷贴图加载失败，使用简单颜色\n");
		bmp_Torpedo = CreateSimpleColorButton(RGB(255, 0, 0), 68, 68, L"雷");
	}

	// 加载通关相关按钮图像
	bmp_NextLevel = (HBITMAP)LoadImageW(
		NULL,
		L"images\\next_level.bmp",           // 下一关按钮位图文件
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);
	if (bmp_NextLevel == NULL) {
		OutputDebugString(L"警告：下一关按钮图像加载失败，使用备用按钮\n");
		bmp_NextLevel = CreateSimpleColorButton(RGB(50, 200, 100),
			BUTTON_NEXT_WIDTH, BUTTON_NEXT_HEIGHT, L"下一关");
	}

	bmp_SelectLevel = (HBITMAP)LoadImageW(
		NULL,
		L"images\\select_level.bmp",         // 选择关卡按钮位图文件
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);
	if (bmp_SelectLevel == NULL) {
		OutputDebugString(L"警告：选择关卡按钮图像加载失败，使用备用按钮\n");
		bmp_SelectLevel = CreateSimpleColorButton(RGB(0, 150, 255),
			BUTTON_SELECT_WIDTH, BUTTON_SELECT_HEIGHT, L"选择关卡");
	}

	bmp_RetryLevel = (HBITMAP)LoadImageW(
		NULL,
		L"images\\retry_level.bmp",          // 重玩本关按钮位图文件
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);
	if (bmp_RetryLevel == NULL) {
		OutputDebugString(L"警告：重玩本关按钮图像加载失败，使用备用按钮\n");
		bmp_RetryLevel = CreateSimpleColorButton(RGB(255, 140, 0),
			BUTTON_RETRY_WIDTH, BUTTON_RETRY_HEIGHT, L"重玩");
	}

	// 加载通关文字位图（在加载其他按钮图像的代码之后添加）
	bmp_tiao = (HBITMAP)LoadImageW(
			NULL,
			L"images\\tiao.bmp",           // "挑"字位图
			IMAGE_BITMAP,
			0, 0,
			LR_LOADFROMFILE | LR_CREATEDIBSECTION
		);
	if (bmp_tiao == NULL) {
		OutputDebugString(L"警告：tiao.bmp加载失败\n");
		// 可以创建备用位图（白色背景+黑色"挑"字）
		bmp_tiao = CreateSimpleColorButton(RGB(255, 255, 255), 90, 96, L"挑");
	}

	bmp_zhan = (HBITMAP)LoadImageW(
		NULL,
		L"images\\zhan.bmp",           // "战"字位图
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);
	if (bmp_zhan == NULL) {
		OutputDebugString(L"警告：zhan.bmp加载失败\n");
		bmp_zhan = CreateSimpleColorButton(RGB(255, 255, 255), 90, 96, L"战");
	}

	bmp_cheng = (HBITMAP)LoadImageW(
		NULL,
		L"images\\cheng.bmp",          // "成"字位图
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);
	if (bmp_cheng == NULL) {
		OutputDebugString(L"警告：cheng.bmp加载失败\n");
		bmp_cheng = CreateSimpleColorButton(RGB(255, 255, 255), 90, 96, L"成");
	}

	bmp_gong = (HBITMAP)LoadImageW(
		NULL,
		L"images\\gong.bmp",           // "功"字位图
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);
	if (bmp_gong == NULL) {
		OutputDebugString(L"警告：gong.bmp加载失败\n");
		bmp_gong = CreateSimpleColorButton(RGB(255, 255, 255), 90, 96, L"功");
	}

	// 加载游戏失败文字位图
	bmp_ren = (HBITMAP)LoadImageW(
		NULL,
		L"images\\ren.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);
	if (bmp_ren == NULL) {
		OutputDebugString(L"警告：ren.bmp加载失败，创建备用位图\n");
		bmp_ren = CreateSimpleColorButton(RGB(255, 255, 255), 90, 96, L"人");
	}

	bmp_wu = (HBITMAP)LoadImageW(
		NULL,
		L"images\\wu.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);
	if (bmp_wu == NULL) {
		OutputDebugString(L"警告：wu.bmp加载失败，创建备用位图\n");
		bmp_wu = CreateSimpleColorButton(RGB(255, 255, 255), 90, 96, L"物");
	}

	bmp_shi = (HBITMAP)LoadImageW(
		NULL,
		L"images\\shi.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);
	if (bmp_shi == NULL) {
		OutputDebugString(L"警告：shi.bmp加载失败，创建备用位图\n");
		bmp_shi = CreateSimpleColorButton(RGB(255, 255, 255), 90, 96, L"失");
	}

	bmp_bai = (HBITMAP)LoadImageW(
		NULL,
		L"images\\bai.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);
	if (bmp_bai == NULL) {
		OutputDebugString(L"警告：bai.bmp加载失败，创建备用位图\n");
		bmp_bai = CreateSimpleColorButton(RGB(255, 255, 255), 90, 96, L"败");
	}

	// 创建通关按钮（初始不可见）
	Button* nextButton = CreateButton(BUTTON_NEXT_LEVEL, bmp_NextLevel,
		BUTTON_NEXT_WIDTH, BUTTON_NEXT_HEIGHT,
		0, 0);  // 位置在ShowLevelCompleteButtons中设置
	nextButton->visible = false;
	buttons.push_back(nextButton);

	Button* retryButton = CreateButton(BUTTON_RETRY_LEVEL, bmp_RetryLevel,
		BUTTON_RETRY_WIDTH, BUTTON_RETRY_HEIGHT,
		0, 0);
	retryButton->visible = false;
	buttons.push_back(retryButton);

	Button* selectButton = CreateButton(BUTTON_SELECT_LEVEL, bmp_SelectLevel,
		BUTTON_SELECT_WIDTH, BUTTON_SELECT_HEIGHT,
		0, 0);
	selectButton->visible = false;
	buttons.push_back(selectButton);

	// 1. 加载所有鱼位图
	LoadAllFishBitmaps(((LPCREATESTRUCT)lParam)->hInstance);

	// 2. 初始化关卡配置
	InitializeStageConfigs();

	// 3. 初始化随机种子
	static bool randInitialized = false;
	if (!randInitialized) {
		srand(static_cast<unsigned>(time(NULL)));
		randInitialized = true;
	}


	//初始化背景
	bmp_Start_Background = InitBackGround(hWnd, bmp_start_bckground);

	//初始化开始场景
	InitStage(hWnd, STAGE_STARTMENU);

	//初始化主计时器
	SetTimer(hWnd, TIMER_GAMETIMER, TIMER_GAMETIMER_ELAPSE, NULL);

	// 初始化解锁状态（确保每次游戏启动时第一关解锁）
	levelUnlocked[1] = true;   // 第一关解锁
	levelUnlocked[2] = false;  // 第二关未解锁
	levelUnlocked[3] = false;  // 第三关未解锁

#ifdef _DEBUG
	OutputDebugString(L"[InitGame] 初始化关卡解锁状态\n");
#endif
	

	
}

// 键盘按下事件处理函数
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_UP:
		keyUpDown = true;
		break;
	case VK_DOWN:
		keyDownDown = true;
		break;
	case VK_LEFT:
		keyLeftDown = true;
		break;
	case VK_RIGHT:
		keyRightDown = true;
		break;


	case VK_ESCAPE:
		if (currentStage != NULL && currentStage->stageID == STAGE_1 &&
			gameState == GAME_RUNNING) {
			bool isPaused = false;
			for (int i = 0; i < buttons.size(); i++) {
				Button* button = buttons[i];
				if (button->buttonID == BUTTON_PAUSE && !button->visible) {
					isPaused = true;
					break;
				}
			}

			if (isPaused) {
				// 恢复游戏
				SetTimer(hWnd, TIMER_GAMETIMER, TIMER_GAMETIMER_ELAPSE, NULL);
				SetTimer(hWnd, TIMER_SPAWN_FISH, SPAWN_INTERVAL, NULL);

				// 隐藏覆盖层按钮
				HideLevelCompleteButtons();

				// 恢复游戏内按钮
				for (int j = 0; j < buttons.size(); j++) {
					Button* btn = buttons[j];
					if (btn->buttonID == BUTTON_PAUSE) btn->visible = true;
					else if (btn->buttonID == BUTTON_RESUME) btn->visible = false;
					else if (btn->buttonID == BUTTON_HELP) btn->visible = true;
				}

				// 根据当前关卡恢复音乐
				if (currentStageLevel == 1)
				{
					ResumeLevel1Music();
				}
				else if (currentStageLevel == 2)
				{
					ResumeLevel2Music();
				}
				else if (currentStageLevel == 3) 
				{
					ResumeLevel3Music();
				}
			}
			else {
				// 暂停游戏
				ShowPauseButtons(hWnd);
			}
		}
		break;
	// === 新增：P键管理员功能 ===
	case 'P':  // P键（大写，不区分大小写）
	case 'p':  // p键（小写）
    // 检查是否在游戏关卡中（STAGE_1）且游戏正在运行
    if (currentStage != NULL && 
        currentStage->stageID == STAGE_1 && 
        gameState == GAME_RUNNING) {
        
        // 设置游戏状态为通关
        gameState = GAME_LEVEL_COMPLETE;
        
        // 调用显示通关按钮的函数
        ShowLevelCompleteButtons(hWnd);
        
#ifdef _DEBUG
        wchar_t debugMsg[100];
        wsprintf(debugMsg, L"[管理员] 按P键直接通关，当前关卡: %d\n", currentStageLevel);
        OutputDebugString(debugMsg);
#endif
    }
    break;

	default:
		break;
	}
}

// 键盘松开事件处理函数
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// TODO
	switch (wParam)
	{
	case VK_UP:
		keyUpDown = false;
		break;
	case VK_DOWN:
		keyDownDown = false;
		break;
	case VK_LEFT:
		keyLeftDown = false;
		break;
	case VK_RIGHT:
		keyRightDown = false;
		break;
	default:
		break;
	}
}

// 鼠标移动事件处理函数
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
}

// 鼠标左键按下事件处理函数
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = true;

	// --- 修复开始：恢复正确的点击检测循环结构 ---
	for (int i = 0; i < buttons.size(); i++)
	{
		Button* button = buttons[i];
		if (button->visible)
		{
			// 判断点击是否在按钮区域内
			if (button->x <= mouseX
				&& button->x + button->width >= mouseX
				&& button->y <= mouseY
				&& button->y + button->height >= mouseY)
			{
				// 调试信息（已简化，仅在实际点击时输出）
#ifdef _DEBUG
				wchar_t debugMsg[100];
				wsprintf(debugMsg, L"[点击] 按钮ID=%d\n", button->buttonID);
				OutputDebugString(debugMsg);
#endif

				// 根据按钮ID执行不同操作
				switch (button->buttonID) {
				case BUTTON_STARTGAME:
					if (currentStage != NULL) {
						if (currentStage->stageID == STAGE_STARTMENU) {
							// 从开始菜单进入选择关卡界面
							InitStage(hWnd, STAGE_SELECT);
						}
						else if (currentStage->stageID == STAGE_SELECT) {
							// 从选择关卡界面返回开始菜单
							InitStage(hWnd, STAGE_STARTMENU);
						}
					}
					break;

				case BUTTON_LEVEL1:
				{
					// 弹出关卡规则介绍窗口
					int result = MessageBox(hWnd,
						L"══════════ 第一关规则 ══════════\n\n"
						L"🎯 游戏目标：\n"
						L"  1. 控制玩家鱼吃掉比自己小的鱼\n"
						L"  2. 进度条达到100%即可通关\n"
						L"  3. 躲避比自己大的鱼，碰撞会导致游戏失败\n\n"
						L"🎮 操作方法：\n"
						L"  • 移动：鼠标控制鱼的方向\n"
						L"  • 暂停：点击暂停按钮或按ESC键\n"
						L"  • 继续：点击继续按钮或再次按ESC\n\n"
						L"⚠️ 注意事项：\n"
						L"  • 敌人鱼比玩家鱼大，更容易碰撞\n"
						L"  • 小鱼会随机游动，需要主动追逐\n"
						L"  • 游戏失败后可重玩或选择其他关卡\n\n"
						L"═══════════════════════════════\n\n"
						L"点击【确定】开始游戏（不要点击右上角的“×”哦）\n"
						L"点击【取消】返回关卡选择",
						L"第一关规则说明",
						MB_OKCANCEL | MB_ICONINFORMATION | MB_DEFBUTTON1);

					// 根据用户选择执行相应操作
					if (result == IDOK) {
						// 用户点击"确定"，进入游戏
						currentStageLevel = 1;
						InitStage(hWnd, STAGE_1);

#ifdef _DEBUG
						OutputDebugString(L"用户点击'确定'，进入第一关\n");
#endif
					}
					// 用户点击"取消"或关闭窗口，保持在关卡选择界面
					else {
#ifdef _DEBUG
						OutputDebugString(L"用户取消进入第一关\n");
#endif
					}
				}
				break;

				case BUTTON_LEVEL2:
				{
					// === 新增：检查关卡是否已解锁 ===
					if (!levelUnlocked[2]) {
						MessageBox(hWnd,
							L"第二关尚未解锁！\n\n"
							L"请先完成第一关。",
							L"关卡未解锁",
							MB_OK | MB_ICONINFORMATION);
						break;  // 停止处理，不进入游戏
					}

					// 弹出关卡规则介绍窗口（使用第一关的规则）
					int result = MessageBox(hWnd,
						L"══════════ 第二关规则 ══════════\n\n"
						L"🎯 游戏目标：\n"
						L"  1. 控制玩家鱼吃掉比自己小的鱼\n"
						L"  2. 进度条达到100%即可通关\n"
						L"  3. 躲避比自己大的鱼，碰撞会导致游戏失败\n\n"
						L"🎮 操作方法：\n"
						L"  • 移动：鼠标控制鱼的方向\n"
						L"  • 暂停：点击暂停按钮或按ESC键\n"
						L"  • 继续：点击继续按钮或再次按ESC\n\n"
						L"⚠️ 注意事项：\n"
						L"  • 敌人鱼比玩家鱼大，更容易碰撞\n"
						L"  • 小鱼会随机游动，需要主动追逐\n"
						L"  • 游戏失败后可重玩或选择其他关卡\n\n"
						L"═══════════════════════════════\n\n"
						L"点击【确定】开始游戏\n"
						L"点击【取消】返回关卡选择",
						L"第二关规则说明",
						MB_OKCANCEL | MB_ICONINFORMATION | MB_DEFBUTTON1);

					// 根据用户选择执行相应操作
					if (result == IDOK) {
						// 用户点击"确定"，进入第二关
						currentStageLevel = 2;  // 关键：设置当前关卡为2
						InitStage(hWnd, STAGE_1);  // 使用STAGE_1场景（内容和第一关一样）

#ifdef _DEBUG
						OutputDebugString(L"用户点击'确定'，进入第二关\n");
#endif
					}
					// 用户点击"取消"或关闭窗口，保持在关卡选择界面
					else {
#ifdef _DEBUG
						OutputDebugString(L"用户取消进入第二关\n");
#endif
					}
				}
				break;

				case BUTTON_LEVEL3:
				{
					// === 新增：检查关卡是否已解锁 ===
					if (!levelUnlocked[3]) {
						MessageBox(hWnd,
							L"第三关尚未解锁！\n\n"
							L"请先完成第二关。",
							L"关卡未解锁",
							MB_OK | MB_ICONINFORMATION);
						break;  // 停止处理，不进入游戏
					}

					// 弹出关卡规则介绍窗口（使用第一关的规则）
					int result = MessageBox(hWnd,
						L"══════════ 第三关规则 ══════════\n\n"
						L"🎯 游戏目标：\n"
						L"  1. 控制玩家鱼吃掉比自己小的鱼\n"
						L"  2. 进度条达到100%即可通关\n"
						L"  3. 躲避比自己大的鱼，碰撞会导致游戏失败\n\n"
						L"🎮 操作方法：\n"
						L"  • 移动：鼠标控制鱼的方向\n"
						L"  • 暂停：点击暂停按钮或按ESC键\n"
						L"  • 继续：点击继续按钮或再次按ESC\n\n"
						L"⚠️ 注意事项：\n"
						L"  • 敌人鱼比玩家鱼大，更容易碰撞\n"
						L"  • 小鱼会随机游动，需要主动追逐\n"
						L"  • 游戏失败后可重玩或选择其他关卡\n\n"
						L"═══════════════════════════════\n\n"
						L"点击【确定】开始游戏\n"
						L"点击【取消】返回关卡选择",
						L"第三关规则说明",
						MB_OKCANCEL | MB_ICONINFORMATION | MB_DEFBUTTON1);

					// 根据用户选择执行相应操作
					if (result == IDOK) {
						// 用户点击"确定"，进入第三关
						currentStageLevel = 3;  // 关键：设置当前关卡为3
						InitStage(hWnd, STAGE_1);  // 使用STAGE_1场景（内容和第一关一样）

#ifdef _DEBUG
						OutputDebugString(L"用户点击'确定'，进入第三关\n");
#endif
					}
					// 用户点击"取消"或关闭窗口，保持在关卡选择界面
					else {
#ifdef _DEBUG
						OutputDebugString(L"用户取消进入第三关\n");
#endif
					}
				}
				break;

				case BUTTON_BACK:
					// 返回开始菜单
					OutputDebugString(L"点击了返回按钮，返回开始菜单\n");
					InitStage(hWnd, STAGE_STARTMENU);
					break;


				case BUTTON_PAUSE:
				{
					// 显示暂停界面
					ShowPauseButtons(hWnd);

					// 暂停游戏计时器
					KillTimer(hWnd, TIMER_GAMETIMER);
					KillTimer(hWnd, TIMER_SPAWN_FISH);

					// 根据当前关卡暂停音乐
					if (currentStageLevel == 1)
					{
						PauseLevel1Music();
					}
					else if (currentStageLevel == 2)
					{
						PauseLevel2Music();
					}
					else if (currentStageLevel == 3) 
					{
						PauseLevel3Music();
					}
				}
				break;
				case BUTTON_RESUME:
				{
					// 重新启动游戏计时器
					SetTimer(hWnd, TIMER_GAMETIMER, TIMER_GAMETIMER_ELAPSE, NULL);
					SetTimer(hWnd, TIMER_SPAWN_FISH, SPAWN_INTERVAL, NULL);

					// 隐藏覆盖层按钮
					HideLevelCompleteButtons();

					// 恢复游戏内按钮
					for (int j = 0; j < buttons.size(); j++) {
						Button* btn = buttons[j];
						if (btn->buttonID == BUTTON_PAUSE) btn->visible = true;
						else if (btn->buttonID == BUTTON_RESUME) btn->visible = false;
						else if (btn->buttonID == BUTTON_HELP) btn->visible = true;
					}
					// 根据当前关卡恢复音乐
					if (currentStageLevel == 1)
					{
						ResumeLevel1Music();
					}
					else if (currentStageLevel == 2)
					{
						ResumeLevel2Music();
					}
				}

				break;

				case BUTTON_HELP:
				{
					// 弹出帮助对话框
					MessageBox(hWnd,
						L"游戏帮助：\n\n1. 点击开始游戏进入游戏\n2. 在游戏中移动鼠标控制鱼的移动\n3. 让两条鱼靠近可以增加进度\n4. 点击暂停按钮可以暂停游戏",
						L"游戏帮助",
						MB_OK | MB_ICONINFORMATION);
					break;
				}
				case BUTTON_NEXT_LEVEL:  // 下一关按钮
				{
#ifdef _DEBUG
					OutputDebugString(L"点击下一关按钮\n");
#endif

					// 隐藏通关按钮
					HideLevelCompleteButtons();

					// === 停止当前关卡的音乐 ===
					if (currentStageLevel == 1)
					{
						StopLevel1Music();  // 停止关卡1音乐
					}
					else if (currentStageLevel == 2)
					{
						StopLevel2Music();  // 停止关卡2音乐
					}
					else if (currentStageLevel == 3)
					{
						StopLevel3Music();  // 停止关卡3音乐
					}

					// 根据当前关卡进入下一关
					if (currentStageLevel < 3) {
						currentStageLevel++;  // 进入下一关
						InitStage(hWnd, STAGE_1);  // 重新开始游戏

#ifdef _DEBUG
						wchar_t debugMsg[100];
						wsprintf(debugMsg, L"进入第%d关\n", currentStageLevel);
						OutputDebugString(debugMsg);
#endif
					}
					else {
						// 已经是第三关，返回关卡选择界面
						InitStage(hWnd, STAGE_SELECT);

#ifdef _DEBUG
						OutputDebugString(L"第三关通关，返回关卡选择界面\n");
#endif
					}
				}
				break;

				case BUTTON_RETRY_LEVEL:  // 重玩本关
				{
#ifdef _DEBUG
					OutputDebugString(L"点击重玩本关按钮\n");
#endif

					// 隐藏通关按钮
					HideLevelCompleteButtons();

					// 处理关卡1音乐
					if (currentStageLevel == 1)
					{
						// 情况1：音乐被暂停了 → 恢复播放（从暂停位置）
						if (g_bLevel1MusicPlaying && g_bLevel1MusicPaused)
						{
							ResumeLevel1Music();  // 从暂停位置恢复
						}
						// 情况2：音乐没有播放 → 开始播放
						else if (!g_bLevel1MusicPlaying)
						{
							PlayLevel1Music();    // 从头开始播放
						}
						// 情况3：音乐正在播放且没有暂停 → 什么都不做（继续播放）
					}
					// 正确处理关卡2音乐
					if (currentStageLevel == 2)
					{
						// 情况1：音乐被暂停了 → 恢复播放（从暂停位置）
						if (g_bLevel2MusicPlaying && g_bLevel2MusicPaused)
						{
							ResumeLevel2Music();  // 从暂停位置恢复
						}
						// 情况2：音乐没有播放 → 开始播放
						else if (!g_bLevel2MusicPlaying)
						{
							PlayLevel2Music();    // 从头开始播放
						}
						// 情况3：音乐正在播放且没有暂停 → 什么都不做（继续播放）
					}
					// 正确处理关卡3音乐
					else if (currentStageLevel == 3)
					{
						// 情况1：音乐被暂停了 → 恢复播放（从暂停位置）
						if (g_bLevel3MusicPlaying && g_bLevel3MusicPaused)
						{
							ResumeLevel3Music();  // 从暂停位置恢复
						}
						// 情况2：音乐没有播放 → 开始播放
						else if (!g_bLevel3MusicPlaying)
						{
							PlayLevel3Music();    // 从头开始播放
						}
						// 情况3：音乐正在播放且没有暂停 → 什么都不做（继续播放）
					}

					// 重新初始化第一关
					InitStage(hWnd, STAGE_1);
				}
				break;

				case BUTTON_SELECT_LEVEL:   // 返回选择关卡界面
				{
#ifdef _DEBUG
					OutputDebugString(L"点击选择关卡按钮\n");
#endif
					// 停止当前关卡的音乐 
					if (currentStageLevel == 1)
					{
						StopLevel1Music();  // 停止关卡1音乐
					}
					else if (currentStageLevel == 2)
					{
						StopLevel2Music();  // 停止关卡2音乐
					}
					else if (currentStageLevel == 3) 
					{
						StopLevel3Music();  // 停止关卡3音乐
					}
					// 隐藏关卡完成按钮
					HideLevelCompleteButtons();

					// 返回选择关卡界面
					InitStage(hWnd, STAGE_SELECT);
				}
				break;
				} // end switch

				// 找到并处理了一个按钮后，跳出循环（关键！）
				break;
			} // end if (点击在区域内)
		} // end if (button->visible)
	} // end for
}

// 鼠标左键松开事件处理函数
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = false;
}

// 定时器事件处理函数
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// 处理游戏主定时器
	if (wParam == TIMER_GAMETIMER) {
		UpdateUnits(hWnd);
		InvalidateRect(hWnd, NULL, FALSE);
	}
	// 处理生成小鱼的定时器
	else if (wParam == TIMER_SPAWN_FISH) {
		// 生成新的小鱼
		SpawnSmallFish(hWnd);

		// 第二关：增加鱼雷生成频率和数量
		if (currentStageLevel == 2) {
				SpawnTorpedo(hWnd);

				// 25%概率额外生成一个鱼雷
				if ((rand() % 100) < 25) {
					SpawnTorpedo(hWnd);
				}
			
		}
	}
	// 处理音乐循环定时器
	else if (wParam == 100) // 音乐重启定时器
	{
		KillTimer(hWnd, 100); // 只执行一次

		if (currentStageLevel == 2 && g_bLevel2MusicPlaying && !g_bLevel2MusicPaused)
		{
			if (g_wLevel2MusicDeviceID != 0 && g_bLevel2MusicInitialized)
			{
				// 定位到开始并重新播放
				mciSendCommand(g_wLevel2MusicDeviceID, MCI_SEEK, MCI_SEEK_TO_START, NULL);

				MCI_PLAY_PARMS mciPlay = { 0 };
				mciPlay.dwCallback = (DWORD_PTR)hWnd;
				mciSendCommand(g_wLevel2MusicDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mciPlay);

#ifdef _DEBUG
				OutputDebugString(L"[音频] 关卡2音乐循环播放\n");
#endif
			}
		}
	}
	else if (wParam == 101)
	{
		KillTimer(hWnd, 101); // 只执行一次

		if (currentStageLevel == 3 && g_bLevel3MusicPlaying && !g_bLevel3MusicPaused)
		{
			if (g_wLevel3MusicDeviceID != 0 && g_bLevel3MusicInitialized)
			{
				// 定位到开始并重新播放
				mciSendCommand(g_wLevel3MusicDeviceID, MCI_SEEK, MCI_SEEK_TO_START, NULL);

				MCI_PLAY_PARMS mciPlay = { 0 };
				mciPlay.dwCallback = (DWORD_PTR)hWnd;
				mciSendCommand(g_wLevel3MusicDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mciPlay);

#ifdef _DEBUG
				OutputDebugString(L"[音频] 关卡3音乐循环播放\n");
#endif
			}
		}
	}
	// 关卡1音乐循环定时器
	else if (wParam == 102) // 关卡1音乐重启定时器
	{
		KillTimer(hWnd, 102); // 只执行一次

		if (currentStageLevel == 1 && g_bLevel1MusicPlaying && !g_bLevel1MusicPaused)
		{
			if (g_wLevel1MusicDeviceID != 0 && g_bLevel1MusicInitialized)
			{
				// 定位到开始并重新播放
				mciSendCommand(g_wLevel1MusicDeviceID, MCI_SEEK, MCI_SEEK_TO_START, NULL);

				MCI_PLAY_PARMS mciPlay = { 0 };
				mciPlay.dwCallback = (DWORD_PTR)hWnd;
				mciSendCommand(g_wLevel1MusicDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mciPlay);

#ifdef _DEBUG
				OutputDebugString(L"[音频] 关卡1音乐循环播放\n");
#endif
			}
		}
	}
}


#pragma endregion


#pragma region 其它游戏状态处理函数



//TODO: 添加游戏需要的更多函数

// ========== 音频控制函数 ==========

// 播放开始界面音乐
void PlayStartMenuMusic()
{
	if (!systemConfig.enableMusic) return;

	if (!g_bStartMenuMusicPlaying)
	{
		// 使用PlaySound播放WAV文件
		// SND_FILENAME: 从文件播放
		// SND_ASYNC: 异步播放（不阻塞程序）
		// SND_LOOP: 循环播放
		// SND_NODEFAULT: 如果文件不存在，不播放默认声音
		if (PlaySound(L"sounds\\start.wav",
			NULL,
			SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT))
		{
			g_bStartMenuMusicPlaying = true;

#ifdef _DEBUG
			OutputDebugString(L"[音频] 开始界面音乐开始播放（循环）\n");
#endif
		}
		else
		{
#ifdef _DEBUG
			DWORD error = GetLastError();
			wchar_t errorMsg[256];
			wsprintf(errorMsg, L"[音频] 错误: 无法加载开始界面音乐，错误代码: %d\n", error);
			OutputDebugString(errorMsg);
#endif
		}
	}
}

// 停止开始界面音乐
void StopStartMenuMusic()
{
	if (g_bStartMenuMusicPlaying)
	{
		// 停止所有由PlaySound播放的声音
		PlaySound(NULL, NULL, 0);
		g_bStartMenuMusicPlaying = false;

#ifdef _DEBUG
		OutputDebugString(L"[音频] 开始界面音乐已停止\n");
#endif
	}
}

// 检查开始界面音乐是否在播放
bool IsStartMenuMusicPlaying()
{
	return g_bStartMenuMusicPlaying;
}

// 找到音频控制函数部分（大约在680行附近，在PlayStartMenuMusic函数之后）
// ========== 关卡1音频控制函数 ==========

void PlayLevel1Music()
{
	if (!systemConfig.enableMusic) return;

	if (!g_bLevel1MusicPlaying)
	{
		// 如果MCI设备未初始化，先初始化
		if (!g_bLevel1MusicInitialized || g_wLevel1MusicDeviceID == 0)
		{
			MCI_OPEN_PARMS mciOpen = { 0 };
			mciOpen.lpstrDeviceType = L"waveaudio";
			mciOpen.lpstrElementName = L"sounds\\level1.wav";

			if (mciSendCommand(0, MCI_OPEN,
				MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
				(DWORD_PTR)&mciOpen) == 0)
			{
				g_wLevel1MusicDeviceID = mciOpen.wDeviceID;
				g_bLevel1MusicInitialized = true;

				// 设置时间格式为毫秒
				MCI_SET_PARMS mciSet = { 0 };
				mciSet.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
				mciSendCommand(g_wLevel1MusicDeviceID, MCI_SET,
					MCI_SET_TIME_FORMAT, (DWORD_PTR)&mciSet);

#ifdef _DEBUG
				OutputDebugString(L"[音频] 关卡1音乐MCI设备初始化成功\n");
#endif
			}
			else
			{
#ifdef _DEBUG
				OutputDebugString(L"[音频] 关卡1音乐MCI设备初始化失败，回退到PlaySound\n");
#endif
				// MCI失败，回退到PlaySound
				if (PlaySound(L"sounds\\level1.wav", NULL,
					SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT))
				{
					g_bLevel1MusicPlaying = true;
					g_bLevel1MusicPaused = false;
				}
				return;
			}
		}

		// 如果有暂停位置，从暂停位置开始；否则从头开始
		if (g_dwLevel1MusicPosition > 0)
		{
			MCI_SEEK_PARMS mciSeek = { 0 };
			mciSeek.dwTo = g_dwLevel1MusicPosition;
			mciSendCommand(g_wLevel1MusicDeviceID, MCI_SEEK,
				MCI_TO, (DWORD_PTR)&mciSeek);
		}
		else
		{
			// 确保定位到开始
			mciSendCommand(g_wLevel1MusicDeviceID, MCI_SEEK,
				MCI_SEEK_TO_START, NULL);
		}

		// 播放音乐（使用MCI_NOTIFY以便循环）
		MCI_PLAY_PARMS mciPlay = { 0 };
		mciPlay.dwCallback = (DWORD_PTR)GetActiveWindow();

		if (mciSendCommand(g_wLevel1MusicDeviceID, MCI_PLAY,
			MCI_NOTIFY, (DWORD_PTR)&mciPlay) == 0)
		{
			g_bLevel1MusicPlaying = true;
			g_bLevel1MusicPaused = false;

#ifdef _DEBUG
			OutputDebugString(L"[音频] 关卡1音乐开始播放(MCI)\n");
#endif
		}
	}
}

void StopLevel1Music()
{
	if (g_bLevel1MusicPlaying)
	{
		if (g_wLevel1MusicDeviceID != 0 && g_bLevel1MusicInitialized)
		{
			// 停止播放并关闭设备
			mciSendCommand(g_wLevel1MusicDeviceID, MCI_STOP, 0, NULL);
			mciSendCommand(g_wLevel1MusicDeviceID, MCI_CLOSE, 0, NULL);
			g_wLevel1MusicDeviceID = 0;
			g_bLevel1MusicInitialized = false;
		}
		if (g_wLevel2MusicDeviceID != 0)
		{
			mciSendCommand(g_wLevel2MusicDeviceID, MCI_STOP, 0, NULL);
			mciSendCommand(g_wLevel2MusicDeviceID, MCI_CLOSE, 0, NULL);
			g_wLevel2MusicDeviceID = 0;
		}

		g_bLevel1MusicPlaying = false;
		g_bLevel1MusicPaused = false;
		g_dwLevel1MusicPosition = 0;

#ifdef _DEBUG
		OutputDebugString(L"[音频] 关卡1音乐停止\n");
#endif
	}
}

void PauseLevel1Music()
{
	if (g_bLevel1MusicPlaying && !g_bLevel1MusicPaused)
	{
		// 获取当前播放位置
		if (g_wLevel1MusicDeviceID != 0 && g_bLevel1MusicInitialized)
		{
			MCI_STATUS_PARMS mciStatus = { 0 };
			mciStatus.dwItem = MCI_STATUS_POSITION;

			if (mciSendCommand(g_wLevel1MusicDeviceID, MCI_STATUS,
				MCI_STATUS_ITEM, (DWORD_PTR)&mciStatus) == 0)
			{
				g_dwLevel1MusicPosition = mciStatus.dwReturn;
			}

			// 暂停播放
			mciSendCommand(g_wLevel1MusicDeviceID, MCI_PAUSE, 0, NULL);
		}

		g_bLevel1MusicPaused = true;

#ifdef _DEBUG
		wchar_t debugMsg[100];
		wsprintf(debugMsg, L"[音频] 关卡1音乐暂停，位置: %d ms\n", g_dwLevel1MusicPosition);
		OutputDebugString(debugMsg);
#endif
	}
}

void ResumeLevel1Music()
{
	if (g_bLevel1MusicPlaying && g_bLevel1MusicPaused)
	{
		if (systemConfig.enableMusic)
		{
			if (g_wLevel1MusicDeviceID != 0 && g_bLevel1MusicInitialized)
			{
				// 定位到暂停位置
				if (g_dwLevel1MusicPosition > 0)
				{
					MCI_SEEK_PARMS mciSeek = { 0 };
					mciSeek.dwTo = g_dwLevel1MusicPosition;
					mciSendCommand(g_wLevel1MusicDeviceID, MCI_SEEK,
						MCI_TO, (DWORD_PTR)&mciSeek);
				}

				// 继续播放
				MCI_PLAY_PARMS mciPlay = { 0 };
				mciPlay.dwCallback = (DWORD_PTR)GetActiveWindow();
				mciSendCommand(g_wLevel1MusicDeviceID, MCI_PLAY,
					MCI_NOTIFY, (DWORD_PTR)&mciPlay);

				g_bLevel1MusicPaused = false;

#ifdef _DEBUG
				OutputDebugString(L"[音频] 关卡1音乐从暂停位置恢复播放\n");
#endif
			}
		}
	}
}

void PauseLevel2Music()
{
	if (g_bLevel2MusicPlaying && !g_bLevel2MusicPaused)
	{
		// 获取当前播放位置
		if (g_wLevel2MusicDeviceID != 0 && g_bLevel2MusicInitialized)
		{
			MCI_STATUS_PARMS mciStatus = { 0 };
			mciStatus.dwItem = MCI_STATUS_POSITION;

			if (mciSendCommand(g_wLevel2MusicDeviceID, MCI_STATUS,
				MCI_STATUS_ITEM, (DWORD_PTR)&mciStatus) == 0)
			{
				g_dwLevel2MusicPosition = mciStatus.dwReturn;
			}

			// 暂停播放
			mciSendCommand(g_wLevel2MusicDeviceID, MCI_PAUSE, 0, NULL);
		}
		else
		{
			// 如果MCI未初始化，使用PlaySound暂停（无法记录位置）
			PlaySound(NULL, NULL, 0);
		}

		g_bLevel2MusicPaused = true;

#ifdef _DEBUG
		wchar_t debugMsg[100];
		wsprintf(debugMsg, L"[音频] 关卡2音乐暂停，位置: %d ms\n", g_dwLevel2MusicPosition);
		OutputDebugString(debugMsg);
#endif
	}
}

void ResumeLevel2Music()
{
	if (g_bLevel2MusicPlaying && g_bLevel2MusicPaused)
	{
		if (systemConfig.enableMusic)
		{
			if (g_wLevel2MusicDeviceID != 0 && g_bLevel2MusicInitialized)
			{
				// 定位到暂停位置
				if (g_dwLevel2MusicPosition > 0)
				{
					MCI_SEEK_PARMS mciSeek = { 0 };
					mciSeek.dwTo = g_dwLevel2MusicPosition;
					mciSendCommand(g_wLevel2MusicDeviceID, MCI_SEEK,
						MCI_TO, (DWORD_PTR)&mciSeek);
				}

				// 继续播放（注意：使用MCI_PLAY而不是MCI_RESUME）
				MCI_PLAY_PARMS mciPlay = { 0 };
				mciPlay.dwCallback = (DWORD_PTR)GetActiveWindow();
				mciSendCommand(g_wLevel2MusicDeviceID, MCI_PLAY,
					MCI_NOTIFY, (DWORD_PTR)&mciPlay);

				g_bLevel2MusicPaused = false;

#ifdef _DEBUG
				OutputDebugString(L"[音频] 关卡2音乐从暂停位置恢复播放\n");
#endif
			}
			else
			{
				// 回退到PlaySound（从头开始）
				if (PlaySound(L"sounds\\level2.wav", NULL,
					SND_FILENAME | SND_ASYNC | SND_LOOP))
				{
					g_bLevel2MusicPaused = false;
				}
			}

#ifdef _DEBUG
			OutputDebugString(L"[音频] 关卡2音乐恢复播放\n");
#endif
		}
	}
}


void PlayLevel2Music()
{
	if (!systemConfig.enableMusic) return;

	if (!g_bLevel2MusicPlaying)
	{
		// 如果MCI设备未初始化，先初始化
		if (!g_bLevel2MusicInitialized || g_wLevel2MusicDeviceID == 0)
		{
			MCI_OPEN_PARMS mciOpen = { 0 };
			mciOpen.lpstrDeviceType = L"waveaudio";
			mciOpen.lpstrElementName = L"sounds\\level2.wav";

			if (mciSendCommand(0, MCI_OPEN,
				MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
				(DWORD_PTR)&mciOpen) == 0)
			{
				g_wLevel2MusicDeviceID = mciOpen.wDeviceID;
				g_bLevel2MusicInitialized = true;

				// 设置时间格式为毫秒
				MCI_SET_PARMS mciSet = { 0 };
				mciSet.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
				mciSendCommand(g_wLevel2MusicDeviceID, MCI_SET,
					MCI_SET_TIME_FORMAT, (DWORD_PTR)&mciSet);

#ifdef _DEBUG
				OutputDebugString(L"[音频] MCI设备初始化成功\n");
#endif
			}
			else
			{
#ifdef _DEBUG
				OutputDebugString(L"[音频] MCI设备初始化失败，回退到PlaySound\n");
#endif
				// MCI失败，回退到原来的PlaySound
				if (PlaySound(L"sounds\\level2.wav", NULL,
					SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT))
				{
					g_bLevel2MusicPlaying = true;
					g_bLevel2MusicPaused = false;
				}
				return;
			}
		}

		// 如果有暂停位置，从暂停位置开始；否则从头开始
		if (g_dwLevel2MusicPosition > 0)
		{
			MCI_SEEK_PARMS mciSeek = { 0 };
			mciSeek.dwTo = g_dwLevel2MusicPosition;
			mciSendCommand(g_wLevel2MusicDeviceID, MCI_SEEK,
				MCI_TO, (DWORD_PTR)&mciSeek);
		}
		else
		{
			// 确保定位到开始
			mciSendCommand(g_wLevel2MusicDeviceID, MCI_SEEK,
				MCI_SEEK_TO_START, NULL);
		}

		// 播放音乐（使用MCI_NOTIFY以便循环）
		MCI_PLAY_PARMS mciPlay = { 0 };
		mciPlay.dwCallback = (DWORD_PTR)GetActiveWindow();

		if (mciSendCommand(g_wLevel2MusicDeviceID, MCI_PLAY,
			MCI_NOTIFY, (DWORD_PTR)&mciPlay) == 0)
		{
			g_bLevel2MusicPlaying = true;
			g_bLevel2MusicPaused = false;

#ifdef _DEBUG
			OutputDebugString(L"[音频] 关卡2音乐开始播放(MCI)\n");
#endif
		}
	}
}

void StopLevel2Music()
{
	if (g_wLevel2MusicDeviceID != 0 && g_bLevel2MusicInitialized)
	{
		// 停止播放并关闭设备
		mciSendCommand(g_wLevel2MusicDeviceID, MCI_STOP, 0, NULL);
		mciSendCommand(g_wLevel2MusicDeviceID, MCI_CLOSE, 0, NULL);
		g_wLevel2MusicDeviceID = 0;
		g_bLevel2MusicInitialized = false;
	}

	g_bLevel2MusicPlaying = false;
	g_bLevel2MusicPaused = false;
	g_dwLevel2MusicPosition = 0;

#ifdef _DEBUG
	OutputDebugString(L"[音频] 关卡2音乐停止\n");
#endif
}


void RestartLevel2Music()
{
	// 重置播放位置
	g_dwLevel2MusicPosition = 0;

	// 停止并重新开始
	StopLevel2Music();
	PlayLevel2Music();
}

// ========== 关卡3音频控制函数 ==========

void PlayLevel3Music()
{
	if (!systemConfig.enableMusic) return;

	if (!g_bLevel3MusicPlaying)
	{
		// 如果MCI设备未初始化，先初始化
		if (!g_bLevel3MusicInitialized || g_wLevel3MusicDeviceID == 0)
		{
			MCI_OPEN_PARMS mciOpen = { 0 };
			mciOpen.lpstrDeviceType = L"waveaudio";
			mciOpen.lpstrElementName = L"sounds\\level3.wav";

			if (mciSendCommand(0, MCI_OPEN,
				MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
				(DWORD_PTR)&mciOpen) == 0)
			{
				g_wLevel3MusicDeviceID = mciOpen.wDeviceID;
				g_bLevel3MusicInitialized = true;

				// 设置时间格式为毫秒
				MCI_SET_PARMS mciSet = { 0 };
				mciSet.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
				mciSendCommand(g_wLevel3MusicDeviceID, MCI_SET,
					MCI_SET_TIME_FORMAT, (DWORD_PTR)&mciSet);

#ifdef _DEBUG
				OutputDebugString(L"[音频] 关卡3音乐MCI设备初始化成功\n");
#endif
			}
			else
			{
#ifdef _DEBUG
				OutputDebugString(L"[音频] 关卡3音乐MCI设备初始化失败，回退到PlaySound\n");
#endif
				// MCI失败，回退到PlaySound
				if (PlaySound(L"sounds\\level3.wav", NULL,
					SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT))
				{
					g_bLevel3MusicPlaying = true;
					g_bLevel3MusicPaused = false;
				}
				return;
			}
		}

		// 如果有暂停位置，从暂停位置开始；否则从头开始
		if (g_dwLevel3MusicPosition > 0)
		{
			MCI_SEEK_PARMS mciSeek = { 0 };
			mciSeek.dwTo = g_dwLevel3MusicPosition;
			mciSendCommand(g_wLevel3MusicDeviceID, MCI_SEEK,
				MCI_TO, (DWORD_PTR)&mciSeek);
		}
		else
		{
			// 确保定位到开始
			mciSendCommand(g_wLevel3MusicDeviceID, MCI_SEEK,
				MCI_SEEK_TO_START, NULL);
		}

		// 播放音乐（使用MCI_NOTIFY以便循环）
		MCI_PLAY_PARMS mciPlay = { 0 };
		mciPlay.dwCallback = (DWORD_PTR)GetActiveWindow();

		if (mciSendCommand(g_wLevel3MusicDeviceID, MCI_PLAY,
			MCI_NOTIFY, (DWORD_PTR)&mciPlay) == 0)
		{
			g_bLevel3MusicPlaying = true;
			g_bLevel3MusicPaused = false;

#ifdef _DEBUG
			OutputDebugString(L"[音频] 关卡3音乐开始播放(MCI)\n");
#endif
		}
	}
}

void StopLevel3Music()
{
	if (g_bLevel3MusicPlaying)
	{
		if (g_wLevel3MusicDeviceID != 0 && g_bLevel3MusicInitialized)
		{
			// 停止播放并关闭设备
			mciSendCommand(g_wLevel3MusicDeviceID, MCI_STOP, 0, NULL);
			mciSendCommand(g_wLevel3MusicDeviceID, MCI_CLOSE, 0, NULL);
			g_wLevel3MusicDeviceID = 0;
			g_bLevel3MusicInitialized = false;
		}

		g_bLevel3MusicPlaying = false;
		g_bLevel3MusicPaused = false;
		g_dwLevel3MusicPosition = 0;

#ifdef _DEBUG
		OutputDebugString(L"[音频] 关卡3音乐停止\n");
#endif
	}
}

void PauseLevel3Music()
{
	if (g_bLevel3MusicPlaying && !g_bLevel3MusicPaused)
	{
		// 获取当前播放位置
		if (g_wLevel3MusicDeviceID != 0 && g_bLevel3MusicInitialized)
		{
			MCI_STATUS_PARMS mciStatus = { 0 };
			mciStatus.dwItem = MCI_STATUS_POSITION;

			if (mciSendCommand(g_wLevel3MusicDeviceID, MCI_STATUS,
				MCI_STATUS_ITEM, (DWORD_PTR)&mciStatus) == 0)
			{
				g_dwLevel3MusicPosition = mciStatus.dwReturn;
			}

			// 暂停播放
			mciSendCommand(g_wLevel3MusicDeviceID, MCI_PAUSE, 0, NULL);
		}

		g_bLevel3MusicPaused = true;

#ifdef _DEBUG
		wchar_t debugMsg[100];
		wsprintf(debugMsg, L"[音频] 关卡3音乐暂停，位置: %d ms\n", g_dwLevel3MusicPosition);
		OutputDebugString(debugMsg);
#endif
	}
}

void ResumeLevel3Music()
{
	if (g_bLevel3MusicPlaying && g_bLevel3MusicPaused)
	{
		if (systemConfig.enableMusic)
		{
			if (g_wLevel3MusicDeviceID != 0 && g_bLevel3MusicInitialized)
			{
				// 定位到暂停位置
				if (g_dwLevel3MusicPosition > 0)
				{
					MCI_SEEK_PARMS mciSeek = { 0 };
					mciSeek.dwTo = g_dwLevel3MusicPosition;
					mciSendCommand(g_wLevel3MusicDeviceID, MCI_SEEK,
						MCI_TO, (DWORD_PTR)&mciSeek);
				}

				// 继续播放
				MCI_PLAY_PARMS mciPlay = { 0 };
				mciPlay.dwCallback = (DWORD_PTR)GetActiveWindow();
				mciSendCommand(g_wLevel3MusicDeviceID, MCI_PLAY,
					MCI_NOTIFY, (DWORD_PTR)&mciPlay);

				g_bLevel3MusicPaused = false;

#ifdef _DEBUG
				OutputDebugString(L"[音频] 关卡3音乐从暂停位置恢复播放\n");
#endif
			}
			else
			{
				// 回退到PlaySound（从头开始）
				if (PlaySound(L"sounds\\level3.wav", NULL,
					SND_FILENAME | SND_ASYNC | SND_LOOP))
				{
					g_bLevel3MusicPaused = false;
				}
			}
		}
	}
}

// 根据场景处理音频 - 这是关键函数，集中管理所有场景的音频
void HandleStageAudio(int stageID)
{
	// 开始界面和选择关卡界面使用相同的音乐
	if (stageID == STAGE_STARTMENU || stageID == STAGE_SELECT)
	{
		// 停止关卡音乐（如果在播放）
		StopLevel1Music();
		StopLevel2Music();
		StopLevel3Music();

		// 确保开始界面音乐播放
		if (systemConfig.enableMusic && !g_bStartMenuMusicPlaying)
		{
			PlayStartMenuMusic();
		}
	}
	else if (stageID == STAGE_1)
	{
		// 停止开始界面音乐（只在进入游戏关卡时停止）
		if (g_bStartMenuMusicPlaying)
		{
			StopStartMenuMusic();
		}

		// 根据当前关卡播放相应的音乐
		if (currentStageLevel == 1)
		{
			// 播放关卡1音乐
			PlayLevel1Music();
		}
		else if (currentStageLevel == 2)
		{
			// 播放关卡2音乐
			PlayLevel2Music();
		}
		else if (currentStageLevel == 3)
		{
			PlayLevel3Music();
		}
		else
		{
			// 其他情况：确保所有关卡音乐停止
			StopLevel1Music();
			StopLevel2Music();
			StopLevel3Music();
		}
	}
}

// 通用音频播放函数（未来可用于播放各种音效和音乐）
bool PlayAudioFile(const wchar_t* filename, bool loop = false, bool async = true)
{
	if (!g_bEnableAudio || !filename) return false;

	DWORD flags = SND_FILENAME | SND_NODEFAULT;
	if (async) flags |= SND_ASYNC;
	if (loop) flags |= SND_LOOP;

	return PlaySound(filename, NULL, flags) != FALSE;
}

// 判断单位是否应该使用动画绘制
bool ShouldUseAnimationDrawing(Unit* unit)
{
	if (!unit || !unit->img) return false;


	// 规则1：玩家鱼总是使用动画绘制（使用bmp_Unit_Fish1贴图）
		if (unit->isPlayer) {
			return true;
		}

	// 规则2：原敌人鱼使用动画绘制（使用bmp_Unit_Fish2贴图）
	if (unit->img == bmp_Unit_Fish2) {
		return true;
	}

	// 规则3：其他鱼（小鱼、enemy_small1、enemy_small2等）都使用静态绘制
	return false;
}


// 添加按钮函数
Button* CreateButton(int buttonID, HBITMAP img, int width, int height, int x, int y)
{
	Button* button = new Button();
	button->buttonID = buttonID;
	button->img = img;
	button->width = width;
	button->height = height;
	button->x = x;
	button->y = y;

	button->visible = false;
	return button;
}

//1123添加9，此处开始
HBITMAP CreateSimpleColorButton(COLORREF color, int width, int height, const wchar_t* text) {
	HDC hdcScreen = GetDC(NULL);
	HDC hdcMem = CreateCompatibleDC(hdcScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);
	// 绘制背景
	HBRUSH brush = CreateSolidBrush(color);
	RECT rect = { 0, 0, width, height };
	FillRect(hdcMem, &rect, brush);
	DeleteObject(brush);
	// 绘制边框
	FrameRect(hdcMem, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
	// 绘制文字
	SetBkMode(hdcMem, TRANSPARENT);
	SetTextColor(hdcMem, RGB(0, 0, 0));
	// 创建字体
	int fontSize = 16;
	// 判断是否为返回按钮
	bool isBackButton = false;
	if (wcsstr(text, L"返回") != NULL) {
		isBackButton = true;
	}
	if (isBackButton) {
		// 返回按钮使用较大字体，基于按钮高度计算
		fontSize = (height * 4) / 5;  
		if (fontSize < 16) fontSize = 16;
		if (fontSize > 30) fontSize = 30;
	}
	else {
		// 其他按钮使用默认大小
		fontSize = (height * 2) / 5;
		if (fontSize < 12) fontSize = 12;
	}

	HFONT hFont = CreateFont(
		-fontSize,       // 负数高度表示按像素计算
		0, 0, 0,
		FW_BOLD,        // 粗体
		FALSE, FALSE, FALSE,
		GB2312_CHARSET, // 中文支持
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"微软雅黑"
	);
	HFONT hOldFont = (HFONT)SelectObject(hdcMem, hFont);

	// 计算文字区域，确保文字在按钮内
	RECT textRect = rect;

	DrawText(hdcMem, text, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// 清理资源
	SelectObject(hdcMem, hOldFont);
	DeleteObject(hFont);
	SelectObject(hdcMem, hOldBitmap);
	DeleteDC(hdcMem);
	ReleaseDC(NULL, hdcScreen);

	return hBitmap;
}
//1123添加9，此处结束


// 添加主角函数
Unit* CreateUnit(int side, int type, int x, int y, int health)
{
	Unit* unit = new Unit();
	unit->side = side;
	if (side == UNIT_SIDE_FISH1) {
		unit->img = bmp_Unit_Fish1;
		unit->direction = UNIT_DIRECT_LEFT;
	}
	else if (side == UNIT_SIDE_FISH2) {
		unit->img = bmp_Unit_Fish2;
		unit->direction = UNIT_DIRECT_RIGHT;
	}

	unit->type = type;
	unit->state = UNIT_STATE_HOLD;


	unit->frame_row = type;
	unit->frame_column = UNIT_LAST_FRAME * unit->direction;

	unit->frame_sequence = FRAMES_HOLD;
	unit->frame_count = FRAMES_HOLD_COUNT;
	unit->frame_id = 0;

	unit->x = x;
	unit->y = y;
	unit->vx = 0;
	unit->vy = 0;
	unit->health = health;
	return unit;
}

Unit* CreateCompatibleUnit(int side, int type, int x, int y, int health, int sizeLevel, bool isPlayer)
{
	// 使用旧系统创建单位
	Unit* unit = CreateUnit(side, type, x, y, health);

	// 添加新系统属性
	unit->useNewSystem = true;
	unit->sizeLevel = sizeLevel;
	unit->isPlayer = isPlayer;

	// 设置fishType
	if (isPlayer) {
		unit->fishType = FISH_TYPE_PLAYER;
	}
	else {
		// 根据side和type判断敌人类型
		if (side == UNIT_SIDE_FISH2) {
			if (sizeLevel == 1) unit->fishType = FISH_TYPE_SMALL_FISH;
			else if (sizeLevel == 2) unit->fishType = FISH_TYPE_ENEMY_SMALL1;
			else if (sizeLevel == 3) unit->fishType = FISH_TYPE_ENEMY_MEDIUM;
			else if (sizeLevel == 4) unit->fishType = FISH_TYPE_ENEMY_LARGE;
			else unit->fishType = FISH_TYPE_ENEMY_GIANT;
		}
	}

	unit->experience = 0;
	return unit;
}


// 初始化游戏场景函数
void InitStage(HWND hWnd, int stageID)
{
	// 隐藏所有关卡相关按钮
	HideLevelCompleteButtons();  
	
	// 重置游戏状态
	gameState = GAME_RUNNING;

	// ====== 新增：音频场景处理 ======
	HandleStageAudio(stageID);

	// 清理当前场景的背景（如果是动态加载的背景）
	if (currentStage != NULL && currentStage->bg != NULL) {
		// 检查是否为需要删除的动态背景
		bool shouldDelete = true;

		// 以下背景不应删除：
		if (currentStage->bg == bmp_Start_Background ||      // 备用背景
			currentStage->bg == bmp_SelectBackground) {      // 选择界面背景
			shouldDelete = false;
		}

		// 如果是第1、2、3关的动态背景，应该删除
		if (shouldDelete) {
			// 这是动态加载的背景（如第1、2、3关背景），需要删除
			DeleteObject(currentStage->bg);
			currentStage->bg = NULL;

#ifdef _DEBUG
			wchar_t debugMsg[100];
			wsprintf(debugMsg, L"[清理] 删除动态背景\n");
			OutputDebugString(debugMsg);
#endif
		}
	}
	currentStage = new Stage();
	currentStage->stageID = stageID;
	currentStage->timerOn = true;  // 确保计时器状态正确

	// 如果重新开始游戏，重置进度
	progressValue = 0;


	if (stageID == STAGE_STARTMENU) {
		currentStage->bg = bmp_Start_Background;
		currentStage->timeCountDown = 0;
		currentStage->timerOn = false;

		//显示开始界面的按钮
		for (int i = 0; i < buttons.size(); i++)
		{
			Button* button = buttons[i];
			if (button->buttonID == BUTTON_STARTGAME || button->buttonID == BUTTON_HELP)
			{
				button->visible = true;
			}
			else
			{
				button->visible = false;
			}
		}

		//1123添加5，此处开始
		// 设置按钮显示状态
		//1123添加5，此处结束

	}

	else if (stageID == STAGE_SELECT) {
		currentStage->bg = bmp_SelectBackground;
		// 如果bmp_SelectBackground为NULL，使用备用背景
		if (currentStage->bg == NULL) {
			currentStage->bg = bmp_Start_Background;
		}
		currentStage->timerOn = false;

		// 隐藏所有现有按钮
		for (int i = 0; i < buttons.size(); i++) {
			buttons[i]->visible = false;
		}

		// 修复：正确检查按钮是否存在
		bool level1Exists = false, level2Exists = false, level3Exists = false, backExists = false;
		for (int i = 0; i < buttons.size(); i++) {
			Button* btn = buttons[i];
			if (btn->buttonID == BUTTON_LEVEL1) level1Exists = true;
			else if (btn->buttonID == BUTTON_LEVEL2) level2Exists = true;
			else if (btn->buttonID == BUTTON_LEVEL3) level3Exists = true;
			else if (btn->buttonID == BUTTON_BACK) backExists = true;
		}

		// 按钮尺寸计算
		const float SCALE_FACTOR = 1.5f;
		int buttonWidth = static_cast<int>(67 * SCALE_FACTOR);
		int buttonHeight = static_cast<int>(62 * SCALE_FACTOR);

		// 计算垂直居中的按钮排列
		int totalHeight = buttonHeight * 3 + BUTTON_LEVEL_SPACING * 2;
		int startY = (WINDOW_HEIGHT - totalHeight) / 2 + 50;
		int centerX = WINDOW_WIDTH / 2 - buttonWidth / 2;

		// 创建或更新关卡1按钮
		if (!level1Exists) {
			Button* level1Button = CreateButton(BUTTON_LEVEL1, bmp_Level1Button,
				buttonWidth, buttonHeight,
				centerX, startY);
			level1Button->visible = true;
			buttons.push_back(level1Button);
		}
		else {
			// 找到并更新已存在的关卡1按钮
			for (int i = 0; i < buttons.size(); i++) {
				Button* btn = buttons[i];
				if (btn->buttonID == BUTTON_LEVEL1) {
					btn->x = centerX;
					btn->y = startY;
					btn->width = buttonWidth;
					btn->height = buttonHeight;
					btn->visible = true;
					break;
				}
			}
		}

		// 创建或更新关卡2按钮
		if (!level2Exists) {
			Button* level2Button = CreateButton(BUTTON_LEVEL2, bmp_Level2Button,
				buttonWidth, buttonHeight,
				centerX, startY + buttonHeight + BUTTON_LEVEL_SPACING);
			level2Button->visible = true;
			buttons.push_back(level2Button);
		}
		else {
			// 找到并更新已存在的关卡2按钮
			for (int i = 0; i < buttons.size(); i++) {
				Button* btn = buttons[i];
				if (btn->buttonID == BUTTON_LEVEL2) {
					btn->x = centerX;
					btn->y = startY + buttonHeight + BUTTON_LEVEL_SPACING;
					btn->width = buttonWidth;
					btn->height = buttonHeight;
					btn->visible = true;
					break;
				}
			}
		}

		// 创建或更新关卡3按钮
		if (!level3Exists) {
			Button* level3Button = CreateButton(BUTTON_LEVEL3, bmp_Level3Button,
				buttonWidth, buttonHeight,
				centerX, startY + 2 * (buttonHeight + BUTTON_LEVEL_SPACING));
			level3Button->visible = true;
			buttons.push_back(level3Button);
		}
		else {
			// 找到并更新已存在的关卡3按钮
			for (int i = 0; i < buttons.size(); i++) {
				Button* btn = buttons[i];
				if (btn->buttonID == BUTTON_LEVEL3) {
					btn->x = centerX;
					btn->y = startY + 2 * (buttonHeight + BUTTON_LEVEL_SPACING);
					btn->width = buttonWidth;
					btn->height = buttonHeight;
					btn->visible = true;
					break;
				}
			}
		}

		// 创建或更新返回按钮
		if (!backExists) {
			int backButtonWidth = 120;
			int backButtonHeight = 50;
			int backButtonX = 20;  
			int backButtonY = 30; 
			// 创建新的返回按钮
			HBITMAP backBmp = CreateSimpleColorButton(RGB(173, 216, 230),
				backButtonWidth, backButtonHeight, L"返回");
			Button* backButton = CreateButton(BUTTON_BACK, backBmp,
				backButtonWidth, backButtonHeight,
				backButtonX, backButtonY);
			backButton->visible = true; 
			buttons.push_back(backButton);

			wchar_t debugMsg[256];
			wsprintf(debugMsg, L"创建新的返回按钮: x=%d, y=%d\n", 20, 30);
			OutputDebugString(debugMsg);
		}
		else {
			// 更新已存在的返回按钮
			for (int i = 0; i < buttons.size(); i++) {
				Button* btn = buttons[i];
				if (btn->buttonID == BUTTON_BACK) {
					int newWidth = 120;
					int newHeight = 50;
					int newX = 20;      // 左侧
					int newY = 30;      // 顶部

					btn->width = newWidth;
					btn->height = newHeight;
					btn->x = newX;
					btn->y = newY;      
					btn->visible = true;

					// 重新创建位图
					if (btn->img) {
						DeleteObject(btn->img);
					}
					btn->img = CreateSimpleColorButton(RGB(173, 216, 230),
						newWidth, newHeight, L"返回");

					// 调试信息
					wchar_t debugMsg[256];
					wsprintf(debugMsg, L"更新返回按钮到左上角: x=%d, y=%d\n",
						btn->x, btn->y);
					OutputDebugString(debugMsg);
					break;
				}
			}
		}
	}

	else if (stageID == STAGE_1) // 第一关场景，但第2、3关也使用这个场景ID
	{
		currentStage->stageID = stageID;

		// 根据 currentStageLevel 设置背景
		wchar_t debugMsg[256];
		wsprintf(debugMsg, L"进入关卡：currentStageLevel=%d\n", currentStageLevel);
		OutputDebugString(debugMsg);

		// === 统一处理所有关卡背景加载 ===
		const wchar_t* bgFile = NULL;
		int expectedWidth = 0, expectedHeight = 0;

		if (currentStageLevel == 1) {
			bgFile = L"images\\bg1.bmp";
			expectedWidth = 640;   // 第一关背景尺寸
			expectedHeight = 480;
		}
		else if (currentStageLevel == 2) {
			bgFile = L"images\\bg2.bmp";
			expectedWidth = 800;   // 第二关背景尺寸
			expectedHeight = 600;
		}
		else if (currentStageLevel == 3) {
			bgFile = L"images\\bg3.bmp";
			expectedWidth = 800;   // 第三关背景尺寸
			expectedHeight = 600;
		}

		bool bgLoaded = false;

		// 尝试加载动态背景
		if (bgFile != NULL) {
			// 直接从文件加载并拉伸到窗口大小
			HBITMAP rawBmp = (HBITMAP)LoadImageW(
				NULL,
				bgFile,
				IMAGE_BITMAP,
				0, 0,
				LR_LOADFROMFILE | LR_CREATEDIBSECTION
			);

			if (rawBmp != NULL) {
				// 验证加载的位图尺寸
				BITMAP bm;
				GetObject(rawBmp, sizeof(BITMAP), &bm);

				wchar_t debugMsg[256];
				wsprintf(debugMsg, L"加载背景：%s，原始尺寸：%dx%d，期望：%dx%d\n",
					bgFile, bm.bmWidth, bm.bmHeight, expectedWidth, expectedHeight);
				OutputDebugString(debugMsg);

				if (bm.bmWidth == expectedWidth && bm.bmHeight == expectedHeight) {
					// 使用 InitBackGround 函数拉伸到1024x768
					currentStage->bg = InitBackGround(hWnd, rawBmp);
					DeleteObject(rawBmp);  // 删除原始位图

					// 验证拉伸后的尺寸
					GetObject(currentStage->bg, sizeof(BITMAP), &bm);
					wsprintf(debugMsg, L"背景拉伸完成，当前尺寸：%dx%d\n",
						bm.bmWidth, bm.bmHeight);
					OutputDebugString(debugMsg);

					bgLoaded = true;
				}
				else {
					wsprintf(debugMsg, L"警告：背景尺寸不匹配！实际：%dx%d，期望：%dx%d\n",
						bm.bmWidth, bm.bmHeight, expectedWidth, expectedHeight);
					OutputDebugString(debugMsg);
					DeleteObject(rawBmp);
				}
			}
		}

		// 如果动态加载失败，使用备用背景
		if (!bgLoaded) {
			currentStage->bg = bmp_Start_Background;  // 使用开始菜单背景作为备用

#ifdef _DEBUG
			wchar_t debugMsg[256];
			wsprintf(debugMsg, L"第%d关背景加载失败，使用备用背景\n", currentStageLevel);
			OutputDebugString(debugMsg);
#endif
		}

		currentStage->timeCountDown = 10000;
		currentStage->timerOn = true;

		//显示游戏界面的按钮
		for (int i = 0; i < buttons.size(); i++)
		{
			Button* button = buttons[i];
			if (false) //TODO：加载游戏界面需要的按钮
			{
				button->visible = true;
			}
			else
			{
				button->visible = false;
			}
		}

		// 清空原有单位
		ClearUnits();

		// 清空小鱼
		ClearSmallFishes();

		// === 新增：清理鱼雷 ===
		ClearTorpedoes();

		// 重置进度
		progressValue = 0;

		// 创建玩家鱼
		// 创建玩家鱼 - 使用兼容函数
		Unit* playerFish = CreateCompatibleUnit(
			UNIT_SIDE_FISH1,
			UNIT_FISH_TYPE1,
			200, 200,
			100,
			FISH_SIZE_LEVEL_2,
			true  // 是玩家
		);
		playerFish->direction = UNIT_DIRECT_LEFT;
		playerFish->state = UNIT_STATE_HOLD;
		playerFish->frame_row = playerFish->type;
		playerFish->frame_column = UNIT_LAST_FRAME * playerFish->direction;
		playerFish->frame_sequence = FRAMES_HOLD;
		playerFish->frame_count = FRAMES_HOLD_COUNT;
		playerFish->frame_id = 0;
		playerFish->x = 200;
		playerFish->y = 200;
		playerFish->vx = 0;
		playerFish->vy = 0;
		playerFish->health = 100;
		playerFish->size = PLAYER_FISH_SIZE;  // 中等大小
		units.push_back(playerFish);

		if (currentStageLevel == 1) {
		Unit* legacyEnemyFish = CreateUnit(
			UNIT_SIDE_FISH2,      // 敌人鱼阵营
			UNIT_FISH_TYPE2,      // 敌人鱼类型
			600, 200,             // 位置
			100                   // 生命值
		);

		// 设置属性
		legacyEnemyFish->direction = UNIT_DIRECT_RIGHT;
		legacyEnemyFish->state = UNIT_STATE_WALK;
		legacyEnemyFish->frame_row = 0;  // 关键：原敌人鱼在Fish1位图的第0行（不是第1行）
		legacyEnemyFish->frame_column = UNIT_LAST_FRAME * legacyEnemyFish->direction;
		legacyEnemyFish->frame_sequence = FRAMES_WALK;
		legacyEnemyFish->frame_count = FRAMES_WALK_COUNT;
		legacyEnemyFish->frame_id = 0;
		legacyEnemyFish->x = 600;
		legacyEnemyFish->y = 200;
		legacyEnemyFish->health = 100;
		// === 新增：设置初始随机速度 ===
// 生成随机角度（0-359度）
		int direction = rand() % 360;
		double angle = direction * PI / 180.0;

		// 设置初始速度（比玩家鱼稍慢），并添加随机速度变化
		double speedVariation = 0.8 + (rand() % 40) * 0.01;  // 0.8-1.2倍随机变化
		legacyEnemyFish->vx = cos(angle) * (UNIT_SPEED * 0.7 * speedVariation);
		legacyEnemyFish->vy = sin(angle) * (UNIT_SPEED * 0.7 * speedVariation);

		// 根据速度方向更新朝向
		if (legacyEnemyFish->vx > 0) {
			legacyEnemyFish->direction = UNIT_DIRECT_RIGHT;
			legacyEnemyFish->frame_column = UNIT_DIRECT_RIGHT * UNIT_LAST_FRAME;
		}
		else {
			legacyEnemyFish->direction = UNIT_DIRECT_LEFT;
			legacyEnemyFish->frame_column = UNIT_DIRECT_LEFT * UNIT_LAST_FRAME;
		}
		legacyEnemyFish->size = 4;

		// 关键：确保贴图正确
		legacyEnemyFish->img = bmp_Unit_Fish2;
		// 关键：明确设置为旧系统
		legacyEnemyFish->useNewSystem = true;  // 旧系统
		legacyEnemyFish->isPlayer = false;      // 不是玩家
		legacyEnemyFish->fishType = FISH_TYPE_ENEMY_LARGE;          // 旧系统没有这个属性

		// 设置新系统属性为默认值（兼容性）
		legacyEnemyFish->sizeLevel = FISH_SIZE_LEVEL_4;
		legacyEnemyFish->experience = 0;
		units.push_back(legacyEnemyFish);
		}

		if (currentStageLevel == 3) {
			Unit* legacyEnemyFish = CreateUnit(
				UNIT_SIDE_FISH2,      // 敌人鱼阵营
				UNIT_FISH_TYPE2,      // 敌人鱼类型
				600, 200,             // 位置
				100                   // 生命值
			);

			// 设置属性
			legacyEnemyFish->direction = UNIT_DIRECT_RIGHT;
			legacyEnemyFish->state = UNIT_STATE_WALK;
			legacyEnemyFish->frame_row = 0;  // 关键：原敌人鱼在Fish1位图的第0行（不是第1行）
			legacyEnemyFish->frame_column = UNIT_LAST_FRAME * legacyEnemyFish->direction;
			legacyEnemyFish->frame_sequence = FRAMES_WALK;
			legacyEnemyFish->frame_count = FRAMES_WALK_COUNT;
			legacyEnemyFish->frame_id = 0;
			legacyEnemyFish->x = 600;
			legacyEnemyFish->y = 200;
			legacyEnemyFish->health = 100;
			// === 新增：设置初始随机速度 ===
	// 生成随机角度（0-359度）
			int direction = rand() % 360;
			double angle = direction * PI / 180.0;

			// 设置初始速度（比玩家鱼稍慢），并添加随机速度变化
			double speedVariation = 0.8 + (rand() % 40) * 0.01;  // 0.8-1.2倍随机变化
			legacyEnemyFish->vx = cos(angle) * (UNIT_SPEED * 0.7 * speedVariation);
			legacyEnemyFish->vy = sin(angle) * (UNIT_SPEED * 0.7 * speedVariation);

			// 根据速度方向更新朝向
			if (legacyEnemyFish->vx > 0) {
				legacyEnemyFish->direction = UNIT_DIRECT_RIGHT;
				legacyEnemyFish->frame_column = UNIT_DIRECT_RIGHT * UNIT_LAST_FRAME;
			}
			else {
				legacyEnemyFish->direction = UNIT_DIRECT_LEFT;
				legacyEnemyFish->frame_column = UNIT_DIRECT_LEFT * UNIT_LAST_FRAME;
			}
			legacyEnemyFish->size = 4;

			// 关键：确保贴图正确
			legacyEnemyFish->img = bmp_Unit_Fish2;
			// 关键：明确设置为旧系统
			legacyEnemyFish->useNewSystem = true;  // 旧系统
			legacyEnemyFish->isPlayer = false;      // 不是玩家
			legacyEnemyFish->fishType = FISH_TYPE_ENEMY_LARGE;          // 旧系统没有这个属性

			// 设置新系统属性为默认值（兼容性）
			legacyEnemyFish->sizeLevel = FISH_SIZE_LEVEL_4;
			legacyEnemyFish->experience = 0;
			units.push_back(legacyEnemyFish);
		}

		

		// 生成几条初始的小鱼
		for (int i = 0; i < 2; i++) {
			SpawnSmallFish(hWnd);
		}

		// 启动生成小鱼的定时器
		SetTimer(hWnd, TIMER_SPAWN_FISH, SPAWN_INTERVAL, NULL);

		//1123添加6，此处开始


		// 设置按钮显示状态
		for (int i = 0; i < buttons.size(); i++) {
			Button* button = buttons[i];
			if (button->buttonID == BUTTON_PAUSE) {
				button->visible = true;    // 显示暂停按钮
			}
			else if (button->buttonID == BUTTON_RESUME) {
				button->visible = false;   // 隐藏继续按钮
			}
			else if (button->buttonID == BUTTON_HELP) {
				button->visible = true;
			}
			// 确保通关按钮隐藏
			else if (button->buttonID == BUTTON_NEXT_LEVEL ||
				button->buttonID == BUTTON_RETRY_LEVEL ||
				button->buttonID == BUTTON_SELECT_LEVEL) {
				button->visible = false;
			}
			wchar_t debugMsg[100];
		}

		//1123添加6，此处结束
		// 确保计时器启动
		SetTimer(hWnd, TIMER_GAMETIMER, TIMER_GAMETIMER_ELAPSE, NULL);
		SetTimer(hWnd, TIMER_SPAWN_FISH, SPAWN_INTERVAL, NULL);
		}

	//刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}


//单位行为函数
void UnitBehaviour_1(Unit* unit) {
	double dirX = mouseX - unit->x;
	double dirY = mouseY - unit->y;
	double dirLen = sqrt(dirX * dirX + dirY * dirY) + 0.0001;

	// 设置朝向
	if (dirX > 0) {
		unit->direction = UNIT_DIRECT_RIGHT;
	}
	else {
		unit->direction = UNIT_DIRECT_LEFT;
	}

	// 判断当前状态
	int next_state = unit->state;
	switch (unit->state) {
	case UNIT_STATE_HOLD:
		if (dirLen > 10) {  // 如果距离大于10像素，就开始移动
			next_state = UNIT_STATE_WALK;
		}
		break;
	case UNIT_STATE_WALK:
		if (dirLen <= 10) {  // 非常接近时切换为攻击状态
			next_state = UNIT_STATE_ATTACK;
		}
		else {
			unit->vx = dirX / dirLen * UNIT_SPEED;
			unit->vy = dirY / dirLen * UNIT_SPEED;
		}
		break;
	case UNIT_STATE_ATTACK:
		if (dirLen > 30) {  // 如果距离又变远了，切回行走状态
			next_state = UNIT_STATE_WALK;
		}
		else if (dirLen > 2) {  // 距离还大于2像素时，继续缓慢接近
			unit->vx = dirX / dirLen * (UNIT_SPEED * 0.3);  // 攻击状态下移动较慢
			unit->vy = dirY / dirLen * (UNIT_SPEED * 0.3);
		}
		else {  // 距离小于2像素，完全停止
			unit->vx = 0;
			unit->vy = 0;
		}
		break;
	};

	if (next_state != unit->state) {
		// 状态变化
		unit->state = next_state;
		unit->frame_id = 0;  // 切换状态时重置动画

		switch (unit->state) {
		case UNIT_STATE_HOLD:
			unit->frame_sequence = FRAMES_HOLD;
			unit->frame_count = FRAMES_HOLD_COUNT;
			unit->vx = 0;
			unit->vy = 0;
			break;
		case UNIT_STATE_WALK:
			unit->frame_sequence = FRAMES_WALK;
			unit->frame_count = FRAMES_WALK_COUNT;
			unit->vx = dirX / dirLen * UNIT_SPEED;
			unit->vy = dirY / dirLen * UNIT_SPEED;
			break;
		case UNIT_STATE_ATTACK:
			unit->frame_sequence = FRAMES_ATTACK;
			unit->frame_count = FRAMES_ATTACK_COUNT;
			if (dirLen > 2) {
				unit->vx = dirX / dirLen * (UNIT_SPEED * 0.3);
				unit->vy = dirY / dirLen * (UNIT_SPEED * 0.3);
			}
			else {
				unit->vx = 0;
				unit->vy = 0;
			}
			break;
		};
	}
	else {
		// 同一状态下，保持速度计算（确保每次更新都重新计算）
		if (unit->state == UNIT_STATE_WALK && dirLen > 10) {
			unit->vx = dirX / dirLen * UNIT_SPEED;
			unit->vy = dirY / dirLen * UNIT_SPEED;
		}
		else if (unit->state == UNIT_STATE_ATTACK && dirLen > 2 && dirLen <= 30) {
			unit->vx = dirX / dirLen * (UNIT_SPEED * 0.3);
			unit->vy = dirY / dirLen * (UNIT_SPEED * 0.3);
		}
	}

	// 更新位置
	unit->x += unit->vx;
	unit->y += unit->vy;

	// 更新动画
	unit->frame_id++;
	unit->frame_id = unit->frame_id % unit->frame_count;

	int column = unit->frame_sequence[unit->frame_id];
	unit->frame_column = column + unit->direction * (UNIT_LAST_FRAME - 2 * column);
}

void UnitBehaviour_2(Unit* unit) {

	{
		// 随机移动的敌人鱼行为
		static int changeDirectionCounter = 0;
		changeDirectionCounter++;

		// 每30帧（约1秒）随机改变一次方向，让移动更随机
		if (changeDirectionCounter % 30 == 0) {
			// 随机生成一个方向（0-359度）
			int direction = rand() % 360;

			// 将角度转换为弧度
			double angle = direction * PI / 180.0;

			// 设置速度（比玩家鱼稍慢）
			unit->vx = cos(angle) * (UNIT_SPEED * 0.7);
			unit->vy = sin(angle) * (UNIT_SPEED * 0.7);

			// 根据x方向设置鱼的朝向
			if (unit->vx > 0) {
				unit->direction = UNIT_DIRECT_RIGHT;
			}
			else if (unit->vx < 0) {
				unit->direction = UNIT_DIRECT_LEFT;
			}
		}

		// 更新位置
		unit->x += unit->vx;
		unit->y += unit->vy;

		// 边界检测和反弹
		const int collisionRadius = 32;

		// 检查左边界
		if (unit->x - collisionRadius < 0) {
			unit->x = collisionRadius;
			unit->vx = -unit->vx;  // 反弹
			unit->direction = UNIT_DIRECT_RIGHT;
		}
		// 检查右边界
		else if (unit->x + collisionRadius > WINDOW_WIDTH) {
			unit->x = WINDOW_WIDTH - collisionRadius;
			unit->vx = -unit->vx;
			unit->direction = UNIT_DIRECT_LEFT;
		}

		// 检查上边界
		if (unit->y - collisionRadius < 0) {
			unit->y = collisionRadius;
			unit->vy = -unit->vy;  // 反弹
		}
		// 检查下边界
		else if (unit->y + collisionRadius > WINDOW_HEIGHT) {
			unit->y = WINDOW_HEIGHT - collisionRadius;
			unit->vy = -unit->vy;
		}

		// 设置状态为行走
		unit->state = UNIT_STATE_WALK;

		// 更新动画帧（使用原有的行走动画序列）
		if (unit->state != UNIT_STATE_HOLD) {
			unit->frame_id = (unit->frame_id + 1) % unit->frame_count;

			int column = unit->frame_sequence[unit->frame_id];
			unit->frame_column = column + unit->direction * (UNIT_LAST_FRAME - 2 * column);
		}

	}
}

// 小鱼行为函数 - 随机移动，方向改变更频繁
void UnitBehaviour_SmallFish(Unit* unit) {
	static int changeDirectionCounter = 0;
	changeDirectionCounter++;

	// 每20帧（约0.6秒）随机改变一次方向 - 比敌人鱼更频繁
	if (changeDirectionCounter % 20 == 0) {
		// 随机生成一个方向（0-359度）
		int direction = rand() % 360;
		double angle = direction * PI / 180.0;

		// 设置速度（比玩家鱼稍慢）
		unit->vx = cos(angle) * (UNIT_SPEED * 0.5);  // 小鱼速度更慢
		unit->vy = sin(angle) * (UNIT_SPEED * 0.5);

		// 根据x方向设置鱼的朝向
		if (unit->vx > 0) {
			unit->direction = UNIT_DIRECT_RIGHT;
		}
		else if (unit->vx < 0) {
			unit->direction = UNIT_DIRECT_LEFT;
		}

		// 除了定时改变方向，也添加一些随机性：10%的概率在非定时改变时也微调方向
		if (rand() % 100 < 10) {
			// 轻微调整方向
			double currentAngle = atan2(unit->vy, unit->vx);
			double angleChange = (rand() % 60 - 30) * PI / 180.0;  // -30到30度
			currentAngle += angleChange;

			unit->vx = cos(currentAngle) * (UNIT_SPEED * 0.5);
			unit->vy = sin(currentAngle) * (UNIT_SPEED * 0.5);

			// 更新朝向
			if (unit->vx > 0) {
				unit->direction = UNIT_DIRECT_RIGHT;
			}
			else if (unit->vx < 0) {
				unit->direction = UNIT_DIRECT_LEFT;
			}
		}
	}

	// 更新位置
	unit->x += unit->vx;
	unit->y += unit->vy;

	// 边界检测和反弹 - 与敌人鱼类似
	const int collisionRadius = 16;  // 小鱼碰撞半径更小

	// 检查左边界
	if (unit->x - collisionRadius < 0) {
		unit->x = collisionRadius;
		unit->vx = -unit->vx;  // 反弹
		unit->direction = UNIT_DIRECT_RIGHT;

		// 反弹后添加随机垂直速度
		unit->vy = (rand() % 20 - 10) * 0.2;
	}
	// 检查右边界
	else if (unit->x + collisionRadius > WINDOW_WIDTH) {
		unit->x = WINDOW_WIDTH - collisionRadius;
		unit->vx = -unit->vx;
		unit->direction = UNIT_DIRECT_LEFT;

		// 反弹后添加随机垂直速度
		unit->vy = (rand() % 20 - 10) * 0.2;
	}

	// 检查上边界
	if (unit->y - collisionRadius < 0) {
		unit->y = collisionRadius;
		unit->vy = -unit->vy;  // 反弹

		// 反弹后添加随机水平速度
		unit->vx = (rand() % 20 - 10) * 0.2;
	}
	// 检查下边界
	else if (unit->y + collisionRadius > WINDOW_HEIGHT) {
		unit->y = WINDOW_HEIGHT - collisionRadius;
		unit->vy = -unit->vy;

		// 反弹后添加随机水平速度
		unit->vx = (rand() % 20 - 10) * 0.2;
	}

	// 设置状态为行走
	unit->state = UNIT_STATE_WALK;

	// 更新动画帧（使用原有的行走动画序列）
	if (unit->state != UNIT_STATE_HOLD) {
		unit->frame_id = (unit->frame_id + 1) % unit->frame_count;

		int column = unit->frame_sequence[unit->frame_id];
		unit->frame_column = column + unit->direction * (UNIT_LAST_FRAME - 2 * column);
	}
}

#pragma endregion


#pragma region 绘图函数
// 绘图函数
void Paint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc_window = BeginPaint(hWnd, &ps);

	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
	HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

	//初始化缓存
	HBITMAP blankBmp = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(hdc_memBuffer, blankBmp);

	// 按场景分类绘制内容到缓存
	if (currentStage->stageID == STAGE_STARTMENU) {
		// 1. 绘制开始菜单背景
		SelectObject(hdc_loadBmp, bmp_Start_Background);
		BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);

		// 2. 添加游戏标题
		SetTextColor(hdc_memBuffer, RGB(255, 255, 255));
		SetBkMode(hdc_memBuffer, TRANSPARENT);

		// 主标题
		HFONT hTitleFont = CreateFont(
			80, 0, 0, 0, FW_BOLD,
			FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_SWISS, L"微软雅黑"
		);
		HFONT hOldFont = (HFONT)SelectObject(hdc_memBuffer, hTitleFont);

		wchar_t titleMain[] = L"吞食鱼";
		RECT titleRect = { 0, 150, WINDOW_WIDTH, 250 };
		DrawText(hdc_memBuffer, titleMain, -1, &titleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		// 副标题
		HFONT hSubtitleFont = CreateFont(
			30, 0, 0, 0, FW_NORMAL,
			FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_SWISS, L"宋体"
		);
		SelectObject(hdc_memBuffer, hSubtitleFont);

		wchar_t titleSub[] = L"Feeding Frenzy";
		RECT subtitleRect = { 0, 220, WINDOW_WIDTH, 280 };
		DrawText(hdc_memBuffer, titleSub, -1, &subtitleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		// 恢复原来的字体
		SelectObject(hdc_memBuffer, hOldFont);
		DeleteObject(hTitleFont);
		DeleteObject(hSubtitleFont);
	}
	else if (currentStage->stageID == STAGE_SELECT) {
		// 1. 绘制选择关卡背景 - 修复：使用currentStage->bg
		if (currentStage->bg != NULL) {
			HBITMAP hOldBmp = (HBITMAP)SelectObject(hdc_loadBmp, currentStage->bg);
			BITMAP bm;
			GetObject(currentStage->bg, sizeof(BITMAP), &bm);

			// 拉伸到整个窗口
			StretchBlt(
				hdc_memBuffer,
				0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
				hdc_loadBmp,
				0, 0, bm.bmWidth, bm.bmHeight,
				SRCCOPY
			);

			SelectObject(hdc_loadBmp, hOldBmp);
		}
		else {
			// 备用背景
			HBRUSH hBlueBrush = CreateSolidBrush(RGB(50, 50, 120));
			RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
			FillRect(hdc_memBuffer, &rect, hBlueBrush);
			DeleteObject(hBlueBrush);
		}

		// 2. 添加标题文字
		SetBkMode(hdc_memBuffer, TRANSPARENT);
		SetTextColor(hdc_memBuffer, RGB(255, 255, 255));

		// 主标题
		HFONT hTitleFont = CreateFont(60, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH, L"微软雅黑");
		HFONT hOldFont = (HFONT)SelectObject(hdc_memBuffer, hTitleFont);

		wchar_t title[] = L"选择关卡";
		RECT titleRect = { 0, 80, WINDOW_WIDTH, 180 };
		DrawText(hdc_memBuffer, title, -1, &titleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		// 副标题
		HFONT hSubFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH, L"宋体");
		SelectObject(hdc_memBuffer, hSubFont);

		wchar_t subtitle[] = L"点击关卡按钮开始游戏";
		RECT subtitleRect = { 0, 160, WINDOW_WIDTH, 220 };
		DrawText(hdc_memBuffer, subtitle, -1, &subtitleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		// 恢复旧字体并删除新字体
		SelectObject(hdc_memBuffer, hOldFont);
		DeleteObject(hTitleFont);
		DeleteObject(hSubFont);
	}
	else if (currentStage->stageID >= STAGE_1 && currentStage->stageID <= STAGE_1) {
		// 1. 绘制游戏场景背景 - 使用当前场景的动态背景
		if (currentStage && currentStage->bg != NULL) {
			SelectObject(hdc_loadBmp, currentStage->bg);
			BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);
		}
		else {
			// 后备：使用默认背景
			SelectObject(hdc_loadBmp, bmp_Stage_Background);
			BitBlt(hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_loadBmp, 0, 0, SRCCOPY);
		}
		// 2. 绘制单位（鱼）
		for (int i = 0; i < units.size(); i++) {
			Unit* unit = units[i];
			if (!unit) continue;

			SelectObject(hdc_loadBmp, unit->img);

			// 计算缩放比例
			float scale = 1.0f;
			if (unit->useNewSystem) {
				if (unit->sizeLevel >= 1 && unit->sizeLevel <= 5) {
					scale = sizeScales[unit->sizeLevel];
				}
			}
			else {
				if (unit->side == UNIT_SIDE_FISH2) {
					scale = 1.5f;  // 原敌人鱼稍大
				}
			}

			// === 判断哪些鱼使用动画绘制 ===
			bool useAnimation = ShouldUseAnimationDrawing(unit);

#ifdef _DEBUG
			wchar_t debugMsg[256];
			wsprintf(debugMsg, L"[绘制] 鱼: 玩家=%d, 贴图=%s, 动画=%d, 行列=(%d,%d)\n",
				unit->isPlayer,
				(unit->img == bmp_Unit_Fish1) ? L"Fish1(玩家)" :
				(unit->img == bmp_Unit_Fish2) ? L"Fish2(原敌人)" : L"其他",
				useAnimation,
				unit->frame_row,
				unit->frame_column);
			OutputDebugString(debugMsg);
#endif

			if (useAnimation) {
				// === 动画绘制：使用动画帧 ===
				int drawWidth = (int)(UNIT_SIZE_X * scale);
				int drawHeight = (int)(UNIT_SIZE_Y * scale);

				// 获取帧坐标
				int frameCol = unit->frame_column;
				int frameRow = unit->frame_row;

				// === 关键修改：如果正在播放攻击动画，强制使用攻击动画帧 ===
				if (unit->attackAnimTimer > 0 && unit->isPlayer) {
					// 攻击动画在第2行（行索引2），使用攻击动画序列
					frameRow = 2;  // 攻击动画所在行

					// 计算攻击动画的帧列
					// 使用攻击动画序列的第几个帧（根据attackAnimTimer计算）
					int attackFrameIndex = (15 - unit->attackAnimTimer) % FRAMES_ATTACK_COUNT;
					int column = FRAMES_ATTACK[attackFrameIndex];
					frameCol = column + unit->direction * (UNIT_LAST_FRAME - 2 * column);
				}

				// 确保帧坐标有效
				if (frameCol < 0) frameCol = 0;
				if (frameCol >= 11) frameCol = 10;

				if (frameRow < 0) frameRow = 0;
				if (frameRow >= 3) frameRow = 2; // 假设最多3行动画

				TransparentBlt(
					hdc_memBuffer,
					unit->x - 0.5 * drawWidth,
					unit->y - 0.5 * drawHeight,
					drawWidth,
					drawHeight,
					hdc_loadBmp,
					UNIT_SIZE_X * frameCol,
					UNIT_SIZE_Y * frameRow,
					UNIT_SIZE_X,
					UNIT_SIZE_Y,
					RGB(255, 255, 255)
				);

				// 在玩家鱼绘制后，添加升级闪烁效果
				if (unit->isPlayer && unit->levelUpFlashTimer > 0) {
					// 计算闪烁强度：0-15帧，快速闪烁2.5次
					int timer = 15 - unit->levelUpFlashTimer;  // 已播放帧数（0-14）
					float flashIntensity = 0.0f;

					// 使用sin函数创建平滑的闪烁：每6帧一个周期（更快闪烁）
					// 这样15帧内会有2.5个完整的闪烁周期
					float phase = (timer % 6) / 6.0f * 3.14159f * 2.0f;
					flashIntensity = (sin(phase) + 1.0f) / 2.0f;  // 0-1之间

					// 闪烁强度足够时才绘制光晕
					if (flashIntensity > 0.3f) {
						// 创建半透明黄色光晕
						int glowSize = drawWidth + (int)(20 * flashIntensity);  // 光晕大小随闪烁变化
						int glowX = unit->x - glowSize / 2;
						int glowY = unit->y - glowSize / 2;

						// 创建半透明黄色画刷
						HBRUSH hGlowBrush = CreateSolidBrush(RGB(
							(int)(255 * flashIntensity),      // R
							(int)(255 * flashIntensity),      // G
							(int)(120 * flashIntensity)       // B（偏黄）
						));

						// 保存原有画刷
						HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc_memBuffer, hGlowBrush);

						// 绘制圆形光晕
						// 使用半透明模式
						SetBkMode(hdc_memBuffer, TRANSPARENT);

						// 使用更亮的颜色和更大的笔刷来创建发光效果
						HBRUSH hInnerGlow = CreateSolidBrush(RGB(
							(int)(255 * flashIntensity),      // R
							(int)(255 * flashIntensity),      // G
							(int)(200 * flashIntensity)       // B（更亮的黄色）
						));
						HBRUSH hOldInner = (HBRUSH)SelectObject(hdc_memBuffer, hInnerGlow);

						// 绘制内层光晕
						int innerSize = glowSize * 3 / 4;
						Ellipse(hdc_memBuffer,
							unit->x - innerSize / 2, unit->y - innerSize / 2,
							unit->x + innerSize / 2, unit->y + innerSize / 2);

						SelectObject(hdc_memBuffer, hOldInner);
						DeleteObject(hInnerGlow);

						// 恢复并删除画刷
						SelectObject(hdc_memBuffer, hOldBrush);
						DeleteObject(hGlowBrush);

						// 调试信息
#ifdef _DEBUG
						static int lastDebugFrame = 0;
						if (unit->attackAnimTimer == 20) {  // 只在第一帧输出
							OutputDebugString(L"[升级闪烁] 绘制黄色光圈\n");
						}
#endif
					}
				}

#ifdef _DEBUG
				wchar_t debugMsg[256];
				wsprintf(debugMsg, L"[动画绘制] 旧系统=%d, 玩家=%d, 行列=(%d,%d)\n",
					!unit->useNewSystem, unit->isPlayer, frameRow, frameCol);
				OutputDebugString(debugMsg);
#endif
			}
			else {
				// === 静态绘制：使用和小鱼相同的翻转逻辑 ===
				BITMAP bm;
				GetObject(unit->img, sizeof(BITMAP), &bm);

				float scale = 1.0f;
				if (unit->useNewSystem && unit->sizeLevel >= 1 && unit->sizeLevel <= 5) {
					scale = sizeScales[unit->sizeLevel];
				}

				int baseWidth = bm.bmWidth;
				int baseHeight = bm.bmHeight;
				int drawWidth = (int)(baseWidth * scale);
				int drawHeight = (int)(baseHeight * scale);

				SelectObject(hdc_loadBmp, unit->img);

				if (unit->direction == UNIT_DIRECT_LEFT) {
					TransparentBlt(
						hdc_memBuffer,
						unit->x - 0.5 * drawWidth,
						unit->y - 0.5 * drawHeight,
						drawWidth,
						drawHeight,
						hdc_loadBmp,
						0, 0,
						baseWidth,
						baseHeight,
						RGB(255, 255, 255)
					);
				}
				else {
					// 向右游时水平翻转
					HDC hdcTemp = CreateCompatibleDC(hdc_loadBmp);
					HBITMAP hbmTemp = CreateCompatibleBitmap(hdc_memBuffer, drawWidth, drawHeight);
					HBITMAP hbmOldTemp = (HBITMAP)SelectObject(hdcTemp, hbmTemp);

					StretchBlt(
						hdcTemp,
						0, 0,
						drawWidth,
						drawHeight,
						hdc_loadBmp,
						baseWidth - 1, 0,
						-baseWidth,
						baseHeight,
						SRCCOPY
					);

					TransparentBlt(
						hdc_memBuffer,
						unit->x - 0.5 * drawWidth,
						unit->y - 0.5 * drawHeight,
						drawWidth,
						drawHeight,
						hdcTemp,
						0, 0,
						drawWidth,
						drawHeight,
						RGB(255, 255, 255)
					);

					SelectObject(hdcTemp, hbmOldTemp);
					DeleteObject(hbmTemp);
					DeleteDC(hdcTemp);
				}

#ifdef _DEBUG
				wchar_t debugMsg[256];
				wsprintf(debugMsg, L"[静态绘制] 鱼类型=%d, 方向=%d\n", unit->fishType, unit->direction);
				OutputDebugString(debugMsg);
#endif
}

			// 等级标签（新系统）
			if (unit->useNewSystem && (unit->isPlayer || unit->sizeLevel >= 3)) {
				// 计算drawWidth和drawHeight用于文本定位
				int drawWidth, drawHeight;
				if (useAnimation) {
					drawWidth = (int)(UNIT_SIZE_X * scale);
					drawHeight = (int)(UNIT_SIZE_Y * scale);
				}
				else {
					BITMAP bm;
					GetObject(unit->img, sizeof(BITMAP), &bm);
					drawWidth = (int)(bm.bmWidth * scale);
					drawHeight = (int)(bm.bmHeight * scale);
				}

				SetBkMode(hdc_memBuffer, TRANSPARENT);
				SetTextColor(hdc_memBuffer, unit->isPlayer ? RGB(255, 255, 0) : RGB(255, 100, 100));

				wchar_t levelText[20];
				wsprintf(levelText, L"Lv.%d", unit->sizeLevel);

				RECT textRect = {
					unit->x - 30,
					unit->y - drawHeight / 2 - 20,
					unit->x + 30,
					unit->y - drawHeight / 2
				};

				DrawText(hdc_memBuffer, levelText, -1, &textRect,
					DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
		}

		// 3. 绘制所有小鱼
		for (int i = 0; i < smallFishes.size(); i++) {
			Unit* fish = smallFishes[i];
			if (!fish) continue;

			int drawWidth, drawHeight;

			if (smallFishBmpWidth < 20 || smallFishBmpHeight < 20) {
				drawWidth = smallFishBmpWidth * 2;
				drawHeight = smallFishBmpHeight * 2;
			}
			else if (smallFishBmpWidth > 100 || smallFishBmpHeight > 100) {
				drawWidth = smallFishBmpWidth / 2;
				drawHeight = smallFishBmpHeight / 2;
			}
			else {
				drawWidth = smallFishBmpWidth;
				drawHeight = smallFishBmpHeight;
			}

			if (drawWidth < 16) drawWidth = 16;
			if (drawHeight < 16) drawHeight = 16;

			SelectObject(hdc_loadBmp, fish->img);

			if (fish->direction == UNIT_DIRECT_LEFT) {
				TransparentBlt(
					hdc_memBuffer,
					fish->x - 0.5 * drawWidth,
					fish->y - 0.5 * drawHeight,
					drawWidth,
					drawHeight,
					hdc_loadBmp,
					0, 0,
					smallFishBmpWidth,
					smallFishBmpHeight,
					RGB(255, 255, 255)
				);
			}
			else {
				HDC hdcTemp = CreateCompatibleDC(hdc_loadBmp);
				HBITMAP hbmTemp = CreateCompatibleBitmap(hdc_memBuffer, drawWidth, drawHeight);
				HBITMAP hbmOldTemp = (HBITMAP)SelectObject(hdcTemp, hbmTemp);

				SelectObject(hdc_loadBmp, fish->img);
				StretchBlt(
					hdcTemp,
					0, 0,
					drawWidth,
					drawHeight,
					hdc_loadBmp,
					smallFishBmpWidth - 1, 0,
					-smallFishBmpWidth,
					smallFishBmpHeight,
					SRCCOPY
				);

				TransparentBlt(
					hdc_memBuffer,
					fish->x - 0.5 * drawWidth,
					fish->y - 0.5 * drawHeight,
					drawWidth,
					drawHeight,
					hdcTemp,
					0, 0,
					drawWidth,
					drawHeight,
					RGB(255, 255, 255)
				);

				SelectObject(hdcTemp, hbmOldTemp);
				DeleteObject(hbmTemp);
				DeleteDC(hdcTemp);
			}
		}

		// 4. 绘制所有鱼雷（仅在第二关）
		if (currentStageLevel == 2) {
			for (int i = 0; i < torpedoes.size(); i++) {
				Unit* torpedo = torpedoes[i];
				if (!torpedo) continue;

				SelectObject(hdc_loadBmp, torpedo->img);

				// 鱼雷大小固定为68x68
				TransparentBlt(
					hdc_memBuffer,
					torpedo->x - 34,  // 居中
					torpedo->y - 34,
					69,
					70,
					hdc_loadBmp,
					0, 0,
					69, 70,
					RGB(255, 255, 255)
				);
			}
		}

		// 4. 检查是否暂停
		bool isPaused = false;
		for (int i = 0; i < buttons.size(); i++) {
			Button* button = buttons[i];
			if (button->buttonID == BUTTON_PAUSE && !button->visible) {
				isPaused = true;
				break;
			}
		}

		if (isPaused) {
			// 暂停界面覆盖层
			HBRUSH darkBrush = CreateSolidBrush(RGB(50, 50, 50));
			RECT screenRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
			FillRect(hdc_memBuffer, &screenRect, darkBrush);
			DeleteObject(darkBrush);

			// 暂停文字
			wchar_t pauseText[] = L"游 戏 暂 停";
			SetBkMode(hdc_memBuffer, TRANSPARENT);
			SetTextColor(hdc_memBuffer, RGB(255, 0, 0));

			HFONT hBigFont = CreateFont(
				48, 0, 0, 0, FW_BOLD,
				FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
				DEFAULT_PITCH, L"微软雅黑"
			);
			HFONT hOldFont = (HFONT)SelectObject(hdc_memBuffer, hBigFont);

			RECT textRect = { 0, WINDOW_HEIGHT / 2 - 50, WINDOW_WIDTH, WINDOW_HEIGHT / 2 + 50 };
			DrawText(hdc_memBuffer, pauseText, -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			SelectObject(hdc_memBuffer, hOldFont);
			DeleteObject(hBigFont);

			// 提示文字
			wchar_t hintText[] = L"点击下方按钮重新开始或返回关卡选择，或按Esc键恢复游戏";
			SetTextColor(hdc_memBuffer, RGB(255, 255, 255));
			HFONT hSmallFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
				DEFAULT_PITCH, L"宋体");
			hOldFont = (HFONT)SelectObject(hdc_memBuffer, hSmallFont);

			RECT hintRect = { 0, WINDOW_HEIGHT / 2 + 60, WINDOW_WIDTH, WINDOW_HEIGHT / 2 + 100 };
			DrawText(hdc_memBuffer, hintText, -1, &hintRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			SelectObject(hdc_memBuffer, hOldFont);
			DeleteObject(hSmallFont);
		}

		// 5. 如果关卡完成，绘制庆祝效果
		if (gameState == GAME_LEVEL_COMPLETE) {
			HBRUSH hGrayBrush = CreateSolidBrush(RGB(50, 50, 50));
			RECT screenRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
			FillRect(hdc_memBuffer, &screenRect, hGrayBrush);
			DeleteObject(hGrayBrush);

			// 绘制"挑战成功"四个字
			const int bitmapWidth = 90;
			const int bitmapHeight = 96;
			const int bitmapSpacing = 10;
			int totalWidth = 4 * bitmapWidth + 3 * bitmapSpacing;
			int startX = (WINDOW_WIDTH - totalWidth) / 2;
			int startY = 120;

			// 绘制"挑"字位图
			if (bmp_tiao != NULL) {
				SelectObject(hdc_loadBmp, bmp_tiao);
				TransparentBlt(
					hdc_memBuffer,
					startX, startY,
					bitmapWidth, bitmapHeight,
					hdc_loadBmp,
					0, 0,
					bitmapWidth, bitmapHeight,
					RGB(255, 255, 255)
				);
			}

			// 绘制"战"字位图
			if (bmp_zhan != NULL) {
				SelectObject(hdc_loadBmp, bmp_zhan);
				TransparentBlt(
					hdc_memBuffer,
					startX + bitmapWidth + bitmapSpacing, startY,
					bitmapWidth, bitmapHeight,
					hdc_loadBmp,
					0, 0,
					bitmapWidth, bitmapHeight,
					RGB(255, 255, 255)
				);
			}

			// 绘制"成"字位图
			if (bmp_cheng != NULL) {
				SelectObject(hdc_loadBmp, bmp_cheng);
				TransparentBlt(
					hdc_memBuffer,
					startX + 2 * (bitmapWidth + bitmapSpacing), startY,
					bitmapWidth, bitmapHeight,
					hdc_loadBmp,
					0, 0,
					bitmapWidth, bitmapHeight,
					RGB(255, 255, 255)
				);
			}

			// 绘制"功"字位图
			if (bmp_gong != NULL) {
				SelectObject(hdc_loadBmp, bmp_gong);
				TransparentBlt(
					hdc_memBuffer,
					startX + 3 * (bitmapWidth + bitmapSpacing), startY,
					bitmapWidth, bitmapHeight,
					hdc_loadBmp,
					0, 0,
					bitmapWidth, bitmapHeight,
					RGB(255, 255, 255)
				);
			}

			// 提示文字
			SetBkMode(hdc_memBuffer, TRANSPARENT);
			SetTextColor(hdc_memBuffer, RGB(255, 255, 255));
			HFONT hHintFont = CreateFont(
				24, 0, 0, 0, FW_NORMAL,
				FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
				DEFAULT_PITCH, L"宋体"
			);
			HFONT hOldFont = (HFONT)SelectObject(hdc_memBuffer, hHintFont);

			wchar_t hintText[] = L"请选择下方按钮继续游戏";
			RECT hintRect = { 0, 250, WINDOW_WIDTH, 320 };
			DrawText(hdc_memBuffer, hintText, -1, &hintRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			SelectObject(hdc_memBuffer, hOldFont);
			DeleteObject(hHintFont);
		}

		// 6. 游戏失败界面绘制
		if (gameState == GAME_LEVEL_FAILED) {
			HBRUSH hGrayBrush = CreateSolidBrush(RGB(50, 50, 50));
			RECT screenRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
			FillRect(hdc_memBuffer, &screenRect, hGrayBrush);
			DeleteObject(hGrayBrush);

			// 绘制"任务失败"四个位图
			int totalWidth = 4 * BITMAP_CHAR_WIDTH + 3 * BITMAP_CHAR_SPACING;
			int startX = (WINDOW_WIDTH - totalWidth) / 2;
			int startY = 120;

			// 绘制四个位图
			DrawFourBitmaps(hdc_memBuffer, hdc_loadBmp, startX, startY,
				bmp_ren, bmp_wu, bmp_shi, bmp_bai);

			// 提示文字
			SetBkMode(hdc_memBuffer, TRANSPARENT);
			SetTextColor(hdc_memBuffer, RGB(255, 255, 255));
			HFONT hHintFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
				DEFAULT_PITCH, L"宋体");
			HFONT hOldFont = (HFONT)SelectObject(hdc_memBuffer, hHintFont);

			wchar_t hintText[] = L"请选择下方按钮重新开始";
			RECT hintRect = { 0, 250, WINDOW_WIDTH, 320 };
			DrawText(hdc_memBuffer, hintText, -1, &hintRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			SelectObject(hdc_memBuffer, hOldFont);
			DeleteObject(hHintFont);
		}

		// 7. 绘制进度条
		if (currentStage->stageID == STAGE_1) {
			// 计算进度（根据配置选择方法）
			int displayProgress = 0;
			if (systemConfig.useNewExperience) {
				// 使用新系统的经验计算
				for (auto unit : units) {
					if (unit && unit->isPlayer) {
						displayProgress = CalculateProgressPercentage(unit);
						break;
					}
				}
			}
			else {
				// 使用旧的progressValue
				displayProgress = progressValue;
			}

			int barWidth = 400;
			int barHeight = 20;
			int barX = (WINDOW_WIDTH - barWidth) / 2;
			int barY = WINDOW_HEIGHT - 120;

			HBRUSH brushBorder = CreateSolidBrush(RGB(0, 0, 0));
			RECT rectBorder = { barX, barY, barX + barWidth, barY + barHeight };
			FrameRect(hdc_memBuffer, &rectBorder, brushBorder);
			DeleteObject(brushBorder);

			int filledWidth = (barWidth * progressValue) / 100;
			RECT filledRect = { barX + 1, barY + 1, barX + filledWidth - 1, barY + barHeight - 1 };
			HBRUSH brushFill = CreateSolidBrush(RGB(0, 200, 0));
			FillRect(hdc_memBuffer, &filledRect, brushFill);
			DeleteObject(brushFill);

			// 显示信息
			wchar_t text[128];
			if (systemConfig.useNewExperience) {
				// 显示等级和经验信息
				Unit* player = nullptr;
				for (auto unit : units) {
					if (unit && unit->isPlayer) {
						player = unit;
						break;
					}
				}
				if (player) {
					wsprintf(text, L"等级:%d  经验:%d/%d  进度:%d%%   小鱼:%d条",
						player->sizeLevel,
						player->experience,
						GetRequiredExpForLevel(player->sizeLevel),
						displayProgress,
						smallFishCount);
				}
				else {
					wsprintf(text, L"进度:%d%%   小鱼:%d条", displayProgress, smallFishCount);
				}
			}
			else {
				// 旧显示方式
				wsprintf(text, L"当前进度:%d%%   小鱼:%d条", displayProgress, smallFishCount);
			}

			SetBkMode(hdc_memBuffer, TRANSPARENT);
			SetTextColor(hdc_memBuffer, RGB(255, 255, 255));
			TextOut(hdc_memBuffer, barX + barWidth / 2 - 100, barY - 20, text, wcslen(text));
		
		}
	}

	// 8. 绘制按钮到缓存（所有场景都需要）
	for (int i = 0; i < buttons.size(); i++) {
		Button* button = buttons[i];
		if (button->visible) {
			if (button->img != NULL) {
				BITMAP bm;
				GetObject(button->img, sizeof(BITMAP), &bm);

				SelectObject(hdc_loadBmp, button->img);

				bool needsTransparency = (
					button->buttonID == BUTTON_STARTGAME ||
					button->buttonID == BUTTON_PAUSE ||
					button->buttonID == BUTTON_RESUME ||
					button->buttonID == BUTTON_HELP ||
					button->buttonID == BUTTON_LEVEL1 ||
					button->buttonID == BUTTON_LEVEL2 ||
					button->buttonID == BUTTON_LEVEL3 ||
					button->buttonID == BUTTON_NEXT_LEVEL ||
					button->buttonID == BUTTON_RETRY_LEVEL ||
					button->buttonID == BUTTON_SELECT_LEVEL
					);

				if (needsTransparency) {
					TransparentBlt(
						hdc_memBuffer,
						button->x, button->y,
						button->width, button->height,
						hdc_loadBmp,
						0, 0,
						bm.bmWidth, bm.bmHeight,
						RGB(255, 255, 255)
					);
				}
				else {
					StretchBlt(
						hdc_memBuffer,
						button->x, button->y,
						button->width, button->height,
						hdc_loadBmp,
						0, 0,
						bm.bmWidth, bm.bmHeight,
						SRCCOPY
					);
				}
			}
			else {
				// 备用按钮
				HBRUSH hBrush;
				if (button->buttonID == BUTTON_LEVEL1) hBrush = CreateSolidBrush(RGB(0, 200, 0));
				else if (button->buttonID == BUTTON_LEVEL2) hBrush = CreateSolidBrush(RGB(200, 150, 0));
				else if (button->buttonID == BUTTON_LEVEL3) hBrush = CreateSolidBrush(RGB(200, 0, 0));
				else hBrush = CreateSolidBrush(RGB(100, 100, 100));

				RECT rect = { button->x, button->y, button->x + button->width, button->y + button->height };
				FillRect(hdc_memBuffer, &rect, hBrush);
				DeleteObject(hBrush);

				// 显示按钮文字
				SetBkMode(hdc_memBuffer, TRANSPARENT);
				SetTextColor(hdc_memBuffer, RGB(255, 255, 255));

				wchar_t buttonText[50];
				if (button->buttonID == BUTTON_LEVEL1) wsprintf(buttonText, L"关卡1");
				else if (button->buttonID == BUTTON_LEVEL2) wsprintf(buttonText, L"关卡2");
				else if (button->buttonID == BUTTON_LEVEL3) wsprintf(buttonText, L"关卡3");
				else if (button->buttonID == BUTTON_STARTGAME) wsprintf(buttonText, L"返回");
				else wsprintf(buttonText, L"按钮%d", button->buttonID);

				DrawText(hdc_memBuffer, buttonText, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
		}
	}

	// 最后将所有的信息绘制到屏幕上
	BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);

	// 回收资源
	DeleteObject(blankBmp);
	DeleteDC(hdc_memBuffer);
	DeleteDC(hdc_loadBmp);

	// 结束绘制
	EndPaint(hWnd, &ps);
}


// 初始化背景函数

HBITMAP InitBackGround(HWND hWnd, HBITMAP bmp_src) {

	PAINTSTRUCT ps;
	HDC hdc_window = BeginPaint(hWnd, &ps);

	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
	HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

	// 初始化缓存
	HBITMAP bmp_output = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(hdc_memBuffer, bmp_output);

	// 加载资源
	SelectObject(hdc_loadBmp, bmp_src);

	// === 修复：获取源位图的真实尺寸 ===
	BITMAP bm;
	GetObject(bmp_src, sizeof(BITMAP), &bm);

#ifdef _DEBUG
	wchar_t debugMsg[256];
	wsprintf(debugMsg, L"[InitBackGround] 源位图尺寸: %dx%d -> 拉伸到: %dx%d\n",
		bm.bmWidth, bm.bmHeight, WINDOW_WIDTH, WINDOW_HEIGHT);
	OutputDebugString(debugMsg);
#endif

	// === 正确拉伸源位图到目标尺寸 ===
	StretchBlt(
		hdc_memBuffer,
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, // 目标尺寸：拉伸到窗口大小（1024x768）
		hdc_loadBmp,
		0, 0, bm.bmWidth, bm.bmHeight,     // 源位图尺寸：使用真实尺寸（800x600）
		SRCCOPY                    // 拷贝整图
	);

	// 回显到窗口（可选，仅初始化时）
	BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);

	DeleteDC(hdc_memBuffer);
	DeleteDC(hdc_loadBmp);
	EndPaint(hWnd, &ps);

	return bmp_output;
}


#pragma endregion

// 清理单位对象
void ClearUnits() {
	for (auto unit : units) {
		if (unit) {
			delete unit;
		}
	}
	units.clear();
}

// 清理按钮对象（不删除图片资源，因为有些是全局的）
void ClearButtons() {
	for (auto button : buttons) {
		if (button) {
			delete button;
		}
	}
	buttons.clear();
}

// 完整的资源清理函数
void CleanupGameResources() {
	// 停止并关闭关卡音乐设备
	if (g_wLevel1MusicDeviceID != 0)
	{
		mciSendCommand(g_wLevel1MusicDeviceID, MCI_STOP, 0, NULL);
		mciSendCommand(g_wLevel1MusicDeviceID, MCI_CLOSE, 0, NULL);
		g_wLevel1MusicDeviceID = 0;
	}
	if (g_wLevel2MusicDeviceID != 0)
	{
		mciSendCommand(g_wLevel2MusicDeviceID, MCI_STOP, 0, NULL);
		mciSendCommand(g_wLevel2MusicDeviceID, MCI_CLOSE, 0, NULL);
		g_wLevel2MusicDeviceID = 0;
	}
	if (g_wLevel3MusicDeviceID != 0)
	{
		mciSendCommand(g_wLevel3MusicDeviceID, MCI_STOP, 0, NULL);
		mciSendCommand(g_wLevel3MusicDeviceID, MCI_CLOSE, 0, NULL);
		g_wLevel3MusicDeviceID = 0;
	}
	// 清理对象
	ClearUnits();
	ClearButtons();

	// 清理小鱼
	ClearSmallFishes();

	if (currentStage) {
		delete currentStage;
		currentStage = NULL;
	}

	// 删除动态创建的按钮位图
	if (bmp_PauseButton && bmp_PauseButton != (HBITMAP)0xFFFFFFFF) {
		DeleteObject(bmp_PauseButton);
	}
	if (bmp_ResumeButton && bmp_ResumeButton != (HBITMAP)0xFFFFFFFF) {
		DeleteObject(bmp_ResumeButton);
	}
	if (bmp_HelpButton && bmp_HelpButton != (HBITMAP)0xFFFFFFFF) {
		DeleteObject(bmp_HelpButton);
	}
	if (bmp_SmallFish && bmp_SmallFish != (HBITMAP)0xFFFFFFFF) {
		DeleteObject(bmp_SmallFish);
	}
	// 如果CreateSimpleColorButton创建了位图，也需要在这里清理
	// 清理关卡选择相关资源
	if (bmp_SelectBackground && bmp_SelectBackground != (HBITMAP)0xFFFFFFFF) {
		DeleteObject(bmp_SelectBackground);
	}
	if (bmp_Level1Button && bmp_Level1Button != (HBITMAP)0xFFFFFFFF) {
		DeleteObject(bmp_Level1Button);
	}
	if (bmp_Level2Button && bmp_Level2Button != (HBITMAP)0xFFFFFFFF) {
		DeleteObject(bmp_Level2Button);
	}
	if (bmp_Level3Button && bmp_Level3Button != (HBITMAP)0xFFFFFFFF) {
		DeleteObject(bmp_Level3Button);
	}
	// 清理通关相关按钮位图
	if (bmp_NextLevel && bmp_NextLevel != (HBITMAP)0xFFFFFFFF) {
		DeleteObject(bmp_NextLevel);
	}
	if (bmp_SelectLevel && bmp_SelectLevel != (HBITMAP)0xFFFFFFFF) {
		DeleteObject(bmp_SelectLevel);
	}
	if (bmp_RetryLevel && bmp_RetryLevel != (HBITMAP)0xFFFFFFFF) {
		DeleteObject(bmp_RetryLevel);
	}
	// 清理通关文字位图
	if (bmp_tiao && bmp_tiao != (HBITMAP)0xFFFFFFFF) {
		DeleteObject(bmp_tiao);
	}
	if (bmp_zhan && bmp_zhan != (HBITMAP)0xFFFFFFFF) {
		DeleteObject(bmp_zhan);
	}
	if (bmp_cheng && bmp_cheng != (HBITMAP)0xFFFFFFFF) {
		DeleteObject(bmp_cheng);
	}
	if (bmp_gong && bmp_gong != (HBITMAP)0xFFFFFFFF) {
		DeleteObject(bmp_gong);
	}
	// 清理游戏失败文字位图
	if (bmp_ren && bmp_ren != (HBITMAP)0xFFFFFFFF) {
		DeleteObject(bmp_ren);
	}
	if (bmp_wu && bmp_wu != (HBITMAP)0xFFFFFFFF) {
		DeleteObject(bmp_wu);
	}
	if (bmp_shi && bmp_shi != (HBITMAP)0xFFFFFFFF) {
		DeleteObject(bmp_shi);
	}
	if (bmp_bai && bmp_bai != (HBITMAP)0xFFFFFFFF) {
		DeleteObject(bmp_bai);
	}

	// 清理 fishBitmaps 但要跳过资源位图
	for (int i = 0; i < fishBitmaps.size(); i++) {
		HBITMAP hBmp = fishBitmaps[i];
		if (hBmp &&
			hBmp != bmp_Unit_Fish1 &&
			hBmp != bmp_Unit_Fish2 &&
			hBmp != bmp_SmallFish &&
			hBmp != (HBITMAP)0xFFFFFFFF) {
			DeleteObject(hBmp);
		}
	}
	fishBitmaps.clear();

	// 清理当前场景的动态背景（如果有）
	if (currentStage && currentStage->bg &&
		currentStage->bg != bmp_Start_Background) {
		DeleteObject(currentStage->bg);
		currentStage->bg = NULL;
	}

	// 停止所有音乐
	///StopStartMenuMusic();
	///StopLevel2Music();

	// 清理鱼雷资源
	ClearTorpedoes();
	if (bmp_Torpedo && bmp_Torpedo != (HBITMAP)0xFFFFFFFF) {
		DeleteObject(bmp_Torpedo);
	}
}

#pragma region 小鱼生成和管理函数

// 生成一个鱼雷（仅在第二关）
void SpawnTorpedo(HWND hWnd)
{
	// 只在第二关生成鱼雷
	if (currentStageLevel != 2 || torpedoes.size() >= 15) return;

	// 从屏幕上方生成，x坐标随机
	int x = 50 + rand() % (WINDOW_WIDTH - 100);
	int y = -70;  // 从屏幕上方开始

	// 创建鱼雷
	Unit* torpedo = new Unit();
	torpedo->img = bmp_Torpedo;
	torpedo->x = x;
	torpedo->y = y;
	torpedo->vx = 0;                      // 水平方向不移动
	torpedo->vy = 12;                      // 垂直方向固定速度向下（8像素/帧）
	torpedo->health = 100;
	torpedo->size = 2;

	// 添加到鱼雷数组
	torpedoes.push_back(torpedo);
	torpedoCount++;
}

// 生成一条小鱼
void SpawnSmallFish(HWND hWnd)
{
	// 如果小鱼数量达到上限，不再生成
	if (smallFishCount >= MAX_SMALL_FISHES) {
		return;
	}

	// 初始化随机种子
	static bool randInitialized = false;
	if (!randInitialized) {
		srand(static_cast<unsigned>(time(NULL)));
		randInitialized = true;
	}

	// === 新增：生成size=2敌人鱼（30%概率） ===
	if (currentStageLevel != 2 && smallEnemyFishCount < MAX_SMALL_ENEMY_FISHES && (rand() % 100) < 30) {
		// 随机选择敌人鱼类型
		bool useType1 = (rand() % 2) == 0;
		int fishType = useType1 ? FISH_TYPE_ENEMY_SMALL1 : FISH_TYPE_ENEMY_SMALL2;

		// 随机决定生成方向
		int spawnEdge = rand() % 4;
		int x, y;
		double vx, vy;

		// 设置初始位置和速度（从小鱼的生成逻辑复制）
		switch (spawnEdge) {
		case 0: // 从上边生成
			x = 50 + rand() % (WINDOW_WIDTH - 100);
			y = -UNIT_SIZE_Y;
			vx = (rand() % 20 - 10) * 0.3;
			vy = UNIT_SPEED * 0.5;
			break;
		case 1: // 从下边生成
			x = 50 + rand() % (WINDOW_WIDTH - 100);
			y = WINDOW_HEIGHT + UNIT_SIZE_Y;
			vx = (rand() % 20 - 10) * 0.3;
			vy = -UNIT_SPEED * 0.5;
			break;
		case 2: // 从左边生成
			x = -UNIT_SIZE_X;
			y = 50 + rand() % (WINDOW_HEIGHT - 100);
			vx = UNIT_SPEED * 0.5;
			vy = (rand() % 20 - 10) * 0.3;
			break;
		case 3: // 从右边生成
		default:
			x = WINDOW_WIDTH + UNIT_SIZE_X;
			y = 50 + rand() % (WINDOW_HEIGHT - 100);
			vx = -UNIT_SPEED * 0.5;
			vy = (rand() % 20 - 10) * 0.3;
			break;
		}

		// 创建敌人鱼
		Unit* enemyFish = CreateFishUnit(fishType, FISH_SIZE_LEVEL_2, x, y, false);
		enemyFish->useNewSystem = true;
		enemyFish->isPlayer = false;
		enemyFish->vx = vx;
		enemyFish->vy = vy;
		enemyFish->direction = (vx > 0) ? UNIT_DIRECT_RIGHT : UNIT_DIRECT_LEFT;
		enemyFish->state = UNIT_STATE_WALK;
		enemyFish->health = 100;

		// 添加到units数组
		units.push_back(enemyFish);
		smallEnemyFishCount++;

#ifdef _DEBUG
		wchar_t debugMsg[256];
		wsprintf(debugMsg, L"生成size=2敌人鱼: type=%d, 位置(%d,%d), 数量=%d\n",
			fishType, x, y, smallEnemyFishCount);
		OutputDebugString(debugMsg);
#endif
	}

	// 随机决定从哪个方向生成
	int spawnEdge = rand() % 4;  // 0=上, 1=下, 2=左, 3=右
	int x, y;
	double vx, vy;

	// 设置初始位置和速度
	switch (spawnEdge) {
	case 0: // 从上边生成
		x = 50 + rand() % (WINDOW_WIDTH - 100);
		y = -UNIT_SIZE_Y;
		vx = (rand() % 20 - 10) * 0.3;  // 随机水平速度
		vy = UNIT_SPEED * 0.4;  // 向下移动
		break;

	case 1: // 从下边生成
		x = 50 + rand() % (WINDOW_WIDTH - 100);
		y = WINDOW_HEIGHT + UNIT_SIZE_Y;
		vx = (rand() % 20 - 10) * 0.3;
		vy = -UNIT_SPEED * 0.4;  // 向上移动
		break;

	case 2: // 从左边生成
		x = -UNIT_SIZE_X;
		y = 50 + rand() % (WINDOW_HEIGHT - 100);
		vx = UNIT_SPEED * 0.4;  // 向右移动
		vy = (rand() % 20 - 10) * 0.3;
		break;

	case 3: // 从右边生成
	default:
		x = WINDOW_WIDTH + UNIT_SIZE_X;
		y = 50 + rand() % (WINDOW_HEIGHT - 100);
		vx = -UNIT_SPEED * 0.4;  // 向左移动
		vy = (rand() % 20 - 10) * 0.3;
		break;
	}

	// 创建小鱼
	Unit* smallFish = new Unit();
	smallFish->side = UNIT_SIDE_FISH2;
	smallFish->type = UNIT_FISH_TYPE1;
	smallFish->img = bmp_SmallFish;

	// 新系统属性
	smallFish->useNewSystem = true;
	smallFish->sizeLevel = FISH_SIZE_LEVEL_1;
	smallFish->fishType = FISH_TYPE_SMALL_FISH;
	smallFish->isPlayer = false;
	smallFish->experience = 0;

	// 设置方向（根据速度方向）
	smallFish->direction = (vx > 0) ? UNIT_DIRECT_RIGHT : UNIT_DIRECT_LEFT;

	smallFish->state = UNIT_STATE_WALK;
	smallFish->frame_row = smallFish->type;
	smallFish->frame_column = UNIT_LAST_FRAME * smallFish->direction;
	smallFish->frame_sequence = FRAMES_WALK;
	smallFish->frame_count = FRAMES_WALK_COUNT;
	smallFish->frame_id = 0;

	smallFish->x = x;
	smallFish->y = y;
	smallFish->vx = vx;
	smallFish->vy = vy;
	smallFish->health = SMALL_FISH_HEALTH;
	smallFish->size = SMALL_FISH_SIZE;  // 设置大小为1（最小）

	// 添加到小鱼数组
	smallFishes.push_back(smallFish);
	smallFishCount++;

}

// 更新所有小鱼的位置
void UpdateSmallFishes()
{
	for (int i = smallFishes.size() - 1; i >= 0; i--) {
		Unit* fish = smallFishes[i];

		// 调用新的行为函数 - 让小鱼随机移动
		UnitBehaviour_SmallFish(fish);

		// 如果游出屏幕，删除
		if (fish->x < -100 || fish->x > WINDOW_WIDTH + 100 ||
			fish->y < -100 || fish->y > WINDOW_HEIGHT + 100) {
			delete fish;
			smallFishes.erase(smallFishes.begin() + i);
			smallFishCount--;
			continue;
		}

		// 注意：动画更新已经在 UnitBehaviour_SmallFish 中处理了
	}
}

// 检测玩家鱼和小鱼的碰撞
void CheckPlayerFishCollision()
{
	if (units.empty()) return;

	Unit* player = units[0];  // 玩家鱼

	for (int i = smallFishes.size() - 1; i >= 0; i--) {
		Unit* smallFish = smallFishes[i];

		// 计算距离
		double dx = player->x - smallFish->x;
		double dy = player->y - smallFish->y;
		double dist = sqrt(dx * dx + dy * dy);

		// 碰撞检测距离（简单使用固定距离）
		int collisionDistance = 50;  // 碰撞距离

		if (dist < collisionDistance) {
			// 玩家鱼吃到小鱼
			if (currentStage != NULL && gameState == GAME_RUNNING) {
				progressValue += 5;  // 增加进度
				if (progressValue > 100) progressValue = 100;

				// 新增：检查是否通关
				if (progressValue >= 100) {
					gameState = GAME_LEVEL_COMPLETE;
					// 注意：这里不能直接调用ShowLevelCompleteButtons，因为不能在碰撞检测中处理UI
					// 在UpdateUnits中处理
				}
			}
			// 删除小鱼
			delete smallFish;
			smallFishes.erase(smallFishes.begin() + i);
			smallFishCount--;

		}
	}
}

// 清理小鱼数组
void ClearSmallFishes()
{
	for (auto fish : smallFishes) {
		if (fish) delete fish;
	}
	smallFishes.clear();
	smallFishCount = 0;

	smallEnemyFishCount = 0;
}

// 清理鱼雷数组
void ClearTorpedoes()
{
	for (auto torpedo : torpedoes) {
		if (torpedo) delete torpedo;
	}
	torpedoes.clear();
	torpedoCount = 0;
}

#if 0
/*
// 显示游戏覆盖层按钮（统一管理函数）
void ShowGameOverlayButtons(HWND hWnd, bool showNextLevel, bool showRetry, bool showSelect)
{
	// 计算按钮位置（统一布局）
	int buttonSpacing = 40;
	int buttonY = WINDOW_HEIGHT * 2 / 3;  // 统一位置

	// 根据显示的按钮计算总宽度和起始位置
	int totalWidth = 0;
	int buttonCount = 0;

	if (showSelect) totalWidth += BUTTON_SELECT_WIDTH;
	if (showRetry) totalWidth += BUTTON_RETRY_WIDTH;
	if (showNextLevel) totalWidth += BUTTON_NEXT_WIDTH;

	if (showSelect && showRetry) totalWidth += buttonSpacing;
	if ((showSelect || showRetry) && showNextLevel) totalWidth += buttonSpacing;

	int startX = (WINDOW_WIDTH - totalWidth) / 2;

	// 设置每个按钮的位置和可见性
	for (int i = 0; i < buttons.size(); i++) {
		Button* button = buttons[i];

		if (button->buttonID == BUTTON_SELECT_LEVEL) {
			if (showSelect) {
				button->x = startX;
				button->y = buttonY - BUTTON_SELECT_HEIGHT / 2;
				button->visible = true;
				if (showRetry || showNextLevel) startX += BUTTON_SELECT_WIDTH + buttonSpacing;
			}
			else {
				button->visible = false;
			}
		}
		else if (button->buttonID == BUTTON_RETRY_LEVEL) {
			if (showRetry) {
				button->x = startX;
				button->y = buttonY - BUTTON_RETRY_HEIGHT / 2;
				button->visible = true;
				if (showNextLevel) startX += BUTTON_RETRY_WIDTH + buttonSpacing;
			}
			else {
				button->visible = false;
			}
		}
		else if (button->buttonID == BUTTON_NEXT_LEVEL) {
			if (showNextLevel) {
				button->x = startX;
				button->y = buttonY - BUTTON_NEXT_HEIGHT / 2;
				button->visible = true;
			}
			else {
				button->visible = false;
			}
		}
		// 隐藏游戏内按钮
		else if (button->buttonID == BUTTON_PAUSE ||
			button->buttonID == BUTTON_RESUME ||
			button->buttonID == BUTTON_HELP) {
			button->visible = false;
		}
	}

	// 强制刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}

// 隐藏游戏覆盖层按钮
void HideGameOverlayButtons()
{
	for (int i = 0; i < buttons.size(); i++) {
		Button* button = buttons[i];
		if (button->buttonID == BUTTON_SELECT_LEVEL ||
			button->buttonID == BUTTON_RETRY_LEVEL ||
			button->buttonID == BUTTON_NEXT_LEVEL) {
			button->visible = false;
		}
	}
}
*/
#endif
// 统一显示覆盖层按钮的函数
// buttonMask: 1=显示下一关, 2=显示重玩, 4=显示选择关卡
void ShowOverlayButtons(HWND hWnd, int buttonMask)
{
	// 计算要显示的按钮数量和总宽度
	int buttonCount = 0;
	int totalWidth = 0;
	int buttonSpacing = 40;

	if (buttonMask & 4) { // 选择关卡按钮
		totalWidth += BUTTON_SELECT_WIDTH;
		buttonCount++;
	}
	if (buttonMask & 2) { // 重玩按钮
		if (buttonCount > 0) totalWidth += buttonSpacing;
		totalWidth += BUTTON_RETRY_WIDTH;
		buttonCount++;
	}
	if (buttonMask & 1) { // 下一关按钮
		if (buttonCount > 0) totalWidth += buttonSpacing;
		totalWidth += BUTTON_NEXT_WIDTH;
		buttonCount++;
	}

	// 计算起始位置（水平居中）
	int startX = (WINDOW_WIDTH - totalWidth) / 2;
	int buttonY = WINDOW_HEIGHT * 2 / 3; // 统一位置

	// 设置每个按钮的位置和可见性
	for (int i = 0; i < buttons.size(); i++) {
		Button* button = buttons[i];

		if (button->buttonID == BUTTON_SELECT_LEVEL) {
			button->visible = ((buttonMask & 4) != 0);
			if (button->visible) {
				button->x = startX;
				button->y = buttonY - BUTTON_SELECT_HEIGHT / 2;
				startX += BUTTON_SELECT_WIDTH + buttonSpacing;
			}
		}
		else if (button->buttonID == BUTTON_RETRY_LEVEL) {
			button->visible = ((buttonMask & 2) != 0);
			if (button->visible) {
				button->x = startX;
				button->y = buttonY - BUTTON_RETRY_HEIGHT / 2;
				startX += BUTTON_RETRY_WIDTH + buttonSpacing;
			}
		}
		else if (button->buttonID == BUTTON_NEXT_LEVEL) {
			button->visible = ((buttonMask & 1) != 0);
			if (button->visible) {
				button->x = startX;
				button->y = buttonY - BUTTON_NEXT_HEIGHT / 2;
				startX += BUTTON_NEXT_WIDTH + buttonSpacing;
			}
		}
		// 隐藏游戏内按钮
		else if (button->buttonID == BUTTON_PAUSE ||
			button->buttonID == BUTTON_RESUME ||
			button->buttonID == BUTTON_HELP) {
			button->visible = false;
		}
	}
}

void ShowLevelCompleteButtons(HWND hWnd)
{
	// === 新增：通关解锁下一关 ===
	if (currentStageLevel >= 1 && currentStageLevel < 3) {
		levelUnlocked[currentStageLevel + 1] = true;

#ifdef _DEBUG
		wchar_t debugMsg[100];
		wsprintf(debugMsg, L"[解锁] 通关第%d关，解锁第%d关\n",
			currentStageLevel, currentStageLevel + 1);
		OutputDebugString(debugMsg);
#endif
	}

	// 停止游戏计时器
	KillTimer(hWnd, TIMER_GAMETIMER);
	KillTimer(hWnd, TIMER_SPAWN_FISH);

	// 使用统一函数显示按钮（显示所有三个按钮）
	ShowOverlayButtons(hWnd, 1 | 2 | 4); // 1(下一关) + 2(重玩) + 4(选择关卡)

	// 强制刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}

// 确保所有单位都启用新系统
void EnsureNewSystemForAllUnits()
{
	// 检查units中的单位
	for (auto unit : units) {
		if (unit && !unit->useNewSystem) {
			unit->useNewSystem = true;
			// 设置默认值
			if (!unit->isPlayer) unit->isPlayer = (unit->side == UNIT_SIDE_FISH1);
			if (unit->sizeLevel == 0) {
				if (unit->isPlayer) unit->sizeLevel = FISH_SIZE_LEVEL_2;
				else unit->sizeLevel = FISH_SIZE_LEVEL_2;
			}
			if (unit->fishType == 0) {
				if (unit->isPlayer) unit->fishType = FISH_TYPE_PLAYER;
				else if (unit->size == SMALL_FISH_SIZE) unit->fishType = FISH_TYPE_SMALL_FISH;
				else unit->fishType = FISH_TYPE_ENEMY_SMALL1;
			}
		}
	}

	// 检查小鱼
	for (auto fish : smallFishes) {
		if (fish && !fish->useNewSystem) {
			fish->useNewSystem = true;
			fish->sizeLevel = FISH_SIZE_LEVEL_1;
			fish->isPlayer = false;
			fish->fishType = FISH_TYPE_SMALL_FISH;
		}
	}
}

// 验证新系统是否正常工作
bool ValidateNewSystem()
{
	bool allGood = true;
	wchar_t debugMsg[256];

	// 检查1：所有单位都有新系统属性
	for (auto unit : units) {
		if (unit && !unit->useNewSystem) {
			wsprintf(debugMsg, L"[验证] 警告：单位未启用新系统 (side=%d, type=%d)\n",
				unit->side, unit->type);
			OutputDebugString(debugMsg);
			allGood = false;
		}
	}

	// 检查2：找到玩家鱼
	Unit* player = nullptr;
	for (auto unit : units) {
		if (unit && unit->isPlayer) {
			player = unit;
			break;
		}
	}

	if (player) {
		wsprintf(debugMsg, L"[验证] 玩家鱼：等级=%d, 经验=%d\n",
			player->sizeLevel, player->experience);
		OutputDebugString(debugMsg);
	}
	else {
		OutputDebugString(L"[验证] 警告：未找到玩家鱼\n");
		allGood = false;
	}

	// 检查3：小鱼数量
	wsprintf(debugMsg, L"[验证] 小鱼数量：%d\n", smallFishCount);
	OutputDebugString(debugMsg);

	return allGood;
}

// 检测玩家鱼和敌人鱼的碰撞
void CheckPlayerEnemyCollision()
{
	if (units.size() < 2) return;

	Unit* player = units[0];  // 玩家鱼
	Unit* enemy = units[1];   // 敌人鱼

	// 计算距离
	double dx = player->x - enemy->x;
	double dy = player->y - enemy->y;
	double dist = sqrt(dx * dx + dy * dy);

	// 碰撞检测距离
	int collisionDistance = 80;

	if (dist < collisionDistance && gameState == GAME_RUNNING) {
		// 触发游戏失败
		gameState = GAME_LEVEL_FAILED;

#ifdef _DEBUG
		OutputDebugString(L"玩家鱼与敌人鱼碰撞，游戏失败！\n");
#endif
	}
}

// 绘制四个水平排列的位图
void DrawFourBitmaps(HDC hdcDest, HDC hdcSrc, int startX, int startY,
	HBITMAP bmp1, HBITMAP bmp2, HBITMAP bmp3, HBITMAP bmp4)
{
	const int bitmapWidth = BITMAP_CHAR_WIDTH;
	const int bitmapHeight = BITMAP_CHAR_HEIGHT;
	const int bitmapSpacing = BITMAP_CHAR_SPACING;

	// 绘制第一个位图
	if (bmp1 != NULL) {
		SelectObject(hdcSrc, bmp1);
		TransparentBlt(hdcDest, startX, startY, bitmapWidth, bitmapHeight,
			hdcSrc, 0, 0, bitmapWidth, bitmapHeight, RGB(255, 255, 255));
	}

	// 绘制第二个位图
	if (bmp2 != NULL) {
		SelectObject(hdcSrc, bmp2);
		TransparentBlt(hdcDest, startX + bitmapWidth + bitmapSpacing, startY,
			bitmapWidth, bitmapHeight, hdcSrc, 0, 0,
			bitmapWidth, bitmapHeight, RGB(255, 255, 255));
	}

	// 绘制第三个位图
	if (bmp3 != NULL) {
		SelectObject(hdcSrc, bmp3);
		TransparentBlt(hdcDest, startX + 2 * (bitmapWidth + bitmapSpacing), startY,
			bitmapWidth, bitmapHeight, hdcSrc, 0, 0,
			bitmapWidth, bitmapHeight, RGB(255, 255, 255));
	}

	// 绘制第四个位图
	if (bmp4 != NULL) {
		SelectObject(hdcSrc, bmp4);
		TransparentBlt(hdcDest, startX + 3 * (bitmapWidth + bitmapSpacing), startY,
			bitmapWidth, bitmapHeight, hdcSrc, 0, 0,
			bitmapWidth, bitmapHeight, RGB(255, 255, 255));
	}
}

// 显示暂停界面按钮
void ShowPauseButtons(HWND hWnd)
{
	// 停止游戏计时器（包括小鱼生成计时器）
	KillTimer(hWnd, TIMER_GAMETIMER);
	KillTimer(hWnd, TIMER_SPAWN_FISH);

	// === 根据当前关卡暂停音乐 ===
	if (currentStageLevel == 1)
	{
		PauseLevel1Music();
	}
	else if (currentStageLevel == 2)
	{
		PauseLevel2Music();
	}
	else if (currentStageLevel == 3) 
	{
		PauseLevel3Music();
	}
	// 使用统一函数显示按钮（只显示重玩和选择关卡）
	ShowOverlayButtons(hWnd, 2 | 4); // 2(重玩) + 4(选择关卡)

	// 强制刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}

// 显示游戏失败界面
void ShowLevelFailedButtons(HWND hWnd)
{
	// 停止游戏计时器
	KillTimer(hWnd, TIMER_GAMETIMER);
	KillTimer(hWnd, TIMER_SPAWN_FISH);

	// 使用统一函数显示按钮（只显示重玩和选择关卡）
	ShowOverlayButtons(hWnd, 2 | 4); // 2(重玩) + 4(选择关卡)

	// 强制刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}

// 隐藏游戏失败界面（按钮）
void HideLevelFailedButtons()
{
	for (int i = 0; i < buttons.size(); i++) {
		Button* button = buttons[i];
		if (button->buttonID == BUTTON_SELECT_LEVEL ||
			button->buttonID == BUTTON_RETRY_LEVEL) {
			button->visible = false;
		}
	}
}

// 隐藏关卡完成按钮
void HideLevelCompleteButtons()
{
	for (int i = 0; i < buttons.size(); i++) {
		Button* button = buttons[i];
		if (button->buttonID == BUTTON_NEXT_LEVEL ||
			button->buttonID == BUTTON_RETRY_LEVEL ||
			button->buttonID == BUTTON_SELECT_LEVEL) {
			button->visible = false;
		}
	}
}

void LoadAllFishBitmaps(HINSTANCE hInstance)
{
	// 确保数组大小足够
	fishBitmaps.resize(15, NULL);

	// 0: 玩家鱼 - 使用动画序列 bmp_Unit_Fish1 (原Fish2)
	fishBitmaps[FISH_TYPE_PLAYER] = bmp_Unit_Fish1;

	// 1: 小鱼 - 使用静态位图 bmp_SmallFish
	if (bmp_SmallFish) {
		fishBitmaps[FISH_TYPE_SMALL_FISH] = bmp_SmallFish;
	}
	else {
		// 备用位图
		fishBitmaps[FISH_TYPE_SMALL_FISH] = CreateSimpleColorButton(
			RGB(100, 200, 100), 32, 32, L"小鱼");
	}

	// 10: 小型敌人鱼类型1 - 使用静态位图 enemy_small1.bmp
	HBITMAP bmpEnemySmall1 = (HBITMAP)LoadImageW(
		NULL, L"images\\enemy_small1.bmp", IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);
	fishBitmaps[FISH_TYPE_ENEMY_SMALL1] = bmpEnemySmall1 ? bmpEnemySmall1 :
		CreateSimpleColorButton(RGB(255, 100, 0), 50, 50, L"敌1");

	// 11: 小型敌人鱼类型2 - 使用静态位图
	HBITMAP bmpEnemySmall2 = (HBITMAP)LoadImageW(
		NULL, L"images\\enemy_small2.bmp", IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);
	fishBitmaps[FISH_TYPE_ENEMY_SMALL2] = bmpEnemySmall2 ? bmpEnemySmall2 :
		CreateSimpleColorButton(RGB(255, 150, 0), 50, 50, L"敌2");

	// 12-14: 中型、大型、巨型敌人鱼 - 暂时使用备用静态位图
	// 注意：根据需求，这些鱼未来会使用静态位图
	fishBitmaps[FISH_TYPE_ENEMY_MEDIUM] = CreateSimpleColorButton(
		RGB(200, 100, 50), 70, 70, L"中敌");
	fishBitmaps[FISH_TYPE_ENEMY_LARGE] = CreateSimpleColorButton(
		RGB(150, 50, 150), 90, 90, L"大敌");
	fishBitmaps[FISH_TYPE_ENEMY_GIANT] = CreateSimpleColorButton(
		RGB(100, 0, 0), 110, 110, L"巨敌");
}

void InitializeStageConfigs()
{
	// 初始化所有关卡配置
	for (int i = 0; i < MAX_STAGES; i++) {
		stageConfigs[i].stageID = i;
		stageConfigs[i].playerStartSize = FISH_SIZE_LEVEL_2;
		stageConfigs[i].targetSize = FISH_SIZE_LEVEL_3;
		stageConfigs[i].maxSmallFishes = 15;

		for (int j = 0; j < 5; j++) {
			stageConfigs[i].fishCount[j] = 0;
			stageConfigs[i].fishTypeCount[j] = 0;
		}
	}

	// ====== 第一关配置 ======
	StageConfig& level1 = stageConfigs[1];
	level1.stageID = 1;
	level1.playerStartSize = FISH_SIZE_LEVEL_2;
	level1.targetSize = FISH_SIZE_LEVEL_5;
	level1.maxSmallFishes = 15;

	level1.fishCount[0] = 12;  // size=1: 12条小鱼
	level1.fishCount[1] = 6;   // size=2: 6条
	level1.fishCount[2] = 2;   // size=3: 2条
	level1.fishCount[3] = 0;   // size=4: 0条
	level1.fishCount[4] = 0;   // size=5: 0条

	level1.fishTypes[0][0] = FISH_TYPE_SMALL_FISH;
	level1.fishTypeCount[0] = 1;

	level1.fishTypes[1][0] = FISH_TYPE_ENEMY_SMALL1;
	level1.fishTypes[1][1] = FISH_TYPE_ENEMY_SMALL2;
	level1.fishTypeCount[1] = 2;

	level1.fishTypes[2][0] = FISH_TYPE_ENEMY_MEDIUM;
	level1.fishTypeCount[2] = 1;

	// ====== 第二关配置 ======
	StageConfig& level2 = stageConfigs[2];
	level2.stageID = 2;
	level2.playerStartSize = FISH_SIZE_LEVEL_2;
	level2.targetSize = FISH_SIZE_LEVEL_5;
	level2.maxSmallFishes = 25;

	level2.fishCount[0] = 25;  // size=1: 12条小鱼
	level2.fishCount[1] = 0;   // size=2: 6条
	level2.fishCount[2] = 0;   // size=3: 2条
	level2.fishCount[3] = 0;   // size=4: 0条
	level2.fishCount[4] = 0;   // size=5: 0条

	level2.fishTypes[0][0] = FISH_TYPE_SMALL_FISH;
	level2.fishTypeCount[0] = 1;

	level2.fishTypes[1][0] = FISH_TYPE_ENEMY_SMALL1;
	level2.fishTypes[1][1] = FISH_TYPE_ENEMY_SMALL2;
	level2.fishTypeCount[1] = 0;

	level2.fishTypes[2][0] = FISH_TYPE_ENEMY_MEDIUM;
	level2.fishTypeCount[2] = 0;

	// ====== 第三关配置 ======
	StageConfig& level3 = stageConfigs[3];
	level3.stageID = 3;
	level3.playerStartSize = FISH_SIZE_LEVEL_2;
	level3.targetSize = FISH_SIZE_LEVEL_5;
	level3.maxSmallFishes = 15;

	level3.fishCount[0] = 12;  // size=1: 12条小鱼
	level3.fishCount[1] = 6;   // size=2: 6条
	level3.fishCount[2] = 2;   // size=3: 2条
	level3.fishCount[3] = 0;   // size=4: 0条
	level3.fishCount[4] = 0;   // size=5: 0条

	level3.fishTypes[0][0] = FISH_TYPE_SMALL_FISH;
	level3.fishTypeCount[0] = 1;

	level3.fishTypes[1][0] = FISH_TYPE_ENEMY_SMALL1;
	level3.fishTypes[1][1] = FISH_TYPE_ENEMY_SMALL2;
	level3.fishTypeCount[1] = 2;

	level3.fishTypes[2][0] = FISH_TYPE_ENEMY_MEDIUM;
	level3.fishTypeCount[2] = 1;
}

void SetupStageConfig(int stageID)
{
	currentStageLevel = stageID;
	StageConfig& config = stageConfigs[stageID];

	// 重置进度
	progressValue = 0;

	// 清空现有鱼
	ClearUnits();
	ClearSmallFishes();

	// 重置小鱼数量限制
	MAX_SMALL_FISHES = config.maxSmallFishes;
}


void ValidateUnitAnimation(Unit* unit)
{
	if (!unit) return;

	// 确保动画帧行有效
	if (unit->frame_row < 0 || unit->frame_row >= 3) { // 假设最多3行动画
		unit->frame_row = 0;
	}

	// 确保动画列有效
	if (unit->frame_column < 0 || unit->frame_column >= UNIT_LAST_FRAME * 2) {
		unit->frame_column = unit->direction * UNIT_LAST_FRAME;
	}

	// 确保有动画序列
	if (!unit->frame_sequence) {
		unit->frame_sequence = FRAMES_WALK;
		unit->frame_count = FRAMES_WALK_COUNT;
	}
}

Unit* CreateFishUnit(int fishType, int sizeLevel, int x, int y, bool isPlayer)
{
	Unit* fish = new Unit();

	// 设置新系统属性
	fish->fishType = fishType;
	fish->sizeLevel = sizeLevel;
	fish->isPlayer = isPlayer;
	fish->experience = 0;
	fish->useNewSystem = true;  // 明确设置为true

	// 设置原有系统属性（为了兼容）
	if (isPlayer) {
		fish->side = UNIT_SIDE_FISH1;
		fish->type = UNIT_FISH_TYPE1;
		fish->direction = UNIT_DIRECT_LEFT;  // 玩家鱼默认向左
	}
	else {
		fish->side = UNIT_SIDE_FISH2;
		fish->type = UNIT_FISH_TYPE2;
		fish->direction = UNIT_DIRECT_RIGHT;  // 敌人鱼默认向右
	}

	// 设置贴图
	if (fishType >= 0 && fishType < (int)fishBitmaps.size() && fishBitmaps[fishType]) {
		fish->img = fishBitmaps[fishType];
	}
	else {
		fish->img = fishBitmaps[FISH_TYPE_SMALL_FISH];  // 默认使用小鱼位图
	}

	// 位置和速度
	fish->x = x;
	fish->y = y;
	fish->vx = 0;
	fish->vy = 0;
	fish->health = 100;

	// 注意：原敌人鱼不会通过这个函数创建
	if (isPlayer) {
		// 玩家鱼：设置完整的动画属性
		fish->state = UNIT_STATE_WALK;
		fish->frame_row = 0;  // 玩家鱼在Fish2位图的第0行
		fish->frame_column = fish->direction * UNIT_LAST_FRAME;
		fish->frame_sequence = FRAMES_WALK;
		fish->frame_count = FRAMES_WALK_COUNT;
		fish->frame_id = rand() % FRAMES_WALK_COUNT;

		// 确保贴图正确
		fish->img = bmp_Unit_Fish1;  // 玩家鱼使用Fish2贴图
	}
	else {
		// 新系统敌人鱼：使用静态绘制
		fish->state = UNIT_STATE_HOLD;
		fish->frame_row = 0;
		fish->frame_column = 0;
		fish->frame_sequence = FRAMES_HOLD;
		fish->frame_count = FRAMES_HOLD_COUNT;
		fish->frame_id = 0;
	}

	return fish;
}

void SpawnStageFishes(HWND hWnd)
{
	StageConfig& config = stageConfigs[currentStageLevel];

	// 生成玩家鱼
	Unit* playerFish = CreateFishUnit(
		FISH_TYPE_PLAYER,
		config.playerStartSize,
		WINDOW_WIDTH / 4,
		WINDOW_HEIGHT / 2,
		true
	);
	units.push_back(playerFish);

	// 生成其他鱼
	for (int size = 1; size <= 5; size++) {
		int fishCount = config.fishCount[size - 1];
		int typeCount = config.fishTypeCount[size - 1];

		if (fishCount <= 0 || typeCount <= 0) continue;

		for (int i = 0; i < fishCount; i++) {
			// 随机选择类型
			int typeIndex = rand() % typeCount;
			int fishType = config.fishTypes[size - 1][typeIndex];

			// 随机位置
			int x, y;
			int spawnEdge = rand() % 4;

			switch (spawnEdge) {
			case 0: x = 100 + rand() % (WINDOW_WIDTH - 200); y = -50; break;
			case 1: x = 100 + rand() % (WINDOW_WIDTH - 200); y = WINDOW_HEIGHT + 50; break;
			case 2: x = -50; y = 100 + rand() % (WINDOW_HEIGHT - 200); break;
			default: x = WINDOW_WIDTH + 50; y = 100 + rand() % (WINDOW_HEIGHT - 200); break;
			}

			// 创建鱼
			Unit* fish = CreateFishUnit(fishType, size, x, y, false);

			// 随机速度
			double angle = (rand() % 360) * 3.14159 / 180.0;
			float speed = UNIT_SPEED * (0.3f + (size - 1) * 0.1f);

			fish->vx = cos(angle) * speed;
			fish->vy = sin(angle) * speed;

			// 添加到相应容器
			if (size == FISH_SIZE_LEVEL_1) {
				smallFishes.push_back(fish);
				smallFishCount++;
			}
			else {
				units.push_back(fish);
			}
		}
	}
}

int GetRequiredExpForLevel(int level)
{
	// 确保level在有效范围内
	if (level < 1 || level > 5) {
		return 0;
	}

	if (level >= 2 && level <= 4) {
		return expRequired[level];
	}
	return 0;
}

int GetExpValueForSize(int size)
{
	if (size >= 1 && size <= 5) {
		return expValues[size];
	}
	return 0;
}

int GetCollisionRadius(int sizeLevel)
{
	// 确保 sizeLevel 在有效范围内
	if (sizeLevel < 1) sizeLevel = 1;
	if (sizeLevel > 5) sizeLevel = 5;
	return COLLISION_RADIUS_BASE + (sizeLevel - 1) * COLLISION_RADIUS_PER_LEVEL;
}

int CalculateProgressPercentage(Unit* player)
{
	if (player->sizeLevel >= MAX_PLAYER_LEVEL) {
		return 100;
	}

	int requiredExp = GetRequiredExpForLevel(player->sizeLevel);
	if (requiredExp <= 0) return 0;

	int percentage = (player->experience * 100) / requiredExp;
	return min(100, percentage);
}

void PlayLevelUpEffect(Unit* player)
{
#ifdef _DEBUG
	OutputDebugString(L"等级提升！\n");
#endif
}

void PlayEatEffect(Unit* food)
{
	// 找到玩家鱼
	Unit* player = nullptr;
	for (auto unit : units) {
		if (unit && unit->isPlayer) {
			player = unit;
			break;
		}
	}

	if (player) {
		// 修复：如果已经有攻击动画，可以重新开始或叠加
		player->attackAnimTimer = 15;  // 每次吃鱼都触发15帧攻击动画

		// 调试信息
#ifdef _DEBUG
		wchar_t debugMsg[100];
		wsprintf(debugMsg, L"触发攻击动画，计时器=%d帧\n", player->attackAnimTimer);
		OutputDebugString(debugMsg);
#endif
	}

	// 保持原有调试输出
#ifdef _DEBUG
	if (food) {
		wchar_t debugMsg[100];
		wsprintf(debugMsg, L"吃掉等级%d的鱼\n", food->sizeLevel);
		OutputDebugString(debugMsg);
	}
#endif
}

// 检查两个鱼是否碰撞
bool CheckFishCollision(Unit* fish1, Unit* fish2)
{
	if (!fish1 || !fish2) {
		return false;
	}

	double dx = fish1->x - fish2->x;
	double dy = fish1->y - fish2->y;
	double distance = sqrt(dx * dx + dy * dy);

	int radius1 = GetCollisionRadius(fish1->sizeLevel);
	int radius2 = GetCollisionRadius(fish2->sizeLevel);

	return distance < (radius1 + radius2) * 0.8f;
}

// 检查所有碰撞
void CheckAllCollisions()
{
#ifdef _DEBUG
	wchar_t debugMsg[256];
	wsprintf(debugMsg, L"[DEBUG] === CheckAllCollisions开始 ===\n");
	OutputDebugString(debugMsg);
	wsprintf(debugMsg, L"[DEBUG] units.size() = %d, gameState = %d\n", units.size(), gameState);
	OutputDebugString(debugMsg);
#endif

	if (units.empty() || gameState != GAME_RUNNING) {
#ifdef _DEBUG
		OutputDebugString(L"[DEBUG] units为空或游戏状态不是RUNNING，返回\n");
#endif
		return;
	}

	// 找到玩家鱼
	Unit* player = NULL;
	for (size_t i = 0; i < units.size(); i++) {
		if (i < units.size() && units[i] && units[i]->isPlayer) {
			player = units[i];
			break;
		}
	}

	if (!player) {
#ifdef _DEBUG
		OutputDebugString(L"[DEBUG] 找不到玩家鱼，返回\n");
#endif
		return;
	}

	// 1. 检测玩家鱼和小鱼的碰撞
#ifdef _DEBUG
	wsprintf(debugMsg, L"[DEBUG] 检测小鱼碰撞，smallFishes.size() = %d\n", smallFishes.size());
	OutputDebugString(debugMsg);
#endif

	for (int i = (int)smallFishes.size() - 1; i >= 0; i--) {
		if (i < 0 || i >= (int)smallFishes.size()) {
#ifdef _DEBUG
			wsprintf(debugMsg, L"[DEBUG] 小鱼索引 %d 超出范围 [0, %d)\n", i, smallFishes.size());
			OutputDebugString(debugMsg);
#endif
			continue;
		}

		Unit* smallFish = smallFishes[i];
		if (!smallFish) continue;

		if (CheckFishCollision(player, smallFish)) {
			HandleFishCollision(player, smallFish, i, true);
			if (gameState == GAME_LEVEL_FAILED) return;
		}
	}

	// 2. 检测玩家鱼和其他鱼的碰撞
#ifdef _DEBUG
	wsprintf(debugMsg, L"[DEBUG] 检测其他鱼碰撞，units.size() = %d\n", units.size());
	OutputDebugString(debugMsg);
#endif

	for (int i = (int)units.size() - 1; i >= 0; i--) {
		if (i < 0 || i >= (int)units.size()) {
#ifdef _DEBUG
			wsprintf(debugMsg, L"[DEBUG] 单位索引 %d 超出范围 [0, %d)\n", i, units.size());
			OutputDebugString(debugMsg);
#endif
			continue;
		}

		if (!units[i]) continue;
		if (units[i]->isPlayer) continue;

		Unit* otherFish = units[i];

		if (CheckFishCollision(player, otherFish)) {
			HandleFishCollision(player, otherFish, i, false);
			if (gameState == GAME_LEVEL_FAILED) return;
		}
	}

#ifdef _DEBUG
	OutputDebugString(L"[DEBUG] === CheckAllCollisions结束 ===\n");
#endif
}

// 处理碰撞
void HandleFishCollision(Unit* player, Unit* otherFish, int otherIndex, bool isSmallFish)
{
	// 比较大小
	if (player->sizeLevel > otherFish->sizeLevel) {
		// 玩家鱼更大，吃掉对方
		EatFish(player, otherFish, otherIndex, isSmallFish);
	}
	else if (player->sizeLevel < otherFish->sizeLevel) {
		// 对方更大，游戏失败
		gameState = GAME_LEVEL_FAILED;
	}
	// 大小相等则无事发生
}

// 玩家鱼升级检查函数
void CheckLevelUp(Unit* player)
{
	if (!player || !player->useNewSystem) {
		return;  // 不是新系统，直接返回
	}

	if (player->sizeLevel >= MAX_PLAYER_LEVEL) {
		// 已经达到最大等级
		return;
	}

	// 获取升级所需经验
	int requiredExp = GetRequiredExpForLevel(player->sizeLevel);

	// 如果经验足够升级，就升级
	while (player->experience >= requiredExp && player->sizeLevel < MAX_PLAYER_LEVEL) {
		// 升级
		player->sizeLevel++;

		// === 正确设置升级闪烁计时器 ===
		player->levelUpFlashTimer = 15;  // 设置15帧升级闪烁动画（约0.5秒）

		player->experience -= requiredExp;
		requiredExp = GetRequiredExpForLevel(player->sizeLevel);
		player->health = 100;

		// 播放升级效果
		PlayLevelUpEffect(player);

		// 调试输出
#ifdef _DEBUG
		wchar_t debugMsg[100];
		wsprintf(debugMsg, L"[升级] 玩家鱼升级到等级 %d！\n", player->sizeLevel);
		OutputDebugString(debugMsg);
#endif

		// 更新进度条（可选）
		progressValue = CalculateProgressPercentage(player);
	}
}

// 吃鱼
void EatFish(Unit* player, Unit* food, int foodIndex, bool isSmallFish)
{
	// 获得经验值
	int expGained = GetExpValueForSize(food->sizeLevel);
	player->experience += expGained;

	// 更新进度
	progressValue = CalculateProgressPercentage(player);

	// 检查升级
	CheckLevelUp(player);

	// 播放特效
	PlayEatEffect(food);

	// 删除被吃的鱼
	if (isSmallFish) {
		delete food;
		smallFishes.erase(smallFishes.begin() + foodIndex);
		smallFishCount--;
	}
	else {
		// 检查是否是size=2敌人鱼
		if (food->sizeLevel == FISH_SIZE_LEVEL_2 &&
			(food->fishType == FISH_TYPE_ENEMY_SMALL1 ||
				food->fishType == FISH_TYPE_ENEMY_SMALL2)) {
			smallEnemyFishCount--;
		}

		delete food;
		units.erase(units.begin() + foodIndex);
	}
}

// 检查升级
void UpdateUnits(HWND hWnd)
{
	// === 新增：更新所有单位的攻击动画计时器 ===
	for (auto unit : units) {
		if (unit && unit->attackAnimTimer > 0) {
			unit->attackAnimTimer--;
		}
	}
	for (auto unit : units) {
		if (unit && unit->levelUpFlashTimer > 0) {
			unit->levelUpFlashTimer--;
		}
	}

	// === 新增：更新鱼雷位置并检测碰撞（仅在第二关） ===
	if (currentStageLevel == 2) {
		// 更新鱼雷位置
		for (int i = torpedoes.size() - 1; i >= 0; i--) {
			Unit* torpedo = torpedoes[i];

			// 向下移动
			torpedo->y += torpedo->vy;

			// 如果到达屏幕底部，删除
			if (torpedo->y > WINDOW_HEIGHT + 100) {
				delete torpedo;
				torpedoes.erase(torpedoes.begin() + i);
				torpedoCount--;
				continue;
			}

			// 检测与玩家鱼的碰撞
			if (gameState == GAME_RUNNING) {
				for (auto unit : units) {
					if (unit && unit->isPlayer) {
						double dx = unit->x - torpedo->x;
						double dy = unit->y - torpedo->y;
						double dist = sqrt(dx * dx + dy * dy);

						// 碰撞半径：鱼雷68px，玩家鱼根据等级变化
						int collisionDistance = GetCollisionRadius(unit->sizeLevel) + 35;

						if (dist < collisionDistance) {
							// 玩家鱼碰到鱼雷，游戏失败
							gameState = GAME_LEVEL_FAILED;

							// 删除鱼雷
							delete torpedo;
							torpedoes.erase(torpedoes.begin() + i);
							torpedoCount--;
							break;
						}
					}
				}
			}
		}
	}

	// === 第1部分：游戏状态检查 ===
	static bool levelFailedShown = false;
	static bool levelCompleteShown = false;

	if (gameState == GAME_LEVEL_FAILED && !levelFailedShown) {
		ShowLevelFailedButtons(hWnd);
		levelFailedShown = true;
		return;
	}
	else if (gameState != GAME_LEVEL_FAILED) {
		levelFailedShown = false;
	}

	if (gameState == GAME_LEVEL_COMPLETE && !levelCompleteShown) {
		ShowLevelCompleteButtons(hWnd);
		levelCompleteShown = true;
		return;
	}
	else if (gameState != GAME_LEVEL_COMPLETE) {
		levelCompleteShown = false;
	}

	if (gameState != GAME_RUNNING) {
		return;
	}

	// === 第2部分：确保所有单位使用新系统 ===
	EnsureNewSystemForAllUnits();

	// === 第3部分：更新单位行为 ===
	for (int i = 0; i < units.size(); i++) {
		Unit* unit = units[i];
		if (!unit) continue;

		// 验证动画信息
		ValidateUnitAnimation(unit);

		// 根据配置选择行为系统
		if (unit->useNewSystem) {
			if (unit->isPlayer) {
				UnitBehaviour_1(unit);
			}
			else {
				UnitBehaviour_2(unit);
			}
		}
		else {
			// 兼容旧系统
			switch (unit->type) {
			case UNIT_FISH_TYPE1:
				UnitBehaviour_1(unit);
				break;
			case UNIT_FISH_TYPE2:
				UnitBehaviour_2(unit);
				break;
			}
		}
	}

	// === 第4部分：更新小鱼 ===
	UpdateSmallFishes();

	// === 第5部分：碰撞检测（根据配置选择） ===
	if (systemConfig.useNewCollision) {
		CheckAllCollisions();
	}
	else {
		CheckPlayerFishCollision();
		CheckPlayerEnemyCollision(); 
	}

	// === 第6部分：检查关卡完成 ===
	if (systemConfig.useNewExperience && CheckLevelComplete()) {
		gameState = GAME_LEVEL_COMPLETE;
	}

	// === 第7部分：调试输出 ===
	if (systemConfig.showDebugInfo) {
		// 可以在这里添加调试信息
	}
}

// 检查关卡完成
bool CheckLevelComplete()
{
	if (units.empty()) return false;

	// 找到玩家鱼
	Unit* player = NULL;
	for (int i = 0; i < units.size(); i++) {
		if (units[i]->isPlayer) {
			player = units[i];
			break;
		}
	}
	if (!player) return false;

	// === 新增：第二关特殊通关条件 ===
	if (currentStageLevel == 2) {
		// 第二关：只需要玩家达到5级
		if (player->sizeLevel >= FISH_SIZE_LEVEL_5) {
			return true;
		}
		return false;
	}

	// 第一关和第三关保持原有通关逻辑
	StageConfig& config = stageConfigs[currentStageLevel];
	

	// 条件1：玩家达到目标等级
	if (player->sizeLevel < config.targetSize) {
		return false;
	}
	// 条件2：检查原敌人鱼是否还存在
	   // 原敌人鱼的特征：使用bmp_Unit_Fish2贴图
	for (int i = 0; i < units.size(); i++) {
		if (!units[i]->isPlayer && units[i]->img == bmp_Unit_Fish2) {
			// 原敌人鱼还存在，关卡未完成
			return false;
		}
	}

	return true;
}


// 自定义的安全访问函数
Unit* SafeGetUnit(int index)
{
#ifdef _DEBUG
	if (index < 0 || index >= units.size()) {
		wchar_t debugMsg[256];
		wsprintf(debugMsg, L"[ERROR] SafeGetUnit: Index %d out of range (size = %d)\n",
			index, units.size());
		OutputDebugString(debugMsg);
		return nullptr;
	}
#endif

	return units[index];
}


#pragma endregion
