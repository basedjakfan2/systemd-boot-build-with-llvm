#ifndef _EFI_DPATH_H
#define _EFI_DPATH_H

#define ALIGN_SIZE(a)                                                          \
  ((a % MIN_ALIGNMENT_SIZE) ? MIN_ALIGNMENT_SIZE - (a % MIN_ALIGNMENT_SIZE) : 0)

typedef struct _UKNOWN_DEVICE_VENDOR_DP {
  VENDOR_DEVICE_PATH DevicePath;
  UINT8 LegacyDriveLetter;
} UNKNOWN_DEVICE_VENDOR_DEVICE_PATH;

static inline VOID CatPrintIPv4(IN OUT POOL_PRINT *Str,
                                IN EFI_IPv4_ADDRESS *Address) {
  CatPrint(Str, L"%d.%d.%d.%d", Address->Addr[0], Address->Addr[1],
           Address->Addr[2], Address->Addr[3]);
}

static inline VOID CatPrintNetworkProtocol(IN OUT POOL_PRINT *Str,
                                           IN UINT16 Proto) {
  if (Proto == 6) {
    CatPrint(Str, L"TCP");
  } else if (Proto == 17) {
    CatPrint(Str, L"UDP");
  } else {
    CatPrint(Str, L"%d", Proto);
  }
}

#define CatPrintIPv6_ADD(x, y) (((UINT16)(x)) << 8 | (y))
static inline VOID CatPrintIPv6(IN OUT POOL_PRINT *Str,
                                IN EFI_IPv6_ADDRESS *Address) {
  CatPrint(Str, L"%x:%x:%x:%x:%x:%x:%x:%x",
           CatPrintIPv6_ADD(Address->Addr[0], Address->Addr[1]),
           CatPrintIPv6_ADD(Address->Addr[2], Address->Addr[3]),
           CatPrintIPv6_ADD(Address->Addr[4], Address->Addr[5]),
           CatPrintIPv6_ADD(Address->Addr[6], Address->Addr[7]),
           CatPrintIPv6_ADD(Address->Addr[8], Address->Addr[9]),
           CatPrintIPv6_ADD(Address->Addr[10], Address->Addr[11]),
           CatPrintIPv6_ADD(Address->Addr[12], Address->Addr[13]),
           CatPrintIPv6_ADD(Address->Addr[14], Address->Addr[15]));
}

static inline BOOLEAN IsNotNullIPv4(IN EFI_IPv4_ADDRESS *Address) {
  UINT8 val;
  val = Address->Addr[0] | Address->Addr[1];
  val |= Address->Addr[2] | Address->Addr[3];
  return val != 0;
}

static inline VOID _DevPathPci(IN OUT POOL_PRINT *Str, IN VOID *DevPath) {
  PCI_DEVICE_PATH *Pci;

  Pci = DevPath;
  CatPrint(Str, L"Pci(0x%x,0x%x)", Pci->Device, Pci->Function);
}

static inline VOID _DevPathPccard(IN OUT POOL_PRINT *Str, IN VOID *DevPath) {
  PCCARD_DEVICE_PATH *Pccard;

  Pccard = DevPath;
  CatPrint(Str, L"Pccard(0x%x)", Pccard->FunctionNumber);
}

static inline VOID _DevPathMemMap(IN OUT POOL_PRINT *Str, IN VOID *DevPath) {
  MEMMAP_DEVICE_PATH *MemMap;

  MemMap = DevPath;
  CatPrint(Str, L"MemMap(%d,0x%x,0x%x)", MemMap->MemoryType,
           MemMap->StartingAddress, MemMap->EndingAddress);
}

static inline VOID _DevPathController(IN OUT POOL_PRINT *Str,
                                      IN VOID *DevPath) {
  CONTROLLER_DEVICE_PATH *Controller;

  Controller = DevPath;
}

