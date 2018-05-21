/**************************************************************************
 * 
 * Project:  OpenCPN
 * Purpose:  Test Plugin Control Dialog
 * Author:   Jon Gough
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register                               *
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
 **************************************************************************/

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "tpControlDialogImpl.h"
#include "testplugin_pi.h"

#include "ocpn_plugin.h"

#if wxCHECK_VERSION(3,0,0) 
#include <wx/valnum.h>
#endif

#include <wx/fontdlg.h>


extern testplugin_pi  *g_testplugin_pi;

tpControlDialogImpl::tpControlDialogImpl( wxWindow* parent ) : tpControlDialogDef( parent )
{
#if wxCHECK_VERSION(3,0,0)
    SetLayoutAdaptationMode(wxDIALOG_ADAPTATION_MODE_ENABLED);
#ifndef __WXMSW__
    wxFloatingPointValidator<double> dBoundaryPointRangeRingSteps(3, &m_dBoundaryPointRangeRingSteps, wxNUM_VAL_DEFAULT);
    dBoundaryPointRangeRingSteps.SetMin(0);
    m_textCtrlBoundaryPointRingStep->SetValidator( dBoundaryPointRangeRingSteps );
    #endif // not defined __WXMSW__ 
#endif // wxCHECK_VERSION(3,0,0)
    
    m_bCreateBoundaryHasFocus = FALSE;
    m_bCreateBoundaryPointHasFocus = FALSE;
    m_pfdDialog = NULL;
    
}

void tpControlDialogImpl::OnButtonClickCreateBoundary( wxCommandEvent& event )
{
    
}

void tpControlDialogImpl::OnButtonClickCreateBoundaryPoint( wxCommandEvent& event )
{
    m_bOK = true;
    
    CreateBoundaryPoint_t *pCBP = new CreateBoundaryPoint_t;
    pCBP->name = m_textCtrlBoundaryPointName->GetValue();
    pCBP->iconname = m_textCtrlBoundaryPointIconName->GetValue();
    pCBP->lat = fromDMM_Plugin( m_textCtrlLatitude->GetValue() );
    pCBP->lon = fromDMM_Plugin( m_textCtrlLongitude->GetValue() );
    pCBP->type = m_choiceBoundaryPointType->GetSelection();
    pCBP->visible = m_checkBoxBoundaryPointVisible->GetValue();
    pCBP->ringsvisible = m_checkBoxRingsVisible->GetValue();
    pCBP->ringsnumber = m_choiceBoundaryPointRingNumber->GetSelection();
    pCBP->ringssteps = atof(m_textCtrlBoundaryPointRingStep->GetValue().mb_str());
    pCBP->ringsunits = m_choiceBoundaryPointRingUnits->GetSelection();
    pCBP->ringscolour = m_colourPickerBoundaryPointRingColour->GetColour().GetAsString();
    g_testplugin_pi->CreateBoundaryPoint(pCBP);
    g_testplugin_pi->ToggleToolbarIcon();
    Show(false);
}

void tpControlDialogImpl::OnButtonClickCreateTextPoint( wxCommandEvent& event )
{
    m_bOK = true;
    CreateTextPoint_t *pCTP = new CreateTextPoint_t;
    pCTP->name = m_textCtrlTextPointName->GetValue();
    pCTP->iconname = m_textCtrlTextPointIconName->GetValue();
    pCTP->lat = fromDMM_Plugin( m_textCtrlLatitude->GetValue() );
    pCTP->lon = fromDMM_Plugin( m_textCtrlLongitude->GetValue() );
    pCTP->Visible = m_checkBoxTextPointVisible->GetValue();
    pCTP->TextToDisplay = m_textCtrlTextPointTextToDisplay->GetValue();
    pCTP->TextPosition = m_choiceTextPointTextPosition->GetSelection();
    pCTP->TextColour = m_colourPickerTextPointTextColour->GetColour().GetAsString();
    pCTP->BackgroundColour = m_colourPickerTextPointTextBackgroundColour->GetColour().GetAsString();
    pCTP->BackgroundTransparancy = m_sliderTextPointBackgroundTransparency->GetValue();
    pCTP->TextFont = m_staticTextTextPointTextFontExample->GetFont();
    pCTP->TextPointDisplayTextWhen = m_radioBoxTextPointTextDisplay->GetSelection();
    //        pCTP->ringsvisible = m_checkBoxTextRingsVisible->GetValue();
    //        pCTP->ringsnumber = m_choiceTextPointRingNumber->GetSelection();
    //        pCTP->ringssteps = atof(m_textCtrlTextPointRingStep->GetValue().mb_str());
    //        pCTP->ringsunits = m_choiceTextPointRingUnits->GetSelection();
    //        pCTP->ringscolour = m_colourPickerTextPointRingColour->GetColour().GetAsString();
    g_testplugin_pi->CreateTextPoint(pCTP);
    g_testplugin_pi->ToggleToolbarIcon();
    Show(false);
}

void tpControlDialogImpl::tpControlCancelClick( wxCommandEvent& event )
{
    m_bOK = false;
    g_testplugin_pi->ToggleToolbarIcon();
    
    Show(false);
}
 
void tpControlDialogImpl::tpControlOnClickProcessJSON( wxCommandEvent& event )
{
    
}

void tpControlDialogImpl::SetLatLon( double lat, double lon )
{
    m_textCtrlLatitude->SetValue( toSDMM_PlugIn( 1, lat ) );
    m_textCtrlLongitude->SetValue( toSDMM_PlugIn( 1, lon ) );
    m_textCtrlTextPointLatitude->SetValue( toSDMM_PlugIn( 1, lat ) );
    m_textCtrlTextPointLongitude->SetValue( toSDMM_PlugIn( 1, lon ) );
}

void tpControlDialogImpl::OnButtonClickFonts( wxCommandEvent& event )
{
    if(m_pfdDialog) delete m_pfdDialog;
    
    wxFontData l_FontData;
    l_FontData.SetInitialFont( m_DisplayTextFont );
    m_pfdDialog = new wxFontDialog( this, l_FontData );
    m_pfdDialog->Centre( wxBOTH );
    
    int iRet = m_pfdDialog->ShowModal();
    if(iRet == wxID_OK) {
        m_staticTextTextPointTextFontExample->SetFont(m_pfdDialog->GetFontData().GetChosenFont());
        SetDialogSize();
    }
}

void tpControlDialogImpl::SetDialogSize( void )
{
    //m_bSizerFill->RecalcSizes();
    /*    
     *    wxSize sz = m_SizerDialogBox->CalcMin();
     *    sz.IncBy( 20 );   // Account for some decorations?
     *    wxSize dsize = ::wxGetDisplaySize();
     *    sz.y = wxMin(sz.y, dsize.y-80);
     *    SetClientSize(sz);
     *    m_defaultClientSize = sz;
     *    //m_panelBasicProperties->SetScrollRate(5, 5);
     * 
     *    wxSize fsize = GetSize();
     *    fsize.y = wxMin(fsize.y, dsize.y-80);
     *    fsize.x = wxMin(fsize.x, dsize.x-80);
     *    SetSize(fsize);
     */    
    
    m_fgSizerTextPoint->Layout();
    m_fgSizerDisplayText->Layout();
    m_SizerControl->Layout();
    this->GetSizer()->Fit( this );
    this->Layout();
}

