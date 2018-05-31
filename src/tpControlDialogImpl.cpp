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
#include <wx/ffile.h>

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
    
    if(!g_testplugin_pi->m_bODCreateBoundary) m_panelODAPICreateBoundary->Disable();
    if(!g_testplugin_pi->m_bODCreateBoundaryPoint) m_panelODAPICreateBoundaryPoint->Disable();
    if(!g_testplugin_pi->m_bODCreateTextPoint) m_panelODAPICreateTextPoint->Disable();
    
}

void tpControlDialogImpl::OnButtonClickCreateBoundary( wxCommandEvent& event )
{
    CreateBoundaryPoint_t *pCBP = new CreateBoundaryPoint_t;
    CreateBoundaryPoint_t *pCBP1 = new CreateBoundaryPoint_t;
    CreateBoundaryPoint_t *pCBP2 = new CreateBoundaryPoint_t;
    CreateBoundaryPoint_t *pCBP3 = new CreateBoundaryPoint_t;
    CreateBoundaryPoint_t *pCBP4 = new CreateBoundaryPoint_t;
    CreateBoundaryPoint_t *pCBP5 = new CreateBoundaryPoint_t;
    CreateBoundaryPoint_t *pCBP6 = new CreateBoundaryPoint_t;
    
    if(m_textCtrlCornerLat->IsEmpty() || m_textCtrlCornerLon->IsEmpty()) {
        return;
    }
    
    double l_lat;
    double l_lon;
    
    CreateBoundary_t *pCB = new CreateBoundary_t;
    pCB->name = m_textCtrlBoundaryName->GetValue();
    pCB->type = m_choiceBoundaryType->GetSelection();
    pCB->pathIsActive = m_checkBoxBoundaryActive->GetValue();
    pCB->visible = m_checkBoxBoundaryVisible->GetValue();
    pCB->lineColour = m_colourPickerBoundaryLineColour->GetColour();
    pCB->fillColour = m_colourPickerBoundaryFillColour->GetColour();
    //pCB->BoundaryPointsList.insert()
    
    pCB->BoundaryPointsList.clear();
    pCBP->name = _T("Corner");
    pCBP->lat = fromDMM_Plugin(m_textCtrlCornerLat->GetValue());
    pCBP->lon = fromDMM_Plugin(m_textCtrlCornerLon->GetValue());
    pCBP->type = m_choiceBoundaryType->GetSelection();
    pCBP->visible = pCB->visible;
    pCBP->ringsvisible = true;
    pCBP->ringsnumber = 1;
    pCBP->ringssteps = 0.5;
    pCBP->ringsunits = 0;
    pCBP->defaultRingColour = false;
    pCBP->ringscolour = m_colourPickerBoundaryLineColour->GetColour();
    pCB->BoundaryPointsList.insert(pCB->BoundaryPointsList.end(), pCBP);
    int i = m_choiceNumberOfPoints->GetSelection() + 3;
    int l_iPointNum = 1;
    if(i == 3) {
        pCBP1->name.Printf(_T("id %i"), l_iPointNum++);
        pCBP1->lat = fromDMM_Plugin(m_textCtrlCornerLat->GetValue());
        pCBP1->lon = fromDMM_Plugin(m_textCtrlCornerLon->GetValue()) + 0.0167;
        pCBP1->visible = pCB->visible;
        pCBP1->ringsvisible = false;
        pCBP1->ringsnumber = 1;
        pCBP1->ringssteps = 1;
        pCBP1->ringsunits = 0;
        pCBP1->defaultRingColour = true;
        pCBP1->ringscolour = m_colourPickerBoundaryLineColour->GetColour();
        pCB->BoundaryPointsList.insert(pCB->BoundaryPointsList.end(), pCBP1);
        pCBP2->name.Printf(_T("id %i"), l_iPointNum++);
        pCBP2->lat = fromDMM_Plugin(m_textCtrlCornerLat->GetValue()) + 0.0167;
        pCBP2->lon = fromDMM_Plugin(m_textCtrlCornerLon->GetValue());
        pCBP2->type = m_choiceBoundaryType->GetSelection();
        pCBP2->visible = pCB->visible;
        pCBP2->ringsvisible = false;
        pCBP2->ringsnumber = 0;
        pCBP2->ringssteps = 1;
        pCBP2->ringsunits = 0;
        pCBP2->ringscolour = m_colourPickerBoundaryLineColour->GetColour();
        pCB->BoundaryPointsList.insert(pCB->BoundaryPointsList.end(), pCBP2);
    } else if(i == 4) {
        pCBP1->name.Printf(_T("id %i"), l_iPointNum++);
        pCBP1->lat = fromDMM_Plugin(m_textCtrlCornerLat->GetValue());
        pCBP1->lon = fromDMM_Plugin(m_textCtrlCornerLon->GetValue()) + 0.0167;
        pCBP1->visible = pCB->visible;
        pCBP1->ringsvisible = false;
        pCBP1->ringsnumber = 1;
        pCBP1->ringssteps = 1;
        pCBP1->ringsunits = 0;
        pCBP1->ringscolour = m_colourPickerBoundaryLineColour->GetColour();
        pCB->BoundaryPointsList.insert(pCB->BoundaryPointsList.end(), pCBP1);
        pCBP2->name.Printf(_T("id %i"), l_iPointNum++);
        pCBP2->lat = fromDMM_Plugin(m_textCtrlCornerLat->GetValue()) + 0.0167;
        pCBP2->lon = fromDMM_Plugin(m_textCtrlCornerLon->GetValue()) + 0.0167;
        pCBP2->type = m_choiceBoundaryType->GetSelection();
        pCBP2->visible = pCB->visible;
        pCBP2->ringsvisible = false;
        pCBP2->ringsnumber = 0;
        pCBP2->ringssteps = 1;
        pCBP2->ringsunits = 0;
        pCBP2->ringscolour = m_colourPickerBoundaryLineColour->GetColour();
        pCB->BoundaryPointsList.insert(pCB->BoundaryPointsList.end(), pCBP2);
        pCBP3->name.Printf(_T("id %i"), l_iPointNum++);
        pCBP3->lat = fromDMM_Plugin(m_textCtrlCornerLat->GetValue()) + 0.0167;
        pCBP3->lon = fromDMM_Plugin(m_textCtrlCornerLon->GetValue());
        pCBP3->type = m_choiceBoundaryType->GetSelection();
        pCBP3->visible = pCB->visible;
        pCBP3->ringsvisible = false;
        pCBP3->ringsnumber = 0;
        pCBP3->ringssteps = 1;
        pCBP3->ringsunits = 0;
        pCBP3->ringscolour = m_colourPickerBoundaryLineColour->GetColour();
        pCB->BoundaryPointsList.insert(pCB->BoundaryPointsList.end(), pCBP3);
    } else if(i == 5) {
        pCBP1->name.Printf(_T("id %i"), l_iPointNum++);
        pCBP1->lat = fromDMM_Plugin(m_textCtrlCornerLat->GetValue());
        pCBP1->lon = fromDMM_Plugin(m_textCtrlCornerLon->GetValue()) + 0.0167;
        pCBP1->visible = pCB->visible;
        pCBP1->ringsvisible = false;
        pCBP1->ringsnumber = 1;
        pCBP1->ringssteps = 1;
        pCBP1->ringsunits = 0;
        pCBP1->ringscolour = m_colourPickerBoundaryLineColour->GetColour();
        pCB->BoundaryPointsList.insert(pCB->BoundaryPointsList.end(), pCBP1);
        pCBP2->name.Printf(_T("id %i"), l_iPointNum++);
        pCBP2->lat = fromDMM_Plugin(m_textCtrlCornerLat->GetValue()) + 0.00835;
        pCBP2->lon = fromDMM_Plugin(m_textCtrlCornerLon->GetValue());
        pCBP2->type = m_choiceBoundaryType->GetSelection();
        pCBP2->visible = pCB->visible;
        pCBP2->ringsvisible = false;
        pCBP2->ringsnumber = 0;
        pCBP2->ringssteps = 1;
        pCBP2->ringsunits = 0;
        pCBP2->ringscolour = m_colourPickerBoundaryLineColour->GetColour();
        pCB->BoundaryPointsList.insert(pCB->BoundaryPointsList.end(), pCBP2);
        pCBP3->name.Printf(_T("id %i"), l_iPointNum++);
        pCBP3->lat = fromDMM_Plugin(m_textCtrlCornerLat->GetValue()) + 0.0167;
        pCBP3->lon = fromDMM_Plugin(m_textCtrlCornerLon->GetValue()) + 0.0167;
        pCBP3->type = m_choiceBoundaryType->GetSelection();
        pCBP3->visible = pCB->visible;
        pCBP3->ringsvisible = false;
        pCBP3->ringsnumber = 0;
        pCBP3->ringssteps = 1;
        pCBP3->ringsunits = 0;
        pCBP3->ringscolour = m_colourPickerBoundaryLineColour->GetColour();
        pCB->BoundaryPointsList.insert(pCB->BoundaryPointsList.end(), pCBP3);
        pCBP4->name.Printf(_T("id %i"), l_iPointNum++);
        pCBP4->lat = fromDMM_Plugin(m_textCtrlCornerLat->GetValue()) + 0.0167;
        pCBP4->lon = fromDMM_Plugin(m_textCtrlCornerLon->GetValue());
        pCBP4->type = m_choiceBoundaryType->GetSelection();
        pCBP4->visible = pCB->visible;
        pCBP4->ringsvisible = false;
        pCBP4->ringsnumber = 0;
        pCBP4->ringssteps = 1;
        pCBP4->ringsunits = 0;
        pCBP4->ringscolour = m_colourPickerBoundaryLineColour->GetColour();
        pCB->BoundaryPointsList.insert(pCB->BoundaryPointsList.end(), pCBP4);
    } else if(i == 6) {
        pCBP1->name.Printf(_T("id %i"), l_iPointNum++);
        pCBP1->lat = fromDMM_Plugin(m_textCtrlCornerLat->GetValue());
        pCBP1->lon = fromDMM_Plugin(m_textCtrlCornerLon->GetValue()) + 0.0167;
        pCBP1->visible = pCB->visible;
        pCBP1->ringsvisible = false;
        pCBP1->ringsnumber = 1;
        pCBP1->ringssteps = 1;
        pCBP1->ringsunits = 0;
        pCBP1->ringscolour = m_colourPickerBoundaryLineColour->GetColour();
        pCB->BoundaryPointsList.insert(pCB->BoundaryPointsList.end(), pCBP1);
        pCBP2->name.Printf(_T("id %i"), l_iPointNum++);
        pCBP2->lat = fromDMM_Plugin(m_textCtrlCornerLat->GetValue()) + 0.00835;
        pCBP2->lon = fromDMM_Plugin(m_textCtrlCornerLon->GetValue());
        pCBP2->type = m_choiceBoundaryType->GetSelection();
        pCBP2->visible = pCB->visible;
        pCBP2->ringsvisible = false;
        pCBP2->ringsnumber = 0;
        pCBP2->ringssteps = 1;
        pCBP2->ringsunits = 0;
        pCBP2->ringscolour = m_colourPickerBoundaryLineColour->GetColour();
        pCB->BoundaryPointsList.insert(pCB->BoundaryPointsList.end(), pCBP2);
        pCBP3->name.Printf(_T("id %i"), l_iPointNum++);
        pCBP3->lat = fromDMM_Plugin(m_textCtrlCornerLat->GetValue()) + 0.0167;
        pCBP3->lon = fromDMM_Plugin(m_textCtrlCornerLon->GetValue()) + 0.0167;
        pCBP3->type = m_choiceBoundaryType->GetSelection();
        pCBP3->visible = pCB->visible;
        pCBP3->ringsvisible = false;
        pCBP3->ringsnumber = 0;
        pCBP3->ringssteps = 1;
        pCBP3->ringsunits = 0;
        pCBP3->ringscolour = m_colourPickerBoundaryLineColour->GetColour();
        pCB->BoundaryPointsList.insert(pCB->BoundaryPointsList.end(), pCBP3);
        pCBP4->name.Printf(_T("id %i"), l_iPointNum++);
        pCBP4->lat = fromDMM_Plugin(m_textCtrlCornerLat->GetValue()) + 0.0167;
        pCBP4->lon = fromDMM_Plugin(m_textCtrlCornerLon->GetValue());
        pCBP4->type = m_choiceBoundaryType->GetSelection();
        pCBP4->visible = pCB->visible;
        pCBP4->ringsvisible = false;
        pCBP4->ringsnumber = 0;
        pCBP4->ringssteps = 1;
        pCBP4->ringsunits = 0;
        pCBP4->ringscolour = m_colourPickerBoundaryLineColour->GetColour();
        pCB->BoundaryPointsList.insert(pCB->BoundaryPointsList.end(), pCBP4);
        pCBP5->name.Printf(_T("id %i"), l_iPointNum++);
        pCBP5->lat = fromDMM_Plugin(m_textCtrlCornerLat->GetValue()) + 0.00835;
        pCBP5->lon = fromDMM_Plugin(m_textCtrlCornerLon->GetValue()) + 0.0167;
        pCBP5->type = m_choiceBoundaryType->GetSelection();
        pCBP5->visible = pCB->visible;
        pCBP5->ringsvisible = false;
        pCBP5->ringsnumber = 0;
        pCBP5->ringssteps = 1;
        pCBP5->ringsunits = 0;
        pCBP5->ringscolour = m_colourPickerBoundaryLineColour->GetColour();
        pCB->BoundaryPointsList.insert(pCB->BoundaryPointsList.end(), pCBP5);
    }
    
    g_testplugin_pi->CreateBoundary(pCB);
    g_testplugin_pi->ToggleToolbarIcon();
    Show(false);
    delete pCBP1;
    delete pCBP2;
    delete pCBP3;
    delete pCBP4;
    delete pCBP5;
    delete pCBP6;
    
    delete pCBP;
    delete pCB;
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
    wxFileName l_FileName = m_filePickerJSON->GetPath();
    if(!l_FileName.IsOk()) {
        OCPNMessageBox_PlugIn( NULL, l_FileName.GetFullPath(), _("File not found"), wxICON_EXCLAMATION | wxCANCEL );
        return;
    }
    wxString l_file = l_FileName.GetFullPath();
    wxFFile *l_ffFile  = new wxFFile(l_FileName.GetFullPath());
    wxString l_jString;
    //l_ffFile->Open(l_FileName.GetFullPath());
    l_ffFile->ReadAll(&l_jString);
    
    wxString l_name = l_FileName.GetFullName();
}

void tpControlDialogImpl::SetLatLon( double lat, double lon )
{
    m_textCtrlCornerLat->SetValue( toSDMM_PlugIn(1, lat));
    m_textCtrlCornerLon->SetValue( toSDMM_PlugIn(1, lon));
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

void tpControlDialogImpl::SetPanels()
{
    if(g_testplugin_pi->m_bODCreateBoundary) m_panelODAPICreateBoundary->Enable();
    else m_panelODAPICreateBoundary->Disable();
    if(g_testplugin_pi->m_bODCreateBoundaryPoint) m_panelODAPICreateBoundaryPoint->Enable();
    else m_panelODAPICreateBoundaryPoint->Disable();
    if(g_testplugin_pi->m_bODCreateTextPoint) m_panelODAPICreateTextPoint->Enable();
    else m_panelODAPICreateTextPoint->Disable();
}

