#pragma once

#pragma region 头文件引用

// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>

// C++ 标准库
#include <vector>
#include <algorithm>

// 资源头文件
#include "resource.h"
#pragma comment(lib, "Msimg32.lib")			//图象处理的函数接口，例如：透明色的位图的绘制TransparentBlt函数
#pragma comment(lib, "winmm.lib")

// TODO:  在此处引用程序需要的其他头文件
#include <math.h>


#pragma endregion


#pragma region 宏定义

#define WINDOW_TITLEBARHEIGHT	32			//标题栏高度
#define WINDOW_WIDTH			1024		//游戏窗口宽度
#define WINDOW_HEIGHT			768			//游戏窗口高度


#define STAGE_STARTMENU			0		//开始画面的ID
#define STAGE_1					1		//第一个游戏场景的ID
#define STAGE_SELECT            2       // 选择关卡界面

#define UNIT_SIZE_X				64		//单位的宽度
#define UNIT_SIZE_Y				64		//单位的高度
#define UNIT_LAST_FRAME			11		//单位的动画帧最大值

//不同鱼的定义
#define UNIT_SIDE_FISH1			10000	//小鱼1
#define UNIT_SIDE_FISH2			10001	//小鱼2


//单位类型定义
#define UNIT_FISH_TYPE1		1		//第一种类型
#define UNIT_FISH_TYPE2		2		//第二种类型

//单位状态定义
#define UNIT_STATE_HOLD			0		//静止
#define UNIT_STATE_WALK			1		//行走
#define UNIT_STATE_ATTACK		2		//攻击

// 游戏状态定义
#define GAME_RUNNING           0        // 游戏运行中
#define GAME_PAUSED            1        // 游戏暂停
#define GAME_LEVEL_COMPLETE     2       // 关卡完成
#define GAME_LEVEL_FAILED       3       // 关卡失败

//单位方向定义
#define UNIT_DIRECT_RIGHT		0		//向右
#define UNIT_DIRECT_LEFT		1		//向左


//背景
#define BG_COLUMNS				16		//背景列数
#define BG_ROWS					12		//背景行数
#define BG_CELL_WIDTH			64		//背景单格宽度
#define BG_CELL_HEIGHT			64		//背景单格高度

//其它定义
// 关卡按钮ID
#define BUTTON_LEVEL1         2001    // 关卡1按钮ID
#define BUTTON_LEVEL2         2002    // 关卡2按钮ID
#define BUTTON_LEVEL3         2003    // 关卡3按钮ID
#define BUTTON_BACK           2004    // 返回按钮ID
#define BUTTON_NEXT_LEVEL       2101    // 下一关按钮ID（新增）
#define BUTTON_RETRY_LEVEL      2102    // 重玩本关按钮ID（新增）
#define BUTTON_SELECT_LEVEL     2103    // 选择关卡按钮ID

// 关卡按钮尺寸
#define BUTTON_LEVEL_WIDTH    150     // 按钮宽度
#define BUTTON_LEVEL_HEIGHT   80      // 按钮高度
#define BUTTON_LEVEL_SPACING  40      // 按钮间距

// 按钮实际尺寸
#define BUTTON_NEXT_WIDTH       72      // 下一关按钮宽度
#define BUTTON_NEXT_HEIGHT      73      // 下一关按钮高度
#define BUTTON_RETRY_WIDTH      74      // 重玩按钮宽度
#define BUTTON_RETRY_HEIGHT     63      // 重玩按钮高度
#define BUTTON_SELECT_WIDTH     78      // 选择关卡按钮宽度
#define BUTTON_SELECT_HEIGHT    61      // 选择关卡按钮高度
#define BITMAP_CHAR_WIDTH       90      // 字符位图宽度
#define BITMAP_CHAR_HEIGHT      96      // 字符位图高度
#define BITMAP_CHAR_SPACING     10      // 字符位图间距

