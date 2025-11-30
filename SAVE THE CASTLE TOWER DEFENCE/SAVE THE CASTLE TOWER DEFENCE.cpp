#include "framework.h"
#include "SAVE THE CASTLE TOWER DEFENCE.h"
#include <mmsystem.h>
#include <d2d1.h>
#include <dwrite.h>
#include "D2BMPLOADER.h"
#include "gifresizer.h"
#include "ErrH.h"
#include "FCheck.h"
#include "TDHelper.h"
#include <chrono>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d2write.lib")
#pragma comment(lib, "d2bmploader.lib")
#pragma comment(lib, "gifresizer.lib")
#pragma comment(lib, "errh.lib")
#pragma comment(lib, "fcheck.lib")
#pragma comment(lib, "tdhelper.lib")

constexpr wchar_t bWinClassName[]{ L"Castle_defence" };
constexpr char tmp_file[]{ ".\\res\\data\\temp.dat" };
constexpr wchar_t Ltmp_file[]{ L".\\res\\data\\temp.dat" };
constexpr wchar_t save_file[]{ L".\\res\\data\\save.dat" };
constexpr wchar_t help_file[]{ L".\\res\\data\\help.dat" };
constexpr wchar_t record_file[]{ L".\\res\\data\\record.dat" };
constexpr wchar_t snd_file[]{ L".\\res\\snd\\sound.wav" };

constexpr int mNew{ 1001 };
constexpr int mExit{ 1002 };
constexpr int mSave{ 1003 };
constexpr int mLoad{ 1004 };
constexpr int mHoF{ 1005 };

constexpr int no_record{ 2001 };
constexpr int first_record{ 2002 };
constexpr int record{ 2003 };

WNDCLASS bWinClass{};
HINSTANCE bIns{ nullptr };
HWND bHwnd{ nullptr };
HICON mainIcon{ nullptr };
HCURSOR mainCursor{ nullptr };
HCURSOR outCursor{ nullptr };
HMENU bBar{ nullptr };
HMENU bMain{ nullptr };
HMENU bStore{ nullptr };
HDC bPaint{ nullptr };
PAINTSTRUCT bPaintStruct{};
MSG bMsg{};
BOOL bRet{ 0 };
POINT cur_pos{};
UINT bTimer{ 0 };

D2D1_RECT_F b1Rect{ 10.0f, 0, scr_width / 3.0f, 50.0f };
D2D1_RECT_F b2Rect{ scr_width / 3.0f + 10.0f, 0, scr_width * 2.0f / 3.0f, 50.0f };
D2D1_RECT_F b3Rect{ scr_width * 2.0f / 3.0f + 10.0f, 0, scr_width - 10.0f, 50.0f };

D2D1_RECT_F b1TxtRect{ 30.0f, 5.0f, scr_width / 3.0f, 50.0f };
D2D1_RECT_F b2TxtRect{ scr_width / 3.0f + 30.0f, 5.0f, scr_width * 2.0f / 3.0f, 50.0f };
D2D1_RECT_F b3TxtRect{ scr_width * 2.0f / 3.0f + 30.0f, 5.0f, scr_width - 10.0f, 50.0f };

float scale_x{ 0 };
float scale_y{ 0 };

bool pause{ false };
bool sound{ true };
bool in_client{ true };
bool show_help{ false };
bool name_set{ false };
bool b1Hglt{ false };
bool b2Hglt{ false };
bool b3Hglt{ false };

wchar_t current_player[16]{ L"A KING" };

int mins{ 0 };
int secs{ 0 };
int level{ 0 };
int score{ 0 };

ID2D1Factory* iFactory{ nullptr };
ID2D1HwndRenderTarget* Draw{ nullptr };

ID2D1RadialGradientBrush* b1Bckg{ nullptr };
ID2D1RadialGradientBrush* b2Bckg{ nullptr };
ID2D1RadialGradientBrush* b3Bckg{ nullptr };

