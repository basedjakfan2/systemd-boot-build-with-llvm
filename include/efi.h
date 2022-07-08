#ifndef _EFI_H
#define _EFI_H

#include <stdint.h>

#include <Uefi.h>
#include <Guid/FileSystemInfo.h>
#include <Guid/FileSystemVolumeLabelInfo.h>
#include <Guid/GlobalVariable.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiLib.h>
#include <Protocol/BlockIo.h>
#include <Protocol/BlockIo2.h>
#include <Protocol/DeviceIo.h>
#include <Protocol/DiskIo.h>
#include <Protocol/DiskIo2.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/LoadFile.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/NetworkInterfaceIdentifier.h>
#include <Protocol/PxeBaseCode.h>
#include <Protocol/PxeBaseCodeCallBack.h>
#include <Protocol/Rng.h>
#include <Protocol/SerialIo.h>
#include <Protocol/Shell.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/SimpleNetwork.h>
#include <Protocol/UnicodeCollation.h>

#ifdef _MSC_EXTENSIONS
#define EFI_UNUSED
#else
#define EFI_UNUSED __attribute__((__unused__))
#endif

#define ASSERT(a)

#define D_ERROR 0x80000000
#define EFI_DBUG_MASK (D_ERROR)

#define EFI_MAXIMUM_VARIABLE_SIZE 1024
#define INTERFACE_DECL(x) struct x

#define SHELL_INTERFACE_PROTOCOL                                               \
  {                                                                            \
    0x47c7b223, 0xc42a, 0x11d2, {                                              \
      0x8e, 0x57, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b                            \
    }                                                                          \
  }

#define ENVIRONMENT_VARIABLE_ID                                                \
  {                                                                            \
    0x47c7b224, 0xc42a, 0x11d2, {                                              \
      0x8e, 0x57, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b                            \
    }                                                                          \
  }

#define DEVICE_PATH_MAPPING_ID                                                 \
  {                                                                            \
    0x47c7b225, 0xc42a, 0x11d2, {                                              \
      0x8e, 0x57, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b                            \
    }                                                                          \
  }

#define PROTOCOL_ID_ID                                                         \
  {                                                                            \
    0x47c7b226, 0xc42a, 0x11d2, {                                              \
      0x8e, 0x57, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b                            \
    }                                                                          \
  }

#define ALIAS_ID                                                               \
  {                                                                            \
    0x47c7b227, 0xc42a, 0x11d2, {                                              \
      0x8e, 0x57, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b                            \
    }                                                                          \
  }

#define VARIABLE_STORE_PROTOCOL                                                \
  {                                                                            \
    0xf088cd91, 0xa046, 0x11d2, {                                              \
      0x8e, 0x42, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b                            \
    }                                                                          \
  }
#define LEGACY_BOOT_PROTOCOL                                                   \
  {                                                                            \
    0x376e5eb2, 0x30e4, 0x11d3, {                                              \
      0xba, 0xe5, 0x0, 0x80, 0xc7, 0x3c, 0x88, 0x81                            \
    }                                                                          \
  }
#define VGA_CLASS_DRIVER_PROTOCOL                                              \
  {                                                                            \
    0xe3d6310, 0x6fe4, 0x11d3, {                                               \
      0xbb, 0x81, 0x0, 0x80, 0xc7, 0x3c, 0x88, 0x81                            \
    }                                                                          \
  }
#define EFI_PART_TYPE_EFI_SYSTEM_PART_GUID                                     \
  {                                                                            \
    0xc12a7328, 0xf81f, 0x11d2, {                                              \
      0xba, 0x4b, 0x00, 0xa0, 0xc9, 0x3e, 0xc9, 0x3b                           \
    }                                                                          \
  }
#define EFI_PART_TYPE_LEGACY_MBR_GUID                                          \
  {                                                                            \
    0x024dee41, 0x33e7, 0x11d3, {                                              \
      0x9d, 0x69, 0x00, 0x08, 0xc7, 0x81, 0xf3, 0x9f                           \
    }                                                                          \
  }
