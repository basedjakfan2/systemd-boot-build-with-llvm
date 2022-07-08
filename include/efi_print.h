#ifndef _EFI_PRINT_H
#define _EFI_PRINT_H

#include <stdarg.h>

#define PRINT_STRING_LEN 200
#define PRINT_ITEM_BUFFER_LEN 100

typedef struct {
  BOOLEAN Ascii;
  UINTN Index;
  CONST CHAR16 *pw;
  CONST CHAR8 *pc;
} POINTER;

typedef struct {
  CHAR16 *str;
  UINTN len;
  UINTN maxlen;
} POOL_PRINT;

typedef struct _pitem {

  POINTER Item;
  CHAR16 Scratch[PRINT_ITEM_BUFFER_LEN];
  UINTN Width;
  UINTN FieldWidth;
  UINTN *WidthParse;
  CHAR16 Pad;
  BOOLEAN PadBefore;
  BOOLEAN Comma;
  BOOLEAN Long;
} PRINT_ITEM;

typedef struct _pstate {
  POINTER fmt;
  va_list args;

  CHAR16 *Buffer;
  CHAR16 *End;
  CHAR16 *Pos;
  UINTN Len;

  UINTN Attr;
  UINTN RestoreAttr;

  UINTN AttrNorm;
  UINTN AttrHighlight;
  UINTN AttrError;

  INTN(EFIAPI *Output)(VOID *context, CHAR16 *str);
  INTN(EFIAPI *SetAttr)(VOID *context, UINTN attr);
  VOID *Context;

  struct _pitem *Item;
} PRINT_STATE;

static struct {
  EFI_STATUS Code;
  WCHAR *Desc;
} ErrorCodeTable[] = {{EFI_SUCCESS, L"Success"},
                      {EFI_LOAD_ERROR, L"Load Error"},
                      {EFI_INVALID_PARAMETER, L"Invalid Parameter"},
                      {EFI_UNSUPPORTED, L"Unsupported"},
                      {EFI_BAD_BUFFER_SIZE, L"Bad Buffer Size"},
                      {EFI_BUFFER_TOO_SMALL, L"Buffer Too Small"},
                      {EFI_NOT_READY, L"Not Ready"},
                      {EFI_DEVICE_ERROR, L"Device Error"},
                      {EFI_WRITE_PROTECTED, L"Write Protected"},
                      {EFI_OUT_OF_RESOURCES, L"Out of Resources"},
                      {EFI_VOLUME_CORRUPTED, L"Volume Corrupt"},
                      {EFI_VOLUME_FULL, L"Volume Full"},
                      {EFI_NO_MEDIA, L"No Media"},
                      {EFI_MEDIA_CHANGED, L"Media changed"},
                      {EFI_NOT_FOUND, L"Not Found"},
                      {EFI_ACCESS_DENIED, L"Access Denied"},
                      {EFI_NO_RESPONSE, L"No Response"},
                      {EFI_NO_MAPPING, L"No mapping"},
                      {EFI_TIMEOUT, L"Time out"},
                      {EFI_NOT_STARTED, L"Not started"},
                      {EFI_ALREADY_STARTED, L"Already started"},
                      {EFI_ABORTED, L"Aborted"},
                      {EFI_ICMP_ERROR, L"ICMP Error"},
                      {EFI_TFTP_ERROR, L"TFTP Error"},
                      {EFI_PROTOCOL_ERROR, L"Protocol Error"},
                      {EFI_INCOMPATIBLE_VERSION, L"Incompatible Version"},
                      {EFI_SECURITY_VIOLATION, L"Security Policy Violation"},
                      {EFI_CRC_ERROR, L"CRC Error"},
                      {EFI_END_OF_MEDIA, L"End of Media"},
                      {EFI_END_OF_FILE, L"End of File"},
                      {EFI_INVALID_LANGUAGE, L"Invalid Languages"},
                      {EFI_COMPROMISED_DATA, L"Compromised Data"},

                      // warnings
                      {EFI_WARN_UNKNOWN_GLYPH, L"Warning Unknown Glyph"},
                      {EFI_WARN_DELETE_FAILURE, L"Warning Delete Failure"},
                      {EFI_WARN_WRITE_FAILURE, L"Warning Write Failure"},
                      {EFI_WARN_BUFFER_TOO_SMALL, L"Warning Buffer Too Small"},
                      {0, NULL}};