static inline VOID _DevPathVendor(IN OUT POOL_PRINT *Str, IN VOID *DevPath) {
  VENDOR_DEVICE_PATH *Vendor;
  CHAR16 *Type;
  UNKNOWN_DEVICE_VENDOR_DEVICE_PATH *UnknownDevPath;

  Vendor = DevPath;
  switch (DevicePathType(&Vendor->Header)) {
  case HARDWARE_DEVICE_PATH:
    Type = L"Hw";
    break;
  case MESSAGING_DEVICE_PATH:
    Type = L"Msg";
    break;
  case MEDIA_DEVICE_PATH:
    Type = L"Media";
    break;
  default:
    Type = L"?";
    break;
  }

  CatPrint(Str, L"Ven%s(%g", Type, &Vendor->Guid);
  if (CompareGuid(&Vendor->Guid, &UnknownDevice) == 0) {
    //
    // GUID used by EFI to enumerate an EDD 1.1 device
    //
    UnknownDevPath = (UNKNOWN_DEVICE_VENDOR_DEVICE_PATH *)Vendor;
    CatPrint(Str, L":%02x)", UnknownDevPath->LegacyDriveLetter);
  } else {
    CatPrint(Str, L")");
  }
}

/*
  Type: 2 (ACPI Device Path) SubType: 1 (ACPI Device Path)
 */
static inline VOID _DevPathAcpi(IN OUT POOL_PRINT *Str, IN VOID *DevPath) {
  ACPI_HID_DEVICE_PATH *Acpi;

  Acpi = DevPath;
  if ((Acpi->HID & PNP_EISA_ID_MASK) == PNP_EISA_ID_CONST) {
    switch (EISA_ID_TO_NUM(Acpi->HID)) {
    case 0x301: {
      CatPrint(Str, L"Keyboard(%d)", Acpi->UID);
      break;
    }
    case 0x401: {
      CatPrint(Str, L"ParallelPort(%d)", Acpi->UID);
      break;
    }
    case 0x501: {
      CatPrint(Str, L"Serial(%d)", Acpi->UID);
      break;
    }
    case 0x604: {
      CatPrint(Str, L"Floppy(%d)", Acpi->UID);
      break;
    }
    case 0xa03: {
      CatPrint(Str, L"PciRoot(%d)", Acpi->UID);
      break;
    }
    case 0xa08: {
      CatPrint(Str, L"PcieRoot(%d)", Acpi->UID);
      break;
    }
    default: {
      CatPrint(Str, L"Acpi(PNP%04x", EISA_ID_TO_NUM(Acpi->HID));
      if (Acpi->UID)
        CatPrint(Str, L",%d", Acpi->UID);
      CatPrint(Str, L")");
      break;
    }
    }
  } else {
    CatPrint(Str, L"Acpi(0x%X", Acpi->HID);
    if (Acpi->UID)
      CatPrint(Str, L",%d", Acpi->UID);
    CatPrint(Str, L")", Acpi->HID, Acpi->UID);
  }
}

static inline VOID _DevPathAtapi(IN OUT POOL_PRINT *Str, IN VOID *DevPath) {
  ATAPI_DEVICE_PATH *Atapi;

  Atapi = DevPath;
  CatPrint(Str, L"Ata(%s,%s)",
           Atapi->PrimarySecondary ? L"Secondary" : L"Primary",
           Atapi->SlaveMaster ? L"Slave" : L"Master");
}

static inline VOID _DevPathScsi(IN OUT POOL_PRINT *Str, IN VOID *DevPath) {
  SCSI_DEVICE_PATH *Scsi;

  Scsi = DevPath;
  CatPrint(Str, L"Scsi(%d,%d)", Scsi->Pun, Scsi->Lun);
}

static inline VOID _DevPathFibre(IN OUT POOL_PRINT *Str, IN VOID *DevPath) {
  FIBRECHANNEL_DEVICE_PATH *Fibre;

  Fibre = DevPath;
  CatPrint(Str, L"Fibre%s(0x%016lx,0x%016lx)",
           DevicePathType(&Fibre->Header) == MSG_FIBRECHANNEL_DP ? L"" : L"Ex",
           Fibre->WWN, Fibre->Lun);
}

static inline VOID _DevPath1394(IN OUT POOL_PRINT *Str, IN VOID *DevPath) {
  F1394_DEVICE_PATH *F1394;

  F1394 = DevPath;
  // Guid has format of IEEE-EUI64
  CatPrint(Str, L"I1394(%016lx)", F1394->Guid);
}

static inline VOID _DevPathUsb(IN OUT POOL_PRINT *Str, IN VOID *DevPath) {
  USB_DEVICE_PATH *Usb;

  Usb = DevPath;
}

static inline VOID _DevPathI2O(IN OUT POOL_PRINT *Str, IN VOID *DevPath) {
  I2O_DEVICE_PATH *I2O;

  I2O = DevPath;
  CatPrint(Str, L"I2O(0x%X)", I2O->Tid);
}

