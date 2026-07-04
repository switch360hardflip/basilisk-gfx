#ifdef _WIN32
#include <windows.h>
#include <winuser.h>
#include <dwmapi.h>

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan.h>
#endif

#include <time.h>

#include <basilisk-core.h>
#include <bs_internal.h>
#include <uxtheme.h>
#include <vsstyle.h>
#include <vssym32.h>
#include <uiribbon.h>

bs_Window* bs_wnd() {
	return &_bs_wnd;
}

bs_IO* bs_io() {
	return &_bs_io;
}

int bs_timeZoneBias() {
	TIME_ZONE_INFORMATION info = { 0 };

	DWORD time_zone_id = 0;
	if ((time_zone_id = GetTimeZoneInformation(&info)) == TIME_ZONE_ID_INVALID)
		bs_throwLastWin32Error(NULL);

	return info.Bias / 60;
}

bs_I64 bs_totalSeconds(const bs_DateTime* date_time) {
	return (bs_I64)mktime(&(struct tm) {
		.tm_year = date_time->years - 1900,
		.tm_mon = date_time->months - 1,
		.tm_mday = date_time->days,
		.tm_hour = date_time->hours,
		.tm_min = date_time->minutes,
		.tm_sec = date_time->seconds,
	});
}

bs_DateTime bs_dateTime() {
	SYSTEMTIME time;
	GetSystemTime(&time);
	return (bs_DateTime) {
		.years = time.wYear,
		.months = time.wMonth,
		.days = time.wDay,
		.hours = time.wHour,
		.minutes = time.wMinute,
		.seconds = time.wSecond,
		.milliseconds = time.wMilliseconds,
		.day_of_week = time.wDayOfWeek
	};
}

bool bs_isLaterThan(const bs_DateTime* a, const bs_DateTime* b) {
	return bs_totalSeconds(a) - bs_totalSeconds(b) > 0;
}

void bs_setCursor(bs_CursorIcon icon) {
	if (_bs_wnd.cursor_icons[icon].handle == NULL)
		_bs_wnd.cursor_icons[icon].handle = LoadCursor(NULL, _bs_wnd.cursor_icons[icon].id);

	if (_bs_wnd.cursor_icon == icon)
		return;

	_bs_wnd.cursor_icon = icon;
}

void bs_resize(bs_U32 width, bs_U32 height) {
//	bsi_resizeObjects();
}

void bs_maximize() {
	ShowWindow(_bs_wnd.hwnd, SW_SHOWMAXIMIZED);
}

void bs_minimize() {
	ShowWindow(_bs_wnd.hwnd, SW_SHOWMINIMIZED);
}

void bs_exit() {
	_bs_instance->alive = false;
}

void bs_pause() {
	_bs_instance->paused = !_bs_instance->paused;
}

void bs_advance() {
	if (!_bs_instance->paused)
		bs_throwBasiliskF(BSXI_INTERNAL | BSX_GENERAL, "Application must be paused to advance a frame");
	_bs_instance->advance = true;
}

double bs_deltaTime() {
//#ifdef _DEBUG
//	if (bs_wnd.delta_time == 0.0)
//		bs_throwBasiliskF(BSX_GENERAL, "Delta time is 0.0"); // some bug is ruining my life
//#endif
	return _bs_wnd.delta_time;
}

double bs_elapsedTime() {
	return _bs_wnd.time;
}

bs_ivec2 bs_queryResolution() {
	return _bs_wnd.dimensions;
	RECT rectangle = { 0 };
	GetWindowRect(_bs_wnd.hwnd, &rectangle);
	return bs_iv2(rectangle.right - rectangle.left, rectangle.bottom - rectangle.top);
}

bs_ivec2 bs_resolution() {
	return _bs_swapchain_->image->image->dim;
}

bs_vec2 bs_cursorPosition() {
	bs_vec2 dim = bs_v2(_bs_swapchain_->image->image->dim.x, _bs_swapchain_->image->image->dim.y);
	bs_vec2 pos = bs_v2Div(_bs_wnd.cursor, dim);
	return bs_v2(pos.x, 1.0 - pos.y);
}

bs_ivec2 bs_getWindowPosition() {
	RECT rectangle = { 0 };
	GetWindowRect(_bs_wnd.hwnd, &rectangle);

	int screen_height = GetSystemMetrics(SM_CYSCREEN);

	return bs_iv2(
		rectangle.left,
		rectangle.bottom
	);
}

bs_vec2 bs_screenCursorPosition() {
	return _bs_wnd.screen_cursor;
}

void bs_setCursorPosition(int x, int y) {
	SetCursorPos(x, y);
}

bool bs_windowIsActive() {
	return _bs_wnd.active;
}

