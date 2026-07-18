
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

#include <basilisk-core.h>
#include <bs_internal.h>

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32 // lol
#include <windows.h>
#include <DbgHelp.h>
#endif
#include <vulkan.h>

#define BS_SECTION_HEADER_START BS_PRINT_DARK_GREEN "\n\n\n  ==============================================================================\n    "
#define BS_SECTION_HEADER_END "\n  ==============================================================================\n\n" BS_PRINT_RESET
#define BS_SECTION_END BS_PRINT_DARK_GREEN "\n  ==============================================================================\n\n" BS_PRINT_RESET

 /** TODO: Make this thread safe */

 /** Log section */
BSAPI char* _bs_logSection(char* message, int message_len) {
    int len = _bs_io_.log->len;
    _bs_io_.log = _bs_appendString(_bs_io_.log, BS_SECTION_HEADER_START, sizeof(BS_SECTION_HEADER_START) - 1);
    _bs_io_.log = _bs_appendString(_bs_io_.log, message, message_len);
    _bs_io_.log = _bs_appendString(_bs_io_.log, BS_SECTION_HEADER_END, sizeof(BS_SECTION_HEADER_END) - 1);
    return _bs_io_.log->value + len;
}

BSAPI char* _postval_bs_logSection(char* message, int message_len, char* _return) {
    printf("%s", _return);
    return _return;
}

 /** Log end of section */
BSAPI char* _bs_logEndOfSection() {
    int len = _bs_io_.log->len;
    _bs_io_.log = _bs_appendString(_bs_io_.log, BS_SECTION_END, sizeof(BS_SECTION_END) - 1);
    return _bs_io_.log->value + len;
}

BSAPI char* _postval_bs_logEndOfSection(char* _return) {
    printf("%s", _return);
    return _return;
}


 /** Log with timestamp */
BSAPI char* _bs_logWithTimestamp(const char* type, int type_len, char* message, int message_len) {
    bs_DateTime dt = _bs_dateTime();

    int len = _bs_io_.log->len;
    _bs_io_.log = _bs_appendStringF(_bs_io_.log, "[%02d-%02d-%02d %02d:%02d:%02d %04d] ", dt.years, dt.months, dt.days, dt.hours, dt.minutes, dt.seconds, dt.milliseconds);
    _bs_io_.log = _bs_appendString(_bs_io_.log, type, type_len);
    _bs_io_.log = _bs_appendString(_bs_io_.log, message, message_len);

    return _bs_io_.log->value + len;
}

BSAPI char* _postval_bs_logWithTimestamp(const char* type, int type_len, const char* format, va_list args, char* _return) {
    printf("%s", _return);
    return _return;
}

BSAPI char* _bs_log(char* message, int message_len) {
    int len = _bs_io_.log->len;
    _bs_io_.log = _bs_appendString(_bs_io_.log, message, message_len);
    return _bs_io_.log->value + len;
}

/** Info log */
BSAPI char* _bs_info(char* message, int message_len) {
    return _bs_logWithTimestamp("", 0, message, message_len);
}

 /** Warning log */
BSAPI char* _bs_warn(char* message, int message_len) {
    return _bs_logWithTimestamp(BS_WARN_HEADER, sizeof(BS_WARN_HEADER) - 1, message, message_len);
}

 /** Critical error log */
BSAPI void _bs_critical(char* message, int message_len) {
    char* result = _bs_logWithTimestamp(BS_ERROR_HEADER, sizeof(BS_ERROR_HEADER) - 1, message, message_len);

    _bs_saveFile(result, strlen(result), BS_CONSTANT_STRING("basilisk.log"));

    /*
    if (_bs_args_.send_bugs)
    {
        int report_bug = MessageBox(
            _bs_wnd.hwnd,
            "The application has crashed :(\nWould you like to create a bug report?",
            _bs_wnd.title,
            MB_ICONERROR | MB_YESNO
        );

        if (report_bug == IDYES) {
            _bs_except(BS_U64_MAX);
            bs_Json json = _bs_emptyJson();
            _bs_ensureJson(&json, _bs_jsonValue(_bs_io_.log->value), "$.Log");
            char* result = _bs_saveJson(&json, 0);

            bs_Json response = _bs_post("http://basilisk-gfx.com/reportbug", 5000, &json);
            char* raw = _bs_saveJson(&response, BS_JSON_PRETTY);

            printf("Bug report response:\n%s\n", raw);

            _bs_destroyJson(&response);
            _bs_free(raw);
            _bs_except(0);
        }
    }

    if (_bs_wnd.destroy)
        _bs_wnd.destroy();
    */

    exit(0);
}

BSAPI const char* _bs_serializeErrno() {
    switch (errno) {
        case  1: return "Operation not permitted";
        case  2: return "No such file or directory";
        case  3: return "No such process";
        case  4: return "Interrupted system call";
        case  5: return "I/O error";
        case  6: return "No such device or address";
        case  7: return "Argument list too long";
        case  8: return "Exec format error";
        case  9: return "Bad file number";
        case 10: return "No child processes";
        case 11: return "Try again";
        case 12: return "Out of memory";
        case 13: return "Permission denied";
        case 14: return "Bad address";
        case 15: return "Block device required";
        case 16: return "Device or resource busy";
        case 17: return "File exists";
        case 18: return "Cross-device link";
        case 19: return "No such device";
        case 20: return "Not a directory";
        case 21: return "Is a directory";
        case 22: return "Invalid argument";
        case 23: return "File table overflow";
        case 24: return "Too many open files";
        case 25: return "Not a typewriter";
        case 26: return "Text file busy";
        case 27: return "File too large";
        case 28: return "No space left on device";
        case 29: return "Illegal seek";
        case 30: return "Read-only file system";
        case 31: return "Too many links";
        case 32: return "Broken pipe";
        case 33: return "Math argument out of domain of func";
        case 34: return "Math result not representable";
    }

    return "Unknown";
}

BSAPI bs_Result _bs_convertVulkanResult(int result) {
    switch (result) {
        case VK_SUCCESS: return BS_RESULT_OK;
    }

    return BS_RESULT_GENERAL_ERROR;
}

BSAPI bs_Result _bs_convertErrno() {
    switch (errno) {
    }

    return BS_RESULT_GENERAL_ERROR;
}

#ifdef _WIN32 
BSAPI bs_Result _bs_convertWin32Error(int code) {
    switch (code) {
        case 0: return BS_RESULT_OK;
    }

    return BS_RESULT_GENERAL_ERROR;
}

BSAPI const char* _bs_serializeWin32Error(int code) {
    switch (code) {
    }

    return "Unknown";
}

BSAPI bs_Result _bs_convertHResult(int code) {
    switch (code) {
        case 0: return BS_RESULT_OK;
    }

    return BS_RESULT_GENERAL_ERROR;
}

#endif