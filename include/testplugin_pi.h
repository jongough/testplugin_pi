/******************************************************************************
 * $Id: testplugin_pi.h,v 1.0 2015/01/28 01:54:37 jongough Exp $
 *
 * Project:  OpenCPN
 * Purpose:  OpenCPN General Drawing Plugin
 * Author:   Jon Gough
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register   *
 *   $EMAIL$   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
 ***************************************************************************
 */
#ifndef _TESTPLUGINPI_H_
#define _TESTPLUGINPI_H_

#ifdef __WXMSW__
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <sstream>
#  define DEBUGSL(x) do { \
std::ostringstream oss; \
oss << x; \
time_t now = time(0); \
tm* localtm = localtime(&now); \
char *stime = asctime(localtm); \
stime[strlen(stime) - 1 ] = 0; \
std::string s1(oss.str()); \
std::string s = stime; \
s += " :: "; \
s += s1; \
s += "\n"; \
std::wstring stemp = std::wstring(s.begin(), s.end()); \
LPCWSTR sw = stemp.c_str(); \
OutputDebugString(sw); } while (0) 

#  define DEBUGST(x) do { \
std::string s(""); \
std::ostringstream oss; \
oss << x; \
time_t now = time(0); \
tm* localtm = localtime(&now); \
char *stime = asctime(localtm); \
stime[strlen(stime) - 1 ] = 0; \
do { \
std::string s1(oss.str()); \
s += stime; \
s += " :: "; \
s += s1; } while (0);

#  define DEBUGCONT(x) do { \
std::ostringstream oss; \
oss << x; \
std::string s1(oss.str()); \
s += s1 ; } while (0);

#  define DEBUGEND(x) do { \
std::string s1(""); \
std::ostringstream oss; \
oss << x; \
s1 = oss.str(); \
s += s1; } while (0); \
s += "\n" ; \
std::wstring stemp = std::wstring(s.begin(), s.end()); \
LPCWSTR sw = stemp.c_str(); \
OutputDebugString(sw); } while (0) 
#else
#  define DEBUGSL(x) do {} while (0)
#  define DEBUGST(x) do {} while (0)
#  define DEBUGCONT(x) do {} while (0)
#  define DEBUGEND(x) do {} while (0)
#endif
#else
#ifdef DEBUG_BUILD
#  define DEBUGSL(x) do { \
time_t now = time(0); \
tm* localtm = localtime(&now); \
char *stime = asctime(localtm); \
stime[strlen(stime) - 1 ] = 0; \
std::cout << stime << " :: "; \
std::cout << x << std::endl ;} while (0)

#  define DEBUGST(x) do { \
time_t now = time(0); \
tm* localtm = localtime(&now); \
char *stime = asctime(localtm); \
stime[strlen(stime) - 1 ] = 0; \
std::cout << stime << " :: " ; \
std::cout << x; } while (0)

#  define DEBUGCONT(x) do { \
std::cout << x ; } while (0)

#  define DEBUGEND(x) do { \
std::cout << x  << std::endl ; } while (0)
#else
#  define DEBUGSL(x) do {} while (0)
#  define DEBUGST(x) do {} while (0)
#  define DEBUGCONT(x) do {} while (0)
#  define DEBUGEND(x) do {} while (0)
#endif
#endif

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#include "wxWTranslateCatalog.h"

#include "ocpn_plugin.h"

#include <wx/string.h>
#include <wx/settings.h>
#include <wx/statline.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/dialog.h>
#include <wx/splitter.h>
#include <wx/fileconf.h>
#include <wx/dynarray.h>

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------
class tpicons;
class tpControlDialogImpl;

#define testplugin_POSITION -1
#define ID_NONE -1
//    Constants for right click menus

#define SELTYPE_UNKNOWN             0x0001
#define SELTYPE_ODPOINT             0x0002
#define SELTYPE_PATHSEGMENT         0x0004
#define SELTYPE_PATHCREATE          0x0008
#define SELTYPE_BOUNDARYLIST        0x0010
#define SELTYPE_PIL                 0x0020
#define TYPE_PATHMGR_PATH_DLG       0x0040
#define TYPE_PATHMGR_POINT_DLG      0x0080
#define TYPE_PATHMGR_LAYER_DLG      0x0100

//#define PI 3.14159265

class testplugin_pi : public opencpn_plugin_113
{
public:

    testplugin_pi(void *ppimgr);
    ~testplugin_pi();

    wxWindow            *m_parent_window;
    wxFileConfig        *m_pODConfig;
    wxTimer             *m_timer;
    

    //    The required PlugIn Methods
    int Init(void);
    bool DeInit(void);

    int GetPlugInVersionMajor();
    int GetPlugInVersionMinor();
    int GetAPIVersionMajor();
    int GetAPIVersionMinor();
    wxBitmap *GetPlugInBitmap();
    wxString GetCommonName();
    wxString GetShortDescription();
    wxString GetLongDescription();
//    void SetColorScheme(PI_ColorScheme cs);
    void GetOriginalColors();
    void SetOriginalColors();

    //    The required override PlugIn Methods
    //     bool RenderOverlay(wxMemoryDC *pmdc, PlugIn_ViewPort *vp);
    //      void SetCursorLatLon(double lat, double lon);


    int GetToolbarToolCount(void);
    void ShowPreferencesDialog( wxWindow* parent );
    void OnToolbarToolCallback(int id);
    void OnToolbarToolDownCallback(int id);
    void OnToolbarToolUpCallback(int id);
    void SetPluginMessage(wxString &message_id, wxString &message_body);

    void loadLayouts(wxWindow * parent);
//    void startLogbook();
    void shutdown(bool menu);
    
    bool KeyboardEventHook( wxKeyEvent &event );
    
    // OD Methods
    void    ProcessTimerEvent(wxTimerEvent& ev);
    void    PopupMenuHandler(wxCommandEvent& ev);
    
    void    SaveConfig( void );
    
    void    SetToolbarTool( void );
    void    ToggleToolbarIcon( void);
    
    
    wxGLContext     *m_pcontext;
    wxMemoryDC      *pmdc;
//    wxGLCanvas      *m_glcc;
    
    int         nBlinkerTick;
    
    void    appendOSDirSlash(wxString* pString);  
    
    tpicons     *m_ptpicons;
    tpControlDialogImpl    *m_tpControlDialogImpl;
    
    bool            eventsEnabled;
    int             m_iCallerId;
    bool            m_btpDialog;
    int             m_testplugin_button_id;
    
    
private:
    void    OnTimer(wxTimerEvent& ev);

    void    LoadConfig();

    void    MenuPrepend( wxMenu *menu, int id, wxString label);
    void    MenuAppend( wxMenu *menu, int id, wxString label);
    void    FindSelectedObject( void )    ;
    
    PlugIn_ViewPort m_VP;
    
    int             m_show_id;
    int             m_hide_id;
    bool            show;
    int             m_config_button_id;
    
    
    
};

#endif