void bs_lockCursorPosition(bool value) {
	_bs_wnd.lock_cursor_position = value;
}

// TODO: could probably done without an additional boolean
void bs_disableUserInputs(bool value) {
	_bs_io.disable_inputs = value;
}

bool bs_keyHeld(bs_U32 code) {
	return !_bs_io.disable_inputs && (code > BS_KEYS_COUNT ? false : BS_GET_BIT(_bs_io.hold_keys, code));
}

bool bs_keyDown(bs_U32 code) {
	return !_bs_io.disable_inputs && (code > BS_KEYS_COUNT ? false : BS_GET_BIT(_bs_io.keys, code));
}

bool bs_keyDownOnce(bs_U32 code) {
	return !_bs_io.disable_inputs && (code > BS_KEYS_COUNT ? false : (BS_GET_BIT(_bs_io.keys, code) && !BS_GET_BIT(_bs_io.keys_old, code)));
}

bool bs_keyUpOnce(bs_U32 code) {
	return !_bs_io.disable_inputs && (code > BS_KEYS_COUNT ? false : (!BS_GET_BIT(_bs_io.keys, code) && BS_GET_BIT(_bs_io.keys_old, code)));
}

bool bs_charDown(unsigned char code) {
	return !_bs_io.disable_inputs && BS_GET_BIT(_bs_io.chars, code);
}

bool bs_charDownOnce(unsigned char code) {
	return !_bs_io.disable_inputs && (code > BS_KEY_BYTES_COUNT ? false : (BS_GET_BIT(_bs_io.chars, code) && !BS_GET_BIT(_bs_io.chars_old, code)));
}

bool bs_charUpOnce(unsigned char code) {
	return !_bs_io.disable_inputs && (code > BS_KEY_BYTES_COUNT ? false : (!BS_GET_BIT(_bs_io.chars, code) && BS_GET_BIT(_bs_io.chars_old, code)));
}

bool bs_middleClick() { return !_bs_io.disable_inputs && _bs_io.middle_clicked; }
bool bs_middleClickOnce() { return !_bs_io.disable_inputs && _bs_io.middle_clicked && !_bs_io.middle_clicked_last; }
bool bs_middleClickUpOnce() { return !_bs_io.disable_inputs && !_bs_io.middle_clicked && _bs_io.middle_clicked_last; }
bool bs_leftClick() { return !_bs_io.disable_inputs && _bs_io.left_clicked; }
bool bs_rightClick() { return !_bs_io.disable_inputs && _bs_io.right_clicked; }
bool bs_rightClickOnce() { return !_bs_io.disable_inputs && _bs_io.right_clicked && !_bs_io.right_clicked_last; }
bool bs_leftClickOnce() { return !_bs_io.disable_inputs && _bs_io.left_clicked && !_bs_io.left_clicked_last; }
bool bs_rightClickUpOnce() { return !_bs_io.disable_inputs && !_bs_io.right_clicked && _bs_io.right_clicked_last; }
bool bs_leftClickUpOnce() { return !_bs_io.disable_inputs && !_bs_io.left_clicked && _bs_io.left_clicked_last; }

int bs_scroll() {
	return _bs_io.scroll;
}

bs_Timer bs_timer() {
	bs_Timer timer = { 0 };
	QueryPerformanceFrequency(&timer.ticks_per_second);
	QueryPerformanceCounter(&timer.last_tick_count);
	return timer;
}

void bs_checkTimer(bs_Timer* timer) {
	QueryPerformanceCounter(&timer->tick_count);
	bs_U64 elapsed_ticks = timer->tick_count.quad_part - timer->last_tick_count.quad_part;
	timer->microseconds = (elapsed_ticks * 1000000) / timer->ticks_per_second.quad_part;
	timer->seconds = (double)timer->microseconds / 1000000.0;
}

void bs_title(const char* title, ...) {
	_bs_wnd.title = title; // todo
}

bool bs_inFixedTick() {
	return _bs_wnd.in_fixed;
}

