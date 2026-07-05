#include <basilisk-core.h>
#include <bs_internal.h>

#include <stdarg.h>
#include <string.h>

#ifdef _WIN32 // lol
#include <windows.h>
#include <DbgHelp.h>
#endif
#include <vulkan.h>

 /** Variadic logs */

#define BS_SECTION_HEADER_START BS_PRINT_DARK_GREEN "\n\n\n  ==============================================================================\n    "
#define BS_SECTION_HEADER_END "\n  ==============================================================================\n\n" BS_PRINT_RESET
#define BS_SECTION_END BS_PRINT_DARK_GREEN "\n  ==============================================================================\n\n" BS_PRINT_RESET

char* bs_logSectionV(const char* format, va_list args) {
    if (_bs_args.skip_log)
        return "";

    int len = _bs_io.log->len;
    _bs_io.log = bs_appendString(_bs_io.log, BS_SECTION_HEADER_START, sizeof(BS_SECTION_HEADER_START) - 1);
    _bs_io.log = bs_appendStringV(_bs_io.log, format, args);
    _bs_io.log = bs_appendString(_bs_io.log, BS_SECTION_HEADER_END, sizeof(BS_SECTION_HEADER_END) - 1);

    if (_bs_args.cmd_log)
        printf("%s", _bs_io.log->value + len);

    return _bs_io.log->value;
}

char* bs_logSectionF(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* result = bs_logSectionV(format, args);
    va_end(args);
    return result;
}

char* bs_logEndOfSection() {
    if (_bs_args.cmd_log)
        printf("%s", BS_SECTION_END);
    _bs_io.log = bs_appendString(_bs_io.log, BS_SECTION_END, sizeof(BS_SECTION_END) - 1);
    return _bs_io.log->value;
}

char* bs_logV(const char* format, va_list args) {
    if (_bs_args.skip_log)
        return "";

    bs_DateTime dt = bs_dateTime();
    if (_bs_args.cmd_log) {
        vprintf(format, args);
    }

    _bs_io.log = bs_appendStringV(_bs_io.log, format, args);

    return _bs_io.log->value;
}

char* bs_logWithTimestampV(const char* type, int type_len, const char* format, va_list args) {
    if (_bs_args.skip_log)
        return "";

    bs_DateTime dt = bs_dateTime();

    int len = _bs_io.log ? _bs_io.log->len : 0;
    _bs_io.log = bs_appendStringF(_bs_io.log, "[%02d-%02d-%02d %02d:%02d:%02d %04d] ", dt.years, dt.months, dt.days, dt.hours, dt.minutes, dt.seconds, dt.milliseconds);
    _bs_io.log = bs_appendString(_bs_io.log, type, type_len);
    _bs_io.log = bs_appendStringV(_bs_io.log, format, args);

    if (_bs_args.cmd_log)
        printf("%s", _bs_io.log->value + len);

    return _bs_io.log->value;
}

char* bs_warnV(const char* format, va_list args) {
    return bs_logWithTimestampV(BS_WARN_HEADER, sizeof(BS_WARN_HEADER) - 1, format, args);
}

void bs_throwV(const char* format, va_list args) {
    char* result = bs_logWithTimestampV(BS_ERROR_HEADER, sizeof(BS_ERROR_HEADER) - 1, format, args);

     if (result)
        bs_saveFile("basilisk.log", result, strlen(result));

#ifdef _DEBUG
    if (_bs_args.send_bugs)
#endif
    {
        int report_bug = MessageBox(
            _bs_wnd.hwnd,
            "The application has crashed :(\nWould you like to create a bug report?",
            _bs_wnd.title,
            MB_ICONERROR | MB_YESNO
        );

        if (report_bug == IDYES) {
            bs_except(BS_U64_MAX);
            bs_Json json = bs_emptyJson();
            bs_ensureJson(&json, bs_jsonValue(_bs_io.log->value), "$.Log");
            char* result = bs_saveJson(&json, 0);

            bs_Json response = bs_post("http://basilisk-gfx.com/reportbug", 5000, &json);
            char* raw = bs_saveJson(&response, BS_JSON_PRETTY);

            printf("Bug report response:\n%s\n", raw);

            bs_destroyJson(&response);
            bs_free(raw);
            bs_except(0);
        }
    }

    if (_bs_wnd.destroy)
        _bs_wnd.destroy();

    exit(0);
}