#define UNKNOWN_DEVICE_GUID                                                    \
  {                                                                            \
    0xcf31fac5, 0xc24e, 0x11d2, {                                              \
      0x85, 0xf3, 0x0, 0xa0, 0xc9, 0x3e, 0xc9, 0x3b                            \
    }                                                                          \
  }
#define TEXT_OUT_SPLITER_PROTOCOL                                              \
  {                                                                            \
    0x56d830a0, 0x7e7a, 0x11d3, {                                              \
      0xbb, 0xa0, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b                            \
    }                                                                          \
  }
#define ERROR_OUT_SPLITER_PROTOCOL                                             \
  {                                                                            \
    0xf0ba9039, 0x68f1, 0x425e, {                                              \
      0xaa, 0x7f, 0xd9, 0xaa, 0xf9, 0x1b, 0x82, 0xa1                           \
    }                                                                          \
  }
#define TEXT_IN_SPLITER_PROTOCOL                                               \
  {                                                                            \
    0xf9a3c550, 0x7fb5, 0x11d3, {                                              \
      0xbb, 0xa0, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b                            \
    }                                                                          \
  }

#define EFI_GLOBAL_VARIABLE                                                    \
  {                                                                            \
    0x8BE4DF61, 0x93CA, 0x11d2, {                                              \
      0xAA, 0x0D, 0x00, 0xE0, 0x98, 0x03, 0x2B, 0x8C                           \
    }                                                                          \
  }

#define EFI_DEVICE_PATH_PROTOCOL_GUID                                          \
  {                                                                            \
    0x9576e91, 0x6d3f, 0x11d2, {                                               \
      0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b                            \
    }                                                                          \
  }

#define EFI_LOADED_IMAGE_PROTOCOL_GUID                                         \
  {                                                                            \
    0x5B1B31A1, 0x9562, 0x11d2, {                                              \
      0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B                           \
    }                                                                          \
  }

#define EFI_SIMPLE_TEXT_INPUT_PROTOCOL_GUID                                    \
  {                                                                            \
    0x387477c1, 0x69c7, 0x11d2, {                                              \
      0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b                            \
    }                                                                          \
  }

#define EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_GUID                                   \
  {                                                                            \
    0x387477c2, 0x69c7, 0x11d2, {                                              \
      0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b                            \
    }                                                                          \
  }

#define EFI_BLOCK_IO_PROTOCOL_GUID                                             \
  {                                                                            \
    0x964e5b21, 0x6459, 0x11d2, {                                              \
      0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b                            \
    }                                                                          \
  }

#define EFI_BLOCK_IO2_PROTOCOL_GUID                                            \
  {                                                                            \
    0xa77b2472, 0xe282, 0x4e9f, {                                              \
      0xa2, 0x45, 0xc2, 0xc0, 0xe2, 0x7b, 0xbc, 0xc1                           \
    }                                                                          \
  }

#define EFI_DISK_IO_PROTOCOL_GUID                                              \
  {                                                                            \
    0xce345171, 0xba0b, 0x11d2, {                                              \
      0x8e, 0x4f, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b                            \
    }                                                                          \
  }

#define EFI_DISK_IO2_PROTOCOL_GUID                                             \
  {                                                                            \
    0x151c8eae, 0x7f2c, 0x472c, {                                              \
      0x9e, 0x54, 0x98, 0x28, 0x19, 0x4f, 0x6a, 0x88                           \
    }                                                                          \
  }

#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID                                   \
  {                                                                            \
    0x964e5b22, 0x6459, 0x11d2, {                                              \
      0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b                            \
    }                                                                          \
  }

#define EFI_LOAD_FILE_PROTOCOL_GUID                                            \
  {                                                                            \
    0x56EC3091, 0x954C, 0x11d2, {                                              \
      0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B                           \
    }                                                                          \
  }

#define EFI_DEVICE_IO_PROTOCOL_GUID                                            \
  {                                                                            \
    0xaf6ac311, 0x84c3, 0x11d2, {                                              \
      0x8e, 0x3c, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b                           \
    }                                                                          \
  }

#define EFI_COMPONENT_NAME_PROTOCOL_GUID                                       \
  {                                                                            \
    0x107A772C, 0xD5E1, 0x11D4, {                                              \
      0x9A, 0x46, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0x4D                           \
    }                                                                          \
  }

