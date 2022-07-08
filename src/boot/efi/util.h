/* SPDX-License-Identifier: LGPL-2.1-or-later */
#pragma once

#include <efi.h>
#include <stddef.h>

#include "string-util-fundamental.h"

#define UINTN_MAX (~(UINTN)0)
#define INTN_MAX ((INTN)(UINTN_MAX>>1))

/* gnu-efi format specifiers for integers are fixed to either 64bit with 'l' and 32bit without a size prefix.
 * We rely on %u/%d/%x to format regular ints, so ensure the size is what we expect. At the same time, we also
 * need specifiers for (U)INTN which are native (pointer) sized. */
assert_cc(sizeof(int) == sizeof(uint32_t));
#if __SIZEOF_POINTER__ == 4
#  define PRIuN L"u"
#  define PRIiN L"d"
#elif __SIZEOF_POINTER__ == 8
#  define PRIuN L"lu"
#  define PRIiN L"ld"
#else
#  error "Unexpected pointer size"
#endif

static inline void free(void *p) {
        if (!p)
                return;

        /* Debugging an invalid free requires trace logging to find the call site or a debugger attached. For
         * release builds it is not worth the bother to even warn when we cannot even print a call stack. */
#ifdef EFI_DEBUG
        assert_se(BS->FreePool(p) == EFI_SUCCESS);
#else
        (void) BS->FreePool(p);
#endif
}

static inline void freep(void *p) {
        free(*(void **) p);
}

#define _cleanup_freepool_ _cleanup_free_
#define _cleanup_free_ _cleanup_(freep)

_malloc_ _alloc_(1) _returns_nonnull_ _warn_unused_result_
static inline void *xmalloc(size_t size) {
        void *p;
        assert_se(BS->AllocatePool(EfiBootServicesData, size, &p) == EFI_SUCCESS);
        return p;
}

_malloc_ _alloc_(1, 2) _returns_nonnull_ _warn_unused_result_
static inline void *xmalloc_multiply(size_t size, size_t n) {
        assert_se(!__builtin_mul_overflow(size, n, &size));
        return xmalloc(size);
}

/* Use malloc attribute as this never returns p like userspace realloc. */
_malloc_ _alloc_(3) _returns_nonnull_ _warn_unused_result_
static inline void *xrealloc(void *p, size_t old_size, size_t new_size) {
        void *r = xmalloc(new_size);
        efi_memcpy(r, p, MIN(old_size, new_size));
        free(p);
        return r;
}

#define xpool_print(fmt, ...) ((char16_t *) ASSERT_SE_PTR(PoolPrint((fmt), ##__VA_ARGS__)))
#define xnew(type, n) ((type *) xmalloc_multiply(sizeof(type), (n)))

EFI_STATUS parse_boolean(const char *v, bool *b);

EFI_STATUS efivar_set(const EFI_GUID *vendor, const char16_t *name, const char16_t *value, uint32_t flags);
EFI_STATUS efivar_set_raw(const EFI_GUID *vendor, const char16_t *name, const void *buf, UINTN size, uint32_t flags);
EFI_STATUS efivar_set_uint_string(const EFI_GUID *vendor, const char16_t *name, UINTN i, uint32_t flags);
EFI_STATUS efivar_set_uint32_le(const EFI_GUID *vendor, const char16_t *NAME, uint32_t value, uint32_t flags);
EFI_STATUS efivar_set_uint64_le(const EFI_GUID *vendor, const char16_t *name, uint64_t value, uint32_t flags);
void efivar_set_time_usec(const EFI_GUID *vendor, const char16_t *name, uint64_t usec);

EFI_STATUS efivar_get(const EFI_GUID *vendor, const char16_t *name, char16_t **value);
EFI_STATUS efivar_get_raw(const EFI_GUID *vendor, const char16_t *name, char **buffer, UINTN *size);
EFI_STATUS efivar_get_uint_string(const EFI_GUID *vendor, const char16_t *name, UINTN *i);
EFI_STATUS efivar_get_uint32_le(const EFI_GUID *vendor, const char16_t *name, uint32_t *ret);
EFI_STATUS efivar_get_uint64_le(const EFI_GUID *vendor, const char16_t *name, uint64_t *ret);
EFI_STATUS efivar_get_boolean_u8(const EFI_GUID *vendor, const char16_t *name, bool *ret);

