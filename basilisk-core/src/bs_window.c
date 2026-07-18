
 /**
  MIT License
  
  Copyright (c) 2026 switch360hardflip <switch360hardflip@gmail.com>
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
  */ 

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



  /*==============================================================================
   * Swapchain
   *============================================================================*/

static bs_Result _bs_querySwapchainFormat(bs_Window* window, VkFormat candidates[], int candidates_count) {
    bs_U32 num_formats = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(_bs_instance_->physical_device, window->surface, &num_formats, NULL);
    VkSurfaceFormatKHR* formats = _bs_malloc(num_formats * sizeof(VkSurfaceFormatKHR));
    vkGetPhysicalDeviceSurfaceFormatsKHR(_bs_instance_->physical_device, window->surface, &num_formats, formats);

    for (int i = 0; i < candidates_count; i++) {
        VkFormat candidate = candidates[i];

        for (int j = 0; j < num_formats; j++) {
            if (candidate == formats[j].format) {
                VkSurfaceFormatKHR result = formats[j];
                memcpy(&window->surface_format, &result, sizeof(window->surface_format));
                _bs_free(formats);
                return BS_RESULT_OK;
            }
        }
    }

    _bs_free(formats);
    for (int i = 0; i < num_formats; i++)
        _bs_infoF("%d\n", formats[i].format);

    _bs_warn(BS_CONSTANT_STRING("Failed to query swapchain surface format, available formats\n"));

    return BS_RESULT_FAILED_TO_QUERY;
}

static bs_Result _bs_querySwapchainMode(bs_Window* window, VkPresentModeKHR candidates[], int candidates_count) {
    bs_U32 num_modes = 0;
    VkPresentModeKHR result = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(_bs_instance_->physical_device, window->surface, &num_modes, NULL);
    VkPresentModeKHR* modes = _bs_malloc(num_modes * sizeof(VkPresentModeKHR));
    vkGetPhysicalDeviceSurfacePresentModesKHR(_bs_instance_->physical_device, window->surface, &num_modes, modes);

    for (int i = 0; i < candidates_count; i++) {
        VkPresentModeKHR candidate = candidates[i];

        for (int j = 0; j < num_modes; j++) {
            VkPresentModeKHR mode = modes[j];
            if (candidate == mode) {
                _bs_free(modes);
                window->present_mode = mode;
                return BS_RESULT_OK;
            }
        }
    }

    _bs_warn(BS_CONSTANT_STRING("Failed to query swapchain present mode, available modes: ("));
    for (int i = 0; i < num_modes; i++) {
        VkPresentModeKHR mode = modes[i];
        _bs_logF("%d");
        if (i != (num_modes - 1))
            _bs_log(BS_CONSTANT_STRING(","));
    }
    _bs_log(BS_CONSTANT_STRING(")\n"));

    _bs_free(modes);

    return BS_RESULT_FAILED_TO_QUERY;
}