#define EFI_COMPONENT_NAME2_PROTOCOL_GUID                                      \
  {                                                                            \
    0x6A7A5CFF, 0xE8D9, 0x4F70, {                                              \
      0xBA, 0xDA, 0x75, 0xAB, 0x30, 0x25, 0xCE, 0x14                           \
    }                                                                          \
  }

#define EFI_FILE_INFO_ID                                                       \
  {                                                                            \
    0x9576e92, 0x6d3f, 0x11d2, {                                               \
      0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b                            \
    }                                                                          \
  }

#define EFI_FILE_SYSTEM_INFO_ID                                                \
  {                                                                            \
    0x9576e93, 0x6d3f, 0x11d2, {                                               \
      0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b                            \
    }                                                                          \
  }

#define EFI_FILE_SYSTEM_VOLUME_LABEL_ID                                        \
  {                                                                            \
    0xDB47D7D3, 0xFE81, 0x11d3, {                                              \
      0x9A, 0x35, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0x4D                           \
    }                                                                          \
  }

#define EFI_UNICODE_COLLATION_PROTOCOL_GUID                                    \
  {                                                                            \
    0x1d85cd7f, 0xf43d, 0x11d2, {                                              \
      0x9a, 0xc, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d                             \
    }                                                                          \
  }

#define EFI_SERIAL_IO_PROTOCOL_GUID                                            \
  {                                                                            \
    0xBB25CF6F, 0xF1D4, 0x11D2, {                                              \
      0x9A, 0x0C, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0xFD                           \
    }                                                                          \
  }

#define EFI_SIMPLE_NETWORK_PROTOCOL_GUID                                       \
  {                                                                            \
    0xA19832B9, 0xAC25, 0x11D3, {                                              \
      0x9A, 0x2D, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0x4D                           \
    }                                                                          \
  }

#define EFI_NETWORK_INTERFACE_IDENTIFIER_PROTOCOL_GUID                         \
  {                                                                            \
    0xE18541CD, 0xF755, 0x4f73, {                                              \
      0x92, 0x8D, 0x64, 0x3C, 0x8A, 0x79, 0xB2, 0x29                           \
    }                                                                          \
  }

#define EFI_PXE_BASE_CODE_PROTOCOL_GUID                                        \
  {                                                                            \
    0x03c4e603, 0xac28, 0x11d3, {                                              \
      0x9a, 0x2d, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d                           \
    }                                                                          \
  }

#define EFI_PXE_BASE_CODE_CALLBACK_PROTOCOL_GUID                               \
  {                                                                            \
    0x245dca21, 0xfb7b, 0x11d3, {                                              \
      0x8f, 0x01, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b                           \
    }                                                                          \
  }

#define EFI_PC_ANSI_GUID                                                       \
  {                                                                            \
    0xe0c14753, 0xf9be, 0x11d2, {                                              \
      0x9a, 0x0c, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d                           \
    }                                                                          \
  }

#define EFI_VT_100_GUID                                                        \
  {                                                                            \
    0xdfa66065, 0xb419, 0x11d3, {                                              \
      0x9a, 0x2d, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d                           \
    }                                                                          \
  }

#define EFI_VT_100_PLUS_GUID                                                   \
  {                                                                            \
    0x7baec70b, 0x57e0, 0x4c76, {                                              \
      0x8e, 0x87, 0x2f, 0x9e, 0x28, 0x08, 0x83, 0x43                           \
    }                                                                          \
  }

#define EFI_VT_UTF8_GUID                                                       \
  {                                                                            \
    0xad15a0d6, 0x8bec, 0x4acf, {                                              \
      0xa0, 0x73, 0xd0, 0x1d, 0xe7, 0x7e, 0x2d, 0x88                           \
    }                                                                          \
  }

#define EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID                                      \
  {                                                                            \
    0x9042a9de, 0x23dc, 0x4a38, {                                              \
      0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a                           \
    }                                                                          \
  }

