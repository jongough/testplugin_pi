/******************************************************************************
 * updated: 4-5-2012
 * Project:  OpenCPN
 * Purpose:  test Plugin
 * Author:   Jon Gough
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register   *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 */


#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers
#include <wx/stdpaths.h>
#include <wx/timer.h>
#include <wx/event.h>
#include <wx/sysopt.h>
#include <wx/dir.h>
#include <wx/stdpaths.h>
#include <wx/filefn.h>
#include <wx/msgdlg.h>
#include <wx/listbook.h>
#include <wx/panel.h>
#include <wx/ffile.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>

#include <wx/aui/aui.h>

#include "crowdsource_pi.h"
#include "version.h"
#include "wxWTranslateCatalog.h"

#include "ODAPI.h"
#include "tpJSON.h"
#include "tpicons.h"
#include "tpControlDialogImpl.h"

#include "wx/jsonwriter.h"


#ifndef DECL_EXP
#ifdef __WXMSW__
#define DECL_EXP     __declspec(dllexport)
#else
#define DECL_EXP
#endif
#endif

#if !defined(NAN)
static const long long lNaN = 0xfff8000000000000;
#define NAN (*(double*)&lNaN)
#endif

crowdsource_pi           *g_crowdsource_pi;
wxString                *g_PrivateDataDir;

wxString                *g_pHome_Locn;
wxString                *g_pData;
wxString                *g_SData_Locn;
wxString                *g_pLayerDir;

PlugIn_ViewPort         *g_pVP;
PlugIn_ViewPort         g_VP;
wxString                *g_tplocale;
void                    *g_ppimgr;

tpJSON                  *g_ptpJSON;
ODAPI                   *g_ptpAPI;
double                  g_dVar;
int                     g_iLocaleDepth;
wxString                *g_tpLocale;
bool                    g_bSaveJSONOnStartup;

wxFont                  *g_pFontTitle;
wxFont                  *g_pFontData;
wxFont                  *g_pFontLabel;
wxFont                  *g_pFontSmall;

wxString                g_ReceivedODAPIMessage;
wxJSONValue             g_ReceivedODAPIJSONMsg;
wxString                g_ReceivedJSONMessage;
wxJSONValue             g_ReceivedJSONJSONMsg;

// Needed for ocpndc.cpp to compile. Normally would be in glChartCanvas.cpp
float g_GLMinSymbolLineWidth;


// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new crowdsource_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}


//---------------------------------------------------------------------------------------------------------
//
//    crowdsource PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

crowdsource_pi::crowdsource_pi(void *ppimgr)
:opencpn_plugin_118(ppimgr)
{
    // Create the PlugIn icons
    g_ppimgr = ppimgr;
//    g_tp_pi_manager = (PlugInManager *) ppimgr;
    g_crowdsource_pi = this;

    wxString *l_pDir = new wxString(*GetpPrivateApplicationDataLocation());
    appendOSDirSlash( l_pDir );
    l_pDir->Append(_T("plugins"));
    appendOSDirSlash( l_pDir );
    if ( !wxDir::Exists(*l_pDir))
        wxMkdir( *l_pDir );
    l_pDir->Append(_T("crowdsource_pi"));
    appendOSDirSlash( l_pDir );
    if ( !wxDir::Exists(*l_pDir))
        wxMkdir( *l_pDir );
    g_PrivateDataDir = new wxString;
    g_PrivateDataDir->Append(*l_pDir);
    g_pData = new wxString(*l_pDir);
    g_pData->append( wxS("data") );
    appendOSDirSlash( g_pData );
    if ( !wxDir::Exists(*g_pData))
        wxMkdir( *g_pData );
    g_pLayerDir = new wxString;
    g_pLayerDir->Append(*l_pDir);
    g_pLayerDir->Append( wxT("Layers") );
    appendOSDirSlash( g_pLayerDir );

    m_ptpicons = new tpicons();

    delete l_pDir;
}

crowdsource_pi::~crowdsource_pi()
{
    delete g_SData_Locn;
    g_SData_Locn = NULL;

    delete g_PrivateDataDir;
    g_PrivateDataDir = NULL;

    delete g_pData;
    g_pData = NULL;

    delete g_pLayerDir;
    g_pLayerDir = NULL;

}