#define BUTTON_STARTGAME			1001	//开始游戏按钮ID
#define BUTTON_PAUSE                1002    // 暂停按钮 ID
#define BUTTON_RESUME               1003    // 继续按钮 ID

//1125添加5，此处开始
#define BUTTON_HELP                 1004    // 帮助按钮 ID
#define BUTTON_HELP_WIDTH           111     // 帮助按钮宽度
#define BUTTON_HELP_HEIGHT          25      // 帮助按钮高度
//1125添加5，此处结束

#define BUTTON_STARTGAME_WIDTH		212		//开始游戏按钮宽度
#define BUTTON_STARTGAME_HEIGHT		76		//开始游戏按钮高度
#define BUTTON_PAUSEGAME_WIDTH      64     // 暂停按钮宽度
#define BUTTON_PAUSEGAME_HEIGHT     64     // 暂停按钮高度

#define TIMER_GAMETIMER				1		//游戏的默认计时器ID
#define TIMER_GAMETIMER_ELAPSE		30		//默认计时器刷新间隔的毫秒数
#define UNIT_SPEED					12.0		//单位行走速度	

#define SMALL_FISH_SIZE 1          // 小鱼的大小等级（1最小）
#define PLAYER_FISH_SIZE 2         // 玩家鱼的大小等级（2中等）
#define ENEMY_FISH_SIZE 2          // 原敌人鱼的大小等级（2中等）

#define TIMER_SPAWN_FISH 2         // 生成小鱼的定时器ID
#define SPAWN_INTERVAL 500        // 生成间隔(毫秒)

#define SMALL_FISH_HEALTH 30       // 小鱼的生命值

// 鱼的大小等级定义
#define FISH_SIZE_LEVEL_1 1   // 小鱼
#define FISH_SIZE_LEVEL_2 2   // 玩家初始、小型敌人鱼
#define FISH_SIZE_LEVEL_3 3   // 中型敌人鱼
#define FISH_SIZE_LEVEL_4 4   // 大型敌人鱼
#define FISH_SIZE_LEVEL_5 5   // 巨型敌人鱼

// 玩家鱼最大等级
#define MAX_PLAYER_LEVEL FISH_SIZE_LEVEL_5

// 鱼的类型定义（用于选择贴图）
#define FISH_TYPE_PLAYER       0
#define FISH_TYPE_SMALL_FISH   1  // 小鱼（size=1）
#define FISH_TYPE_ENEMY_SMALL1 10  // 小型敌人鱼类型1（size=2）
#define FISH_TYPE_ENEMY_SMALL2 11  // 小型敌人鱼类型2（size=2）
#define FISH_TYPE_ENEMY_MEDIUM 12  // 中型敌人鱼（size=3）
#define FISH_TYPE_ENEMY_LARGE  13  // 大型敌人鱼（size=4）
#define FISH_TYPE_ENEMY_GIANT  14 // 巨型敌人鱼（size=5）

// 升级所需经验（不同等级不同）
#define EXP_FOR_LEVEL_2_TO_3   5   // 2->3需要5条小鱼
#define EXP_FOR_LEVEL_3_TO_4   8   // 3->4需要8条小鱼（或等价经验）
#define EXP_FOR_LEVEL_4_TO_5   12  // 4->5需要12条小鱼（或等价经验）

// 不同大小鱼的经验值
#define EXP_SMALL_FISH    1   // size=1: 1点经验
#define EXP_SMALL_ENEMY   2   // size=2: 2点经验
#define EXP_MEDIUM_ENEMY  4   // size=3: 4点经验
#define EXP_LARGE_ENEMY   6   // size=4: 6点经验
#define EXP_GIANT_ENEMY   8   // size=5: 8点经验

// 关卡相关
#define MAX_STAGES        10  // 最大关卡数

// 碰撞半径相关
#define COLLISION_RADIUS_BASE 25   // 基础碰撞半径
#define COLLISION_RADIUS_PER_LEVEL 15  // 每级增加的碰撞半径