ID2D1SolidColorBrush* StatBrush{ nullptr };
ID2D1SolidColorBrush* TxtBrush{ nullptr };
ID2D1SolidColorBrush* HgltBrush{ nullptr };
ID2D1SolidColorBrush* InactBrush{ nullptr };

IDWriteFactory* iWriteFactory{ nullptr };
IDWriteTextFormat* nrmFormat{ nullptr };
IDWriteTextFormat* midFormat{ nullptr };
IDWriteTextFormat* bigFormat{ nullptr };

ID2D1Bitmap* bmpField{ nullptr };
ID2D1Bitmap* bmpLogo{ nullptr };
ID2D1Bitmap* bmpRock{ nullptr };
ID2D1Bitmap* bmpSmallTree{ nullptr };
ID2D1Bitmap* bmpMidTree{ nullptr };
ID2D1Bitmap* bmpBigTree{ nullptr };
ID2D1Bitmap* bmpWall{ nullptr };

ID2D1Bitmap* bmpArrowUpL{ nullptr };
ID2D1Bitmap* bmpArrowDownL{ nullptr };
ID2D1Bitmap* bmpArrowUpR{ nullptr };
ID2D1Bitmap* bmpArrowDownR{ nullptr };

ID2D1Bitmap* bmpCannonBall{ nullptr };

ID2D1Bitmap* bmpSpell[64]{ nullptr };

ID2D1Bitmap* bmpIntro[8]{ nullptr };
ID2D1Bitmap* bmpExplosion[24]{ nullptr };

ID2D1Bitmap* bmpCastle[5]{ nullptr };
ID2D1Bitmap* bmpArchTower[8]{ nullptr };
ID2D1Bitmap* bmpCannonSmall[58]{ nullptr };
ID2D1Bitmap* bmpCannonMid[10]{ nullptr };
ID2D1Bitmap* bmpCannonBig[91]{ nullptr };
ID2D1Bitmap* bmpMageSmall[37]{ nullptr };
ID2D1Bitmap* bmpMageMid[35]{ nullptr };
ID2D1Bitmap* bmpMageBig[98]{ nullptr };

ID2D1Bitmap* bmpOrc1L[8]{ nullptr };
ID2D1Bitmap* bmpOrc1R[8]{ nullptr };

ID2D1Bitmap* bmpOrc2L[10]{ nullptr };
ID2D1Bitmap* bmpOrc2R[10]{ nullptr };

ID2D1Bitmap* bmpHealerL[46]{ nullptr };
ID2D1Bitmap* bmpHealerR[46]{ nullptr };

ID2D1Bitmap* bmpCrusherL[40]{ nullptr };
ID2D1Bitmap* bmpCrusherR[40]{ nullptr };

ID2D1Bitmap* bmpFlyerL[34]{ nullptr };
ID2D1Bitmap* bmpFlyerR[34]{ nullptr };

ID2D1Bitmap* bmpChampionL[10]{ nullptr };
ID2D1Bitmap* bmpChampionR[10]{ nullptr };

///////////////////////////////////////////////////////







////////////////////////////////////////////////////////