#define EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL_GUID                                 \
  {                                                                            \
    0xdd9e7534, 0x7762, 0x4698, {                                              \
      0x8c, 0x14, 0xf5, 0x85, 0x17, 0xa6, 0x25, 0xaa                           \
    }                                                                          \
  }

#define uefi_call_wrapper(func, va_num, ...) func(__VA_ARGS__)

typedef EFI_DEVICE_PATH_PROTOCOL EFI_DEVICE_PATH;

#define EFI_DP_TYPE_MASK 0x7F
#define EFI_DP_TYPE_UNPACKED 0x80

#define END_DEVICE_PATH_TYPE 0x7f

#define END_ENTIRE_DEVICE_PATH_SUBTYPE 0xff
#define END_INSTANCE_DEVICE_PATH_SUBTYPE 0x01
#define END_DEVICE_PATH_LENGTH (sizeof(EFI_DEVICE_PATH_PROTOCOL))

#define DP_IS_END_TYPE(a)
#define DP_IS_END_SUBTYPE(a)        ( ((a)->SubType == END_ENTIRE_DEVICE_PATH_SUBTYPE )

#define DevicePathType(a) (((a)->Type) & EFI_DP_TYPE_MASK)
#define DevicePathSubType(a) ((a)->SubType)
#define DevicePathNodeLength(a) (((a)->Length[0]) | ((a)->Length[1] << 8))
#define NextDevicePathNode(a)                                                  \
  ((EFI_DEVICE_PATH_PROTOCOL *)(((UINT8 *)(a)) + DevicePathNodeLength(a)))
#define IsDevicePathEndType(a) (DevicePathType(a) == END_DEVICE_PATH_TYPE)
#define IsDevicePathEndSubType(a)                                              \
  ((a)->SubType == END_ENTIRE_DEVICE_PATH_SUBTYPE)
#define IsDevicePathEnd(a) (IsDevicePathEndType(a) && IsDevicePathEndSubType(a))
#define IsDevicePathUnpacked(a) ((a)->Type & EFI_DP_TYPE_UNPACKED)

#define SetDevicePathNodeLength(a, l)                                          \
  {                                                                            \
    (a)->Length[0] = (UINT8)(l);                                               \
    (a)->Length[1] = (UINT8)((l) >> 8);                                        \
  }

#define SetDevicePathEndNode(a)                                                \
  {                                                                            \
    (a)->Type = END_DEVICE_PATH_TYPE;                                          \
    (a)->SubType = END_ENTIRE_DEVICE_PATH_SUBTYPE;                             \
    (a)->Length[0] = sizeof(EFI_DEVICE_PATH_PROTOCOL);                         \
    (a)->Length[1] = 0;                                                        \
  }

#if defined(_M_ARM64)
#define MIN_ALIGNMENT_SIZE 8
typedef short WCHAR;
#elif defined(_M_ARM)
#define MIN_ALIGNMENT_SIZE 4
typedef short WCHAR;
#elif defined(_M_AMD64)
#define MIN_ALIGNMENT_SIZE 4
typedef short WCHAR;
#elif defined(_M_IX86)
#define MIN_ALIGNMENT_SIZE 4
typedef short WCHAR;
#endif

typedef EFI_UNICODE_COLLATION_PROTOCOL EFI_UNICODE_COLLATION_INTERFACE;

extern EFI_GUID DevicePathProtocol;
extern EFI_MEMORY_TYPE PoolAllocationType;

extern BOOLEAN LibInitialized;
extern BOOLEAN LibFwInstance;
extern EFI_HANDLE LibImageHandle;

extern EFI_SYSTEM_TABLE *ST;
extern EFI_BOOT_SERVICES *BS;
extern EFI_RUNTIME_SERVICES *RT;

extern UINTN EFIDebug;

extern EFI_GUID GenericFileInfo;