static inline VOID _DevPathMacAddr(IN OUT POOL_PRINT *Str, IN VOID *DevPath) {
  MAC_ADDR_DEVICE_PATH *MAC;
  UINTN HwAddressSize;
  UINTN Index;

  MAC = DevPath;

  /* HwAddressSize = sizeof(EFI_MAC_ADDRESS); */
  HwAddressSize = DevicePathNodeLength(&MAC->Header);
  HwAddressSize -= sizeof(MAC->Header);
  HwAddressSize -= sizeof(MAC->IfType);
  if (MAC->IfType == 0x01 || MAC->IfType == 0x00) {
    HwAddressSize = 6;
  }

  CatPrint(Str, L"Mac(");

  for (Index = 0; Index < HwAddressSize; Index++) {
    CatPrint(Str, L"%02x", MAC->MacAddress.Addr[Index]);
  }
  if (MAC->IfType != 0) {
    CatPrint(Str, L",%d", MAC->IfType);
  }
  CatPrint(Str, L")");
}

static inline VOID _DevPathIPv4(IN OUT POOL_PRINT *Str, IN VOID *DevPath) {
  IPv4_DEVICE_PATH *IP;
  BOOLEAN show;

  IP = DevPath;
  CatPrint(Str, L"IPv4(");
  CatPrintIPv4(Str, &IP->RemoteIpAddress);
  CatPrint(Str, L",");
  CatPrintNetworkProtocol(Str, IP->Protocol);
  CatPrint(Str, L",%s", IP->StaticIpAddress ? L"Static" : L"DHCP");
  show = IsNotNullIPv4(&IP->LocalIpAddress);
  if (!show && DevicePathNodeLength(&IP->Header) == sizeof(IPv4_DEVICE_PATH)) {
    /* only version 2 includes gateway and netmask */
    show |= IsNotNullIPv4(&IP->GatewayIpAddress);
    show |= IsNotNullIPv4(&IP->SubnetMask);
  }
  if (show) {
    CatPrint(Str, L",");
    CatPrintIPv4(Str, &IP->LocalIpAddress);
    if (DevicePathNodeLength(&IP->Header) == sizeof(IPv4_DEVICE_PATH)) {
      /* only version 2 includes gateway and netmask */
      show = IsNotNullIPv4(&IP->GatewayIpAddress);
      show |= IsNotNullIPv4(&IP->SubnetMask);
      if (show) {
        CatPrint(Str, L",");
        CatPrintIPv4(Str, &IP->GatewayIpAddress);
        if (IsNotNullIPv4(&IP->SubnetMask)) {
          CatPrint(Str, L",");
          CatPrintIPv4(Str, &IP->SubnetMask);
        }
      }
    }
  }
  CatPrint(Str, L")");
}

static inline VOID _DevPathIPv6(IN OUT POOL_PRINT *Str, IN VOID *DevPath) {
  IPv6_DEVICE_PATH *IP;

  IP = DevPath;
  CatPrint(Str, L"IPv6(");
  CatPrintIPv6(Str, &IP->RemoteIpAddress);
  CatPrint(Str, L",");
  CatPrintNetworkProtocol(Str, IP->Protocol);
  CatPrint(Str, L",%s,",
           IP->IpAddressOrigin
               ? (IP->IpAddressOrigin == 1 ? L"StatelessAutoConfigure"
                                           : L"StatefulAutoConfigure")
               : L"Static");
  CatPrintIPv6(Str, &IP->LocalIpAddress);
  if (DevicePathNodeLength(&IP->Header) == sizeof(IPv6_DEVICE_PATH)) {
    CatPrint(Str, L",");
    CatPrintIPv6(Str, &IP->GatewayIpAddress);
    CatPrint(Str, L",");
    CatPrint(Str, L"%d", &IP->PrefixLength);
  }
  CatPrint(Str, L")");
}

static inline VOID _DevPathUri(IN OUT POOL_PRINT *Str, IN VOID *DevPath) {
  URI_DEVICE_PATH *Uri;

  Uri = DevPath;

  CatPrint(Str, L"Uri(%a)", Uri->Uri);
}