template<typename T>concept HasRelease = requires(T var)
{
	var.Release();
};
template<HasRelease T> bool FreeHeap(T** var)
{
	if ((*var))
	{
		(*var)->Release();
		(*var) = nullptr;
		return true;
	}

	return false;
}
void LogErr(LPCWSTR what)
{
	std::wofstream err(L".\\res\\data\\error.log", std::ios::app);
	err << what << L" time stamp: " << std::chrono::system_clock::now() << std::endl;
	err.close();
}
void ClearResources()
{
	if (!(FreeHeap(&iFactory)))LogErr(L"Error clearing iFactory !");
	if (!(FreeHeap(&Draw)))LogErr(L"Error clearing Draw !");
	
	if (!(FreeHeap(&b1Bckg)))LogErr(L"Error clearing B1Bckg !");
	if (!(FreeHeap(&b2Bckg)))LogErr(L"Error clearing B2Bckg !");
	if (!(FreeHeap(&b3Bckg)))LogErr(L"Error clearing B3Bckg !");
	
	if (!(FreeHeap(&StatBrush)))LogErr(L"Error clearing StatBrush !");
	if (!(FreeHeap(&TxtBrush)))LogErr(L"Error clearing TxtBrush !");
	if (!(FreeHeap(&HgltBrush)))LogErr(L"Error clearing HgltBrush !");
	if (!(FreeHeap(&InactBrush)))LogErr(L"Error clearing InactBrush !");

	if (!(FreeHeap(&iWriteFactory)))LogErr(L"Error clearing iWriteFactory !");
	if (!(FreeHeap(&nrmFormat)))LogErr(L"Error clearing nrmFormat !");
	if (!(FreeHeap(&midFormat)))LogErr(L"Error clearing midFormat !");
	if (!(FreeHeap(&bigFormat)))LogErr(L"Error clearing bigFormat !");

	if (!(FreeHeap(&bmpField)))LogErr(L"Error clearing bmpField !");
	if (!(FreeHeap(&bmpLogo)))LogErr(L"Error clearing bmpLogo !");
	if (!(FreeHeap(&bmpRock)))LogErr(L"Error clearing bmpRock !");
	if (!(FreeHeap(&bmpSmallTree)))LogErr(L"Error clearing bmpSmallTree !");
	if (!(FreeHeap(&bmpMidTree)))LogErr(L"Error clearing bmpMidTree !");
	if (!(FreeHeap(&bmpBigTree)))LogErr(L"Error clearing bmpBigTree !");
	if (!(FreeHeap(&bmpWall)))LogErr(L"Error clearing bmpWall !");

	if (!(FreeHeap(&bmpArrowUpL)))LogErr(L"Error clearing bmpArrowUpL !");
	if (!(FreeHeap(&bmpArrowDownL)))LogErr(L"Error clearing bmpArrowDownL !");
	if (!(FreeHeap(&bmpArrowUpR)))LogErr(L"Error clearing bmpArrowUpR !");
	if (!(FreeHeap(&bmpArrowDownR)))LogErr(L"Error clearing bmpArrowDownR !");
	if (!(FreeHeap(&bmpCannonBall)))LogErr(L"Error clearing bmpCannonBall !");

	for (int i = 0; i < 8; ++i)if (!(FreeHeap(&bmpIntro[i])))LogErr(L"Error clearing bmpIntro !");
	for (int i = 0; i < 24; ++i)if (!(FreeHeap(&bmpExplosion[i])))LogErr(L"Error clearing bmpExplosion !");

	for (int i = 0; i < 64; ++i)if (!(FreeHeap(&bmpSpell[i])))LogErr(L"Error clearing bmpSpell !");

	for (int i = 0; i < 5; ++i)if (!(FreeHeap(&bmpCastle[i])))LogErr(L"Error clearing bmpCastle !");
	for (int i = 0; i < 8; ++i)if (!(FreeHeap(&bmpArchTower[i])))LogErr(L"Error clearing bmpArchTower !");

	for (int i = 0; i < 58; ++i)if (!(FreeHeap(&bmpCannonSmall[i])))LogErr(L"Error clearing bmpCannonSmall !");
	for (int i = 0; i < 10; ++i)if (!(FreeHeap(&bmpCannonMid[i])))LogErr(L"Error clearing bmpCannonMid !");
	for (int i = 0; i < 91; ++i)if (!(FreeHeap(&bmpCannonBig[i])))LogErr(L"Error clearing bmpCannonBig !");

	for (int i = 0; i < 37; ++i)if (!(FreeHeap(&bmpMageSmall[i])))LogErr(L"Error clearing bmpMageSmall !");
	for (int i = 0; i < 35; ++i)if (!(FreeHeap(&bmpMageMid[i])))LogErr(L"Error clearing bmpMageMid !");
	for (int i = 0; i < 98; ++i)if (!(FreeHeap(&bmpMageBig[i])))LogErr(L"Error clearing bmpMageBig !");

	for (int i = 0; i < 8; ++i)if (!(FreeHeap(&bmpOrc1L[i])))LogErr(L"Error clearing bmpOrc1L !");
	for (int i = 0; i < 8; ++i)if (!(FreeHeap(&bmpOrc1R[i])))LogErr(L"Error clearing bmpOrc1R !");

	for (int i = 0; i < 10; ++i)if (!(FreeHeap(&bmpOrc2L[i])))LogErr(L"Error clearing bmpOrc2L !");
	for (int i = 0; i < 10; ++i)if (!(FreeHeap(&bmpOrc2R[i])))LogErr(L"Error clearing bmpOrc2R !");

	for (int i = 0; i < 46; ++i)if (!(FreeHeap(&bmpHealerL[i])))LogErr(L"Error clearing bmpHealerL !");
	for (int i = 0; i < 46; ++i)if (!(FreeHeap(&bmpHealerR[i])))LogErr(L"Error clearing bmpHealerR !");

	for (int i = 0; i < 40; ++i)if (!(FreeHeap(&bmpCrusherL[i])))LogErr(L"Error clearing bmpCrusherL !");
	for (int i = 0; i < 40; ++i)if (!(FreeHeap(&bmpCrusherR[i])))LogErr(L"Error clearing bmpCrusherR !");

	for (int i = 0; i < 34; ++i)if (!(FreeHeap(&bmpFlyerL[i])))LogErr(L"Error clearing bmpFlyerL !");
	for (int i = 0; i < 34; ++i)if (!(FreeHeap(&bmpFlyerR[i])))LogErr(L"Error clearing bmpFlyerR !");

	for (int i = 0; i < 10; ++i)if (!(FreeHeap(&bmpChampionL[i])))LogErr(L"Error clearing bmpChampionL !");
	for (int i = 0; i < 10; ++i)if (!(FreeHeap(&bmpChampionR[i])))LogErr(L"Error clearing bmpChampionR !");
}
void ErrExit(int what)
{
	MessageBeep(MB_ICONERROR);
	MessageBox(NULL, ErrHandle(what), L"Критична грешка !", MB_OK | MB_APPLMODAL | MB_ICONERROR);

	std::remove(tmp_file);
	ClearResources();
	exit(1);
}
int IntroFrame()
{
	static int intro_frame_delay{ 8 };
	static int intro_frame{ 0 };

	--intro_frame_delay;
	if (intro_frame_delay <= 0)
	{
		intro_frame_delay = 8;
		++intro_frame;
		if (intro_frame > 7)intro_frame = 0;
	}

	return intro_frame;
}