void bs_tick(bs_Callback tick, bs_Callback fixed_tick) {
    double target_fps = 60.0;
    double target_frame_time = 1.0 / target_fps;

    double dt = 0.0;

    int new_time_index = 0;
    int last_fixed_update_times[2] = { 0 };

    bs_Timer timer = bs_timer();

    HDC hdc = GetDC(_bs_wnd.hwnd);
    MSG msg;
    _bs_wnd.time_old = 0.0;
    double last_fixed_time = _bs_wnd.time_old;
    double current_time = 0.0;

    _bs_instance->alive = true;

    while (_bs_instance->alive) {
        bs_checkTimer(&timer);
        double frame_start = timer.seconds;

        _bs_io.scroll = 0;

        memset(_bs_io.chars, 0, sizeof(_bs_io.chars));

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			switch (msg.message) {
			case WM_QUIT: PostQuitMessage(0); 
				_bs_instance->alive = false;
				goto end;

			case WM_LBUTTONDOWN: _bs_io.left_clicked = true; break;
			case WM_LBUTTONUP: _bs_io.left_clicked = false; break;

			case WM_RBUTTONDOWN: _bs_io.right_clicked = true; break;
			case WM_RBUTTONUP: _bs_io.right_clicked = false; break;

			case WM_MBUTTONDOWN: _bs_io.middle_clicked = true; break;
			case WM_MBUTTONUP: _bs_io.middle_clicked = false; break;
			case WM_MOUSEWHEEL: {
				_bs_io.scroll = (SHORT)HIWORD(msg.wParam) / 120.0;
			} break;
			case WM_CHAR: {
				if (msg.wParam < 256)
					BS_SET_BIT(_bs_io.chars, (bs_U32)msg.wParam);
			} break;
			case WM_KEYDOWN: {
				if (msg.wParam < 256) {
					BS_SET_BIT(_bs_io.keys, (bs_U32)msg.wParam);
					BS_SET_BIT(_bs_io.hold_keys, (bs_U32)msg.wParam);
				}
			} break;
			case WM_KEYUP: {
				if (msg.wParam < 256)
					BS_CLEAR_BIT(_bs_io.keys, (bs_U32)msg.wParam);
			} break;
			case WM_SYSKEYDOWN: {
				if (msg.wParam < 256)
					BS_SET_BIT(_bs_io.keys, (bs_U32)msg.wParam);
			} break;
			case WM_SYSKEYUP: {
				if (msg.wParam < 256)
					BS_CLEAR_BIT(_bs_io.keys, (bs_U32)msg.wParam);
			} break;
			}
		}

		if (BS_GET_BIT(_bs_io.keys, BS_KEY_ALT) && BS_GET_BIT(_bs_io.keys, BS_KEY_F4))
			bs_exit();

        if (bs_leftClickOnce() || bs_rightClickOnce() || bs_middleClickOnce())
            SetCapture(_bs_wnd.hwnd);
        if (bs_leftClickUpOnce() || bs_rightClickUpOnce() || bs_middleClickUpOnce())
            ReleaseCapture();

        POINT p;
        if (GetCursorPos(&p)) {
            _bs_wnd.screen_cursor = bs_v2(p.x, p.y);
            if (ScreenToClient(_bs_wnd.hwnd, &p))
                _bs_wnd.cursor = bs_v2(p.x, p.y);
        }

        bs_checkTimer(&timer);
        _bs_wnd.time = timer.seconds;
		_bs_wnd.active = _bs_wnd.hwnd == GetForegroundWindow();

        if (fixed_tick) {
            _bs_wnd.in_fixed = true;
            for (int i = 0; _bs_instance->advance || (current_time < _bs_wnd.time && i < 200 && !_bs_instance->paused); i++) {
                new_time_index = !new_time_index;
                last_fixed_update_times[new_time_index] = _bs_wnd.fixed_time;

                _bs_wnd.delta_time = _bs_wnd.fixed_time;
                fixed_tick();
                current_time += _bs_wnd.fixed_time;
                _bs_instance->advance = false;
            }
        }
        float newer_time = last_fixed_update_times[new_time_index];
        float older_time = last_fixed_update_times[!new_time_index];

        if (newer_time != older_time)
            _bs_wnd.fixed_interpolation =
            (_bs_wnd.time - newer_time) / (newer_time - older_time);
        else
            _bs_wnd.fixed_interpolation = 1.0f;

        _bs_wnd.delta_time = _bs_wnd.time - _bs_wnd.time_old;
        _bs_wnd.in_fixed = false;

        tick();

		if (_bs_wnd.active && !_bs_io.disable_inputs && _bs_wnd.lock_cursor_position) {
			bs_ivec2 window_pos = bs_getWindowPosition();

			float center_x = (int)(window_pos.x + bs_resolution().x * 0.5f);
			float center_y = (int)(window_pos.y - bs_resolution().y * 0.5f);
			bs_setCursorPosition(center_x, center_y);
		}

        _bs_io.scroll_old = _bs_io.scroll;
        _bs_wnd.time_old = _bs_wnd.time;
        _bs_io.left_clicked_last = _bs_io.left_clicked;
        _bs_io.right_clicked_last = _bs_io.right_clicked;
        _bs_io.middle_clicked_last = _bs_io.middle_clicked;
        memset(_bs_io.hold_keys, 0, sizeof(_bs_io.hold_keys));
        memcpy(_bs_io.keys_old, _bs_io.keys, sizeof(_bs_io.keys_old));
        memcpy(_bs_io.chars_old, _bs_io.chars, sizeof(_bs_io.chars_old));

        bs_checkTimer(&timer);

        while ((timer.seconds - frame_start) < target_frame_time) {
            Sleep(0);
            bs_checkTimer(&timer);
        }

    end:
        void;
    }

    if (_bs_wnd.destroy)
        _bs_wnd.destroy();
}