extern EFI_GUID ShellInterfaceProtocol;
extern EFI_GUID SEnvId;
extern EFI_GUID SMapId;
extern EFI_GUID SProtId;
extern EFI_GUID SAliasId;
extern EFI_GUID VariableStoreProtocol;
extern EFI_GUID LegacyBootProtocol;
extern EFI_GUID VgaClassProtocol;
extern EFI_GUID EfiPartTypeSystemPartitionGuid;
extern EFI_GUID EfiPartTypeLegacyMbrGuid;
extern EFI_GUID UnknownDevice;
extern EFI_GUID TextOutSpliterProtocol;
extern EFI_GUID ErrorOutSpliterProtocol;
extern EFI_GUID TextInSpliterProtocol;
extern EFI_GUID NullGuid;
extern EFI_GUID SimpleTextInputExProtocol;

#define EfiGlobalVariable gEfiGlobalVariableGuid
#define BlockIoProtocol gEfiBlockIoProtocolGuid
#define FileSystemProtocol gEfiSimpleFileSystemProtocolGuid
#define GraphicsOutputProtocol gEfiGraphicsOutputProtocolGuid
#define LoadedImageProtocol gEfiLoadedImageProtocolGuid

static struct {
  EFI_GUID *Guid;
  WCHAR *GuidName;
} KnownGuids[] = {{&NullGuid, L"G0"},
                  {&gEfiGlobalVariableGuid, L"EfiVar"},

                  {&VariableStoreProtocol, L"VarStore"},
                  {&gEfiDevicePathProtocolGuid, L"DevPath"},
                  {&gEfiLoadedImageProtocolGuid, L"LdImg"},
                  {&gEfiSimpleTextInProtocolGuid, L"TxtIn"},
                  {&gEfiSimpleTextOutProtocolGuid, L"TxtOut"},
                  {&gEfiBlockIoProtocolGuid, L"BlkIo"},
                  {&gEfiBlockIo2ProtocolGuid, L"BlkIo2"},
                  {&gEfiDiskIoProtocolGuid, L"DskIo"},
                  {&gEfiDiskIo2ProtocolGuid, L"DskIo2"},
                  {&gEfiSimpleFileSystemProtocolGuid, L"Fs"},
                  {&gEfiLoadFileProtocolGuid, L"LdFile"},
                  {&gEfiDeviceIoProtocolGuid, L"DevIo"},
                  {&gEfiComponentNameProtocolGuid, L"CName"},
                  {&gEfiComponentName2ProtocolGuid, L"CName2"},

                  {&gEfiFileInfoGuid, L"FileInfo"},
                  {&gEfiFileSystemInfoGuid, L"FsInfo"},
                  {&gEfiFileSystemVolumeLabelInfoIdGuid, L"FsVolInfo"},

                  {&gEfiUnicodeCollationProtocolGuid, L"Unicode"},
                  {&LegacyBootProtocol, L"LegacyBoot"},
                  {&gEfiSerialIoProtocolGuid, L"SerIo"},
                  {&VgaClassProtocol, L"VgaClass"},
                  {&gEfiSimpleNetworkProtocolGuid, L"Net"},
                  {&gEfiNetworkInterfaceIdentifierProtocolGuid, L"Nii"},
                  {&gEfiPxeBaseCodeProtocolGuid, L"Pxe"},
                  {&gEfiPxeBaseCodeCallbackProtocolGuid, L"PxeCb"},

                  {&TextOutSpliterProtocol, L"TxtOutSplit"},
                  {&ErrorOutSpliterProtocol, L"ErrOutSplit"},
                  {&TextInSpliterProtocol, L"TxtInSplit"},
                  {&gEfiPcAnsiGuid, L"PcAnsi"},
                  {&gEfiVT100Guid, L"Vt100"},
                  {&gEfiVT100PlusGuid, L"Vt100Plus"},
                  {&gEfiVTUTF8Guid, L"VtUtf8"},
                  {&UnknownDevice, L"UnknownDev"},

                  {&EfiPartTypeSystemPartitionGuid, L"ESP"},
                  {&EfiPartTypeLegacyMbrGuid, L"GPT MBR"},

                  {&ShellInterfaceProtocol, L"ShellInt"},
                  {&SEnvId, L"SEnv"},
                  {&SProtId, L"ShellProtId"},
                  {&SMapId, L"ShellDevPathMap"},
                  {&SAliasId, L"ShellAlias"},

                  {NULL, L""}};

