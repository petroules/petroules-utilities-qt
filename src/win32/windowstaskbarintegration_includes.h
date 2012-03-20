#if !defined(WINDOWSTASKBARINTEGRATION_INCLUDE_H) && defined(Q_WS_WIN)
#define WINDOWSTASKBARINTEGRATION_INCLUDE_H

#include <windows.h>
#include <initguid.h>
#include <objidl.h>
#include <shellapi.h>
#include <shlobj.h>
#include <shlwapi.h>

// Include this for the interfaces... MinGW doesn't have it at all
// which is basically why we need to redeclare everything we need.
// However this will gracefully degrade for either compiler (MinGW
// or MSVC) if such things as the Windows 7 SDK are not installed
// thereby allowing compilation of this library on Windows XP and Vista,
// or 7 without the Windows 7 SDK installed
#ifdef Q_CC_MSVC
#include <shobjidl.h>
#endif

// For thumb bar
#ifndef IDTB_FIRST
#define IDTB_FIRST 3000
#endif

// For window thumbnail preview
#ifndef WM_DWMSENDICONICTHUMBNAIL
#define WM_DWMSENDICONICTHUMBNAIL 0x0323
#endif

#ifndef WM_DWMSENDICONICLIVEPREVIEWBITMAP
#define WM_DWMSENDICONICLIVEPREVIEWBITMAP 0x0326
#endif

// Definitions for thumb bar - we *assume* that if ITaskbarList3 is defined, then these enums are
#ifndef __ITaskbarList3_FWD_DEFINED__
typedef enum THUMBBUTTONMASK
{
    THB_BITMAP = 0x1,
    THB_ICON = 0x2,
    THB_TOOLTIP	= 0x4,
    THB_FLAGS = 0x8
} THUMBBUTTONMASK;

typedef enum THUMBBUTTONFLAGS
{
    THBF_ENABLED = 0,
    THBF_DISABLED = 0x1,
    THBF_DISMISSONCLICK	= 0x2,
    THBF_NOBACKGROUND = 0x4,
    THBF_HIDDEN	= 0x8,
    THBF_NONINTERACTIVE	= 0x10
} THUMBBUTTONFLAGS;

typedef struct THUMBBUTTON
{
    THUMBBUTTONMASK dwMask;
    UINT iId;
    UINT iBitmap;
    HICON hIcon;
    WCHAR szTip[260];
    THUMBBUTTONFLAGS dwFlags;
} THUMBBUTTON;
typedef struct THUMBBUTTON *LPTHUMBBUTTON;

typedef enum TBPFLAG
{
    TBPF_NOPROGRESS = 0,
    TBPF_INDETERMINATE = 0x1,
    TBPF_NORMAL = 0x2,
    TBPF_ERROR = 0x4,
    TBPF_PAUSED = 0x8
} TBPFLAG;
#endif

#ifndef __ITaskbarList4_INTERFACE_DEFINED__
typedef enum STPFLAG
{
    STPF_NONE = 0,
    STPF_USEAPPTHUMBNAILALWAYS = 0x1,
    STPF_USEAPPTHUMBNAILWHENACTIVE = 0x2,
    STPF_USEAPPPEEKALWAYS = 0x4,
    STPF_USEAPPPEEKWHENACTIVE = 0x8
} STPFLAG;
#endif
/*
typedef enum
{
#ifndef KF_FLAG_DEFAULT
    KF_FLAG_DEFAULT = 0x00000000,
#endif
#ifndef KF_FLAG_CREATE
    KF_FLAG_CREATE = 0x00008000,
#endif
#ifndef KF_FLAG_DONT_VERIFY
    KF_FLAG_DONT_VERIFY = 0x00004000,
#endif
#ifndef KF_FLAG_DONT_UNEXPAND
    KF_FLAG_DONT_UNEXPAND = 0x00002000,
#endif
#ifndef KF_FLAG_NO_ALIAS
    KF_FLAG_NO_ALIAS = 0x00001000,
#endif
#ifndef KF_FLAG_INIT
    KF_FLAG_INIT = 0x00000800,
#endif
#ifndef KF_FLAG_DEFAULT_PATH
    KF_FLAG_DEFAULT_PATH = 0x00000400,
#endif
#ifndef KF_FLAG_NOT_PARENT_RELATIVE
    KF_FLAG_NOT_PARENT_RELATIVE = 0x00000200,
#endif
#ifndef KF_FLAG_SIMPLE_IDLIST
    KF_FLAG_SIMPLE_IDLIST = 0x00000100,
#endif
    KF_FLAG_ALIAS_ONLY = 0x80000000
} KNOWN_FOLDER_FLAG;
*/
#ifndef __ITaskbarList_FWD_DEFINED__
DECLARE_INTERFACE_(ITaskbarList, IUnknown)
{
    STDMETHOD (HrInit) (THIS) PURE;
    STDMETHOD (AddTab) (THIS_ HWND hwnd) PURE;
    STDMETHOD (DeleteTab) (THIS_ HWND hwnd) PURE;
    STDMETHOD (ActivateTab) (THIS_ HWND hwnd) PURE;
    STDMETHOD (SetActiveAlt) (THIS_ HWND hwnd) PURE;
};
typedef ITaskbarList *LPITaskbarList;
#endif