LRESULT CALLBACK bs_windowProcedure(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
    switch (msg) {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_SYSCHAR:
		return 0;
	case WM_SETCURSOR: {
		SetCursor(_bs_wnd.cursor_icons[_bs_wnd.cursor_icon].handle);
	} break;
	default: return DefWindowProc(hwnd, msg, w_param, l_param);
    }
    return 0;
}

void bs_moveWindow(int x, int y) {
	bs_ivec2 resolution = bs_resolution();
	SetWindowPos(_bs_wnd.hwnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
}

void bs_window(bs_U32 width, bs_U32 height, const char* title, ...) {
	_bs_instance = bs_calloc(1, sizeof(bs_Instance));
	_bs_wnd.title = title;
	_bs_wnd.dimensions = bs_iv2(width, height);

    const char class_name[] = "class";
    HINSTANCE hinstance = GetModuleHandle(0);
    WNDCLASSEX wc = {
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_OWNDC,
        .lpfnWndProc = bs_windowProcedure,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = hinstance,
        .hIcon = LoadIcon(NULL, IDI_APPLICATION),
        .hCursor = LoadCursor(NULL, IDC_ARROW),
		.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
		.lpszMenuName = NULL,
        .lpszClassName = class_name,
        .hIconSm = LoadIcon(NULL, IDI_APPLICATION),
    };

    if (!RegisterClassEx(&wc))
        bs_throwLastWin32Error(NULL);

	_bs_wnd.hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
	//	WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,
        class_name,
        title,
		WS_OVERLAPPEDWINDOW,
	//	WS_POPUP |WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, hinstance, NULL);

    if (!_bs_wnd.hwnd)
        bs_throwLastWin32Error(NULL);

    PIXELFORMATDESCRIPTOR pixel_format_descriptor = {
        .nSize = sizeof(PIXELFORMATDESCRIPTOR),
        .nVersion = 1,
        .dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER,
        .iPixelType = PFD_TYPE_RGBA,
        .cColorBits = 32,
        .cRedBits = 0, .cRedShift = 0, .cGreenBits = 0, .cGreenShift = 0, .cBlueBits = 0, .cBlueShift = 0,
        .cAlphaBits = 0,
        .cAlphaShift = 0,
        .cAccumBits = 0,
        .cAccumRedBits = 0, .cAccumGreenBits = 0, .cAccumBlueBits = 0, .cAccumAlphaBits = 0,
        .cDepthBits = 24,
        .cStencilBits = 8,
        .cAuxBuffers = 0,
        .iLayerType = PFD_MAIN_PLANE,
        .dwLayerMask = 0, .dwVisibleMask = 0, .dwDamageMask = 0
    };
 
     HDC hdc = GetDC(_bs_wnd.hwnd);
     int pixel_format = ChoosePixelFormat(hdc, &pixel_format_descriptor);
	 SetPixelFormat(hdc, pixel_format, &pixel_format_descriptor);
//	 SetWindowLong(bs_wnd.hwnd, GWL_STYLE, 0);
//
//	 COLORREF DARK_COLOR = 0x00000000;
//	 BOOL SET_CAPTION_COLOR = SUCCEEDED(DwmSetWindowAttribute(
//		 bs_wnd.hwnd, DWMWA_CAPTION_COLOR,
//		 &DARK_COLOR, sizeof(DARK_COLOR)));
	 //SetWindowLong(bs_wnd.hwnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW);

		//SetWindowPos(bs_wnd.hwnd, 0, 0, 0, width, height, SWP_FRAMECHANGED); //some trick to redraw window ShowWindow(hwnd, SW_SHOW);

	// DWM_WINDOW_CORNER_PREFERENCE preference = DWMWCP_ROUND;
	//
	// DwmSetWindowAttribute(
	//	 bs_wnd.hwnd,
	//	 DWMWA_WINDOW_CORNER_PREFERENCE,
	//	 &preference,
	//	 sizeof(preference)
	// );

     ShowWindow(_bs_wnd.hwnd, SW_SHOW);
     UpdateWindow(_bs_wnd.hwnd);

	 bs_setCursor(BS_CURSOR_DEFAULT);
 }