char* bs_logF(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* log = bs_logV(format, args);
    va_end(args);
    return log;
}

char* bs_infoF(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* log = bs_logWithTimestampV(BS_INFO_HEADER, sizeof(BS_INFO_HEADER) - 1, format, args);
    va_end(args);
    return log;
}

void bs_warnF(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* log = bs_warnV(format, args);
    va_end(args);
    return log;
}

void bs_throwF(const char* format, ...) {
    va_list args;
    va_start(args, format);
    bs_throwV(format, args);
    va_end(args);
}

// - common errors -

static inline const char* bs_errnoName() {
    switch (errno) {
    case ENOENT: return "No such file or directory";
    case EACCES: return "Permission denied";
    }
    return "";
}

bs_U64 bs_except(bs_U64 exceptions) {
    bs_U64 caught = _bs_io.caught;
    _bs_io.exceptions = exceptions;
    _bs_io.caught = 0;
    return caught;
}

bool bs_caught() {
    return bs_except(0);
}

const char* bs_exceptionName(bs_U64 code) {
    switch (code) {
    case 0: return NULL;
    case BSX_UNKNOWN: return "Unknown error";
    case BSX_GENERAL: return "General error";
    case BSX_NO_GPU: return "No GPU available";
    case BSX_TIMEOUT: return "Timed out";
    case BSX_NOT_IMPLEMENTED: return "Not implemented";
    case BSX_CORRUPTED: return "Corrupted";
    case BSX_FAILED_TO_CONVERT: return "Failed to convert";
    case BSX_INVALID_TYPE: return "Invalid type";
    case BSX_UNKNOWN_TYPE_SIZE: return "Unknown type size";
    case BSX_FAILED_TO_QUERY: return "Failed to query";
    case BSX_FAILED_TO_CREATE: return "Failed to create";
    case BSX_NOT_FOUND: return "Not found";
    case BSX_INVALID_PARAM: return "Invalid parameter";
    case BSX_INVALID_FORMAT: return "Invalid format";
    case BSX_ENSURE_RETURN: return "Ensure return";
    case BSX_DEVICE_LOST: return "Device lost";
    case BSX_FAILED_TO_ACQUIRE: return "Failed to acquire";
    case BSX_DUPLICATE: return "Duplicate";
    case BSX_UNMAPPED_BUFFER: return "Unmapped buffer";
    case BSX_INVALID_VERTEX_ATTRIBUTE: return "Invalid vertex attribute, you might need to configure it with bs_configureAttribute(...)";
    case BSX_UNKNOWN_LAYOUT_TRANSITION: return "Unknown layout transition";
    case BSX_NO_DATA: return "No data";
    case BSX_NO_SHADER_STAGES: return "No shader stages";
    case BSX_TOO_MANY_OPEN_FILES: return "Too many open files";
    case BSX_PERMISSION_DENIED: return "Permission denied";
    case BSX_BAD_PATH_NAME: return "Bad path name";
    case BSX_INVALID_NAME: return "Invalid name";
    case BSX_ALREADY_EXISTS: return "Already exists";
    case BSX_INVALID_FILE_TYPE: return "Invalid file type";
    case BSX_INVALID_FILE_EXTENSION: return "Invalid file extension";
    case BSX_FAILED_TO_WRITE: return "Failed to write";
    case BSX_INVALID_HANDLE: return "Invalid handle";
    case BSX_FAILED_TO_ALLOCATE: return "Failed to allocate";
    case BSX_OUT_OF_BOUNDS: return "Out of bounds";
    case BSX_EXPECTED_UNSIGNED: return "Expected unsigned value";
    case BSX_UNINITIALIZED: return "Uninitialized";
    case BSX_MISMATCH: return "Mismatch";
    case BSX_INTEGER_OVERFLOW: return "Integer overflow";
    case BSX_OUT_OF_ORDER: return "Out of order";
    case BSX_IMMUTABLE: return "Attempted to edit immutable value";
    case BSX_EXPECTED_END: return "Expected end";
    case BSX_CANT_CONNECT: return "Can't connect";
    case BSX_INVALID_URL: return "Invalid URL";
    case BSX_INVALID_CONNECTION: return "Invalid connection";
    case BSX_NOT_A_NUMBER: return "Not a number";
    case BSX_FAILED_TO_INVOKE: return "Failed to invoke";
    case BSX_VALIDATION: return "Vulkan validation error";
    case BSX_ASSERTION: return "Assertion failed";
    case BSX_INVALID_STATE: return "Invalid state";
    case BSX_NOT_SUPPORTED: return "Not supported";
    default: return "Unknown";
    }
}

