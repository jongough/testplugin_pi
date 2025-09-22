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

#include "testplugin_pi.h"
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

testplugin_pi           *g_testplugin_pi;
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
    return new testplugin_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}


//---------------------------------------------------------------------------------------------------------
//
//    testplugin PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

testplugin_pi::testplugin_pi(void *ppimgr)
:opencpn_plugin_118(ppimgr)
{
    // Create the PlugIn icons
    g_ppimgr = ppimgr;
//    g_tp_pi_manager = (PlugInManager *) ppimgr;
    g_testplugin_pi = this;

    wxString *l_pDir = new wxString(*GetpPrivateApplicationDataLocation());
    appendOSDirSlash( l_pDir );
    l_pDir->Append(_T("plugins"));
    appendOSDirSlash( l_pDir );
    if ( !wxDir::Exists(*l_pDir))
        wxMkdir( *l_pDir );
    l_pDir->Append(_T("testplugin_pi"));
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

testplugin_pi::~testplugin_pi()
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

int testplugin_pi::Init(void)
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
    m_bODFindAllPathsGUIDS = false;
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
    m_pODFindAllPathsGUIDS = NULL;
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
    m_testplugin_button_id  = InsertPlugInToolSVG(_("Test Plugin"), m_ptpicons->m_s_testplugin_grey_pi, m_ptpicons->m_s_testplugin_pi, m_ptpicons->m_s_testplugin_toggled_pi, wxITEM_CHECK,
                                                  _("Test Plugin"), wxS(""), NULL, testplugin_POSITION, 0, this);
#else
    m_testplugin_button_id  = InsertPlugInTool(_("Test Plugin"), &m_ptpicons->m_bm_testplugin_grey_pi, &m_ptpicons->m_bm_testplugin_pi, wxITEM_CHECK,
                                             _("Test Plugin"), wxS(""), NULL, testplugin_POSITION, 0, this);
#endif

    //    In order to avoid an ASSERT on msw debug builds,
    //    we need to create a dummy menu to act as a surrogate parent of the created MenuItems
    //    The Items will be re-parented when added to the real context meenu
    wxMenu dummy_menu;

    // Create an OCPN Draw event handler
    //g_WVEventHandler = new WVEventHandler( g_testplugin_pi );

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

void testplugin_pi::LateInit(void)
{
    SendPluginMessage(wxS("TESTPLUGIN_PI_READY_FOR_REQUESTS"), wxS("TRUE"));
    m_bReadyForRequests = true;
    return;
}

bool testplugin_pi::DeInit(void)
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

int testplugin_pi::GetAPIVersionMajor()
{
      return OCPN_API_VERSION_MAJOR;
}

int testplugin_pi::GetAPIVersionMinor()
{
      return OCPN_API_VERSION_MINOR;
}

int testplugin_pi::GetPlugInVersionMajor()
{
      return PLUGIN_VERSION_MAJOR;
}

int testplugin_pi::GetPlugInVersionMinor()
{
      return PLUGIN_VERSION_MINOR;
}

int testplugin_pi::GetPlugInVersionPatch()
{
    return PLUGIN_VERSION_PATCH;
}

int testplugin_pi::GetPlugInVersionPost()
{
    return PLUGIN_VERSION_TWEAK;
}

wxString testplugin_pi::GetCommonName()
{
    return _T(PLUGIN_COMMON_NAME);
}

wxString testplugin_pi::GetShortDescription()
{
    return _(PLUGIN_SHORT_DESCRIPTION);
}

wxString testplugin_pi::GetLongDescription()
{
    return _(PLUGIN_LONG_DESCRIPTION);

}

int testplugin_pi::GetToolbarToolCount(void)
{
      return 1;
}

void testplugin_pi::OnToolbarToolCallback(int id)
{
    m_iCallerId = id;
    ToggleToolbarIcon();
}

void testplugin_pi::OnToolbarToolDownCallback(int id)
{
    return;
}

void testplugin_pi::OnToolbarToolUpCallback(int id)
{
    m_ptpicons->SetScaleFactor();
    return;
}

void testplugin_pi::ShowPreferencesDialog( wxWindow* parent )
{

}

void testplugin_pi::SetPluginMessage(wxString &message_id, wxString &message_body)
{
    g_ptpJSON->ProcessMessage(message_id, message_body);
    return;
}

bool testplugin_pi::KeyboardEventHook( wxKeyEvent &event )
{
    bool bret = FALSE;

    if( event.GetKeyCode() < 128 )            //ascii
    {
        int key_char = event.GetKeyCode();

        if ( event.ControlDown() )
            key_char -= 64;

        switch( key_char ) {
            case WXK_CONTROL_W:                      // Ctrl W
                if ( event.ShiftDown() ) { // Shift-Ctrl-W
                    if(event.GetEventType() == wxEVT_KEY_DOWN) {
                        OnToolbarToolDownCallback( m_testplugin_button_id);
                    }
                    bret = TRUE;
                } else bret = FALSE;
                break;
        }
    }
    if(bret) RequestRefresh(m_parent_window);
    return bret;
}

bool testplugin_pi::MouseEventHook( wxMouseEvent &event )
{
    bool bret = FALSE;

    if(m_tpControlDialogImpl->IsVisible()) {
        if(event.LeftDown()) {
            m_click_lat = m_cursor_lat;
            m_click_lon = m_cursor_lon;
            m_tpControlDialogImpl->SetLatLon( m_cursor_lat, m_cursor_lon );
            bret = TRUE;
        }

        if(event.LeftUp()) {
            bret = TRUE;
        }
    }

    return bret;
}

void testplugin_pi::SetCursorLatLon(double lat, double lon)
{
    if(m_tpControlDialogImpl->IsShown()) {
        m_cursor_lat = lat;
        m_cursor_lon = lon;
    }
}

wxBitmap *testplugin_pi::GetPlugInBitmap()
{
    return &m_ptpicons->m_bm_testplugin_pi;
}

void testplugin_pi::appendOSDirSlash(wxString* pString)
{
    wxChar sep = wxFileName::GetPathSeparator();

    if (pString->Last() != sep)
        pString->Append(sep);
}

void testplugin_pi::ToggleToolbarIcon( void )
{
    if(m_btpDialog) {
        m_btpDialog = false;
        SetToolbarItemState( m_testplugin_button_id, false );
        m_tpControlDialogImpl->Hide();
    } else {
        m_btpDialog = true;
        SetToolbarItemState( m_testplugin_button_id, true  );
        if(!m_bDoneODAPIVersionCall) GetODAPI();
        m_tpControlDialogImpl->SetPanels();
        m_tpControlDialogImpl->Show();
    }
}

void testplugin_pi::SaveConfig()
{
    #ifndef __WXMSW__
    wxString *l_locale = new wxString(wxSetlocale(LC_NUMERIC, NULL));
    #if wxCHECK_VERSION(3,0,0)  && !defined(_WXMSW_)
    //#if wxCHECK_VERSION(3,0,0)
    wxSetlocale(LC_NUMERIC, "C");
    #else
    setlocale(LC_NUMERIC, "C");
    #endif
    #endif

    wxFileConfig *pConf = m_pTPConfig;

    if(pConf) {
        pConf->SetPath( wxS( "/Settings/testplugin_pi" ) );
        if(m_bRecreateConfig) {
            pConf->DeleteGroup( "/Settings/testplugin_pi" );
        } else {
            pConf->Write( wxS( "SaveJSONOnStartup" ), g_bSaveJSONOnStartup );
            pConf->Write( wxS( "JSONSaveFile" ), m_fnOutputJSON.GetFullPath());
            pConf->Write( wxS( "JSONInputFile" ), m_fnInputJSON.GetFullPath());
            pConf->Write( wxS( "CloseSaveFileAferEachWrite" ), m_bCloseSaveFileAfterEachWrite);
            pConf->Write( wxS( "AppendToSaveFile" ), m_bAppendToSaveFile);
            pConf->Write( wxS( "SaveIncommingJSONMessages" ), m_bSaveIncommingJSONMessages);
        }
    }
}

void testplugin_pi::LoadConfig()
{
    #ifndef __WXMSW__
    wxString *l_locale = new wxString(wxSetlocale(LC_NUMERIC, NULL));
    #if wxCHECK_VERSION(3,0,0)
    wxSetlocale(LC_NUMERIC, "C");
    #else
    setlocale(LC_NUMERIC, "C");
    #endif
    #endif

    wxFileConfig *pConf = m_pTPConfig;

    if(pConf)
    {
        wxString val;
        pConf->SetPath( wxS( "/Settings/testplugin_pi" ) );
        wxString  l_wxsColour;
        pConf->Read( wxS( "SaveJSONOnStartup"), &g_bSaveJSONOnStartup, false );
        if(g_bSaveJSONOnStartup) m_tpControlDialogImpl->SetSaveJSONOnStartup(g_bSaveJSONOnStartup);
        wxString l_filepath;
        pConf->Read( wxS("JSONSaveFile"), &l_filepath, wxEmptyString);
        m_fnOutputJSON.Assign(l_filepath);
        if(m_fnOutputJSON != wxEmptyString) m_tpControlDialogImpl->SetJSONSaveFile(m_fnOutputJSON.GetFullPath());
        pConf->Read( wxS( "JSONInputFile" ), &l_filepath, wxEmptyString);
        m_fnInputJSON.Assign(l_filepath);
        if(m_fnInputJSON != wxEmptyString) m_tpControlDialogImpl->SetJSONInputFile(m_fnInputJSON.GetFullPath());
        pConf->Read( wxS( "CloseSaveFileAferEachWrite" ), &m_bCloseSaveFileAfterEachWrite, true);
        m_tpControlDialogImpl->SetCloseFileAfterEachWrite(m_bCloseSaveFileAfterEachWrite);
        pConf->Read( wxS( "AppendToSaveFile" ), &m_bAppendToSaveFile, true);
        m_tpControlDialogImpl->SetAppendToSaveFile(m_bAppendToSaveFile);
        pConf->Read( wxS( "SaveIncommingJSONMessages" ), &m_bSaveIncommingJSONMessages, false);
        m_tpControlDialogImpl->SetIncommingJSONMessages(m_bSaveIncommingJSONMessages);
    }
}
void testplugin_pi::GetODAPI()
{
    wxJSONValue jMsg;
    wxJSONWriter writer;
    wxString    MsgString;

    jMsg[wxT("Source")] = "TESTPLUGIN_PI";
    jMsg[wxT("Type")] = wxT("Request");
    jMsg[wxT("Msg")] = wxT("Version");
    jMsg[wxT("MsgId")] = wxT("Version");
    writer.Write( jMsg, MsgString );
    SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
    if(g_ReceivedODAPIMessage != wxEmptyString &&  g_ReceivedODAPIJSONMsg[wxT("MsgId")].AsString() == wxS("Version")) {
        m_iODVersionMajor = g_ReceivedODAPIJSONMsg[wxS("Major")].AsInt();
        m_iODVersionMinor = g_ReceivedODAPIJSONMsg[wxS("Minor")].AsInt();
        m_iODVersionPatch = g_ReceivedODAPIJSONMsg[wxS("Patch")].AsInt();
    }
    m_bDoneODAPIVersionCall = true;

    wxJSONValue jMsg1;
    jMsg1[wxT("Source")] = wxT("TESTPLUGIN_PI");
    jMsg1[wxT("Type")] = wxT("Request");
    jMsg1[wxT("Msg")] = wxS("GetAPIAddresses");
    jMsg1[wxT("MsgId")] = wxS("GetAPIAddresses");
    writer.Write( jMsg1, MsgString );
    SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
    if(g_ReceivedODAPIMessage != wxEmptyString &&  g_ReceivedODAPIJSONMsg[wxT("MsgId")].AsString() == wxS("GetAPIAddresses")) {
        m_iODAPIVersionMajor = g_ReceivedODAPIJSONMsg[_T("ODAPIVersionMajor")].AsInt();
        m_iODAPIVersionMinor = g_ReceivedODAPIJSONMsg[_T("ODAPIVersionMinor")].AsInt();

        wxString sptr = g_ReceivedODAPIJSONMsg[_T("OD_FindPointInAnyBoundary")].AsString();
        if(sptr != _T("null")) {
            sscanf(sptr.To8BitData().data(), "%p", &m_pOD_FindPointInAnyBoundary);
            m_bOD_FindPointInAnyBoundary = true;
        }

        sptr = g_ReceivedODAPIJSONMsg[_T("OD_FindClosestBoundaryLineCrossing")].AsString();
        if(sptr != _T("null")) {
            sscanf(sptr.To8BitData().data(), "%p", &m_pODFindClosestBoundaryLineCrossing);
            m_bODFindClosestBoundaryLineCrossing = true;
        }
        sptr = g_ReceivedODAPIJSONMsg[_T("OD_FindFirstBoundaryLineCrossing")].AsString();
        if(sptr != _T("null")) {
            sscanf(sptr.To8BitData().data(), "%p", &m_pODFindFirstBoundaryLineCrossing);
            m_bODFindFirstBoundaryLineCrossing = true;
        }
        sptr = g_ReceivedODAPIJSONMsg[_T("OD_CreateBoundary")].AsString();
        if(sptr != _T("null")) {
            sscanf(sptr.To8BitData().data(), "%p", &m_pODCreateBoundary);
            m_bODCreateBoundary = true;
        }
        sptr = g_ReceivedODAPIJSONMsg[_T("OD_CreateBoundaryPoint")].AsString();
        if(sptr != _T("null")) {
            sscanf(sptr.To8BitData().data(), "%p", &m_pODCreateBoundaryPoint);
            m_bODCreateBoundaryPoint = true;
        }
        sptr = g_ReceivedODAPIJSONMsg[_T("OD_CreateTextPoint")].AsString();
        if(sptr != _T("null")) {
            sscanf(sptr.To8BitData().data(), "%p", &m_pODCreateTextPoint);
            m_bODCreateTextPoint = true;
        }
        sptr = g_ReceivedODAPIJSONMsg[_T("OD_DeleteBoundary")].AsString();
        if(sptr != _T("null")) {
            sscanf(sptr.To8BitData().data(), "%p", &m_pODDeleteBoundary);
            m_bODDeleteBoundary = true;
        }
        sptr = g_ReceivedODAPIJSONMsg[_T("OD_DeleteBoundaryPoint")].AsString();
        if(sptr != _T("null")) {
            sscanf(sptr.To8BitData().data(), "%p", &m_pODDeleteBoundaryPoint);
            m_bODDeleteBoundaryPoint = true;
        }
        sptr = g_ReceivedODAPIJSONMsg[_T("OD_DeleteTextPoint")].AsString();
        if(sptr != _T("null")) {
            sscanf(sptr.To8BitData().data(), "%p", &m_pODDeleteTextPoint);
            m_bODDeleteTextPoint = true;
        }
        sptr = g_ReceivedODAPIJSONMsg[_T("OD_AddPointIcon")].AsString();
        if(sptr != _T("null")) {
            sscanf(sptr.To8BitData().data(), "%p", &m_pODAddPointIcon);
            m_bODAddPointIcon = true;
        }
        sptr = g_ReceivedODAPIJSONMsg[_T("OD_DeletePointIcon")].AsString();
        if(sptr != _T("null")) {
            sscanf(sptr.To8BitData().data(), "%p", &m_pODDeletePointIcon);
            m_bODDeletePointIcon = true;
        }
        sptr = g_ReceivedODAPIJSONMsg[_T("OD_FindAllPathsGUIDS")].AsString();
        if(sptr != _T("null")) {
          sscanf(sptr.To8BitData().data(), "%p", &m_pODFindAllPathsGUIDS);
          m_bODFindAllPathsGUIDS = true;
        }
        sptr = g_ReceivedODAPIJSONMsg[_T("OD_FindAllPointsGUIDS")].AsString();
        if(sptr != _T("null")) {
          sscanf(sptr.To8BitData().data(), "%p", &m_pODFindAllPointsGUIDS);
          m_bODFindAllPointsGUIDS = true;
        }
    }

    wxString l_msg;
    wxString l_avail;
    wxString l_notavail;
    l_msg.Printf(_("OD Version: Major: %i, Minor: %i, Patch: %i, ODAPI Version: Major: %i, Minor: %i\n"), m_iODVersionMajor, m_iODVersionMinor, m_iODVersionPatch, m_iODAPIVersionMajor, m_iODAPIVersionMinor);
    if(m_bOD_FindPointInAnyBoundary) l_avail.Append(_("OD_FindPointInAnyBoundary\n"));
    if(m_bODFindClosestBoundaryLineCrossing) l_avail.Append(_("OD_FindClosestBoundaryLineCrossing\n"));
    if(m_bODFindFirstBoundaryLineCrossing) l_avail.Append(_("OD_FindFirstBoundaryLineCrossing\n"));
    if(m_bODCreateBoundary) l_avail.Append(_("OD_CreateBoundary\n"));
    if(m_bODCreateBoundaryPoint) l_avail.Append(_("OD_CreateBoundaryPoint\n"));
    if(m_bODCreateTextPoint) l_avail.Append(_("OD_CreateTextPoint\n"));
    if(m_bODAddPointIcon) l_avail.Append(_("OD_AddPointIcon\n"));
    if(m_bODDeletePointIcon) l_avail.Append(_("OD_DeletePointIcon\n"));
    if(m_bODFindAllPathsGUIDS) l_avail.Append(_("OD_FindAllPathsGUIDS\n"));
    if(m_bODFindAllPointsGUIDS) l_avail.Append(_("OD_FindAllPointsGUIDS\n"));
    if(l_avail.Length() > 0) {
        l_msg.Append(_("The following ODAPI's are available: \n"));
        l_msg.Append(l_avail);
    }

    if(!m_bOD_FindPointInAnyBoundary) l_notavail.Append(_("OD_FindPointInAnyBoundary\n"));
    if(!m_bODFindClosestBoundaryLineCrossing) l_notavail.Append(_("OD_FindClosestBoundaryLineCrossing\n"));
    if(!m_bODFindFirstBoundaryLineCrossing) l_notavail.Append(_("OD_FindFirstBoundaryLineCrossing\n"));
    if(!m_bODCreateBoundary) l_notavail.Append(_("OD_CreateBoundary\n"));
    if(!m_bODCreateBoundaryPoint) l_notavail.Append(_("OD_CreateBoundaryPoint\n"));
    if(!m_bODCreateTextPoint) l_notavail.Append(_("OD_CreateTextPoint\n"));
    if(!m_bODAddPointIcon) l_notavail.Append(_("OD_AddPointIcon\n"));
    if(!m_bODDeletePointIcon) l_notavail.Append(_("OD_DeletePointIcon\n"));
    if(!m_bODFindAllPathsGUIDS) l_notavail.Append(_("OD_FindAllPathsGUIDS\n"));
    if(!m_bODFindAllPointsGUIDS) l_notavail.Append(_("OD_FindAllPointsGUIDS"));
    if(l_notavail.Length() > 0) {
        l_msg.Append(_("The following ODAPI's are not available:\n"));
        l_msg.Append(l_notavail);
    }

    OCPNMessageBox_PlugIn( m_parent_window, l_msg, _("TESTPLUGIN"), (long) wxYES );

}

void testplugin_pi::FindClosestBoundaryLineCrossing(FindClosestBoundaryLineCrossing_t *pFCPBLC)
{
    if((*m_pODFindClosestBoundaryLineCrossing)(pFCPBLC)) {
        delete pFCPBLC;
    }
    delete pFCPBLC;
}

void testplugin_pi::GetGUIDList(GUIDList_t *pGL)
{
  if(pGL->GUIDType == "" || pGL->GUIDType == "Boundary" || pGL->GUIDType == "EBL") {
    pGL->GUIDList = (*m_pODFindAllPathsGUIDS)(pGL);
  }

  if(pGL->GUIDType == "Boundary Point" || pGL->GUIDType == "Text Point") {
    pGL->GUIDList = (*m_pODFindAllPointsGUIDS)(pGL);
  }

}

bool testplugin_pi::CreateBoundaryPoint(CreateBoundaryPoint_t* pCBP)
{
    bool l_bRet = (*m_pODCreateBoundaryPoint)(pCBP);
    DEBUGST("Boundary Point created: ");
    DEBUGEND(l_bRet);
    return true;
}

bool testplugin_pi::CreateBoundary(CreateBoundary_t* pCB)
{
    wxString l_GUID;
    bool l_bRet = (*m_pODCreateBoundary)(pCB);
    DEBUGST("Boundary GUID: ");
    DEBUGEND(pCB->GUID);
    return l_bRet;;
}

bool testplugin_pi::CreateTextPoint(CreateTextPoint_t* pCTP)
{
    wxString l_GUID;
    bool l_bRet = (*m_pODCreateTextPoint)(pCTP);
    DEBUGST("Text Point GUID: ");
    DEBUGEND(l_GUID);
    return true;
}

bool testplugin_pi::DeleteBoundaryPoint(DeleteBoundaryPoint_t* pDBP)
{
    bool l_bRet = (*m_pODDeleteBoundaryPoint)(pDBP);
    DEBUGST("Boundary Point Deleted: ");
    DEBUGEND(l_bRet);
    return true;
}

bool testplugin_pi::DeleteBoundary(DeleteBoundary_t* pDB)
{
    bool l_bRet = (*m_pODDeleteBoundary)(pDB);
    DEBUGST("Boundary deleted: ");
    DEBUGEND(l_bRet);
    return true;
}

bool testplugin_pi::DeleteTextPoint(DeleteTextPoint_t* pDTP)
{
    bool l_bRet = (*m_pODDeleteTextPoint)(pDTP);
    DEBUGST("Text Point created: ");
    DEBUGEND(l_bRet);
    return true;
}

void testplugin_pi::AddPointIcon(AddPointIcon_t* pAPI)
{
    (*m_pODAddPointIcon)(pAPI);
    return;
}

void testplugin_pi::DeletePointIcon(DeletePointIcon_t* pDPI)
{
    (*m_pODDeletePointIcon)(pDPI);
    return;
}

bool testplugin_pi::ImportJSONFile()
{
    wxFFile l_ffile;
    l_ffile.Open(m_fnInputJSON.GetFullPath(), "r");
    if(!l_ffile.IsOpened()) {
        OCPNMessageBox_PlugIn(NULL, m_fnInputJSON.GetFullPath(), _("File not found"), wxICON_EXCLAMATION | wxCANCEL);
        return false;
    }
    wxString l_str;
    l_ffile.ReadAll(&l_str);
    wxFileInputStream l_input( m_fnInputJSON.GetFullPath() );
    wxTextInputStream l_text( l_input );
/*    for(size_t i = 0; i < l_str.Length();) {
        //wxString l_ext = l_str.Mid(i, l_str_find)
        //wxStringTokenizer tokenizer("first:second:third:fourth", ":");
    }
*/
    wxJSONValue jMsg;
    wxJSONWriter writer;
    wxString    MsgString;

    writer.Write( jMsg, MsgString );
    SendPluginMessage( wxS("OCPN_DRAW_PI"), l_str );
    return true;
}

void testplugin_pi::UpdateCloseAfterSave(bool bCloseAfterSave)
{
    if(m_bCloseSaveFileAfterEachWrite != bCloseAfterSave) {
        m_bCloseSaveFileAfterEachWrite = bCloseAfterSave;
        if(bCloseAfterSave) {
            g_ptpJSON->CloseJSONOutputFile();
        }
    }
}

void testplugin_pi::UpdateAppendToFile(bool bAppendToFile)
{
}