static void _bs_swapchain(bs_Window* window) {
    VkResult result;

    const VkPresentModeKHR modes[] = {
        VK_PRESENT_MODE_IMMEDIATE_KHR,
        VK_PRESENT_MODE_FIFO_KHR,
        VK_PRESENT_MODE_MAILBOX_KHR,
        VK_PRESENT_MODE_FIFO_RELAXED_KHR,
    };

    const VkFormat formats[] = {
        VK_FORMAT_R8G8B8A8_UNORM,
        VK_FORMAT_B8G8R8A8_UNORM,
        VK_FORMAT_R8G8B8A8_SRGB,
        VK_FORMAT_B8G8R8A8_SRGB,
    };

    if (_bs_querySwapchainMode(window, modes, sizeof(modes) / sizeof(*modes)) != BS_RESULT_OK) {
        return;
    }

    if (_bs_querySwapchainFormat(window, formats, sizeof(formats) / sizeof(*formats)) != BS_RESULT_OK) {
        return;
    }

    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_bs_instance_->physical_device, window->surface, &capabilities);

    const bool same_family = true; // TODO: this shouldn't always be true

    window->color_space = window->surface_format.color_space;
    window->swapchain_image = window->swapchain_image ? window->swapchain_image : NULL;

    bs_ivec2 resolution = { capabilities.minImageExtent.width, capabilities.minImageExtent.height };
    bs_Image image = {
        .flags = BS_IMAGE_SWAPS_BIT,
        .format = window->surface_format.format,
        .dim = resolution
    };

    const int frames_in_flight_target = 2;
    const int frames_in_flight_max = 3;

    window->frames_in_flight = _bs_clamp(frames_in_flight_target, capabilities.minImageCount, frames_in_flight_max);

    VkSwapchainCreateInfoKHR swapchain_ci = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = window->surface,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .minImageCount = window->frames_in_flight,
        .imageExtent = { resolution.x, resolution.y },
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = same_family ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT,
        .queueFamilyIndexCount = same_family ? 0 : 2,
        .pQueueFamilyIndices = same_family ? NULL : NULL,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .clipped = VK_TRUE,
        .preTransform = capabilities.currentTransform,
        .presentMode = window->present_mode,
        .imageFormat = (VkFormat)window->surface_format.format,
        .imageColorSpace = window->surface_format.color_space,
    };

    result = vkCreateSwapchainKHR(_bs_instance_->device, &swapchain_ci, NULL, &window->swapchain);
    if (result != VK_SUCCESS) {
        _bs_warnF("Failed to create swapchain for window \"%s\"\n", window->title);
        return;
    }

    /**
     Swapchain images
     */
    VkImage images[3];
    vkGetSwapchainImagesKHR(_bs_instance_->device, window->swapchain, &window->frames_in_flight, images);
    _bs_infoF("Swapchain\n  Format: %d\n  Mode: %d\n  Images: %d\n", swapchain_ci.imageFormat, swapchain_ci.presentMode, window->frames_in_flight);

    if (window->swapchain_image == NULL)
        window->swapchain_image = BS_OBJECT(bs_Image, -1, 0, window->frames_in_flight, BS_OBJECT_HAS_SWAPS_BIT);

    memcpy(window->swapchain_image->image, &image, sizeof(image));

    /**
     Swapchain image views
     */
    for (int i = 0; i < window->frames_in_flight; i++) {
        window->swapchain_image->image->_[i].vk_image = images[i];

        VkImageViewCreateInfo image_view_ci = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = window->swapchain_image->image->_[i].vk_image,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = (VkFormat)window->surface_format.format,
            .subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .subresourceRange.levelCount = 1,
            .subresourceRange.layerCount = 1,
        };

        result = vkCreateImageView(_bs_instance_->device, &image_view_ci, NULL, &window->swapchain_image->image->_[i].vk_image_view);
        if (result != VK_SUCCESS) {
            _bs_warnF("Failed to create swapchain image view for window \"%s\"\n", window->title);
        }
    }

    /**
     Swapchain semaphores
     */
    VkSemaphoreCreateInfo semaphore_ci = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    };

    for (int i = 0; i < window->frames_in_flight; i++) {
        result = vkCreateSemaphore(_bs_instance_->device, &semaphore_ci, NULL, &window->_[i].semaphore);
        if (result != VK_SUCCESS) {
            _bs_warnF("Failed to create swapchain semaphore for window \"%s\"\n", window->title);
        }
    }
}



  /*==============================================================================
   * Window
   *============================================================================*/

BSAPI bs_Result _bs_timeZoneBias(int* out) {
#ifdef _WIN32
	TIME_ZONE_INFORMATION info = { 0 };

	DWORD time_zone_id = 0;
	if ((time_zone_id = GetTimeZoneInformation(&info)) == TIME_ZONE_ID_INVALID) {
		_bs_warnF("GetTimeZoneInformation failed (GetLastError() = %d)\n", GetLastError());
		return _bs_convertWin32Error(GetLastError());
	}

	*out = info.Bias / 60;

	return BS_RESULT_OK;
#else
	return BS_RESULT_NOT_SUPPORTED;
#endif
}

BSAPI bs_I64 _bs_totalSeconds(const bs_DateTime* date_time) {
	return (bs_I64)mktime(&(struct tm) {
		.tm_year = date_time->years - 1900,
		.tm_mon = date_time->months - 1,
		.tm_mday = date_time->days,
		.tm_hour = date_time->hours,
		.tm_min = date_time->minutes,
		.tm_sec = date_time->seconds,
	});
}

BSAPI bs_DateTime _bs_dateTime() {
#ifdef _WIN32
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
#else
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);

    time_t t = ts.tv_sec;
    struct tm tm;

    gmtime_r(&t, &tm);

    return (bs_DateTime) {
        .years = tm.tm_year + 1900,
        .months = tm.tm_mon + 1,
        .days = tm.tm_mday,
        .hours = tm.tm_hour,
        .minutes = tm.tm_min,
        .seconds = tm.tm_sec,
        .milliseconds = (int)(ts.tv_nsec / 1000000),
        .day_of_week = tm.tm_wday
    };