void bs_logBasilisk(bs_U64 code) {
    if (!code)
        return;

    if (code & BSXI_STEAM) {
        bs_infoF(BS_PRINT_COLOR("BASLISK THREW (Steam)\n", BS_PRINT_RED));
        code &= ~BSXI_STEAM;
    }
    if (code & BSXI_INTERNAL) {
        bs_infoF(BS_PRINT_COLOR("BASILISK THREW\n", BS_PRINT_RED));
        code &= ~BSXI_INTERNAL;
    }
    else
        bs_infoF(BS_PRINT_COLOR("%s THREW\n", BS_PRINT_RED), _bs_wnd.title);

#ifndef BS_SKIP_ERROR_MESSAGES
    bs_infoF("%s\n", bs_exceptionName(code));
#endif
}

void bsi_throwBasiliskF(bs_U64 code, const char* format, ...) {
    if (_bs_io.exceptions & code) {
        _bs_io.caught |= code;
        if (_bs_io.exceptions & BSXI_LOG_CAUGHT) {
            bs_logBasilisk(code);

            if (format) {
                va_list args;
                va_start(args, format);
                bs_logWithTimestampV(BS_ERROR_HEADER, sizeof(BS_ERROR_HEADER) - 1, format, args);
                va_end(args);
            }
            bs_infoF("\n");
        }
        return;
    }

    bs_logBasilisk(code);

    if (format) {
        va_list args;
        va_start(args, format);
        bs_logWithTimestampV(BS_ERROR_HEADER, sizeof(BS_ERROR_HEADER) - 1, format, args);
        va_end(args);
    }

    bs_throwF("\n");
}

static void bs_throwUnknownBasiliskM(const char* signature, bs_I64 underlying, const char* message) {
    bs_infoF("BASILISK THREW\n");
    bsi_throwBasiliskF(BSX_UNKNOWN, "An unknown %s error occurred (%d)\n%s", signature, underlying, message);
}

void bs_throwBasilisk(bs_U64 code) {
    bsi_throwBasiliskF(code, NULL);
}

void bs_throwErrno(const char* message) {
    switch (errno) {
    case 0: return;
    case ENOENT: bsi_throwBasiliskF(BSXI_INTERNAL | BSX_NOT_FOUND, message ? message : ""); break;
    case EACCES: bsi_throwBasiliskF(BSXI_INTERNAL | BSX_PERMISSION_DENIED, message ? message : ""); break;
    case EINVAL: bsi_throwBasiliskF(BSXI_INTERNAL | BSX_INVALID_PARAM, message ? message : ""); break;
    default: bs_throwUnknownBasiliskM("ERRNO", errno, message); break;
    }
}

bs_Result bs_convertVulkanResult(int result) {
    switch (result) {
    case VK_SUCCESS: return BS_RESULT_OK;
    default:
        return BS_RESULT_GENERAL_ERROR;
    }
}

void bs_throwVulkan(int result) {
    switch (result) {
    case VK_SUCCESS: return;
    case VK_ERROR_DEVICE_LOST: bs_throwBasilisk(BSXI_INTERNAL | BSX_DEVICE_LOST); return;
    case VK_TIMEOUT: bs_throwBasilisk(BSXI_INTERNAL | BSX_TIMEOUT); return;
    default: bs_throwUnknownBasiliskM("VULKAN", result, NULL); break;
    }
}

void bs_throwNotImplemented() {
    bs_throwBasilisk(BSXI_INTERNAL | BSX_NOT_IMPLEMENTED);
}

void bs_throwIfBufferTooSmall(bs_Buffer* buffer, bs_U32 num_bytes) {
    if (buffer->num_bytes < num_bytes)
        bs_throwBasilisk(BSXI_INTERNAL | BSX_OUT_OF_BOUNDS);
}

void bs_throwIfNotMapped(bs_Buffer* buffer) {
    if (!buffer->_->data)
        bs_throwBasilisk(BSXI_INTERNAL | BSX_UNMAPPED_BUFFER);
}

// - json -

void bs_throwIfStringConversionFail(char* orig, char* output) {
    if (orig == output)
        bs_throwBasilisk(BSXI_INTERNAL | BSX_FAILED_TO_CONVERT);
}