int crowdsource_pi::Init(void)
{
    g_tplocale = NULL;
    m_bReadyForRequests = false;
    m_bDoneODAPIVersionCall = false;
    m_btpDialog = false;
    m_tpControlDialogImpl = NULL;
    m_cursor_lat = 0.0;
    m_cursor_lon = 0.0;
    m_click_lat = 0.0;
    m_click_lon = 0.0;
    m_bOD_FindPointInAnyBoundary = false;
    m_bODFindClosestBoundaryLineCrossing = false;
    m_bODFindFirstBoundaryLineCrossing = false;
    m_bODCreateBoundary = false;
    m_bODCreateBoundaryPoint = false;
    m_bODCreateTextPoint = false;
    m_bODAddPointIcon = false;
    m_bODDeletePointIcon = false;
    m_pOD_FindPointInAnyBoundary = NULL;
    m_pODFindClosestBoundaryLineCrossing = NULL;
    m_pODFindFirstBoundaryLineCrossing = NULL;
    m_pODFindFirstBoundaryLineCrossing = NULL;
    m_pODCreateBoundary = NULL;
    m_pODCreateBoundaryPoint = NULL;
    m_pODCreateTextPoint = NULL;
    m_pODDeleteBoundary = NULL;
    m_pODDeleteBoundaryPoint = NULL;
    m_pODDeleteTextPoint = NULL;
    m_pODAddPointIcon = NULL;
    m_pODDeletePointIcon = NULL;
    m_iODVersionMajor = 0;
    m_iODVersionMinor = 0;
    m_iODVersionPatch = 0;
    m_iODAPIVersionMajor = 0;
    m_iODAPIVersionMinor = 0;
    m_bSaveIncommingJSONMessages = false;
    m_fnOutputJSON = wxEmptyString;
    m_fnInputJSON = wxEmptyString;
    m_bCloseSaveFileAfterEachWrite = true;
    m_bAppendToSaveFile = true;
    m_bRecreateConfig = false;

    // Adds local language support for the plugin to OCPN
    AddLocaleCatalog( PLUGIN_CATALOG_NAME );

    eventsEnabled = true;

    // Get a pointer to the opencpn display canvas, to use as a parent for windows created
    m_parent_window = GetOCPNCanvasWindow();
    m_pTPConfig = GetOCPNConfigObject();

    m_tpControlDialogImpl = new tpControlDialogImpl(m_parent_window);
    m_tpControlDialogImpl->Fit();
    m_tpControlDialogImpl->Layout();
    m_tpControlDialogImpl->Hide();
    LoadConfig();

    g_ptpJSON = new tpJSON;


#ifdef PLUGIN_USE_SVG
    m_crowdsource_button_id  = InsertPlugInToolSVG(_("Test Plugin"), m_ptpicons->m_s_crowdsource_grey_pi, m_ptpicons->m_s_crowdsource_pi, m_ptpicons->m_s_crowdsource_toggled_pi, wxITEM_CHECK,
                                                  _("Test Plugin"), wxS(""), NULL, crowdsource_POSITION, 0, this);
#else
    m_crowdsource_button_id  = InsertPlugInTool(_("Test Plugin"), &m_ptpicons->m_bm_crowdsource_grey_pi, &m_ptpicons->m_bm_crowdsource_pi, wxITEM_CHECK,
                                             _("Test Plugin"), wxS(""), NULL, crowdsource_POSITION, 0, this);
#endif

    //    In order to avoid an ASSERT on msw debug builds,
    //    we need to create a dummy menu to act as a surrogate parent of the created MenuItems
    //    The Items will be re-parented when added to the real context meenu
    wxMenu dummy_menu;

    // Create an OCPN Draw event handler
    //g_WVEventHandler = new WVEventHandler( g_crowdsource_pi );

    // Get item into font list in options/user interface
    AddPersistentFontKey( wxT("tp_Label") );
    AddPersistentFontKey( wxT("tp_Data") );
    g_pFontTitle = GetOCPNScaledFont_PlugIn( wxS("tp_Title") );
    g_pFontLabel = GetOCPNScaledFont_PlugIn( wxS("tp_Label") );
    g_pFontData = GetOCPNScaledFont_PlugIn( wxS("tp_Data") );
    g_pFontSmall = GetOCPNScaledFont_PlugIn( wxS("tp_Small") );
    wxColour l_fontcolour = GetFontColour_PlugIn( wxS("tp_Label") );
    l_fontcolour = GetFontColour_PlugIn( wxS("tp_Data") );

    m_pOD_FindPointInAnyBoundary = NULL;
    m_pODFindClosestBoundaryLineCrossing = NULL;

    return (
        WANTS_CURSOR_LATLON       |
        WANTS_TOOLBAR_CALLBACK    |
        INSTALLS_TOOLBAR_TOOL     |
//        WANTS_CONFIG              |
        INSTALLS_TOOLBOX_PAGE     |
        INSTALLS_CONTEXTMENU_ITEMS  |
//        WANTS_NMEA_EVENTS         |
//        WANTS_NMEA_SENTENCES        |
        //    USES_AUI_MANAGER            |
//        WANTS_PREFERENCES         |
        //    WANTS_ONPAINT_VIEWPORT      |
        WANTS_PLUGIN_MESSAGING    |
        WANTS_LATE_INIT           |
        WANTS_MOUSE_EVENTS        |
        WANTS_KEYBOARD_EVENTS
    );
}