#ifdef _DEBUG
#define DEBUG_LOG(msg) OutputDebugString(msg)
#define DEBUG_LOG_FORMAT(format, ...) \
        do { \
            wchar_t debugMsg[256]; \
            wsprintf(debugMsg, format, __VA_ARGS__); \
            OutputDebugString(debugMsg); \
        } while(0)
#else
#define DEBUG_LOG(msg) ((void)0)
#define DEBUG_LOG_FORMAT(format, ...) ((void)0)
#endif
// 严重错误总是记录
#define ERROR_LOG(msg) OutputDebugString(msg)


#pragma endregion


#pragma region 结构体声明

// 场景结构体
struct Stage
{
	int stageID;		//场景编号
	HBITMAP bg;			//背景图片
	int timeCountDown;	//游戏时间倒计时
	bool timerOn;		//计时器是否运行（游戏是否被暂停）

};


// 按钮结构体
struct Button
{
	int buttonID;	//按钮编号
	bool visible;	//按钮是否可见
	HBITMAP img;	//图片
	int x;			//坐标x
	int y;			//坐标y
	int width;		//宽度
	int height;		//高度
};

// 单位结构体
struct Unit
{
	HBITMAP img;	//图片

	int frame_row;			//当前显示的是图像的第几行
	int frame_column;		//当前显示的是图像的第几列

	int* frame_sequence;		//当前的帧序列
	int frame_count;		//帧序列的长度
	int frame_id;			//当前显示的是帧序列的第几个


	int side;		//单位阵营
	int type;		//单位类型
	int state;		//单位状态
	int direction;	//单位方向

	int x;			//坐标x
	int y;			//坐标y
	double vx;		//速度x
	double vy;		//速度y
	int health;		//生命值
	int size;

	int sizeLevel;      // 大小等级（1-5）
	int fishType;       // 鱼的详细类型（FISH_TYPE_XXX）
	int experience;     // 当前等级积累的经验值
	bool isPlayer;      // 是否是玩家鱼
	bool useNewSystem;  // 是否使用新系统（默认为false）

	// 新增：攻击动画计时器
	int attackAnimTimer;      // 吃鱼攻击动画计时器（15帧）
	int levelUpFlashTimer;    // 新增：升级闪烁计时器（33帧）

	// 构造函数初始化所有字段
	Unit() : img(NULL), frame_row(0), frame_column(0),
		frame_sequence(NULL), frame_count(0), frame_id(0),
		side(0), type(0), state(0), direction(0),
		x(0), y(0), vx(0), vy(0), health(100),
		sizeLevel(0), fishType(0), experience(0),
		isPlayer(false), useNewSystem(false),
		attackAnimTimer(0),
		levelUpFlashTimer(0) {  // 新增初始化
	}
};




//TODO: 添加游戏需要的更多种数据（地物、砖块等）

// 关卡配置结构体
struct StageConfig
{
	int stageID;                    // 关卡ID
	int playerStartSize;           // 玩家初始等级
	int targetSize;                // 目标等级（胜利条件）
	int fishCount[5];              // 每种size的鱼数量（size1-5）
	int fishTypes[5][3];           // 每种size对应的鱼类型数组
	int fishTypeCount[5];          // 每种size的鱼类型数量
	const wchar_t* background;     // 背景图片路径
	int maxSmallFishes;            // 最大小鱼数量
};

#pragma endregion

//1123添加1，此处开始
#pragma region 全局变量声明

// 游戏状态
extern HBITMAP bmp_PauseButton;
extern HBITMAP bmp_ResumeButton;
extern HBITMAP bmp_HelpButton;  

// 小鱼相关全局变量
extern std::vector<Unit*> smallFishes;
extern int smallFishCount;
extern HBITMAP bmp_SmallFish;
extern int smallFishBmpWidth;
extern int smallFishBmpHeight;