#ifndef __ITaskbarList2_FWD_DEFINED__
DECLARE_INTERFACE_(ITaskbarList2, ITaskbarList)
{
    STDMETHOD (MarkFullscreenWindow) (THIS_ HWND hwnd, int fFullscreen) PURE;
};
typedef ITaskbarList2 *LPITaskbarList2;
#endif

#ifndef __ITaskbarList3_FWD_DEFINED__
DECLARE_INTERFACE_(ITaskbarList3, ITaskbarList2)
{
    STDMETHOD (SetProgressValue) (THIS_ HWND hwnd, ULONGLONG ullCompleted, ULONGLONG ullTotal) PURE;
    STDMETHOD (SetProgressState) (THIS_ HWND hwnd, TBPFLAG tbpFlags) PURE;
    STDMETHOD (RegisterTab) (THIS_ HWND hwndTab, HWND hwndMDI) PURE;
    STDMETHOD (UnregisterTab) (THIS_ HWND hwndTab) PURE;
    STDMETHOD (SetTabOrder) (THIS_ HWND hwndTab, HWND hwndInsertBefore) PURE;
    STDMETHOD (SetTabActive) (THIS_ HWND hwndTab, HWND hwndMDI, DWORD dwReserved) PURE;
    STDMETHOD (ThumbBarAddButtons) (THIS_ HWND hwnd, UINT cButtons, LPTHUMBBUTTON pButton) PURE;
    STDMETHOD (ThumbBarUpdateButtons) (THIS_ HWND hwnd, UINT cButtons, LPTHUMBBUTTON pButton) PURE;
    STDMETHOD (ThumbBarSetImageList) (THIS_ HWND hwnd, HIMAGELIST himl) PURE;
    STDMETHOD (SetOverlayIcon) (THIS_ HWND hwnd, HICON hIcon, LPCWSTR pszDescription) PURE;
    STDMETHOD (SetThumbnailTooltip) (THIS_ HWND hwnd, LPCWSTR pszTip) PURE;
    STDMETHOD (SetThumbnailClip) (THIS_ HWND hwnd, RECT *prcClip) PURE;
};
typedef ITaskbarList3 *LPITaskbarList3;
#endif

#ifndef __ITaskbarList4_FWD_DEFINED__
DECLARE_INTERFACE_(ITaskbarList4, ITaskbarList3)
{
    STDMETHOD (SetTabProperties) (HWND hwndTab, STPFLAG stpFlags) PURE;
};
typedef ITaskbarList4 *LPITaskbarList4;
#endif

// For window thumbnail preview
#define DWMWA_FORCE_ICONIC_REPRESENTATION 7
#define DWMWA_HAS_ICONIC_BITMAP 10
#define DWM_SIT_DISPLAYFRAME 1

extern "C"
{
    typedef HRESULT (WINAPI *t_DwmInvalidateIconicBitmaps)(HWND hwnd);
    typedef HRESULT (WINAPI *t_DwmSetIconicThumbnail)(HWND hwnd, HBITMAP hbmp, DWORD dwSITFlags);
    typedef HRESULT (WINAPI *t_DwmSetWindowAttribute)(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);
    typedef HRESULT (WINAPI *t_DwmSetIconicLivePreviewBitmap)(HWND hwnd, HBITMAP hbmp, POINT *pptClient, DWORD dwSITFlags);
}

void DwmInvalidateIconicBitmaps(HWND hwnd);
void DwmSetIconicThumbnail(HWND hwnd, HBITMAP hbmp, DWORD dwSITFlags);
void DwmSetWindowAttribute(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);
void DwmSetIconicLivePreviewBitmap(HWND hwnd, HBITMAP hbmp, POINT *pptClient, DWORD dwSITFlags);

#endif // WINDOWSTASKBARINTEGRATION_INCLUDE_H