static inline VOID _DevPathInfiniBand(IN OUT POOL_PRINT *Str,
                                      IN VOID *DevPath) {
  INFINIBAND_DEVICE_PATH *InfiniBand;

  InfiniBand = DevPath;
  CatPrint(Str, L"Infiniband(0x%x,%g,0x%lx,0x%lx,0x%lx)",
           InfiniBand->ResourceFlags, InfiniBand->PortGid,
           InfiniBand->ServiceId, InfiniBand->TargetPortId,
           InfiniBand->DeviceId);
}

static inline VOID _DevPathUart(IN OUT POOL_PRINT *Str, IN VOID *DevPath) {
  UART_DEVICE_PATH *Uart;
  CHAR8 Parity;

  Uart = DevPath;
  switch (Uart->Parity) {
  case 0:
    Parity = 'D';
    break;
  case 1:
    Parity = 'N';
    break;
  case 2:
    Parity = 'E';
    break;
  case 3:
    Parity = 'O';
    break;
  case 4:
    Parity = 'M';
    break;
  case 5:
    Parity = 'S';
    break;
  default:
    Parity = 'x';
    break;
  }

  if (Uart->BaudRate == 0) {
    CatPrint(Str, L"Uart(DEFAULT,");
  } else {
    CatPrint(Str, L"Uart(%ld,", Uart->BaudRate);
  }

  if (Uart->DataBits == 0) {
    CatPrint(Str, L"DEFAULT,");
  } else {
    CatPrint(Str, L"%d,", Uart->DataBits);
  }

  CatPrint(Str, L"%c,", Parity);

  switch (Uart->StopBits) {
  case 0:
    CatPrint(Str, L"D)");
    break;
  case 1:
    CatPrint(Str, L"1)");
    break;
  case 2:
    CatPrint(Str, L"1.5)");
    break;
  case 3:
    CatPrint(Str, L"2)");
    break;
  default:
    CatPrint(Str, L"x)");
    break;
  }
}

static inline VOID _DevPathSata(IN OUT POOL_PRINT *Str, IN VOID *DevPath) {
  SATA_DEVICE_PATH *Sata;

  Sata = DevPath;
  CatPrint(Str, L"Sata(0x%x,0x%x,0x%x)", Sata->HBAPortNumber,
           Sata->PortMultiplierPortNumber, Sata->Lun);
}

static inline VOID _DevPathHardDrive(IN OUT POOL_PRINT *Str, IN VOID *DevPath) {
  HARDDRIVE_DEVICE_PATH *Hd;

  Hd = DevPath;
  switch (Hd->SignatureType) {
  case SIGNATURE_TYPE_MBR:
    CatPrint(Str, L"HD(%d,MBR,0x%08x)", Hd->PartitionNumber,
             *((UINT32 *)(&(Hd->Signature[0]))));
    break;
  case SIGNATURE_TYPE_GUID:
    CatPrint(Str, L"HD(%d,GPT,%g)", Hd->PartitionNumber,
             (EFI_GUID *)&(Hd->Signature[0]));
    break;
  default:
    CatPrint(Str, L"HD(%d,%d,0)", Hd->PartitionNumber, Hd->SignatureType);
    break;
  }
}

static inline VOID _DevPathCDROM(IN OUT POOL_PRINT *Str, IN VOID *DevPath) {
  CDROM_DEVICE_PATH *Cd;

  Cd = DevPath;
  CatPrint(Str, L"CDROM(0x%x)", Cd->BootEntry);
}

static inline VOID _DevPathFilePath(IN OUT POOL_PRINT *Str, IN VOID *DevPath) {
  FILEPATH_DEVICE_PATH *Fp;

  Fp = DevPath;
  CatPrint(Str, L"%s", Fp->PathName);
}

static inline VOID _DevPathMediaProtocol(IN OUT POOL_PRINT *Str,
                                         IN VOID *DevPath) {
  MEDIA_PROTOCOL_DEVICE_PATH *MediaProt;

  MediaProt = DevPath;
  CatPrint(Str, L"%g", &MediaProt->Protocol);
}