#endif
}

BSAPI bool _bs_isLaterThan(const bs_DateTime* a, const bs_DateTime* b) {
	return _bs_totalSeconds(a) - _bs_totalSeconds(b) > 0;
}

BSAPI void _bs_setCursor(bs_CursorIcon icon) {
	_bs_warnF("_bs_setCursor has not been implemented yet\n");
	/*
	if (window->cursor_icons[icon].handle == NULL)
		window->cursor_icons[icon].handle = LoadCursor(NULL, _bs_wnd.cursor_icons[icon].id);

	if (_bs_wnd.cursor_icon == icon)
		return;

	_bs_wnd.cursor_icon = icon;
	*/
}

BSAPI void _bs_resizeWindow(bs_U32 width, bs_U32 height) {
//	bsi_resizeObjects();
}

BSAPI void _bs_maximizeWindow() {
    bs_Window* window = _bs_scope_.window;

#ifdef _WIN32
	ShowWindow(window->hwnd, SW_SHOWMAXIMIZED);
#else
	_bs_warnF("_bs_maximizeWindow has not been implemented for this OS yet\n");
#endif
}

BSAPI void _bs_minimizeWindow() {
    bs_Window* window = _bs_scope_.window;
#ifdef _WIN32
	ShowWindow(window->hwnd, SW_SHOWMINIMIZED);
#else
	_bs_warnF("_bs_minimizeWindow has not been implemented for this OS yet\n");
#endif
}

BSAPI void _bs_exit() {
    _bs_instance_->alive = false;
}

BSAPI void _bs_pause() {
    bs_Window* window = _bs_scope_.window;
    window->paused = !window->paused;
}

BSAPI void _val_bs_advance() {
    bs_Window* window = _bs_scope_.window;
    BS_VALIDATE(window->paused == true,,);
    _bs_advance(window);
}

BSAPI void _bs_advance() {
    bs_Window* window = _bs_scope_.window;
    window->advance = true;
}

BSAPI double _bs_deltaTime() {
    bs_Window* window = _bs_scope_.window;
//#ifdef _DEBUG
//	if (_bs_wnd.delta_time == 0.0)
//		_bs_throwBasiliskF(BSX_GENERAL, "Delta time is 0.0"); // some bug is ruining my life
//#endif
	return window->delta_time;
}

BSAPI double _bs_elapsedTime() {
    bs_Window* window = _bs_scope_.window;
    return window->time;
}

BSAPI bs_ivec2 _bs_resolution() {
    bs_Window* window = _bs_scope_.window;
    return window->swapchain_image->image->dim;
}

BSAPI bs_vec2 _bs_cursorPosition() {
    bs_Window* window = _bs_scope_.window;

    bs_vec2 dim = { window->swapchain_image->image->dim.x, window->swapchain_image->image->dim.y };
    bs_vec2 pos;

    bs_v2Div(&window->cursor, &dim, &pos);
	return BS_V2(pos.x, 1.0 - pos.y);
}

BSAPI bs_ivec2 _bs_windowPosition() {
    bs_Window* window = _bs_scope_.window;

#ifdef _WIN32
	RECT rectangle = { 0 };
	GetWindowRect(window->hwnd, &rectangle);

	int screen_height = GetSystemMetrics(SM_CYSCREEN);

	return (bs_ivec2) {
		rectangle.left,
		rectangle.bottom
	};

#elif defined(__linux__)
    Window child;
    int x, y;
    XTranslateCoordinates(
        window->display,
        window->window,
        DefaultRootWindow(window->display),
        0, 0,
        &x, &y,
        &child
    );

    return (bs_ivec2) {
        .x = x,
        .y = y
    };

#elif defined(__APPLE__)
    _bs_warnF("_bs_windowPosition has not been implemented for macOS yet\n");
    return (bs_ivec2) { 0, 0 };
#else
    return (bs_ivec2) { 0, 0 };
#endif
}

BSAPI bs_vec2 _bs_screenCursorPosition() {
	return _bs_instance_->screen_cursor;
}

BSAPI void _bs_lockCursorPosition(bool value) {
    bs_Window* window = _bs_scope_.window;
    window->lock_cursor_position = value;
}

// TODO: could probably done without an additional boolean
BSAPI void _bs_disableUserInputs(bool value) {
	_bs_io_.disable_inputs = value;
}