// 关卡选择相关
extern HBITMAP bmp_SelectBackground;
extern HBITMAP bmp_Level1Button;
extern HBITMAP bmp_Level2Button;
extern HBITMAP bmp_Level3Button;
extern int selectedLevel;  // 当前选中的关卡（1-3）
extern int gameState;                    // 游戏整体状态：0=游戏中,1=暂停,2=关卡完成,3=关卡失败

extern HBITMAP bmp_NextLevel;            // 下一关按钮图像（从文件加载）
extern HBITMAP bmp_SelectLevel;          // 选择关卡按钮图像（从文件加载）
extern HBITMAP bmp_RetryLevel;           // 重玩本关按钮图像（从文件加载）

extern HBITMAP bmp_ren;                  // "人"字位图
extern HBITMAP bmp_wu;                   // "物"字位图
extern HBITMAP bmp_shi;                  // "失"字位图
extern HBITMAP bmp_bai;                  // "败"字位图

extern HBITMAP bmp_tiao;      // "挑"字位图
extern HBITMAP bmp_zhan;      // "战"字位图
extern HBITMAP bmp_cheng;     // "成"字位图
extern HBITMAP bmp_gong;      // "功"字位图

extern StageConfig stageConfigs[MAX_STAGES];
extern int currentStageLevel;
extern float sizeScales[6];        // 等级对应的缩放比例（索引1-5）
extern int expRequired[6];         // 升级所需经验（索引为当前等级）
extern int expValues[6];           // 吃不同size鱼的经验值（索引为鱼的size）
extern std::vector<HBITMAP> fishBitmaps;  // 鱼位图数组
extern int MAX_SMALL_FISHES;       // 最大小鱼数量
extern int MAX_SMALL_ENEMY_FISHES;
extern int smallEnemyFishCount;

extern std::vector<Unit*> units;
extern std::vector<Button*> buttons;

// 鱼雷相关声明
extern HBITMAP bmp_Torpedo;
extern std::vector<Unit*> torpedoes;
extern int torpedoCount;
void SpawnTorpedo(HWND hWnd);
void ClearTorpedoes();

// 音频相关全局变量
extern bool g_bStartMenuMusicPlaying;   // 开始界面音乐播放状态
extern bool g_bEnableAudio;             // 全局音频开关

extern bool g_bLevel2MusicPlaying;      // 关卡2音乐播放状态
extern bool g_bLevel2MusicPaused;       // 关卡2音乐暂停状态
extern MCIDEVICEID g_wLevel2MusicDeviceID;    // MCI设备ID
extern DWORD g_dwLevel2MusicPosition;         // 记录暂停时的播放位置
extern bool g_bLevel2MusicInitialized;    // MCI设备是否已初始化

// 关卡1音频相关全局变量
extern bool g_bLevel1MusicPlaying;      // 关卡1音乐播放状态
extern bool g_bLevel1MusicPaused;       // 关卡1音乐暂停状态
extern MCIDEVICEID g_wLevel1MusicDeviceID;    // MCI设备ID
extern DWORD g_dwLevel1MusicPosition;         // 记录暂停时的播放位置
extern bool g_bLevel1MusicInitialized;    // MCI设备是否已初始化

// 关卡3音频相关全局变量
extern bool g_bLevel3MusicPlaying;      // 关卡3音乐播放状态
extern bool g_bLevel3MusicPaused;       // 关卡3音乐暂停状态
extern MCIDEVICEID g_wLevel3MusicDeviceID;    // MCI设备ID
extern DWORD g_dwLevel3MusicPosition;         // 记录暂停时的播放位置
extern bool g_bLevel3MusicInitialized;    // MCI设备是否已初始化
#pragma endregion
//1123添加2，此处结束


#pragma region 事件处理函数声明


// 初始化游戏窗体函数
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 键盘按下事件处理函数
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 键盘松开事件处理函数
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标移动事件处理函数
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标左键按下事件处理函数
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标左键松开事件处理函数
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 定时器事件处理函数
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);