static inline VOID _DevPathBssBss(IN OUT POOL_PRINT *Str, IN VOID *DevPath) {
  BBS_BBS_DEVICE_PATH *Bss;
  CHAR16 *Type;

  Bss = DevPath;
  switch (Bss->DeviceType) {
  case BBS_TYPE_FLOPPY:
    Type = L"Floppy";
    break;
  case BBS_TYPE_HARDDRIVE:
    Type = L"Harddrive";
    break;
  case BBS_TYPE_CDROM:
    Type = L"CDROM";
    break;
  case BBS_TYPE_PCMCIA:
    Type = L"PCMCIA";
    break;
  case BBS_TYPE_USB:
    Type = L"Usb";
    break;
  case BBS_TYPE_EMBEDDED_NETWORK:
    Type = L"Net";
    break;
  default:
    Type = L"?";
    break;
  }

  CatPrint(Str, L"Bss-%s(%a)", Type, Bss->String);
}

static inline VOID _DevPathEndInstance(IN OUT POOL_PRINT *Str,
                                       IN VOID *DevPath EFI_UNUSED) {
  CatPrint(Str, L",");
}

/**
 * Print unknown device node.
 * UEFI 2.4 § 9.6.1.6 table 89.
 */

static inline VOID _DevPathNodeUnknown(IN OUT POOL_PRINT *Str,
                                       IN VOID *DevPath) {
  EFI_DEVICE_PATH *Path;
  UINT8 *value;
  int length, index;
  Path = DevPath;
  value = DevPath;
  value += 4;
  switch (Path->Type) {
  case HARDWARE_DEVICE_PATH: { /* Unknown Hardware Device Path */
    CatPrint(Str, L"HardwarePath(%d", Path->SubType);
    break;
  }
  case ACPI_DEVICE_PATH: { /* Unknown ACPI Device Path */
    CatPrint(Str, L"AcpiPath(%d", Path->SubType);
    break;
  }
  case MESSAGING_DEVICE_PATH: { /* Unknown Messaging Device Path */
    CatPrint(Str, L"Msg(%d", Path->SubType);
    break;
  }
  case MEDIA_DEVICE_PATH: { /* Unknown Media Device Path */
    CatPrint(Str, L"MediaPath(%d", Path->SubType);
    break;
  }
  case BBS_DEVICE_PATH: { /* Unknown BIOS Boot Specification Device Path */
    CatPrint(Str, L"BbsPath(%d", Path->SubType);
    break;
  }
  default: { /* Unknown Device Path */
    CatPrint(Str, L"Path(%d,%d", Path->Type, Path->SubType);
    break;
  }
  }
  length = DevicePathNodeLength(Path);
  for (index = 0; index < length; index++) {
    if (index == 0)
      CatPrint(Str, L",0x");
    CatPrint(Str, L"%02x", *value);
    value++;
  }
  CatPrint(Str, L")");
}

static struct {
  UINT8 Type;
  UINT8 SubType;
  VOID (*Function)(POOL_PRINT *, VOID *);
} DevPathTable[] = {
    {HARDWARE_DEVICE_PATH, HW_PCI_DP, _DevPathPci},
    {HARDWARE_DEVICE_PATH, HW_PCCARD_DP, _DevPathPccard},
    {HARDWARE_DEVICE_PATH, HW_MEMMAP_DP, _DevPathMemMap},
    {HARDWARE_DEVICE_PATH, HW_VENDOR_DP, _DevPathVendor},
    {HARDWARE_DEVICE_PATH, HW_CONTROLLER_DP, _DevPathController},
    {ACPI_DEVICE_PATH, ACPI_DP, _DevPathAcpi},
    {MESSAGING_DEVICE_PATH, MSG_ATAPI_DP, _DevPathAtapi},
    {MESSAGING_DEVICE_PATH, MSG_SCSI_DP, _DevPathScsi},
    {MESSAGING_DEVICE_PATH, MSG_FIBRECHANNEL_DP, _DevPathFibre},
    {MESSAGING_DEVICE_PATH, MSG_1394_DP, _DevPath1394},
    {MESSAGING_DEVICE_PATH, MSG_USB_DP, _DevPathUsb},
    {MESSAGING_DEVICE_PATH, MSG_I2O_DP, _DevPathI2O},
    {MESSAGING_DEVICE_PATH, MSG_MAC_ADDR_DP, _DevPathMacAddr},
    {MESSAGING_DEVICE_PATH, MSG_IPv4_DP, _DevPathIPv4},
    {MESSAGING_DEVICE_PATH, MSG_IPv6_DP, _DevPathIPv6},
    {MESSAGING_DEVICE_PATH, MSG_URI_DP, _DevPathUri},
    {MESSAGING_DEVICE_PATH, MSG_INFINIBAND_DP, _DevPathInfiniBand},
    {MESSAGING_DEVICE_PATH, MSG_UART_DP, _DevPathUart},
    {MESSAGING_DEVICE_PATH, MSG_SATA_DP, _DevPathSata},
    {MESSAGING_DEVICE_PATH, MSG_VENDOR_DP, _DevPathVendor},
    {MEDIA_DEVICE_PATH, MEDIA_HARDDRIVE_DP, _DevPathHardDrive},
    {MEDIA_DEVICE_PATH, MEDIA_CDROM_DP, _DevPathCDROM},
    {MEDIA_DEVICE_PATH, MEDIA_VENDOR_DP, _DevPathVendor},
    {MEDIA_DEVICE_PATH, MEDIA_FILEPATH_DP, _DevPathFilePath},
    {MEDIA_DEVICE_PATH, MEDIA_PROTOCOL_DP, _DevPathMediaProtocol},
    {BBS_DEVICE_PATH, BBS_BBS_DP, _DevPathBssBss},
    {END_DEVICE_PATH_TYPE, END_INSTANCE_DEVICE_PATH_SUBTYPE,
     _DevPathEndInstance},
    {0, 0, NULL}};