BSAPI bool _bs_keyHeld(bs_U32 code) {
	return !_bs_io_.disable_inputs && (code > BS_KEYS_COUNT ? false : BS_GET_BIT(_bs_io_.hold_keys, code));
}

BSAPI bool _bs_keyDown(bs_U32 code) {
	return !_bs_io_.disable_inputs && (code > BS_KEYS_COUNT ? false : BS_GET_BIT(_bs_io_.keys, code));
}

BSAPI bool _bs_keyDownOnce(bs_U32 code) {
	return !_bs_io_.disable_inputs && (code > BS_KEYS_COUNT ? false : (BS_GET_BIT(_bs_io_.keys, code) && !BS_GET_BIT(_bs_io_.keys_old, code)));
}

BSAPI bool _bs_keyUpOnce(bs_U32 code) {
	return !_bs_io_.disable_inputs && (code > BS_KEYS_COUNT ? false : (!BS_GET_BIT(_bs_io_.keys, code) && BS_GET_BIT(_bs_io_.keys_old, code)));
}

BSAPI bool _bs_charDown(unsigned char code) {
	return !_bs_io_.disable_inputs && BS_GET_BIT(_bs_io_.chars, code);
}

BSAPI bool _bs_charDownOnce(unsigned char code) {
	return !_bs_io_.disable_inputs && (code > BS_KEY_BYTES_COUNT ? false : (BS_GET_BIT(_bs_io_.chars, code) && !BS_GET_BIT(_bs_io_.chars_old, code)));
}

BSAPI bool _bs_charUpOnce(unsigned char code) {
	return !_bs_io_.disable_inputs && (code > BS_KEY_BYTES_COUNT ? false : (!BS_GET_BIT(_bs_io_.chars, code) && BS_GET_BIT(_bs_io_.chars_old, code)));
}

BSAPI bool _bs_middleClick() { return !_bs_io_.disable_inputs && _bs_io_.middle_clicked; }
BSAPI bool _bs_middleClickOnce() { return !_bs_io_.disable_inputs && _bs_io_.middle_clicked && !_bs_io_.middle_clicked_last; }
BSAPI bool _bs_middleClickUpOnce() { return !_bs_io_.disable_inputs && !_bs_io_.middle_clicked && _bs_io_.middle_clicked_last; }
BSAPI bool _bs_leftClick() { return !_bs_io_.disable_inputs && _bs_io_.left_clicked; }
BSAPI bool _bs_rightClick() { return !_bs_io_.disable_inputs && _bs_io_.right_clicked; }
BSAPI bool _bs_rightClickOnce() { return !_bs_io_.disable_inputs && _bs_io_.right_clicked && !_bs_io_.right_clicked_last; }
BSAPI bool _bs_leftClickOnce() { return !_bs_io_.disable_inputs && _bs_io_.left_clicked && !_bs_io_.left_clicked_last; }
BSAPI bool _bs_rightClickUpOnce() { return !_bs_io_.disable_inputs && !_bs_io_.right_clicked && _bs_io_.right_clicked_last; }
BSAPI bool _bs_leftClickUpOnce() { return !_bs_io_.disable_inputs && !_bs_io_.left_clicked && _bs_io_.left_clicked_last; }

BSAPI int _bs_scroll() {
	return _bs_io_.scroll;
}

BSAPI bs_Timer _bs_timer() {
    bs_Timer timer = { 0 };

#ifdef _WIN32
    QueryPerformanceFrequency(&timer.ticks_per_second);
    QueryPerformanceCounter(&timer.last_tick_count);
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    timer.last_tick_count.quad_part =
        (long long)ts.tv_sec * 1000000000LL +
        (long long)ts.tv_nsec;

    timer.ticks_per_second.quad_part = 1000000000LL;
#endif

    return timer;
}

BSAPI void _bs_checkTimer(bs_Timer* timer) {
#ifdef _WIN32
	QueryPerformanceCounter(&timer->tick_count);
	bs_U64 elapsed_ticks = timer->tick_count.quad_part - timer->last_tick_count.quad_part;
	timer->microseconds = (elapsed_ticks * 1000000) / timer->ticks_per_second.quad_part;

#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    timer->tick_count.quad_part =
        (long long)ts.tv_sec * 1000000000LL +
        (long long)ts.tv_nsec;

    long long elapsed =
        timer->tick_count.quad_part -
        timer->last_tick_count.quad_part;

    timer->microseconds = (bs_U64)(elapsed / 1000LL);
#endif

    timer->seconds = timer->microseconds / 1000000.0;
}