void GameOver()
{
	PlaySound(NULL, NULL, NULL);
	KillTimer(bHwnd, bTimer);

	bMsg.message = WM_QUIT;
	bMsg.wParam = 0;
}
void InitGame()
{
	mins = 0;
	secs = 0;
	level = 0;
	score = 0;
	bTimer = 0;

	wcscpy_s(current_player, L"A KING");
	name_set = false;



}

INT_PTR CALLBACK bDlgProc(HWND hwnd, UINT ReceivedMsg, WPARAM wParam, LPARAM lParam)
{
	switch (ReceivedMsg)
	{
	case WM_INITDIALOG:
		SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)(mainIcon));
		return true;

	case WM_CLOSE:
		EndDialog(hwnd, IDCANCEL);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			break;

		case IDOK:
			if (GetDlgItemText(hwnd, IDC_NAME, current_player, 16) < 1)
			{
				wcscpy_s(current_player, L"A KING");
				name_set = false;
				if (sound)mciSendString(L"play .\\res\\snd\\exclamation.wav", NULL, NULL, NULL);
				MessageBox(bHwnd, L"Ха, ха, ха ! Забрави си името !", L"Забраватор !", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
				EndDialog(hwnd, IDCANCEL);
				break;
			}
			EndDialog(hwnd, IDOK);
			break;
		}
		break;
	}

	return (INT_PTR)(FALSE);
}
LRESULT CALLBACK bWinProc(HWND hwnd, UINT ReceivedMsg, WPARAM wParam, LPARAM lParam)
{
	switch (ReceivedMsg)
	{
	case WM_CREATE:
		if (bIns)
		{
			SetTimer(hwnd, bTimer, 1000, NULL);

			bBar = CreateMenu();
			bMain = CreateMenu();
			bStore = CreateMenu();

			AppendMenu(bBar, MF_POPUP, (UINT_PTR)(bMain), L"Основно меню");
			AppendMenu(bBar, MF_POPUP, (UINT_PTR)(bStore), L"Меню за данни");

			AppendMenu(bMain, MF_STRING, mNew, L"Нова игра");
			AppendMenu(bMain, MF_SEPARATOR, NULL, NULL);
			AppendMenu(bMain, MF_STRING, mExit, L"Изход");

			AppendMenu(bStore, MF_STRING, mSave, L"Запазване на игра");
			AppendMenu(bStore, MF_STRING, mLoad, L"зареждане на игра");
			AppendMenu(bStore, MF_SEPARATOR, NULL, NULL);
			AppendMenu(bStore, MF_STRING, mHoF, L"Зала на славата");

			SetMenu(hwnd, bBar);
			InitGame();
		}
		break;

	case WM_CLOSE:
		pause = true;
		if (sound)mciSendString(L"play .\\res\\snd\\exclamation.wav", NULL, NULL, NULL);
		if (MessageBox(hwnd, L"Ако излезеш без запис, ще загубиш тази игра !\n\nНаистина ли излизаш ?",
			L"Изход", MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION) == IDNO)
		{
			pause = false;
			break;
		}
		GameOver();
		break;

	case WM_TIMER:
		if (pause)break;
		++secs;
		mins = secs / 60;
		break;

	case WM_PAINT:
		bPaint = BeginPaint(hwnd, &bPaintStruct);
		FillRect(bPaint, &bPaintStruct.rcPaint, CreateSolidBrush(RGB(0, 0, 0)));
		EndPaint(hwnd, &bPaintStruct);
		break;

	case WM_SETCURSOR:
		GetCursorPos(&cur_pos);
		ScreenToClient(hwnd, &cur_pos);
		if (LOWORD(lParam) == HTCLIENT)
		{
			if (!in_client)
			{
				in_client = true;
				pause = false;
			}

			if (cur_pos.y * scale_y <= 50)
			{
				if (cur_pos.x * scale_x >= b1Rect.left && cur_pos.x * scale_x <= b1Rect.right)
				{
					if (!b1Hglt)
					{
						if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
						b1Hglt = true;
						b2Hglt = false;
						b3Hglt = false;
					}
				}
				if (cur_pos.x * scale_x >= b2Rect.left && cur_pos.x * scale_x <= b2Rect.right)
				{
					if (!b2Hglt)
					{
						if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
						b1Hglt = false;
						b2Hglt = true;
						b3Hglt = false;
					}
				}
				if (cur_pos.x * scale_x >= b3Rect.left && cur_pos.x * scale_x <= b3Rect.right)
				{
					if (!b3Hglt)
					{
						if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
						b1Hglt = false;
						b2Hglt = false;
						b3Hglt = true;
					}
				}

				SetCursor(outCursor);
				return true;
			}

			if (b1Hglt || b2Hglt || b3Hglt)
			{
				if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
				b1Hglt = false;
				b2Hglt = false;
				b3Hglt = false;
			}

			SetCursor(mainCursor);

			return true;
		}
		else
		{
			if (in_client)
			{
				in_client = false;
				pause = true;
			}

			if (b1Hglt || b2Hglt || b3Hglt)
			{
				if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
				b1Hglt = false;
				b2Hglt = false;
				b3Hglt = false;
			}

			SetCursor(LoadCursor(NULL, IDC_ARROW));

			return true;
		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case mNew:
			pause = true;
			if (sound)mciSendString(L"play .\\res\\snd\\exclamation.wav", NULL, NULL, NULL);
			if (MessageBox(hwnd, L"Ако рестартираш без запис, ще загубиш тази игра !\n\nНаистина ли рестартираш ?",
				L"Рестарт", MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION) == IDNO)
			{
				pause = false;
				break;
			}
			InitGame();
			break;

		case mExit:
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			break;

		}
		break;





	default: return DefWindowProc(hwnd, ReceivedMsg, wParam, lParam);
	}

	return (LRESULT)(FALSE);
}