STATIC CHAR8 Hex[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                      '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

static inline INTN _SPrint(IN VOID *Context, IN CHAR16 *Buffer);

static inline INTN _PoolPrint(IN VOID *Context, IN CHAR16 *Buffer);

static inline UINTN _Print(IN PRINT_STATE *ps);

static inline UINT64 DivU64x32_ext(IN UINT64 Dividend, IN UINTN Divisor,
                                   OUT UINTN *Remainder OPTIONAL) {
  if (Remainder)
    *Remainder = Dividend % Divisor;
  return Dividend / Divisor;
}

static inline INTN IsLocalPrint(void *func) {
  if (func == _SPrint || func == _PoolPrint)
    return 1;
  return 0;
}

static inline VOID ValueToString(IN CHAR16 *Buffer, IN BOOLEAN Comma,
                                 IN INT64 v) {
  STATIC CHAR8 ca[] = {3, 1, 2};
  CHAR8 str[40], *p1;
  CHAR16 *p2;
  UINTN c, r;

  if (!v) {
    Buffer[0] = '0';
    Buffer[1] = 0;
    return;
  }

  p1 = str;
  p2 = Buffer;

  if (v < 0) {
    *(p2++) = '-';
    v = -v;
  }

  while (v) {
    v = (INT64)DivU64x32_ext((UINT64)v, 10, &r);
    *(p1++) = (CHAR8)r + '0';
  }

  c = (UINTN)(Comma ? ca[(p1 - str) % 3] : 999) + 1;
  while (p1 != str) {

    c -= 1;
    if (!c) {
      *(p2++) = ',';
      c = 3;
    }

    *(p2++) = *(--p1);
  }
  *p2 = 0;
}

static inline INTN _SPrint(IN VOID *Context, IN CHAR16 *Buffer) {
  UINTN len;
  POOL_PRINT *spc;

  spc = Context;
  len = StrLen(Buffer);

  if (spc->len + len > spc->maxlen) {
    len = spc->maxlen - spc->len;
  }

  CopyMem(spc->str + spc->len, Buffer, len * sizeof(CHAR16));
  spc->len += len;

  if (spc->len < spc->maxlen) {
    spc->str[spc->len] = 0;
  } else if (spc->maxlen) {
    spc->str[spc->maxlen] = 0;
  }

  return 0;
}

static inline INTN _PoolPrint(IN VOID *Context, IN CHAR16 *Buffer) {
  UINTN newlen;
  POOL_PRINT *spc;

  spc = Context;
  newlen = spc->len + StrLen(Buffer) + 1;

  if (newlen > spc->maxlen) {
    newlen += PRINT_STRING_LEN;
    spc->maxlen = newlen;
    spc->str = ReallocatePool(spc->str, spc->len * sizeof(CHAR16),
                              spc->maxlen * sizeof(CHAR16));

    if (!spc->str) {
      spc->len = 0;
      spc->maxlen = 0;
    }
  }

  return _SPrint(Context, Buffer);
}

static inline VOID
_PoolCatPrint(IN CONST CHAR16 *fmt, IN va_list args, IN OUT POOL_PRINT *spc,
              IN INTN(EFIAPI *Output)(VOID *context, CHAR16 *str)) {
  PRINT_STATE ps;

  ZeroMem(&ps, sizeof(ps));
  ps.Output = Output;
  ps.Context = spc;
  ps.fmt.pw = fmt;
  va_copy(ps.args, args);
  _Print(&ps);
  va_end(ps.args);
}

static inline UINTN UnicodeVSPrint(OUT CHAR16 *Str, IN UINTN StrSize,
                                   IN CONST CHAR16 *fmt, va_list args) {
  POOL_PRINT spc;

  spc.str = Str;
  spc.maxlen = StrSize / sizeof(CHAR16) - 1;
  spc.len = 0;

  _PoolCatPrint(fmt, args, &spc, _SPrint);

  return spc.len;
}

static inline UINTN UnicodeSPrint(OUT CHAR16 *Str, IN UINTN StrSize,
                                  IN CONST CHAR16 *fmt, ...) {
  va_list args;
  UINTN len;

  va_start(args, fmt);
  len = UnicodeVSPrint(Str, StrSize, fmt, args);
  va_end(args);

  return len;
}

static inline VOID ValueToHex(IN CHAR16 *Buffer, IN UINT64 v) {
  CHAR8 str[30], *p1;
  CHAR16 *p2;

  if (!v) {
    Buffer[0] = '0';
    Buffer[1] = 0;
    return;
  }

  p1 = str;
  p2 = Buffer;

  while (v) {
    *(p1++) = Hex[(UINTN)(v & 0xf)];
    v = RShiftU64(v, 4);
  }

  while (p1 != str) {
    *(p2++) = *(--p1);
  }
  *p2 = 0;
}

static inline VOID FloatToString(IN CHAR16 *Buffer, IN BOOLEAN Comma,
                                 IN double v) {
  INTN i = (INTN)v;
  ValueToString(Buffer, Comma, i);

  UINTN x = StrLen(Buffer);
  Buffer[x] = L'.';
  x++;

  float f = (float)(v - i);
  if (f < 0)
    f = -f;

  f *= 10.0;
  while ((f != 0) && ((INTN)f == 0)) {
    Buffer[x] = L'0';
    x++;
    f *= 10.0;
  }

  while ((float)(INTN)f != f) {
    f *= 10;
  }
  ValueToString(Buffer + x, FALSE, (INTN)f);
  return;
}

static inline VOID TimeToString(OUT CHAR16 *Buffer, IN EFI_TIME *Time) {
  UINTN Hour, Year;
  CHAR16 AmPm;

  AmPm = 'a';
  Hour = Time->Hour;
  if (Time->Hour == 0) {
    Hour = 12;
  } else if (Time->Hour >= 12) {
    AmPm = 'p';
    if (Time->Hour >= 13) {
      Hour -= 12;
    }
  }

  Year = Time->Year % 100;

  UnicodeSPrint(Buffer, 0, L"%02d/%02d/%02d  %02d:%02d%c", Time->Month,
                Time->Day, Year, Hour, Time->Minute, AmPm);
}

static inline VOID StatusToString(OUT CHAR16 *Buffer, IN EFI_STATUS Status) {
  UINTN Index;

  for (Index = 0; ErrorCodeTable[Index].Desc; Index += 1) {
    if (ErrorCodeTable[Index].Code == Status) {
      StrCpy(Buffer, ErrorCodeTable[Index].Desc);
      return;
    }
  }

  UnicodeSPrint(Buffer, 0, L"%X", Status);
}

static inline VOID PFLUSH(IN OUT PRINT_STATE *ps) {
  *ps->Pos = 0;
  if (IsLocalPrint(ps->Output))
    ps->Output(ps->Context, ps->Buffer);
  else
    uefi_call_wrapper(ps->Output, 2, ps->Context, ps->Buffer);
  ps->Pos = ps->Buffer;
}

static inline VOID PSETATTR(IN OUT PRINT_STATE *ps, IN UINTN Attr) {
  PFLUSH(ps);

  ps->RestoreAttr = ps->Attr;
  if (ps->SetAttr) {
    uefi_call_wrapper(ps->SetAttr, 2, ps->Context, Attr);
  }

  ps->Attr = Attr;
}

static inline VOID PPUTC(IN OUT PRINT_STATE *ps, IN CHAR16 c) {
  // if this is a newline, add a carraige return
  if (c == '\n') {
    PPUTC(ps, '\r');
  }

  *ps->Pos = c;
  ps->Pos += 1;
  ps->Len += 1;

  // if at the end of the buffer, flush it
  if (ps->Pos >= ps->End) {
    PFLUSH(ps);
  }
}

static inline CHAR16 PGETC(IN POINTER *p) {
  CHAR16 c;

  c = p->Ascii ? p->pc[p->Index] : p->pw[p->Index];
  p->Index += 1;

  return c;
}

static inline VOID PITEM(IN OUT PRINT_STATE *ps) {
  UINTN Len, i;
  PRINT_ITEM *Item;
  CHAR16 c;

  Item = ps->Item;
  Item->Item.Index = 0;
  while (Item->Item.Index < Item->FieldWidth) {
    c = PGETC(&Item->Item);
    if (!c) {
      Item->Item.Index -= 1;
      break;
    }
  }
  Len = Item->Item.Index;

  if (Item->FieldWidth == (UINTN)-1) {
    Item->FieldWidth = Len;
  }

  if (Len > Item->Width) {
    Item->Width = Len;
  }

  if (Item->PadBefore) {
    for (i = Item->Width; i < Item->FieldWidth; i += 1) {
      PPUTC(ps, ' ');
    }
  }

  for (i = Len; i < Item->Width; i++) {
    PPUTC(ps, Item->Pad);
  }

  Item->Item.Index = 0;
  while (Item->Item.Index < Len) {
    PPUTC(ps, PGETC(&Item->Item));
  }

  if (!Item->PadBefore) {
    for (i = Item->Width; i < Item->FieldWidth; i += 1) {
      PPUTC(ps, ' ');
    }
  }
}

static inline INTN CompareGuid(IN EFI_GUID *Guid1, IN EFI_GUID *Guid2) {
  INT32 *g1, *g2, r;

  g1 = (INT32 *)Guid1;
  g2 = (INT32 *)Guid2;

  r = g1[0] - g2[0];
  r |= g1[1] - g2[1];
  r |= g1[2] - g2[2];
  r |= g1[3] - g2[3];

  return r;
}

static inline VOID GuidToString(OUT CHAR16 *Buffer, IN EFI_GUID *Guid) {

  UINTN Index;

  for (Index = 0; KnownGuids[Index].Guid; Index++) {
    if (CompareGuid(Guid, KnownGuids[Index].Guid) == 0) {
      UnicodeSPrint(Buffer, 0, KnownGuids[Index].GuidName);
      return;
    }
  }

  UnicodeSPrint(Buffer, 0, L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                Guid->Data1, Guid->Data2, Guid->Data3, Guid->Data4[0],
                Guid->Data4[1], Guid->Data4[2], Guid->Data4[3], Guid->Data4[4],
                Guid->Data4[5], Guid->Data4[6], Guid->Data4[7]);
}

static inline UINTN _Print(IN PRINT_STATE *ps) {
  CHAR16 c;
  UINTN Attr;
  PRINT_ITEM Item;
  CHAR16 Buffer[PRINT_STRING_LEN];

  ps->Len = 0;
  ps->Buffer = Buffer;
  ps->Pos = Buffer;
  ps->End = Buffer + PRINT_STRING_LEN - 1;
  ps->Item = &Item;

  ps->fmt.Index = 0;
  while ((c = PGETC(&ps->fmt))) {

    if (c != '%') {
      PPUTC(ps, c);
      continue;
    }

    Item.FieldWidth = (UINTN)-1;
    Item.Width = 0;
    Item.WidthParse = &Item.Width;
    Item.Pad = ' ';
    Item.PadBefore = TRUE;
    Item.Comma = FALSE;
    Item.Long = FALSE;
    Item.Item.Ascii = FALSE;
    Item.Item.pw = NULL;
    ps->RestoreAttr = 0;
    Attr = 0;

    while ((c = PGETC(&ps->fmt))) {

      switch (c) {

      case '%':
        Item.Scratch[0] = '%';
        Item.Scratch[1] = 0;
        Item.Item.pw = Item.Scratch;
        break;

      case '0':
        Item.Pad = '0';
        break;

      case '-':
        Item.PadBefore = FALSE;
        break;

      case ',':
        Item.Comma = TRUE;
        break;

      case '.':
        Item.WidthParse = &Item.FieldWidth;
        break;

      case '*':
        *Item.WidthParse = va_arg(ps->args, UINTN);
        break;

      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        *Item.WidthParse = 0;
        do {
          *Item.WidthParse = *Item.WidthParse * 10 + c - '0';
          c = PGETC(&ps->fmt);
        } while (c >= '0' && c <= '9');
        ps->fmt.Index -= 1;
        break;

      case 'a':
        Item.Item.pc = va_arg(ps->args, CHAR8 *);
        Item.Item.Ascii = TRUE;
        if (!Item.Item.pc) {
          Item.Item.pc = (CHAR8 *)"(null)";
        }
        break;

      case 's':
        Item.Item.pw = va_arg(ps->args, CHAR16 *);
        if (!Item.Item.pw) {
          Item.Item.pw = L"(null)";
        }
        break;

      case 'c':
        Item.Scratch[0] = (CHAR16)va_arg(ps->args, UINTN);
        Item.Scratch[1] = 0;
        Item.Item.pw = Item.Scratch;
        break;

      case 'l':
        Item.Long = TRUE;
        break;

      case 'X':
        Item.Width = Item.Long ? 16 : 8;
        Item.Pad = '0';
#if __GNUC__ >= 7
        __attribute__((fallthrough));
#endif
      case 'x':
        ValueToHex(Item.Scratch, Item.Long ? va_arg(ps->args, UINT64)
                                           : va_arg(ps->args, UINT32));
        Item.Item.pw = Item.Scratch;

        break;

      case 'g':
        GuidToString(Item.Scratch, va_arg(ps->args, EFI_GUID *));
        Item.Item.pw = Item.Scratch;
        break;

      case 'u':
        ValueToString(Item.Scratch, Item.Comma,
                      Item.Long ? va_arg(ps->args, UINT64)
                                : va_arg(ps->args, UINT32));
        Item.Item.pw = Item.Scratch;
        break;

      case 'd':
        ValueToString(Item.Scratch, Item.Comma,
                      Item.Long ? va_arg(ps->args, INT64)
                                : va_arg(ps->args, INT32));
        Item.Item.pw = Item.Scratch;
        break;

      case 'f':
        FloatToString(Item.Scratch, Item.Comma, va_arg(ps->args, double));
        Item.Item.pw = Item.Scratch;
        break;

      case 't':
        TimeToString(Item.Scratch, va_arg(ps->args, EFI_TIME *));
        Item.Item.pw = Item.Scratch;
        break;

      case 'r':
        StatusToString(Item.Scratch, va_arg(ps->args, EFI_STATUS));
        Item.Item.pw = Item.Scratch;
        break;

      case 'n':
        PSETATTR(ps, ps->AttrNorm);
        break;

      case 'h':
        PSETATTR(ps, ps->AttrHighlight);
        break;

      case 'e':
        PSETATTR(ps, ps->AttrError);
        break;

      case 'N':
        Attr = ps->AttrNorm;
        break;

      case 'H':
        Attr = ps->AttrHighlight;
        break;

      case 'E':
        Attr = ps->AttrError;
        break;

      default:
        Item.Scratch[0] = '?';
        Item.Scratch[1] = 0;
        Item.Item.pw = Item.Scratch;
        break;
      }

      if (Item.Item.pw) {
        PITEM(ps);
        break;
      }

      if (Attr) {
        PSETATTR(ps, Attr);
        ps->RestoreAttr = 0;
        break;
      }
    }

    if (ps->RestoreAttr) {
      PSETATTR(ps, ps->RestoreAttr);
    }
  }

  PFLUSH(ps);
  return ps->Len;
}

static inline CHAR16 *VPoolPrint(IN CONST CHAR16 *fmt, va_list args) {
  POOL_PRINT spc;
  ZeroMem(&spc, sizeof(spc));
  _PoolCatPrint(fmt, args, &spc, _PoolPrint);
  return spc.str;
}

static inline CHAR16 *PoolPrint(IN CONST CHAR16 *fmt, ...) {
  va_list args;
  CHAR16 *pool;
  va_start(args, fmt);
  pool = VPoolPrint(fmt, args);
  va_end(args);
  return pool;
}

static inline UINTN _IPrint(IN UINTN Column, IN UINTN Row,
                            IN SIMPLE_TEXT_OUTPUT_INTERFACE *Out,
                            IN CONST CHAR16 *fmt, IN CONST CHAR8 *fmta,
                            IN va_list args) {
  PRINT_STATE ps;
  UINTN back;

  ZeroMem(&ps, sizeof(ps));
  ps.Context = Out;
  ps.Output = (INTN(EFIAPI *)(VOID *, CHAR16 *))Out->OutputString;
  ps.SetAttr = (INTN(EFIAPI *)(VOID *, UINTN))Out->SetAttribute;
  ps.Attr = Out->Mode->Attribute;

  back = (ps.Attr >> 4) & 0xF;
  ps.AttrNorm = EFI_TEXT_ATTR(EFI_LIGHTGRAY, back);
  ps.AttrHighlight = EFI_TEXT_ATTR(EFI_WHITE, back);
  ps.AttrError = EFI_TEXT_ATTR(EFI_YELLOW, back);

  if (fmt) {
    ps.fmt.pw = fmt;
  } else {
    ps.fmt.Ascii = TRUE;
    ps.fmt.pc = fmta;
  }

  va_copy(ps.args, args);

  if (Column != (UINTN)-1) {
    uefi_call_wrapper(Out->SetCursorPosition, 3, Out, Column, Row);
  }

  back = _Print(&ps);
  va_end(ps.args);
  return back;
}

static inline UINTN VPrint(IN CONST CHAR16 *fmt, va_list args) {
  return _IPrint((UINTN)-1, (UINTN)-1, ST->ConOut, fmt, NULL, args);
}

static inline CHAR16 *CatPrint(IN OUT POOL_PRINT *Str, IN CONST CHAR16 *fmt,
                               ...) {
  va_list args;

  va_start(args, fmt);
  _PoolCatPrint(fmt, args, Str, _PoolPrint);
  va_end(args);
  return Str->str;
}

static inline UINTN Print(IN CONST CHAR16 *fmt, ...) {
  va_list args;
  UINTN back;

  va_start(args, fmt);
  back = _IPrint((UINTN)-1, (UINTN)-1, ST->ConOut, fmt, NULL, args);
  va_end(args);
  return back;
}

#define SPrint UnicodeSPrint

#endif