BSAPI void _bs_titleWindow(char* name, int name_length) {
    bs_Window* window = _bs_scope_.window;
	window->title = name; // todo
}

BSAPI bool _bs_inFixedTick() {
    bs_Window* window = _bs_scope_.window;
    return window->in_fixed;
}

BSAPI void _bs_setTargetFramerate(int fps) {
    bs_Window* window = _bs_scope_.window;
    window->target_frame_time = 1.0 / (double)fps;
}

BSAPI void _bs_tickWindow(bs_Window* window, bs_Callback tick, bs_Callback fixed_tick) {
   //  _bs_checkTimer(&window->timer);
    double frame_start = window->timer.seconds;

    _bs_io_.scroll = 0;
    memset(_bs_io_.chars, 0, sizeof(_bs_io_.chars));

#ifdef _WIN32
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        switch (msg.message) {
        case WM_QUIT: PostQuitMessage(0); _bs_instance_->alive = false; return;

        case WM_LBUTTONDOWN: _bs_io_.left_clicked = true; break;
        case WM_LBUTTONUP: _bs_io_.left_clicked = false; break;

        case WM_RBUTTONDOWN: _bs_io_.right_clicked = true; break;
        case WM_RBUTTONUP: _bs_io_.right_clicked = false; break;

        case WM_MBUTTONDOWN: _bs_io_.middle_clicked = true; break;
        case WM_MBUTTONUP: _bs_io_.middle_clicked = false; break;
        case WM_MOUSEWHEEL: {
            _bs_io_.scroll = (SHORT)HIWORD(msg.wParam) / 120.0;
        } break;
        case WM_CHAR: {
            if (msg.wParam < 256)
                BS_SET_BIT(_bs_io_.chars, (bs_U32)msg.wParam);
        } break;
        case WM_KEYDOWN: {
            if (msg.wParam < 256) {
                BS_SET_BIT(_bs_io_.keys, (bs_U32)msg.wParam);
                BS_SET_BIT(_bs_io_.hold_keys, (bs_U32)msg.wParam);
            }
        } break;
        case WM_KEYUP: {
            if (msg.wParam < 256)
                BS_CLEAR_BIT(_bs_io_.keys, (bs_U32)msg.wParam);
        } break;
        case WM_SYSKEYDOWN: {
            if (msg.wParam < 256)
                BS_SET_BIT(_bs_io_.keys, (bs_U32)msg.wParam);
        } break;
        case WM_SYSKEYUP: {
            if (msg.wParam < 256)
                BS_CLEAR_BIT(_bs_io_.keys, (bs_U32)msg.wParam);
        } break;
        }
    }

    if (_bs_leftClickOnce() || _bs_rightClickOnce() || _bs_middleClickOnce())
        SetCapture(window->hwnd);
    if (_bs_leftClickUpOnce() || _bs_rightClickUpOnce() || _bs_middleClickUpOnce())
        ReleaseCapture();

    POINT p;
    if (GetCursorPos(&p)) {
        _bs_instance_->screen_cursor = BS_V2(p.x, p.y);
        if (ScreenToClient(window->hwnd, &p))
            window->cursor = BS_V2(p.x, p.y);
    }

    _bs_checkTimer(&window->timer);
    window->time = window->timer.seconds;
    window->active = window->hwnd == GetForegroundWindow();

