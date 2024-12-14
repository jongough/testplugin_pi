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
#include <wx/mstream.h>

#include <wx/aui/aui.h>

#include "crowdsource_pi.h"
#include "version.h"
#include "wxWTranslateCatalog.h"

#include "ODAPI.h"

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

void                    *g_ppimgr;
crowdsource_pi          *g_crowdsource_pi;

wxString                *g_PrivateDataDir;
wxString                *g_pHome_Locn;
wxString                *g_pData;
wxString                *g_SData_Locn;

wxString                *g_tplocale;
int                     g_iLocaleDepth;
wxString                *g_tpLocale;

wxBitmap *m_pdeficon;

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


    wxMemoryInputStream sm(
        "\211PNG\r\n\032\n\000\000\000\rIHDR\000\000\000 \000\000\000 "
        "\b\006\000\000\000szz\364\000\000\000\004sBIT\b\b\b\b|"
        "\bd\210\000\000\000\011pHYs\000\000\r\327\000\000\r\327\001B("
        "\233x\000\000\000\031tEXtSoftware\000www.inkscape.org\233\356<\032\000\000\004("
        "IDATX\205\355\327]\210]"
        "\325\025\a\360\337:\223\304\304 "
        ">h5\305I\250J\321Z\025\035\343G\374(\001!:\3274\264\005\261\004%\212\017*R\205@#"
        "\021\212\004A\321B\020?"
        "b\373Vh\bC\"h\2339\367\232\001\351\230\207\306\230\351D\215\204\210Z\252\031!"
        "h\255H\035G0sW\037\346\344:"
        "\367\316GF\252\304\a\027\234\207\265\367Z\377\365\337k\357\263\327\332\221\231N\244\024'4:"
        "\346}"
        "\035\343\030\210\245\276\364\013\205\233\244\263q\026\232\322\373\302\277\244]\346{!"
        "W\345\3419c\316e\013b0\026\372\334#\270\017\257H;\025\336\222F&\014tk:"
        "O\3709\256\302\223N\366`\256\314/"
        "\376o\002Q\217K\244\255\3028n\315Z\0368\216\375E\330*u\011\267f-_\233\315~"
        "\3263\020\215\330\200=B\335\250\313]c$"
        "\376\023\253\342\223\270v&\237\254\345\001\243.\027\352\330Sa\314\034c\246\014D="
        "\326b\013\326d-w\307\307q="
        "\302i\006\r\232\3473\027\353r\031\272\214\373\2071\257\345\3159\326\201\3613<\217\337d-"
        "\267\315\231@4\342\a8$"
        "\315\027\356q\225\367\025>\260\327g\306=\255\251W8$"
        "\355\257\266\346R\351\247\302\240\246\273su~0\211\304m\330,"
        "\\\220\275\371\321\334\b\324\243O:E\227\215B\303\251\372|\354\025\205'"
        "\244g\235\351\261\274,\277l\363\331\021]"
        "\026\273\017\033\2601k\371\247\326\\\031\245\360\337\254\345\257\247\004\313\314\266O\2775"
        "J\237\252\353\316LvZ\246tX"
        "\277O\354\324\323i?\305\177\247\363\225^\327\357\316\326X]"
        "\267\322\247\372\255\351\264\237\356\020\336\205\315\331\233#\225\376\271T("
        "\374S\330\030\203\261p\272\275l-hu\036\022~"
        "%<\034\003\261\024*\254\315\025v\233L%\020z\204=\223\364'\024\376`\221k*:"
        "\003\361\32783\032\261)\352\261/\352\261/"
        "\032\261)v\304\202\026\211\336|Wz\324Q\317N\302\331#\364\314J "
        "v\305\017\261\304\270a\210MQ\b5\213\374>W\346\027\206\334\202\375\346y\027?"
        "\021\326\011\353\244e\026{1D\264\300\206<"
        "\203\253cG,"
        "\202\nsI\324c\311\314\0318\252\a\207su\376\033\\\351\307\0309v\243\345C\331\024\376."
        "\034\221V\b\363\2627\017f-\357\020B\351\226V\026\036\312&"
        "\336\264\330\305Pa\036V\264g\241\235\300\304\344pKO\313\305$}"
        "b\354:M\217\342\001M/"
        "\035\333gM[\205\316\013j\030\313\333\364\346l\004\322\361\013CJ!\262\226\333tY\321*<!"
        "\346\344\337!"
        "\235\031\030f\022\3030$\247\034\234\335\322\272\020\2217\344;U\354\220\326)"
        "\274\334a\333\203\2416\275h\317h;\201\246a,"
        "\215\3768\035\354\3656\272'\377zyS\356P\030\325\260-\372\243'"
        "\372\243G\3036\205\321\354\315\347Z\3347E\201\013\215z\003*\314\245U\214\351\011d-"
        "\217\340\210\256\211UW\207n\2271\277m#\272\310j\034P\330\242\260\005\a\252\261\257d\271{"
        "\261\267U\037&0\217T1Z2\265!"
        "I\303X\201\201\312\361~"
        "\343\366\307\213\361\227\2741\337\200\\\231G\361H\365M\221h\304\271\302\203&z\203c\270+*"
        "\3546\231\356&\374#\326G#\272!W\345\207\270_S_4\342\202\351\002\266\005/"
        "\343\034l\307\357\262\226\357U\204\272\261\276\302\356X\360t\367y\251O\277\262\243F\334\\"
        "\325\204\a\324\2354\305g\310|"
        "u\367(\215(\335\336\341[*\365M\027k\346r\234\016b}"
        "\326\362\317\255\361\2018\303\270\247\245\032\016\342U\214\343\n\\\204\277}#"
        "\345\270r\\\213\247\360\313\254\345\356\266\271\301X\370M5$"
        "\263\227\326\272\rJcJ\217\333n\301\361Jq\313o\273\005J\217+"
        "\215\251\3330\233\355\011oJ\277\373my\233\361\261\207I\270A\372\221\260\014\276\365\207"
        "\311\267)'\374m\370="
        "\201\377\001\312\351\235\226\272B\301\307\000\000\000\000IEND\256B`\202",
        1195);
    m_pdeficon = new wxBitmap(wxImage(sm));

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
}

int crowdsource_pi::Init(void)
{
    g_tplocale = NULL;

    // Adds local language support for the plugin to OCPN
    AddLocaleCatalog( PLUGIN_CATALOG_NAME );

    // Get a pointer to the opencpn display canvas, to use as a parent for windows created
    m_parent_window = GetOCPNCanvasWindow();
    m_pTPConfig = GetOCPNConfigObject();


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
    return;
}

bool crowdsource_pi::DeInit(void)
{
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

wxBitmap *crowdsource_pi::GetPlugInBitmap()
{
    return m_pdeficon;
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

void crowdsource_pi::appendOSDirSlash(wxString* pString)
{
    wxChar sep = wxFileName::GetPathSeparator();

    if (pString->Last() != sep)
        pString->Append(sep);
}

void crowdsource_pi::GetODAPI()
{
}