static inline EFI_DEVICE_PATH *UnpackDevicePath(IN EFI_DEVICE_PATH *DevPath) {
  EFI_DEVICE_PATH *Src, *Dest, *NewPath;
  UINTN Size;

  Src = DevPath;
  Size = 0;
  for (;;) {
    Size += DevicePathNodeLength(Src);
    Size += ALIGN_SIZE(Size);

    if (IsDevicePathEnd(Src)) {
      break;
    }

    Src = NextDevicePathNode(Src);
  }

  NewPath = AllocateZeroPool(Size);
  if (NewPath) {

    ASSERT(((UINTN)NewPath) % MIN_ALIGNMENT_SIZE == 0);

    Src = DevPath;
    Dest = NewPath;
    for (;;) {
      Size = DevicePathNodeLength(Src);
      CopyMem(Dest, Src, Size);
      Size += ALIGN_SIZE(Size);
      SetDevicePathNodeLength(Dest, Size);
      Dest->Type |= EFI_DP_TYPE_UNPACKED;
      Dest = (EFI_DEVICE_PATH *)(((UINT8 *)Dest) + Size);

      if (IsDevicePathEnd(Src)) {
        break;
      }

      Src = NextDevicePathNode(Src);
    }
  }

  return NewPath;
}

static inline CHAR16 *DevicePathToStr(EFI_DEVICE_PATH *DevPath) {
  POOL_PRINT Str;
  EFI_DEVICE_PATH *DevPathNode;
  VOID (*DumpNode)(POOL_PRINT *, VOID *);
  UINTN Index, NewSize;

  ZeroMem(&Str, sizeof(Str));

  DevPath = UnpackDevicePath(DevPath);
  ASSERT(DevPath);

  DevPathNode = DevPath;
  while (!IsDevicePathEnd(DevPathNode)) {

    DumpNode = NULL;
    for (Index = 0; DevPathTable[Index].Function; Index += 1) {

      if (DevicePathType(DevPathNode) == DevPathTable[Index].Type &&
          DevicePathSubType(DevPathNode) == DevPathTable[Index].SubType) {
        DumpNode = DevPathTable[Index].Function;
        break;
      }
    }

    if (!DumpNode) {
      DumpNode = _DevPathNodeUnknown;
    }

    if (Str.len && DumpNode != _DevPathEndInstance) {
      CatPrint(&Str, L"/");
    }
    DumpNode(&Str, DevPathNode);

    DevPathNode = NextDevicePathNode(DevPathNode);
  }

  FreePool(DevPath);
  NewSize = (Str.len + 1) * sizeof(CHAR16);
  Str.str = ReallocatePool(Str.str, NewSize, NewSize);
  Str.str[Str.len] = 0;
  return Str.str;
}

static inline EFI_DEVICE_PATH *DevicePathFromHandle(IN EFI_HANDLE Handle) {
  EFI_STATUS Status;
  EFI_DEVICE_PATH *DevicePath;

  Status = uefi_call_wrapper(BS->HandleProtocol, 3, Handle, &DevicePathProtocol,
                             (VOID *)&DevicePath);
  if (EFI_ERROR(Status)) {
    DevicePath = NULL;
  }

  return DevicePath;
}

#endif