#endif

    if (BS_GET_BIT(_bs_io_.keys, BS_KEY_ALT) && BS_GET_BIT(_bs_io_.keys, BS_KEY_F4))
        _bs_exit();

    if (fixed_tick) {
        window->in_fixed = true;
        for (int i = 0; window->advance || (window->elapsed_time < window->time && i < 200 && !window->paused); i++) {
            window->new_time_index = !window->new_time_index;
            window->last_fixed_update_times[window->new_time_index] = window->fixed_time;

            window->delta_time = window->fixed_time;
            fixed_tick();
            window->elapsed_time += window->fixed_time;
            window->advance = false;
        }
    }
    float newer_time = window->last_fixed_update_times[window->new_time_index];
    float older_time = window->last_fixed_update_times[!window->new_time_index];

    if (newer_time != older_time)
        window->fixed_interpolation =
        (window->time - newer_time) / (newer_time - older_time);
    else
        window->fixed_interpolation = 1.0f;

    window->delta_time = window->time - window->time_old;
    window->in_fixed = false;

    tick();

    /*
    if (window->active && !_bs_io_.disable_inputs && window->lock_cursor_position) {
        bs_ivec2 window_pos = _bs_windowPosition(window);

        float center_x = (int)(window_pos.x + _bs_resolution(window).x * 0.5f);
        float center_y = (int)(window_pos.y - _bs_resolution(window).y * 0.5f);
        _bs_setCursorPosition(center_x, center_y);
    }
    */

    _bs_io_.scroll_old = _bs_io_.scroll;
    window->time_old = window->time;
    _bs_io_.left_clicked_last = _bs_io_.left_clicked;
    _bs_io_.right_clicked_last = _bs_io_.right_clicked;
    _bs_io_.middle_clicked_last = _bs_io_.middle_clicked;
    memset(_bs_io_.hold_keys, 0, sizeof(_bs_io_.hold_keys));
    memcpy(_bs_io_.keys_old, _bs_io_.keys, sizeof(_bs_io_.keys_old));
    memcpy(_bs_io_.chars_old, _bs_io_.chars, sizeof(_bs_io_.chars_old));

    _bs_checkTimer(&window->timer);

    while ((window->timer.seconds - frame_start) < window->target_frame_time) {
        Sleep(0);
        _bs_checkTimer(&window->timer);
    }
}

BSAPI void _bs_tick(bs_Window* window, bs_Callback tick, bs_Callback fixed_tick) {
    _bs_instance_->alive = true;

    while (_bs_instance_->alive) {
        tick();
    }
}

LRESULT CALLBACK _bs_windowProcedure(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
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
		// SetCursor(window->cursor_icons[_bs_wnd.cursor_icon].handle);
	} break;
	default: return DefWindowProc(hwnd, msg, w_param, l_param);
    }
    return 0;
}

BSAPI void _bs_moveWindow(int x, int y) {
    bs_Window* window = _bs_scope_.window;
    bs_ivec2 resolution = _bs_resolution(window);
	SetWindowPos(window->hwnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
}

BSAPI bs_Result _bs_window(bs_Object* object, bs_U32 width, bs_U32 height, const char* title) {
    bs_Window* window = object->window;

    window->title = title;
    window->fixed_time = 0.025;
    window->dimensions = (bs_ivec2) { width, height };

    bs_Timer timer = _bs_timer();
    _bs_setTargetFramerate(60);

    const char class_name[] = "class";
    HINSTANCE hinstance = GetModuleHandle(0);
    WNDCLASSEX wc = {
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_OWNDC,
        .lpfnWndProc = _bs_windowProcedure,
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

    if (!RegisterClassEx(&wc)) {
        BS_WARN_WIN32_PATH("RegisterClassEx", title);
        return _bs_convertWin32Error(GetLastError());
    }

    window->hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
	//	WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,
        class_name,
        title,
		WS_OVERLAPPEDWINDOW,
	//	WS_POPUP |WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, hinstance, NULL);

    if (!window->hwnd) {
        BS_WARN_WIN32_PATH("CreateWindowEx", title);
        return _bs_convertWin32Error(GetLastError());
    }

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
 
     HDC hdc = GetDC(window->hwnd);
     int pixel_format = ChoosePixelFormat(hdc, &pixel_format_descriptor);
	 SetPixelFormat(hdc, pixel_format, &pixel_format_descriptor);
//	 SetWindowLong(_bs_wnd.hwnd, GWL_STYLE, 0);
//
//	 COLORREF DARK_COLOR = 0x00000000;
//	 BOOL SET_CAPTION_COLOR = SUCCEEDED(DwmSetWindowAttribute(
//		 _bs_wnd.hwnd, DWMWA_CAPTION_COLOR,
//		 &DARK_COLOR, sizeof(DARK_COLOR)));
	 //SetWindowLong(_bs_wnd.hwnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW);

		//SetWindowPos(_bs_wnd.hwnd, 0, 0, 0, width, height, SWP_FRAMECHANGED); //some trick to redraw window ShowWindow(hwnd, SW_SHOW);

	// DWM_WINDOW_CORNER_PREFERENCE preference = DWMWCP_ROUND;
	//
	// DwmSetWindowAttribute(
	//	 _bs_wnd.hwnd,
	//	 DWMWA_WINDOW_CORNER_PREFERENCE,
	//	 &preference,
	//	 sizeof(preference)
	// );

     ShowWindow(window->hwnd, SW_SHOW);
     UpdateWindow(window->hwnd);

	 _bs_setCursor(BS_CURSOR_DEFAULT);

     return BS_RESULT_OK;
 }
