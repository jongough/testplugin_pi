/******************************************************************************
 *
 * Project:  OpenCPN
 *
 ***************************************************************************
 *   Copyright (C) 2013 by David S. Register                               *
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

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "testplugin_pi.h"
#include "tpicons.h"
#include "ocpn_plugin.h"

#include <wx/mstream.h>
#include <wx/filename.h>

#include <wx/stdpaths.h>


extern wxString *g_SData_Locn;
extern wxString *g_pHome_locn;
extern wxString *g_pData;
extern wxString *g_pImage;

tpicons::tpicons()
{
    m_dScaleFactor = 1.0;
    m_iDisplayScaleFactor = 32;
    m_iToolScaleFactor = GetOCPNGUIToolScaleFactor_PlugIn();
    m_iImageRefSize = m_iDisplayScaleFactor * m_iToolScaleFactor;
    m_bUpdateIcons = false;
    m_ColourScheme = PI_GLOBAL_COLOR_SCHEME_RGB;

    initialize_images();
}

tpicons::~tpicons()
{
    delete g_SData_Locn;
    g_SData_Locn = NULL;
}
void tpicons::initialize_images(void)
{
    wxFileName fn;
//#ifdef __WXOSX__
// Not in this case - the icons are part of the plugin package, not it's configuration data, so they have nothing to do in the user's preferences directory
//    wxStandardPathsBase& std_path = wxStandardPathsBase::Get();
//    fn.SetPath(std_path.GetUserConfigDir());  // should be ~/Library/Preferences
//    fn.AppendDir(_T("opencpn"));
//    fn.AppendDir( wxT("plugins") );
//    fn.AppendDir(wxT("testplugin_pi"));
//#else
//    fn.SetPath(*GetpSharedDataLocation());
    //const char *sPluginName = "testplugin_pi";
    //fn.SetPath(GetPluginDataDir(sPluginName));
    //const char *sPluginName = "testplugin_pi";
    fn.SetPath(GetPluginDataDir("testplugin_pi"));
    //    fn.AppendDir( wxT("plugins") );
//    fn.AppendDir(wxT("testplugin_pi"));
    fn.AppendDir(wxT("data"));
    g_SData_Locn = new wxString(fn.GetFullPath().c_str());
//#endif
    wxString s = _("testplugin_pi data location");
    wxLogMessage( wxT("%s: %s"), s.c_str(), fn.GetFullPath().c_str());

    m_failedBitmapLoad = false;

#ifdef PLUGIN_USE_SVG
    fn.SetFullName(wxT("testplugin.svg"));
    m_s_testplugin_pi = fn.GetFullPath();
    m_bm_testplugin_pi = LoadSVG( fn.GetFullPath() );
    fn.SetFullName(wxT("testplugingrey.svg"));
    m_s_testplugin_grey_pi = fn.GetFullPath();
    m_bm_testplugin_grey_pi = LoadSVG( fn.GetFullPath() );
    fn.SetFullName(wxT("testplugin-toggled.svg"));
    m_s_testplugin_toggled_pi = fn.GetFullPath();
    m_bm_testplugin_toggled_pi = LoadSVG( fn.GetFullPath() );
#else
    fn.SetFullName(wxT("testplugin.png"));
    m_p_bm_testplugin_pi = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    if(!m_p_bm_testplugin_pi->IsOk())  m_failedBitmapLoad = true;
#endif

    if(m_failedBitmapLoad) {
        int ret = OCPNMessageBox_PlugIn( NULL, _("Failed to load all testplugin_pi icons, check OCPN log for details"), _("OpenCPN Alert"), wxOK );
    } else {
        CreateSchemeIcons();
        ScaleIcons();
    }
}

#ifdef PLUGIN_USE_SVG
wxBitmap tpicons::LoadSVG( const wxString filename, unsigned int width, unsigned int height )
{
    if( width == -1 ) width = m_iImageRefSize;
    if( height == -1 ) height = m_iImageRefSize;

    wxString s = _("testplugin_pi LoadSVG");
    wxLogMessage( wxT("%s: filename: %s,  width: %u, height: %u"), s.c_str(), filename, width, height);

    wxBitmap l__Bitmap = GetBitmapFromSVGFile(filename , width, height);
    if(!l__Bitmap.IsOk()) {
        m_failedBitmapLoad = true;
    }

    return l__Bitmap;
}

wxBitmap tpicons::ScaleIcon( wxBitmap bitmap, const wxString filename, double sf )
{
    int w = bitmap.GetWidth();
    int h = bitmap.GetHeight();
    w *= sf;
    h *= sf;

    wxBitmap svgbm = GetBitmapFromSVGFile(filename, w, h);
    if(svgbm.GetWidth() > 0 && svgbm.GetHeight() > 0)
        return svgbm;
    return wxBitmap(32 * sf, 32 * sf); //scalled default blank bitmap
}
#endif // PLUGIN_USE_SVG

wxBitmap *tpicons::ScaleIcon( wxBitmap bitmap, double sf )
{
    wxImage scaled_image = bitmap.ConvertToImage();
    return new wxBitmap(scaled_image.Scale(scaled_image.GetWidth() * sf, scaled_image.GetHeight() * sf, wxIMAGE_QUALITY_HIGH));
}

bool tpicons::ScaleIcons()
{
    if(!SetScaleFactor()) return false;


#ifdef PLUGIN_USE_SVG

    // Dont scale the OD manager as that should be done by the OCPN toolbar
    //m_bm_testplugin_pi = ScaleIcon( m_p_svgd_testplugin_pi, m_p_img_testplugin_pi, m_dScaleFactor );
    //m_bm_testplugin_grey_pi = ScaleIcon( m_p_svgd_testplugin_grey_pi, m_p_img_testplugin_grey_pi, m_dScaleFactor );

#else
#endif // PLUGIN_USE_SVG

    CreateSchemeIcons();

    return true;
}

bool tpicons::SetScaleFactor()
{
    if(m_dScaleFactor != GetOCPNGUIToolScaleFactor_PlugIn()) {
        m_dScaleFactor = GetOCPNGUIToolScaleFactor_PlugIn();
        return true;
    }
    return false;
}

void tpicons::SetColourScheme( PI_ColorScheme cs )
{
    if(m_ColourScheme == cs) m_bUpdateIcons = false;
    else {
        m_bUpdateIcons = true;
        m_ColourScheme = cs;
        ChangeScheme();
    }
}

void tpicons::ChangeScheme(void)
{
    switch(m_ColourScheme) {
        case PI_GLOBAL_COLOR_SCHEME_RGB:
        case PI_GLOBAL_COLOR_SCHEME_DAY:
            m_bm_testplugin_grey_pi = m_bm_day_testplugin_grey_pi;
            break;
        case PI_GLOBAL_COLOR_SCHEME_DUSK:
            m_bm_testplugin_grey_pi = m_bm_dusk_testplugin_grey_pi;
            break;
        case PI_GLOBAL_COLOR_SCHEME_NIGHT:
            m_bm_testplugin_grey_pi = m_bm_night_testplugin_grey_pi;
            break;
        default:
            break;
    }
}

void tpicons::CreateSchemeIcons()
{
    m_bm_day_testplugin_grey_pi = m_bm_testplugin_grey_pi;
    m_bm_day_testplugin_toggled_pi = m_bm_testplugin_toggled_pi;
    m_bm_day_testplugin_pi = m_bm_testplugin_pi;
    m_bm_dusk_testplugin_grey_pi = BuildDimmedToolBitmap(m_bm_testplugin_grey_pi, 128);
    m_bm_dusk_testplugin_pi = BuildDimmedToolBitmap(m_bm_testplugin_pi, 128);
    m_bm_dusk_testplugin_toggled_pi = BuildDimmedToolBitmap(m_bm_testplugin_toggled_pi, 128);
    m_bm_night_testplugin_grey_pi = BuildDimmedToolBitmap(m_bm_testplugin_grey_pi, 32);
    m_bm_night_testplugin_pi = BuildDimmedToolBitmap(m_bm_testplugin_pi, 32);
    m_bm_night_testplugin_toggled_pi = BuildDimmedToolBitmap(m_bm_testplugin_toggled_pi, 32);

}

wxBitmap tpicons::BuildDimmedToolBitmap(wxBitmap bmp_normal, unsigned char dim_ratio)
{
    wxImage img_dup = bmp_normal.ConvertToImage();

    if( !img_dup.IsOk() )
        return bmp_normal;

    if(dim_ratio < 200)
    {
        //  Create a dimmed version of the image/bitmap
        int gimg_width = img_dup.GetWidth();
        int gimg_height = img_dup.GetHeight();

        double factor = (double)(dim_ratio) / 256.0;

        for(int iy=0 ; iy < gimg_height ; iy++)
        {
            for(int ix=0 ; ix < gimg_width ; ix++)
            {
                if(!img_dup.IsTransparent(ix, iy))
                {
                    wxImage::RGBValue rgb(img_dup.GetRed(ix, iy), img_dup.GetGreen(ix, iy), img_dup.GetBlue(ix, iy));
                    wxImage::HSVValue hsv = wxImage::RGBtoHSV(rgb);
                    hsv.value = hsv.value * factor;
                    wxImage::RGBValue nrgb = wxImage::HSVtoRGB(hsv);
                    img_dup.SetRGB(ix, iy, nrgb.red, nrgb.green, nrgb.blue);
                }
            }
        }
    }

    //  Make a bitmap
    wxBitmap toolBarBitmap;

#ifdef __WXMSW__
    wxBitmap tbmp(img_dup.GetWidth(),img_dup.GetHeight(),-1);
    wxMemoryDC dwxdc;
    dwxdc.SelectObject(tbmp);

    toolBarBitmap = wxBitmap(img_dup, (wxDC &)dwxdc);
#else
    toolBarBitmap = wxBitmap(img_dup);
#endif

    // store it
    return toolBarBitmap;
}