#ifdef _WIN32 
bool bs_throwWin32Error(bs_U64 code, char* path) {
    const char* format = "%s\nGetLastError code %lld";

    path = path ? path : "";
    switch (code) {
    case 0: return false;
    case ERROR_FILE_NOT_FOUND: bsi_throwBasiliskF(BSXI_INTERNAL | BSX_NOT_FOUND, format, path, code); break;
    case ERROR_PATH_NOT_FOUND: bsi_throwBasiliskF(BSXI_INTERNAL | BSX_NOT_FOUND, format, path, code); break;
    case ERROR_TOO_MANY_OPEN_FILES: bsi_throwBasiliskF(BSXI_INTERNAL | BSX_TOO_MANY_OPEN_FILES, format, path, code); break;
    case ERROR_ACCESS_DENIED: bsi_throwBasiliskF(BSXI_INTERNAL | BSX_PERMISSION_DENIED, format, path, code); break;
    case ERROR_INVALID_HANDLE: bsi_throwBasiliskF(BSXI_INTERNAL | BSX_INVALID_HANDLE, format, path, code); break;
    case ERROR_NOT_ENOUGH_MEMORY: bsi_throwBasiliskF(BSXI_INTERNAL | BSX_OUT_OF_BOUNDS, format, path, code); break;
    case ERROR_BAD_PATHNAME: bsi_throwBasiliskF(BSXI_INTERNAL | BSX_BAD_PATH_NAME, format, path, code); break;
    case ERROR_INVALID_NAME: bsi_throwBasiliskF(BSXI_INTERNAL | BSX_INVALID_NAME, format, path, code); break;
    case ERROR_ALREADY_EXISTS: bsi_throwBasiliskF(BSXI_INTERNAL | BSX_ALREADY_EXISTS, format, path, code); break;
    case ERROR_DIRECTORY: bsi_throwBasiliskF(BSXI_INTERNAL | BSX_INVALID_NAME, format, path, code); break;
    case ERROR_CONNECTION_INVALID: bsi_throwBasiliskF(BSXI_INTERNAL | BSX_INVALID_CONNECTION, format, path, code);
    case 12002: bsi_throwBasiliskF(BSXI_INTERNAL | BSX_TIMEOUT, format, path, code); break; // ERROR_WINHTTP_TIMEOUT
    case 12005: bsi_throwBasiliskF(BSXI_INTERNAL | BSX_INVALID_URL, format, path, code); break; // ERROR_WINHTTP_INVALID_URL
    case 12029: bsi_throwBasiliskF(BSXI_INTERNAL | BSX_CANT_CONNECT, format, path, code); break; // ERROR_WINHTTP_CANNOT_CONNECT
    case 12030: bsi_throwBasiliskF(BSXI_INTERNAL | BSX_INVALID_CONNECTION, format, path, code); break; // ERROR_WINHTTP_CONNECTION_ERROR
    default: bs_throwUnknownBasiliskM("Win32", code, NULL); break;
    }
    return true;
}

bool bs_throwLastWin32Error(char* path) {
    return bs_throwWin32Error(GetLastError(), path);
}

void bs_throwHResult(long code, char* str) {
    switch (code) {
    case CO_E_CLASSSTRING: bsi_throwBasiliskF(BSX_INVALID_FORMAT, "%s", str); break;
    case 0: return;
    default: bs_throwUnknownBasiliskM("HRESULT", code, str); break;
    }
}

// - object diff log -

static inline const char* bs_queueTypeName(bs_QueueBits queues) {
    if (queues & BS_QUEUE_GRAPHICS_BIT) {
        if (queues & BS_QUEUE_COMPUTE_BIT) {
            if (queues & BS_QUEUE_TRANSFER_BIT)
                return "graphics/compute/transfer queue";
            else return "graphics/compute queue";
        }
        else return "graphics queue";
    }
    else if (queues & BS_QUEUE_COMPUTE_BIT) {
        if (queues & BS_QUEUE_TRANSFER_BIT)
            return "compute/transfer queue";
        else return "compute queue";
    }
    else if (queues & BS_QUEUE_TRANSFER_BIT)
        return "transfer queue";
    return NULL;
}