static inline UINT64 RShiftU64(IN UINT64 Operand, IN UINTN Count) {
  return Operand >> Count;
}

static inline UINTN StrLen(IN CONST CHAR16 *s1) {
  UINTN len;

  for (len = 0; *s1; s1 += 1, len += 1)
    ;
  return len;
}

static inline VOID StrCpy(IN CHAR16 *Dest, IN CONST CHAR16 *Src) {
  while (*Src) {
    *(Dest++) = *(Src++);
  }
  *Dest = 0;
}

static inline VOID ZeroMem(IN VOID *Buffer, IN UINTN Size) {
  INT8 *pt;

  pt = Buffer;
  while (Size--) {
    *(pt++) = 0;
  }
}

static inline VOID *AllocatePool(IN UINTN Size) {
  EFI_STATUS Status;
  VOID *p;

  Status = uefi_call_wrapper(BS->AllocatePool, 3, PoolAllocationType, Size, &p);
  if (EFI_ERROR(Status)) {
    p = NULL;
  }
  return p;
}

static inline VOID *AllocateZeroPool(IN UINTN Size) {
  VOID *p;

  p = AllocatePool(Size);
  if (p) {
    ZeroMem(p, Size);
  }

  return p;
}

static inline VOID CopyMem(IN VOID *Dest, IN CONST VOID *Src, IN UINTN len) {
  CHAR8 *d = (CHAR8 *)Dest;
  CHAR8 *s = (CHAR8 *)Src;

  if (d == NULL || s == NULL || s == d)
    return;

  if ((d > s) && (d < s + len)) {
    for (d += len, s += len; len--;)
      *--d = *--s;
  } else {
    while (len--)
      *d++ = *s++;
  }
}

static inline VOID FreePool(IN VOID *Buffer) {
  uefi_call_wrapper(BS->FreePool, 1, Buffer);
}

static inline VOID *ReallocatePool(IN VOID *OldPool, IN UINTN OldSize,
                                   IN UINTN NewSize) {
  VOID *NewPool;

  NewPool = NULL;
  if (NewSize) {
    NewPool = AllocatePool(NewSize);
  }

  if (OldPool) {
    if (NewPool) {
      CopyMem(NewPool, OldPool, OldSize < NewSize ? OldSize : NewSize);
    }

    FreePool(OldPool);
  }

  return NewPool;
}

static inline VOID
InitializeLibPlatform(IN EFI_HANDLE ImageHandle EFI_UNUSED,
                      IN EFI_SYSTEM_TABLE *SystemTable EFI_UNUSED) {}

static inline VOID InitializeGuid(VOID) {}

static inline VOID EFIDebugVariable(VOID) {
  EFI_STATUS Status;
  UINT32 Attributes;
  UINTN DataSize;
  UINTN NewEFIDebug;

  DataSize = sizeof(EFIDebug);
  Status =
      uefi_call_wrapper(RT->GetVariable, 5, L"EFIDebug", &EfiGlobalVariable,
                        &Attributes, &DataSize, &NewEFIDebug);
  if (!EFI_ERROR(Status)) {
    EFIDebug = NewEFIDebug;
  }
}

static inline VOID InitializeLib(IN EFI_HANDLE ImageHandle,
                                 IN EFI_SYSTEM_TABLE *SystemTable) {
  EFI_LOADED_IMAGE *LoadedImage;
  EFI_STATUS Status;

  if (!LibInitialized) {
    LibInitialized = TRUE;
    LibFwInstance = FALSE;
    LibImageHandle = ImageHandle;

    ST = SystemTable;
    BS = SystemTable->BootServices;
    RT = SystemTable->RuntimeServices;

    if (ImageHandle) {
      Status = uefi_call_wrapper(BS->HandleProtocol, 3, ImageHandle,
                                 &LoadedImageProtocol, (VOID *)&LoadedImage);

      if (!EFI_ERROR(Status)) {
        PoolAllocationType = LoadedImage->ImageDataType;
      }
      EFIDebugVariable();
    }

    InitializeGuid();

    InitializeLibPlatform(ImageHandle, SystemTable);
  }
}

#include "efi_print.h"
#include "efi_dpath.h"

#endif