char16_t *xstra_to_path(const char *stra);
char16_t *xstra_to_str(const char *stra);

EFI_STATUS file_read(EFI_FILE *dir, const char16_t *name, UINTN off, UINTN size, char **content, UINTN *content_size);

static inline void file_closep(EFI_FILE **handle) {
        if (!*handle)
                return;

        (*handle)->Close(*handle);
}

static inline void unload_imagep(EFI_HANDLE *image) {
        if (*image)
                (void) BS->UnloadImage(*image);
}

/*
 * Allocated random UUID, intended to be shared across tools that implement
 * the (ESP)\loader\entries\<vendor>-<revision>.conf convention and the
 * associated EFI variables.
 */
#define LOADER_GUID \
        &(const EFI_GUID) { 0x4a67b082, 0x0a4c, 0x41cf, { 0xb6, 0xc7, 0x44, 0x0b, 0x29, 0xbb, 0x8c, 0x4f } }
#define EFI_GLOBAL_GUID &(const EFI_GUID) EFI_GLOBAL_VARIABLE

void log_error_stall(const char16_t *fmt, ...);
EFI_STATUS log_oom(void);

/* This works just like log_error_errno() from userspace, but requires you
 * to provide err a second time if you want to use %r in the message! */
#define log_error_status_stall(err, fmt, ...) \
        ({ \
                log_error_stall(fmt, ##__VA_ARGS__); \
                err; \
        })

void print_at(UINTN x, UINTN y, UINTN attr, const char16_t *str);
void clear_screen(UINTN attr);

typedef int (*compare_pointer_func_t)(const void *a, const void *b);
void sort_pointer_array(void **array, UINTN n_members, compare_pointer_func_t compare);

EFI_STATUS get_file_info_harder(EFI_FILE *handle, EFI_FILE_INFO **ret, UINTN *ret_size);

EFI_STATUS readdir_harder(EFI_FILE *handle, EFI_FILE_INFO **buffer, UINTN *buffer_size);

bool is_ascii(const char16_t *f);

char16_t **strv_free(char16_t **l);

static inline void strv_freep(char16_t ***p) {
        strv_free(*p);
}

EFI_STATUS open_directory(EFI_FILE *root_dir, const char16_t *path, EFI_FILE **ret);

/* Conversion between EFI_PHYSICAL_ADDRESS and pointers is not obvious. The former is always 64bit, even on
 * 32bit archs. And gcc complains if we cast a pointer to an integer of a different size. Hence let's do the
 * conversion indirectly: first into UINTN (which is defined by UEFI to have the same size as a pointer), and
 * then extended to EFI_PHYSICAL_ADDRESS. */
static inline EFI_PHYSICAL_ADDRESS POINTER_TO_PHYSICAL_ADDRESS(const void *p) {
        return (EFI_PHYSICAL_ADDRESS) (UINTN) p;
}

static inline void *PHYSICAL_ADDRESS_TO_POINTER(EFI_PHYSICAL_ADDRESS addr) {
#if __SIZEOF_POINTER__ == 4
        /* On 32bit systems the address might not be convertible (as pointers are 32bit but
         * EFI_PHYSICAL_ADDRESS 64bit) */
        assert(addr <= UINT32_MAX);
#elif __SIZEOF_POINTER__ != 8
        #error "Unexpected pointer size"
#endif

        return (void*) (UINTN) addr;
}

uint64_t get_os_indications_supported(void);

#ifdef EFI_DEBUG
void debug_break(void);
extern uint8_t _text, _data;
/* Report the relocated position of text and data sections so that a debugger
 * can attach to us. See debug-sd-boot.sh for how this can be done. */
#  define debug_hook(identity) Print(identity L"@0x%lx,0x%lx\n", POINTER_TO_PHYSICAL_ADDRESS(&_text), POINTER_TO_PHYSICAL_ADDRESS(&_data))
#else
#  define debug_hook(identity)
#endif

#if defined(__i386__) || defined(__x86_64__)
void beep(UINTN beep_count);
#else
static inline void beep(UINTN beep_count) {}
#endif

EFI_STATUS open_volume(EFI_HANDLE device, EFI_FILE **ret_file);
EFI_STATUS make_file_device_path(EFI_HANDLE device, const char16_t *file, EFI_DEVICE_PATH **ret_dp);