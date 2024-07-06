#pragma once

#ifndef WM_NCUAHDRAWCAPTION
#define WM_NCUAHDRAWCAPTION (0x00ae)
#endif

#ifndef WM_NCUAHDRAWFRAME
#define WM_NCUAHDRAWFRAME (0x00af)
#endif

#define START_SCOPE(x) namespace x {
#define END_SCOPE }
#define START_BLOCK {
#define END_BLOCK }

#pragma warning(disable:4189)
#pragma warning(disable:4191)
#pragma warning(disable:4100)
#pragma warning(disable:5039)
#pragma warning(disable:5045)

#define NOT_USING(x) (x)
#define CWM_DRAW 0x8001
#define CWM_ENTERMOUSE 0x8002
#define CWM_EXITMOUSE 0x8003
#define CWM_MOVEWINDOW 0x8004
#define CWM_DRAWCHILD 0x8005
#define CWM_CHECKINTERSECT 0x8006
#define CWM_PRUNECHILDLIST 0x8007
#define CWM_MOUSESCROLLHORIZONTAL 0x8008
#define CWM_MOUSESCROLLVERTICAL 0x8009
#define CWM_MOUSEEXIT 0x800a
#define CWM_MOUSEENTER 0x800b
#define CWM_SCROLLUPDATE 0x800c
#define CWM_TOGGLEVISIBILITY 0x800d
#define CWM_CONTEXTMENU 0x800e
#define CWM_MOUSELEFTCLICKUP 0x800f
#define CWM_MOUSELEFTCLICKDOWN 0x8010
#define CWM_MOUSERIGHTCLICKUP 0x8011
#define CWM_MOUSERIGHTCLICKDOWN 0x8012
#define CWM_SHOW 0x8013
#define CWM_HIDE 0x8014
#define CWM_SCROLLXAXIS 0x8015
#define CWM_SCROLLYAXIS 0x8016
#define CWM_CREATEAUDIOCLIP 0x8017
#define CWM_HORIZONTALZOOM 0x8018
#define CWM_MOUSEHOVER 0x8019
#define CWM_CREATEAUDIOTRACK 0x801a