void crowdsource_pi::LateInit(void)
{
    SendPluginMessage(wxS("CROWDSOURCE_PI_READY_FOR_REQUESTS"), wxS("TRUE"));
    m_bReadyForRequests = true;
    return;
}

bool crowdsource_pi::DeInit(void)
{
    if(m_tpControlDialogImpl)
    {
        m_tpControlDialogImpl->Close();
        delete m_tpControlDialogImpl;
        m_tpControlDialogImpl = NULL;
    }
    if(m_pTPConfig) SaveConfig();

    return true;
}

int crowdsource_pi::GetAPIVersionMajor()
{
      return OCPN_API_VERSION_MAJOR;
}

int crowdsource_pi::GetAPIVersionMinor()
{
      return OCPN_API_VERSION_MINOR;
}

int crowdsource_pi::GetPlugInVersionMajor()
{
      return PLUGIN_VERSION_MAJOR;
}

int crowdsource_pi::GetPlugInVersionMinor()
{
      return PLUGIN_VERSION_MINOR;
}

int crowdsource_pi::GetPlugInVersionPatch()
{
    return PLUGIN_VERSION_PATCH;
}

int crowdsource_pi::GetPlugInVersionPost()
{
    return PLUGIN_VERSION_TWEAK;
}

wxString crowdsource_pi::GetCommonName()
{
    return _T(PLUGIN_COMMON_NAME);
}

wxString crowdsource_pi::GetShortDescription()
{
    return _(PLUGIN_SHORT_DESCRIPTION);
}

wxString crowdsource_pi::GetLongDescription()
{
    return _(PLUGIN_LONG_DESCRIPTION);

}

int crowdsource_pi::GetToolbarToolCount(void)
{
      return 1;
}

void crowdsource_pi::OnToolbarToolCallback(int id)
{
}

void crowdsource_pi::OnToolbarToolDownCallback(int id)
{
    return;
}

void crowdsource_pi::OnToolbarToolUpCallback(int id)
{
    m_ptpicons->SetScaleFactor();
    return;
}

void crowdsource_pi::ShowPreferencesDialog( wxWindow* parent )
{

}

void crowdsource_pi::SetPluginMessage(wxString &message_id, wxString &message_body)
{
    return;
}

bool crowdsource_pi::KeyboardEventHook( wxKeyEvent &event )
{
    bool bret = FALSE;
    return bret;
}

bool crowdsource_pi::MouseEventHook( wxMouseEvent &event )
{
    bool bret = FALSE;
    return bret;
}

void crowdsource_pi::SetCursorLatLon(double lat, double lon)
{
}

wxBitmap *crowdsource_pi::GetPlugInBitmap()
{
    return &m_ptpicons->m_bm_crowdsource_pi;
}

void crowdsource_pi::appendOSDirSlash(wxString* pString)
{
    wxChar sep = wxFileName::GetPathSeparator();

    if (pString->Last() != sep)
        pString->Append(sep);
}

void crowdsource_pi::ToggleToolbarIcon( void )
{
}

void crowdsource_pi::SaveConfig()
{
}

void crowdsource_pi::LoadConfig()
{
}
void crowdsource_pi::GetODAPI()
{
}

void crowdsource_pi::FindClosestBoundaryLineCrossing(FindClosestBoundaryLineCrossing_t *pFCPBLC)
{
}

bool crowdsource_pi::CreateBoundaryPoint(CreateBoundaryPoint_t* pCBP)
{
    return false;
}

bool crowdsource_pi::CreateBoundary(CreateBoundary_t* pCB)
{
    return false;
}

bool crowdsource_pi::CreateTextPoint(CreateTextPoint_t* pCTP)
{
    return true;
}

bool crowdsource_pi::DeleteBoundaryPoint(DeleteBoundaryPoint_t* pDBP)
{
    return true;
}

bool crowdsource_pi::DeleteBoundary(DeleteBoundary_t* pDB)
{
    return true;
}

bool crowdsource_pi::DeleteTextPoint(DeleteTextPoint_t* pDTP)
{
    return true;
}

void crowdsource_pi::AddPointIcon(AddPointIcon_t* pAPI)
{
    return;
}

void crowdsource_pi::DeletePointIcon(DeletePointIcon_t* pDPI)
{
    return;
}

bool crowdsource_pi::ImportJSONFile()
{
    return false;
}

void crowdsource_pi::UpdateCloseAfterSave(bool bCloseAfterSave)
{
}

void crowdsource_pi::UpdateAppendToFile(bool bAppendToFile)
{
}