void CleanupGameResources();  // 清理所有游戏资源
void ClearUnits();           // 清理单位对象
void ClearButtons();         // 清理按钮对象（但保留图片资源）
void UnitBehaviour_SmallFish(Unit* unit);



#pragma endregion


#pragma region 其它游戏状态处理函数声明


// 添加按钮函数
Button* CreateButton(int buttonID, HBITMAP img, int width, int height, int x, int y);

// 添加单位函数
Unit* CreateUnit(int side, int type, int x, int y, int health);


// 初始化场景函数
void InitStage(HWND hWnd, int stageID);

// 刷新单位状态函数
void UpdateUnits(HWND hWnd);



//单位行为函数
void UnitBehaviour_1(Unit* unit);
void UnitBehaviour_2(Unit* unit);


//TODO: 添加游戏需要的更多函数

// 小鱼相关函数
void SpawnSmallFish(HWND hWnd);
void UpdateSmallFishes();
void CheckPlayerFishCollision();
void ClearSmallFishes();  // 清理小鱼

// 关卡管理函数
void InitializeStageConfigs();
void SetupStageConfig(int stageID);
void SpawnStageFishes(HWND hWnd);

// 鱼管理函数
Unit* CreateFishUnit(int fishType, int sizeLevel, int x, int y, bool isPlayer = false);
void LoadAllFishBitmaps(HINSTANCE hInstance);

// 碰撞和升级系统
void CheckAllCollisions();
bool CheckFishCollision(Unit* fish1, Unit* fish2);
void HandleFishCollision(Unit* player, Unit* otherFish, int otherIndex, bool isSmallFish);
void EatFish(Unit* player, Unit* food, int foodIndex, bool isSmallFish);
void CheckLevelUp(Unit* player);
bool CheckLevelComplete();

// 工具函数
int GetRequiredExpForLevel(int level);
int GetExpValueForSize(int size);
int GetCollisionRadius(int sizeLevel);
void PlayLevelUpEffect(Unit* player);
void PlayEatEffect(Unit* food);
int CalculateProgressPercentage(Unit* player);

// 兼容性函数
Unit* CreateCompatibleUnit(int side, int type, int x, int y, int health, int sizeLevel = 1, bool isPlayer = false);
bool ValidateNewSystem();  // 验证新系统函数
void EnsureNewSystemForAllUnits();  // 确保所有单位都启用新系统
bool ShouldUseAnimationDrawing(Unit* unit);

void ShowPauseButtons(HWND hWnd);
void ShowOverlayButtons(HWND hWnd, int buttonMask);  // 新的统一函数

// 音频相关函数声明
void PlayStartMenuMusic();         // 播放开始界面音乐
void StopStartMenuMusic();         // 停止开始界面音乐
bool IsStartMenuMusicPlaying();    // 检查开始界面音乐是否在播放

void PlayLevel2Music();            // 播放关卡2音乐
void StopLevel2Music();            // 停止关卡2音乐
void PauseLevel2Music();           // 暂停关卡2音乐
void ResumeLevel2Music();          // 恢复关卡2音乐
void RestartLevel2Music();         // 重新开始关卡2音乐

// 关卡1音频控制函数
void PlayLevel1Music();            // 播放关卡1音乐
void StopLevel1Music();            // 停止关卡1音乐
void PauseLevel1Music();           // 暂停关卡1音乐
void ResumeLevel1Music();          // 恢复关卡1音乐

// 关卡3音频控制函数
void PlayLevel3Music();            // 播放关卡3音乐
void StopLevel3Music();            // 停止关卡3音乐
void PauseLevel3Music();           // 暂停关卡3音乐
void ResumeLevel3Music();          // 恢复关卡3音乐
void RestartLevel3Music();         // 重新开始关卡3音乐

void HandleStageAudio(int stageID); // 根据场景处理音频

#pragma endregion 


#pragma region 绘图函数声明

// 绘图函数
void Paint(HWND hWnd);

// 初始化背景函数
HBITMAP InitBackGround(HWND hWnd, HBITMAP bmp_src);