void CreateResources()
{
	int result{ 0 };
	CheckFile(Ltmp_file, &result);
	if (result == FILE_EXIST)ErrExit(eStarted);
	else
	{
		std::wofstream start(Ltmp_file);
		start << L"Game started at: " << std::chrono::system_clock::now();
		start.close();
	}

	int win_x = GetSystemMetrics(SM_CXSCREEN) / 2 - (int)(scr_width / 2.0f);
	int win_y = 10;

	if (win_x + (int)(scr_width) > GetSystemMetrics(SM_CXSCREEN) ||
		win_y + (int)(scr_height) > GetSystemMetrics(SM_CYSCREEN))ErrExit(eScreen);

	mainIcon = (HICON)(LoadImage(NULL, L".\\res\\main.ico", IMAGE_ICON, 255, 255, LR_LOADFROMFILE));
	if (!mainIcon)ErrExit(eIcon);
	mainCursor = LoadCursorFromFileW(L".\\res\\main.ani");
	outCursor = LoadCursorFromFileW(L".\\res\\out.ani");
	if (!mainCursor || !outCursor)ErrExit(eCursor);

	bWinClass.lpszClassName = bWinClassName;
	bWinClass.hInstance = bIns;
	bWinClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	bWinClass.lpfnWndProc = &bWinProc;
	bWinClass.hIcon = mainIcon;
	bWinClass.hCursor = mainCursor;
	bWinClass.style = CS_DROPSHADOW;

	if (!RegisterClass(&bWinClass))ErrExit(eClass);

	bHwnd = CreateWindow(bWinClassName, L"DEFEND THE CASTLE !", WS_CAPTION | WS_SYSMENU, win_x, win_y,
		(int)(scr_width), (int)(scr_height), NULL, NULL, bIns, NULL);
	if (!bHwnd)ErrExit(eWindow);
	else
	{
		ShowWindow(bHwnd, SW_SHOWDEFAULT);

		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &iFactory);
		if (hr != S_OK)
		{
			LogErr(L"Error creating iFactory !");
			ErrExit(eD2D);
		}

		if (iFactory)
		{
			hr = iFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(bHwnd,
				D2D1::SizeU((UINT32)(scr_width), (UINT32)(scr_height))), &Draw);
			if (hr != S_OK)
			{
				LogErr(L"Error creating hwndRenderTarget !");
				ErrExit(eD2D);
			}
		}

		if (Draw)
		{
			D2D1_GRADIENT_STOP gStops[2]{};
			ID2D1GradientStopCollection* gColl{ nullptr };

			gStops[0].position = 0;
			gStops[0].color = D2D1::ColorF(D2D1::ColorF::Chocolate);
			gStops[1].position = 1.0f;
			gStops[1].color = D2D1::ColorF(D2D1::ColorF::Brown);

			hr = Draw->CreateGradientStopCollection(gStops, 2, &gColl);
			if (hr != S_OK)
			{
				LogErr(L"Error creating GradientStopCollection !");
				ErrExit(eD2D);
			}

			if (gColl)
			{
				hr = Draw->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(D2D1::Point2F(b1Rect.left +
					(b1Rect.right - b1Rect.left) / 2.0f, 25.0f), D2D1::Point2F(0, 0), (b1Rect.right - b1Rect.left) / 2.0f, 25.0f),
					gColl, &b1Bckg);
				hr = Draw->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(D2D1::Point2F(b2Rect.left +
					(b2Rect.right - b2Rect.left) / 2.0f, 25.0f), D2D1::Point2F(0, 0), (b2Rect.right - b2Rect.left) / 2.0f, 25.0f),
					gColl, &b2Bckg);
				hr = Draw->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(D2D1::Point2F(b3Rect.left +
					(b3Rect.right - b3Rect.left) / 2.0f, 25.0f), D2D1::Point2F(0, 0), (b3Rect.right - b3Rect.left) / 2.0f, 25.0f),
					gColl, &b3Bckg);

				if (hr != S_OK)
				{
					LogErr(L"Error creating RadialGradientBrushes !");
					ErrExit(eD2D);
				}

				FreeHeap(&gColl);
			}

			hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Cyan), &StatBrush);
			hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Lime), &TxtBrush);
			hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gold), &HgltBrush);
			hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Indigo), &InactBrush);

			if (hr != S_OK)
			{
				LogErr(L"Error creating SolidColorBrushes !");
				ErrExit(eD2D);
			}

			bmpField = Load(L".\\res\\img\\field\\field.png", Draw);
			if (!bmpField)
			{
				LogErr(L"Error creating bmpField !");
				ErrExit(eD2D);
			}
			bmpLogo = Load(L".\\res\\img\\field\\logo.png", Draw);
			if (!bmpLogo)
			{
				LogErr(L"Error creating bmpField !");
				ErrExit(eD2D);
			}
			bmpRock = Load(L".\\res\\img\\field\\rock.png", Draw);
			if (!bmpRock)
			{
				LogErr(L"Error creating bmpRock !");
				ErrExit(eD2D);
			}
			bmpWall = Load(L".\\res\\img\\field\\wall.png", Draw);
			if (!bmpWall)
			{
				LogErr(L"Error creating bmpWall !");
				ErrExit(eD2D);
			}
			bmpSmallTree = Load(L".\\res\\img\\field\\SmallTree.png", Draw);
			if (!bmpSmallTree)
			{
				LogErr(L"Error creating bmpSmallTree !");
				ErrExit(eD2D);
			}
			bmpMidTree = Load(L".\\res\\img\\field\\MidTree.png", Draw);
			if (!bmpMidTree)
			{
				LogErr(L"Error creating bmpMidTree !");
				ErrExit(eD2D);
			}
			bmpBigTree = Load(L".\\res\\img\\field\\BigTree.png", Draw);
			if (!bmpBigTree)
			{
				LogErr(L"Error creating bmpBigTree !");
				ErrExit(eD2D);
			}

			bmpArrowUpL = Load(L".\\res\\img\\shots\\arrow\\left\\up.png", Draw);
			if (!bmpArrowUpL)
			{
				LogErr(L"Error creating bmpArrowUpL !");
				ErrExit(eD2D);
			}
			bmpArrowDownL = Load(L".\\res\\img\\shots\\arrow\\left\\down.png", Draw);
			if (!bmpArrowDownL)
			{
				LogErr(L"Error creating bmpArrowDownL !");
				ErrExit(eD2D);
			}

			bmpArrowUpR = Load(L".\\res\\img\\shots\\arrow\\right\\up.png", Draw);
			if (!bmpArrowUpR)
			{
				LogErr(L"Error creating bmpArrowUpR !");
				ErrExit(eD2D);
			}
			bmpArrowDownR = Load(L".\\res\\img\\shots\\arrow\\right\\down.png", Draw);
			if (!bmpArrowDownR)
			{
				LogErr(L"Error creating bmpArrowDownR !");
				ErrExit(eD2D);
			}
			bmpCannonBall = Load(L".\\res\\img\\shots\\cannonball.png", Draw);
			if (!bmpCannonBall)
			{
				LogErr(L"Error creating bmpCannonBall !");
				ErrExit(eD2D);
			}

			for (int i = 0; i < 64; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\shots\\spell\\0" };
				wchar_t add[5]{ L"\0" };

				if (i < 10)wcscat_s(name, L"0");
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpSpell[i] = Load(name, Draw);
				if (!bmpSpell[i])
				{
					LogErr(L"Error creating bmpSpell !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 8; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\field\\intro\\" };
				wchar_t add[5]{ L"\0" };

				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpIntro[i] = Load(name, Draw);
				if (!bmpIntro[i])
				{
					LogErr(L"Error creating bmpIntro !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 24; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\field\\explosion\\" };
				wchar_t add[5]{ L"\0" };

				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpExplosion[i] = Load(name, Draw);
				if (!bmpExplosion[i])
				{
					LogErr(L"Error creating bmpExplosion !");
					ErrExit(eD2D);
				}
			}

			for (int i = 0; i < 5; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\buildings\\castle\\" };
				wchar_t add[5]{ L"\0" };

				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpCastle[i] = Load(name, Draw);
				if (!bmpCastle[i])
				{
					LogErr(L"Error creating bmpCastle !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 8; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\buildings\\towers\\archer\\" };
				wchar_t add[5]{ L"\0" };

				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpArchTower[i] = Load(name, Draw);
				if (!bmpArchTower[i])
				{
					LogErr(L"Error creating bmpArchTower !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 58; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\buildings\\towers\\cannon\\small\\0" };
				wchar_t add[5]{ L"\0" };

				if (i < 10)wcscat_s(name, L"0");
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpCannonSmall[i] = Load(name, Draw);
				if (!bmpCannonSmall[i])
				{
					LogErr(L"Error creating bmpCannonSmall !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 10; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\buildings\\towers\\cannon\\mid\\0" };
				wchar_t add[5]{ L"\0" };

				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpCannonMid[i] = Load(name, Draw);
				if (!bmpCannonMid[i])
				{
					LogErr(L"Error creating bmpCannonMid !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 91; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\buildings\\towers\\cannon\\big\\0" };
				wchar_t add[5]{ L"\0" };

				if (i < 10)wcscat_s(name, L"0");
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpCannonBig[i] = Load(name, Draw);
				if (!bmpCannonBig[i])
				{
					LogErr(L"Error creating bmpCannonBig !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 37; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\buildings\\towers\\mage\\small\\0" };
				wchar_t add[5]{ L"\0" };

				if (i < 10)wcscat_s(name, L"0");
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpMageSmall[i] = Load(name, Draw);
				if (!bmpMageSmall[i])
				{
					LogErr(L"Error creating bmpMageSmall !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 35; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\buildings\\towers\\mage\\mid\\0" };
				wchar_t add[5]{ L"\0" };

				if (i < 10)wcscat_s(name, L"0");
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpMageMid[i] = Load(name, Draw);
				if (!bmpMageMid[i])
				{
					LogErr(L"Error creating bmpMageMid !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 98; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\buildings\\towers\\mage\\big\\0" };
				wchar_t add[5]{ L"\0" };

				if (i < 10)wcscat_s(name, L"0");
				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpMageBig[i] = Load(name, Draw);
				if (!bmpMageBig[i])
				{
					LogErr(L"Error creating bmpMageBig !");
					ErrExit(eD2D);
				}
			}

			for (int i = 0; i < 8; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\orcs\\1\\l\\" };
				wchar_t add[5]{ L"\0" };

				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpOrc1L[i] = Load(name, Draw);
				if (!bmpOrc1L[i])
				{
					LogErr(L"Error creating bmpOrc1L !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 8; ++i)
			{
				wchar_t name[100]{ L".\\res\\img\\orcs\\1\\r\\" };
				wchar_t add[5]{ L"\0" };

				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpOrc1R[i] = Load(name, Draw);
				if (!bmpOrc1R[i])
				{
					LogErr(L"Error creating bmpOrc1R !");
					ErrExit(eD2D);
				}
			}



		}



	}









}

/// /////////////////////////////////////////////////////

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	bIns = hInstance;
	if (!bIns)
	{
		LogErr(L"Error obtaining hInstance from Windows !");
		ErrExit(eClass);
	}

	CreateResources();












	std::remove(tmp_file);
	ClearResources();

    return (int) bMsg.wParam;
}