static inline void bs_logBatchName(bs_Batch* batch) {
    bs_infoF("batch (");
    for (int i = 0; i < batch->attributes_count; i++) {
        bs_Attribute* attribute = batch->attributes + i;
        if (i == batch->attributes_count - 1)
            bs_infoF("%s)", attribute->name);
        else bs_infoF("%s, ", attribute->name);
    }
}

static inline const char* bs_objectName(int type) {
    switch (type) {
        case BS_OBJECT_IMAGE: return "image";
        case BS_OBJECT_SAMPLER: return "sampler";
        case BS_OBJECT_BUFFER: return "buffer";
        case BS_OBJECT_QUEUE: return "queue";
        case BS_OBJECT_BATCH: return "batch";
        case BS_OBJECT_RENDERER: return "renderer";
        default: return "unknown object type";
    }
}

void bs_logUnchangedObjects() {
    /*
    int last_id = -1;
    for (int i = _bs_instance_->objects_count - 1; i >= 0; i--) {
        if (_bs_instance_->objects[i].object.type != 0 &&
            _bs_instance_->objects[i].object.status == 0) {
            last_id = i;
            break;
        }
    }

    const int new_line_every = 3;
    for (int i = 0, is = 0; i < last_id - 1; i++) {
        bs_Object* object = &_bs_instance_->objects[i].object;
        if (object->type == 0)
            continue;

        if (object->status == 0) {
            if (_bs_args.color_log)
                bs_logF(BS_PRINT_GRAY "(%d) %s, ", object->head->id, bs_idName(object->head->id));
            else
                bs_logF("(%d) %s, ", object->head->id, bs_idName(object->head->id));

            if (++is % new_line_every == 0)
                bs_logF("\n");
        }
    }

    if (last_id != -1)
        bs_logF("(%d) %s%s\n", last_id, bs_idName(_bs_instance_->objects[last_id].object.head->id), (_bs_args.color_log ? BS_PRINT_RESET : ""));
        */
}

void bs_logObjectDiff() {
    /*
    int size = 0;
    for (int i = 0; i < _bs_instance_->objects_count; i++) {
        bs_Object* object = &_bs_instance_->objects[i].object;
        if (object->type == 0)
            continue;

        if (object->strikes == BS_NUM_STRIKES_RULE) {
            if (_bs_args.color_log)
                bs_infoF(BS_PRINT_RED "-" BS_PRINT_RESET  " (%d) ", object->head->id);
            else
                bs_infoF("- (%d) ", object->head->id);
        }
        else if (object->status == BS_OBJECT_WAS_CREATED) {
            if (_bs_args.color_log)
                bs_infoF(BS_PRINT_GREEN "+" BS_PRINT_RESET  " (%d) ", object->head->id);
            else
                bs_infoF("+ (%d) ", object->head->id);
        }
        else if (object->status == BS_OBJECT_WAS_ALTERED) {
            if (_bs_args.color_log)
                bs_infoF(BS_PRINT_YELLOW "/" BS_PRINT_RESET  " (%d) ", object->head->id);
            else
                bs_infoF("/ (%d) ", object->head->id);
        }
        else continue;

        bs_infoF("%s " BS_PRINT_CYAN "%s\n" BS_PRINT_RESET, bs_objectName(object->type), bs_idName(object->head->id));

    }
    */
}

void bs_logBindings() {
    for (int i = 0; i < _bs_instance_->bind_sets_count; i++) {
        bs_BindSet* bind_set = _bs_instance_->bind_sets + i;
        if (bind_set->bindings_count == 0) continue;

        for (int j = 0; j < bind_set->bindings_count; j++) {
            bs_Binding* binding = bind_set->bindings + j;
            
            /*
        const char* suffix = NULL;
            if (binding->object.type == BS_BUFFER)
                suffix = binding->object.buffer->_->data ? " (mapped)" : NULL;

            if (_bs_args.color_log) {
                int id = binding->object.head ? binding->object.head->id : -1;
                bs_infoF("(%d, %d) binding" BS_PRINT_MAGENTA " %s " BS_PRINT_RESET "(%d)" BS_PRINT_CYAN " %s\n" BS_PRINT_RESET,
                    bind_set->slot, 
                    binding->slot, 
                    binding->name ? binding->name : "",
                    id, 
                    id == 0 ? "" : bs_idName(id));
            }
            */
        }
    }
}

void bs_infoF4(bs_mat4* m) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            printf("%s%6.5f ", m->a[i][j] < 0 ? "" : " ", m->a[i][j]);
        printf("\n");
    }
}


#endif