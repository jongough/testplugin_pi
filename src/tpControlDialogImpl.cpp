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
#include <wx/base64.h>
#include <wx/mstream.h>

#include "tpControlDialogImpl.h"
#include "testplugin_pi.h"

#include "ocpn_plugin.h"

#if wxCHECK_VERSION(3,0,0)
#include <wx/valnum.h>
#endif

#include <wx/fontdlg.h>

#include "wx/jsonwriter.h"


#ifdef OD_JSON_SCHEMA_VALIDATOR
#if defined(snprintf) && defined(_MSC_VER)
#undef snprintf
#endif
#include "nlohmann/json-schema.hpp"
using nlohmann::json;
using nlohmann::json_schema::json_validator;
#endif

extern testplugin_pi    *g_testplugin_pi;
extern bool             g_bSaveJSONOnStartup;
extern wxString         g_ReceivedJSONMessage;
extern wxJSONValue      g_ReceivedJSONJSONMsg;


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

    if(!g_testplugin_pi->m_bODCreateBoundary) m_buttonCreateBoundaryODAPI->Disable();
    if(!g_testplugin_pi->m_bODCreateBoundaryPoint) m_buttonCreateBoundaryPointODAPI->Disable();
    if(!g_testplugin_pi->m_bODCreateTextPoint) m_buttonCreateTextPointODAPI->Disable();
    if(g_testplugin_pi->m_fnOutputJSON == wxEmptyString) m_checkBoxSaveJSON->Disable();
    else m_checkBoxSaveJSON->Enable();
}

void tpControlDialogImpl::OnButtonClickCreateBoundaryODAPI( wxCommandEvent& event )
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
    pCB->temporary = false;
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
    pCBP->BoundaryPointHyperLinkList.clear();
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
        pCBP1->BoundaryPointHyperLinkList.clear();
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
        pCBP2->BoundaryPointHyperLinkList.clear();
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

void tpControlDialogImpl::OnButtonClickDeleteBoundaryODAPI(wxCommandEvent& event)
{
    DeleteBoundary_t *pDB = new DeleteBoundary_t;
    pDB->GUID = m_textCtrDeleteBoundaryGUID->GetValue();
    g_testplugin_pi->DeleteBoundary(pDB);
}

void tpControlDialogImpl::OnButtonClickDeleteBoundaryJSON( wxCommandEvent& event )
{
    m_bOK = true;
    wxJSONWriter writer;
    wxJSONValue jMsg;
    wxString    MsgString;

    jMsg[wxT("Source")] = wxT("TESTPLUGIN_PI");
    jMsg[wxT("Type")] = wxT("Request");
    jMsg[wxT("Msg")] = wxS("DeleteBoundary");
    srand((unsigned)time(0));
    int l_rand = (rand()%10000) + 1;
    jMsg[wxT("MsgId")] = wxString::Format(wxT("%i"), l_rand);

    jMsg[wxT("GUID")] = m_textCtrDeleteBoundaryGUID->GetValue();;

    writer.Write( jMsg, MsgString );
    SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
    if(g_ReceivedJSONMessage != wxEmptyString &&  g_ReceivedJSONJSONMsg[wxT("MsgId")].AsString() == wxS("DeleteBoundary")) {
    }

    g_testplugin_pi->ToggleToolbarIcon();
    Show(false);
}

void tpControlDialogImpl::OnButtonClickCreateBoundaryPointODAPI( wxCommandEvent& event )
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
    bool ret = g_testplugin_pi->CreateBoundaryPoint(pCBP);
    g_testplugin_pi->ToggleToolbarIcon();
    Show(false);
}

void tpControlDialogImpl::OnButtonClickDeleteBoundaryPointODAPI(wxCommandEvent& event)
{
    DeleteBoundaryPoint_t *pDBP = new DeleteBoundaryPoint_t;
    pDBP->GUID = m_textCtrDeleteBoundaryPointGUID->GetValue();
    g_testplugin_pi->DeleteBoundaryPoint(pDBP);
}

void tpControlDialogImpl::OnButtonClickDeleteBoundaryPointJSON( wxCommandEvent& event )
{
    m_bOK = true;
    wxJSONWriter writer;
    wxJSONValue jMsg;
    wxString    MsgString;

    jMsg[wxT("Source")] = wxT("TESTPLUGIN_PI");
    jMsg[wxT("Type")] = wxT("Request");
    jMsg[wxT("Msg")] = wxS("DeleteBoundaryPoint");
    srand((unsigned)time(0));
    int l_rand = (rand()%10000) + 1;
    jMsg[wxT("MsgId")] = wxString::Format(wxT("%i"), l_rand);

    jMsg[wxT("GUID")] = m_textCtrDeleteBoundaryPointGUID->GetValue();;

    writer.Write( jMsg, MsgString );
    SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
    if(g_ReceivedJSONMessage != wxEmptyString &&  g_ReceivedJSONJSONMsg[wxT("MsgId")].AsString() == wxS("DeleteBoundaryPoint")) {
    }

    g_testplugin_pi->ToggleToolbarIcon();
    Show(false);
}

void tpControlDialogImpl::OnButtonClickCreateTextPointODAPI( wxCommandEvent& event )
{
    m_bOK = true;
    CreateTextPoint_t *pCTP = new CreateTextPoint_t;
    pCTP->name = m_textCtrlTextPointName->GetValue();
    pCTP->iconname = m_textCtrlTextPointIconName->GetValue();
    pCTP->lat = fromDMM_Plugin( m_textCtrlLatitude->GetValue() );
    pCTP->lon = fromDMM_Plugin( m_textCtrlLongitude->GetValue() );
    pCTP->Visible = m_checkBoxTextPointVisible->GetValue();
    pCTP->temporary = false;
    pCTP->TextToDisplay = m_textCtrlTextPointTextToDisplay->GetValue();
    pCTP->TextPosition = m_choiceTextPointTextPosition->GetSelection();
    pCTP->TextColour = m_colourPickerTextPointTextColour->GetColour().GetAsString();
    pCTP->BackgroundColour = m_colourPickerTextPointTextBackgroundColour->GetColour().GetAsString();
    pCTP->BackgroundTransparancy = m_sliderTextPointBackgroundTransparency->GetValue();
    pCTP->TextFont = m_staticTextTextPointTextFontExample->GetFont();
    pCTP->TextPointDisplayTextWhen = m_radioBoxTextPointTextDisplay->GetSelection();
    pCTP->defaultRangeRings = true;
    g_testplugin_pi->CreateTextPoint(pCTP);
    g_testplugin_pi->ToggleToolbarIcon();
    Show(false);
}

void tpControlDialogImpl::OnButtonClickDeleteTextPointODAPI(wxCommandEvent& event)
{
    DeleteTextPoint_t *pDTP = new DeleteTextPoint_t;
    pDTP->GUID = m_textCtrlDeleteTextPointGUID->GetValue();
    g_testplugin_pi->DeleteTextPoint(pDTP);
}

void tpControlDialogImpl::OnButtonClickDeleteTextPointJSON( wxCommandEvent& event )
{
    m_bOK = true;
    wxJSONWriter writer;
    wxJSONValue jMsg;
    wxString    MsgString;

    jMsg[wxT("Source")] = wxT("TESTPLUGIN_PI");
    jMsg[wxT("Type")] = wxT("Request");
    jMsg[wxT("Msg")] = wxS("DeleteTextPoint");
    srand((unsigned)time(0));
    int l_rand = (rand()%10000) + 1;
    jMsg[wxT("MsgId")] = wxString::Format(wxT("%i"), l_rand);

    jMsg[wxT("GUID")] = m_textCtrlDeleteTextPointGUID->GetValue();;

    writer.Write( jMsg, MsgString );
    SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
    if(g_ReceivedJSONMessage != wxEmptyString &&  g_ReceivedJSONJSONMsg[wxT("MsgId")].AsString() == wxS("DeleteTextPoint")) {
    }

    g_testplugin_pi->ToggleToolbarIcon();
    Show(false);
}

void tpControlDialogImpl::OnButtonClickPointIconODAPI(wxCommandEvent& event)
{
    if(m_radioBoxPointIcon->GetSelection() == 0) {
        AddPointIcon_t *pAPI = new AddPointIcon_t;
        wxMemoryInputStream sm("\211PNG\r\n\032\n\000\000\000\rIHDR\000\000\000 \000\000\000%\b\006\000\000\000#\267\353G\000\000\000\001sRGB\000\256\316\034\351\000\000\000\006bKGD\000\377\000\377\000\377\240\275\247\223\000\000\000\011pHYs\000\000\013\023\000\000\013\023\001\000\232\234\030\000\000\000\atIME\a\335\006\033\003\026\bz)\003\323\000\000\002\322IDATX\303\355\230=L\023a\034\306\177o\271J,\021\224:0\310\200\212`\342\000:p\261Q\373A\200\204Hbc\030pq a\201\311\004''\023\023H\210\203]X\214\023\0031\016\032\"\224\266\370\305@b\023\006\027\025\343\300|U \022\n\355\275\016\265'\005z\275;\212\014\362L\327\267\327\377\363\364\371\177\274\357\035\034\341\220!v.\250\252*+++q\273\335\306\232\224\322<\210\020\005\367J)\021B \204@\327u\204\020H)Q\024\205h4*\212\nhkk\223\311\315V\332\373\206\231\236\211\227\365\237vu\206\210M\214\0228\375\215\331\331Y\203\327\265\375&\217\307s \344\000\3233q\332\373\206\311f\263\005\353\005\002\024E9\020\362\355\"\266\247k\227\000]\327m\333\332\325\031\262Wt;\004(f_\232\021w\267\3240\020\252\006`\274%\314\324\342\212#\367\n\004H)\367\350\213\335\350n\251\241?TE\317\3302\000/\357\325\377\261\270\364owv\224\342$\227S\213+\014\204\252\rb-\273\302\324\342\252\243\024\270\234\026\323x<G\270\246\257\363\374\215p\\\274.'5\00004\362\002\200\023.\217q\355\004\266k\240\2533\304\203\233^\343\363\232\276\316|\244\027\200\207\257\264\222N\230\326\200\035\a.5V\030\0164\237_\377w\016L\317\304\231\236\3119\221/\302;\217\265\362\324\300a\340\377\023P\2269\260\037\330\356\202\371H/\256\232U>~\262\336\357O\356\207\271\\\237\013\355\033\234\334_\n|\203\223\250ug\214\300Vq\265\251zOr\323\024\024;zEb)\006\333k-\211X~\335\303@\250\232H,U\276A\224\263>\314m\277\244N\361\342k\b3\377\035\334B1v\307G\267\302\264\236\2535\004\027K\227\251\000\263A\224\027\00197.\236M\263\252\377\002\240?T\205\0337\221X\n_\003\266\366\006[\2438\037xh\0046\243w9.*\376\006\352xj\255\357]\256\3425\220\315f-\037\261\216u<\333\363\272\324F\226\311d\314\267\343\330\304\250e\021=c\313\306\251\310\352\261<\235N\233?\230\004\203A\231\267*\377\200!\245\344m\252\221\214\367\212\265\274jI\374\336\245\202\265\255\255-666XXX\020\246\002\212!\030\014\312w?.\224\024\241hIn\324~%\036\217[\212my\024'\022\011\341\367.\241h\311\262\221\333r\240\224\023\212\226\344\372\251/$\022\011[1m\013\000\360\373\375\362\303J\263!B\321\222\\;\371\231\271\2719\333\361\034\011\000\b\004\002\362\375\317&\000\307\344\373\206\252\252RUUy\364\226c?\370\r\006\316 \022WO\242\r\000\000\000\000IEND\256B`\202", 850);

        pAPI->PointIcon =  wxBitmap(wxImage(sm));
        pAPI->PointIconName = _T("blue_anchor");
        pAPI->PointIconDescription = _("Blue Anchor");

        g_testplugin_pi->AddPointIcon(pAPI);
        g_testplugin_pi->ToggleToolbarIcon();
        Show(false);
    } else {
        DeletePointIcon_t *pDPI = new DeletePointIcon_t;
        pDPI->PointIconName = _T("blue_anchor");
        g_testplugin_pi->DeletePointIcon(pDPI);
        g_testplugin_pi->ToggleToolbarIcon();
        Show(false);
    }
}

void tpControlDialogImpl::OnButtonClickGetGUIDSODAPI(wxCommandEvent& event)
{
  GUIDList_t *pGL = new GUIDList_t;
  switch(m_radioBoxGUIDTypes->GetSelection())
  {
    case 0: // All Paths
    {
      pGL->GUIDType = "";
      break;
    }
    case 1: // Boundaries
    {
      pGL->GUIDType = "Boundary";
      break;
    }
    case 2:
    {
      pGL->GUIDType = "EBL";
      break;
    }
    case 3: // Boundary Point
    {
      pGL->GUIDType = "Boundary Point";
      break;
    }
    case 4: // Text Point
    {
      pGL->GUIDType = "Text Point";
      break;
    }
  }
  pGL->InLayer = m_checkBoxInLayer->IsChecked();
  g_testplugin_pi->GetGUIDList(pGL);
  m_listBoxGUIDList->Clear();
  m_listBoxGUIDList->InsertItems( pGL->GUIDList, 0);
}

void tpControlDialogImpl::OnButtonClickGetGUIDSJSON(wxCommandEvent& event)
{
  GUIDList_t *pGL = new GUIDList_t;
  switch(m_radioBoxGUIDTypes->GetSelection())
  {
    case 0: // All Paths
    {
      pGL->GUIDType = "";
      break;
    }
    case 1: // Boundaries
    {
      pGL->GUIDType = "Boundary";
      break;
    }
    case 2:
    {
      pGL->GUIDType = "EBL";
      break;
    }
    case 3: // Boundary Point
    {
      pGL->GUIDType = "Boundary Point";
      break;
    }
    case 4: // Text Point
    {
      pGL->GUIDType = "Text Point";
      break;
    }
  }
  pGL->InLayer = m_checkBoxInLayer->IsChecked();

  m_bOK = true;
  wxJSONWriter writer;
  wxJSONValue jMsg;
  wxString    MsgString;

  jMsg[wxT("Source")] = wxT("TESTPLUGIN_PI");
  jMsg[wxT("Type")] = wxT("Request");
  jMsg[wxT("Msg")] = wxS("GetGUID");
  srand((unsigned)time(0));
  int l_rand = (rand()%10000) + 1;
  jMsg[wxT("MsgId")] = wxString::Format(wxT("%i"), l_rand);

  jMsg[wxT("GUIDType")] = pGL->GUIDType;
  jMsg[wxT("InLayer")] = pGL->InLayer;

  writer.Write( jMsg, MsgString );
  SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );

  if(g_ReceivedJSONMessage != wxEmptyString &&  g_ReceivedJSONJSONMsg[wxT("Msg")].AsString() == wxS("GetGUID") && g_ReceivedJSONJSONMsg[wxT("MsgId")].AsString() == wxString::Format(wxT("%i"), l_rand)) {
    m_listBoxGUIDList->Clear();
    if(g_ReceivedJSONJSONMsg[wxT("GUIDS")].AsArray() != 0) {

      for(int i = (int)g_ReceivedJSONJSONMsg[wxT("GUIDS")].AsArray()->size() - 1; i >= 0; i--) {
        wxString l_string = wxString(g_ReceivedJSONJSONMsg[wxT("GUIDS")][i].AsString());
        m_listBoxGUIDList->InsertItems(1, &l_string, 0);
      }
    }
  }
}

void tpControlDialogImpl::OnButtonClickCreateBoundaryJSON( wxCommandEvent& event )
{
    if(m_textCtrlCornerLat->IsEmpty() || m_textCtrlCornerLon->IsEmpty()) {
        return;
    }

    double l_lat;
    double l_lon;
    wxJSONValue jMsg;
    wxJSONValue jMsgB;
    wxJSONValue jMsgBP;
    wxJSONValue jMsgBP1;
    wxJSONValue jMsgBP2;
    wxJSONValue jMsgBP3;
    wxJSONValue jMsgBP4;
    wxJSONValue jMsgBP5;
    wxJSONValue jMsgBPHL;
    wxJSONWriter writer;
    wxString    MsgString;
    wxString    l_sname;

    jMsg[wxT("Source")] = wxT("TESTPLUGIN_PI");
    jMsg[wxT("Type")] = wxT("Request");
    jMsg[wxT("Msg")] = wxS("CreateBoundary");
    srand((unsigned)time(0));
    int l_rand = (rand()%10000) + 1;
    jMsg[wxT("MsgId")] = wxString::Format(wxT("%i"), l_rand);

    wxString l_name = m_textCtrlBoundaryName->GetValue();
    if(l_name.Length() < 1) l_name = wxEmptyString;
    jMsgB[wxT("BoundaryName")] = l_name;
    jMsgB[wxT("BoundaryType")] = m_choiceBoundaryType->GetString(m_choiceBoundaryType->GetSelection());
    jMsgB[wxT("Active")] = m_checkBoxBoundaryActive->GetValue();
    if(m_checkBoxBoundaryVisible->IsChecked()) jMsgB[wxT("visible")] = true;
    else jMsgB[wxT("visible")] = false;
    jMsgB[wxT("lineColour")] = m_colourPickerBoundaryLineColour->GetColour().GetAsString();
    jMsgB[wxT("fillColour")] = m_colourPickerBoundaryFillColour->GetColour().GetAsString();


    jMsgBP[wxT("Name")] = _T("Corner");
    jMsgBP[wxT("Lat")] = fromDMM_Plugin(m_textCtrlCornerLat->GetValue());
    jMsgBP[wxT("Lon")] = fromDMM_Plugin(m_textCtrlCornerLon->GetValue());
    jMsgBP[wxT("BoundaryPointType")] = m_choiceBoundaryType->GetString(m_choiceBoundaryType->GetSelection());
    if(m_checkBoxBoundaryPointVisible->IsChecked()) jMsgBP[wxT("visible")] = true;
    else jMsgBP[wxT("visible")] = false;
    if(m_checkBoxBoundaryPointRangeRingsVisible->IsChecked()) jMsgBP[wxT("ringsvisible")] = true;
    else jMsgBP[wxT("ringsvisible")] = false;
    jMsgBP[wxT("ringsnumber")] = m_choiceBoundaryBoundaryPointRingNumber->GetSelection();
    jMsgBP[wxT("ringssteps")] = atof(m_textCtrlBoundaryBoundaryPointRingStep->GetValue().mb_str());
    jMsgBP[wxT("ringsunits")] = m_choiceBoundaryBoundaryPointRingUnits->GetSelection();
    jMsgBP[wxT("ringscolour")] = m_colourPickerBoundaryBoundaryPointRingColour->GetColour().GetAsString();
    jMsgBPHL[wxT("LinkDescription")] = _("Test Link");
    jMsgBPHL[wxT("LinkURL")] = wxT("http://opencpn.org");
    jMsgBP[wxT("HyperLinks")].Item(0) = jMsgBPHL;
    jMsgB[wxT("BoundaryPoints")].Item(0) = jMsgBP;
    DEBUGSL(jMsgB.AsString());
    int i = wxAtoi( m_choiceNumberOfPoints->GetStringSelection());
    int l_iPointNum = 1;
    if(i == 2) {
        l_sname.Printf(_T("id %i"), l_iPointNum);
        jMsgBP1[wxT("Name")] = l_sname;
        jMsgBP1[wxT("Lat")] = fromDMM_Plugin(m_textCtrlCornerLat->GetValue());
        jMsgBP1[wxT("Lon")] = fromDMM_Plugin(m_textCtrlCornerLon->GetValue()) + 0.0167;
        jMsgBP1[wxT("BoundaryPointType")] = m_choiceBoundaryType->GetString(m_choiceBoundaryType->GetSelection());
        if(m_checkBoxBoundaryPointVisible->IsChecked()) jMsgBP1[wxT("visible")] = true;
        else jMsgBP1[wxT("visible")] = false;
        if(m_checkBoxBoundaryPointRangeRingsVisible->IsChecked()) jMsgBP1[wxT("ringsvisible")] = true;
        else jMsgBP1[wxT("ringsvisible")] = false;
        jMsgBP1[wxT("ringsnumber")] = m_choiceBoundaryBoundaryPointRingNumber->GetSelection();
        jMsgBP1[wxT("ringssteps")] = atof(m_textCtrlBoundaryBoundaryPointRingStep->GetValue().mb_str());
        jMsgBP1[wxT("ringsunits")] = m_choiceBoundaryBoundaryPointRingUnits->GetSelection();
        jMsgBP1[wxT("ringscolour")] = m_colourPickerBoundaryBoundaryPointRingColour->GetColour().GetAsString();
        jMsgB[wxT("BoundaryPoints")].Item(l_iPointNum++) = jMsgBP1;
    } else if(i == 3) {
        l_sname.Printf(_T("id %i"), l_iPointNum);
        jMsgBP1[wxT("Name")] = l_sname;
        jMsgBP1[wxT("Lat")] = fromDMM_Plugin(m_textCtrlCornerLat->GetValue());
        jMsgBP1[wxT("Lon")] = fromDMM_Plugin(m_textCtrlCornerLon->GetValue()) + 0.0167;
        jMsgBP1[wxT("BoundaryPointType")] = m_choiceBoundaryType->GetString(m_choiceBoundaryType->GetSelection());
        if(m_checkBoxBoundaryPointVisible->IsChecked()) jMsgBP1[wxT("visible")] = true;
        else jMsgBP1[wxT("visible")] = false;
        if(m_checkBoxBoundaryPointRangeRingsVisible->IsChecked()) jMsgBP1[wxT("ringsvisible")] = true;
        else jMsgBP1[wxT("ringsvisible")] = false;
        jMsgBP1[wxT("ringsnumber")] = m_choiceBoundaryBoundaryPointRingNumber->GetSelection();
        jMsgBP1[wxT("ringssteps")] = atof(m_textCtrlBoundaryBoundaryPointRingStep->GetValue().mb_str());
        jMsgBP1[wxT("ringsunits")] = m_choiceBoundaryBoundaryPointRingUnits->GetSelection();
        jMsgBP1[wxT("ringscolour")] = m_colourPickerBoundaryBoundaryPointRingColour->GetColour().GetAsString();
        jMsgB[wxT("BoundaryPoints")].Item(l_iPointNum++) = jMsgBP1;
        l_sname.Printf(_T("id %i"), l_iPointNum);
        jMsgBP2[wxT("Name")] = l_sname;
        jMsgBP2[wxT("Lat")] = fromDMM_Plugin(m_textCtrlCornerLat->GetValue()) + 0.0167;
        jMsgBP2[wxT("Lon")] = fromDMM_Plugin(m_textCtrlCornerLon->GetValue());
        jMsgBP2[wxT("BoundaryPointType")] = m_choiceBoundaryType->GetString(m_choiceBoundaryType->GetSelection());
        if(m_checkBoxBoundaryPointVisible->IsChecked()) jMsgBP2[wxT("visible")] = true;
        else jMsgBP2[wxT("visible")] = false;
        if(m_checkBoxBoundaryPointRangeRingsVisible->IsChecked()) jMsgBP2[wxT("ringsvisible")] = true;
        else jMsgBP2[wxT("ringsvisible")] = false;
        jMsgBP2[wxT("ringsnumber")] = m_choiceBoundaryBoundaryPointRingNumber->GetSelection();
        jMsgBP2[wxT("ringssteps")] = atof(m_textCtrlBoundaryBoundaryPointRingStep->GetValue().mb_str());
        jMsgBP2[wxT("ringsunits")] = m_choiceBoundaryBoundaryPointRingUnits->GetSelection();
        jMsgBP2[wxT("ringscolour")] = m_colourPickerBoundaryBoundaryPointRingColour->GetColour().GetAsString();
        jMsgB[wxT("BoundaryPoints")].Item(l_iPointNum++) = jMsgBP2;
    } else if(i == 4) {
        l_sname.Printf(_T("id %i"), l_iPointNum);
        jMsgBP1[wxT("Name")] = l_sname;
        jMsgBP1[wxT("Lat")] = fromDMM_Plugin(m_textCtrlCornerLat->GetValue());
        jMsgBP1[wxT("Lon")] = fromDMM_Plugin(m_textCtrlCornerLon->GetValue()) + 0.0167;
        jMsgBP1[wxT("BoundaryPointType")] = m_choiceBoundaryType->GetString(m_choiceBoundaryType->GetSelection());
        if(m_checkBoxBoundaryPointVisible->IsChecked()) jMsgBP1[wxT("visible")] = true;
        else jMsgBP1[wxT("visible")] = false;
        if(m_checkBoxBoundaryPointRangeRingsVisible->IsChecked()) jMsgBP1[wxT("ringsvisible")] = true;
        else jMsgBP1[wxT("ringsvisible")] = false;
        jMsgBP1[wxT("ringsnumber")] = m_choiceBoundaryBoundaryPointRingNumber->GetSelection();
        jMsgBP1[wxT("ringssteps")] = atof(m_textCtrlBoundaryBoundaryPointRingStep->GetValue().mb_str());
        jMsgBP1[wxT("ringsunits")] = m_choiceBoundaryBoundaryPointRingUnits->GetSelection();
        jMsgBP1[wxT("ringscolour")] = m_colourPickerBoundaryBoundaryPointRingColour->GetColour().GetAsString();
        jMsgB[wxT("BoundaryPoints")].Item(l_iPointNum++) = jMsgBP1;
        l_sname.Printf(_T("id %i"), l_iPointNum);
        jMsgBP2[wxT("Name")] = l_sname;
        jMsgBP2[wxT("Lat")] = fromDMM_Plugin(m_textCtrlCornerLat->GetValue()) + 0.0167;
        jMsgBP2[wxT("Lon")] = fromDMM_Plugin(m_textCtrlCornerLon->GetValue()) + 0.0167;
        jMsgBP2[wxT("BoundaryPointType")] = m_choiceBoundaryType->GetString(m_choiceBoundaryType->GetSelection());
        if(m_checkBoxBoundaryPointVisible->IsChecked()) jMsgBP2[wxT("visible")] = true;
        else jMsgBP2[wxT("visible")] = false;
        if(m_checkBoxBoundaryPointRangeRingsVisible->IsChecked()) jMsgBP2[wxT("ringsvisible")] = true;
        else jMsgBP2[wxT("ringsvisible")] = false;
        jMsgBP2[wxT("ringsnumber")] = m_choiceBoundaryBoundaryPointRingNumber->GetSelection();
        jMsgBP2[wxT("ringssteps")] = atof(m_textCtrlBoundaryBoundaryPointRingStep->GetValue().mb_str());
        jMsgBP2[wxT("ringsunits")] = m_choiceBoundaryBoundaryPointRingUnits->GetSelection();
        jMsgBP2[wxT("ringscolour")] = m_colourPickerBoundaryBoundaryPointRingColour->GetColour().GetAsString();
        jMsgB[wxT("BoundaryPoints")].Item(l_iPointNum++) = jMsgBP2;
        l_sname.Printf(_T("id %i"), l_iPointNum);
        jMsgBP3[wxT("Name")] = l_sname;
        jMsgBP3[wxT("Lat")] = fromDMM_Plugin(m_textCtrlCornerLat->GetValue()) + 0.0167;
        jMsgBP3[wxT("Lon")] = fromDMM_Plugin(m_textCtrlCornerLon->GetValue());
        jMsgBP3[wxT("BoundaryPointType")] = m_choiceBoundaryType->GetString(m_choiceBoundaryType->GetSelection());
        if(m_checkBoxBoundaryPointVisible->IsChecked()) jMsgBP3[wxT("visible")] = true;
        else jMsgBP3[wxT("visible")] = false;
        if(m_checkBoxBoundaryPointRangeRingsVisible->IsChecked()) jMsgBP3[wxT("ringsvisible")] = true;
        else jMsgBP3[wxT("ringsvisible")] = false;
        jMsgBP3[wxT("ringsnumber")] = m_choiceBoundaryBoundaryPointRingNumber->GetSelection();
        jMsgBP3[wxT("ringssteps")] = atof(m_textCtrlBoundaryBoundaryPointRingStep->GetValue().mb_str());
        jMsgBP3[wxT("ringsunits")] = m_choiceBoundaryBoundaryPointRingUnits->GetSelection();
        jMsgBP3[wxT("ringscolour")] = m_colourPickerBoundaryBoundaryPointRingColour->GetColour().GetAsString();
        jMsgBP3[wxT("ringscolour")] = m_colourPickerBoundaryLineColour->GetColour().GetAsString();
        jMsgB[wxT("BoundaryPoints")].Item(l_iPointNum++) = jMsgBP3;
    } else if(i == 5) {
        l_sname.Printf(_T("id %i"), l_iPointNum);
        jMsgBP1[wxT("Name")] = l_sname;
        jMsgBP1[wxT("Lat")] = fromDMM_Plugin(m_textCtrlCornerLat->GetValue());
        jMsgBP1[wxT("Lon")] = fromDMM_Plugin(m_textCtrlCornerLon->GetValue()) + 0.0167;
        jMsgBP1[wxT("BoundaryPointType")] = m_choiceBoundaryType->GetString(m_choiceBoundaryType->GetSelection());
        if(m_checkBoxBoundaryPointVisible->IsChecked()) jMsgBP1[wxT("visible")] = true;
        else jMsgBP1[wxT("visible")] = false;
        if(m_checkBoxBoundaryPointRangeRingsVisible->IsChecked()) jMsgBP1[wxT("ringsvisible")] = true;
        else jMsgBP1[wxT("ringsvisible")] = false;
        jMsgBP1[wxT("ringsnumber")] = m_choiceBoundaryBoundaryPointRingNumber->GetSelection();
        jMsgBP1[wxT("ringssteps")] = atof(m_textCtrlBoundaryBoundaryPointRingStep->GetValue().mb_str());
        jMsgBP1[wxT("ringsunits")] = m_choiceBoundaryBoundaryPointRingUnits->GetSelection();
        jMsgBP1[wxT("ringscolour")] = m_colourPickerBoundaryBoundaryPointRingColour->GetColour().GetAsString();
        jMsgB[wxT("BoundaryPoints")].Item(l_iPointNum++) = jMsgBP1;
        l_sname.Printf(_T("id %i"), l_iPointNum);
        jMsgBP2[wxT("Name")] = l_sname;
        jMsgBP2[wxT("Lat")] = fromDMM_Plugin(m_textCtrlCornerLat->GetValue()) + 0.00835;
        jMsgBP2[wxT("Lon")] = fromDMM_Plugin(m_textCtrlCornerLon->GetValue());
        jMsgBP2[wxT("BoundaryPointType")] = m_choiceBoundaryType->GetString(m_choiceBoundaryType->GetSelection());
        if(m_checkBoxBoundaryPointVisible->IsChecked()) jMsgBP2[wxT("visible")] = true;
        else jMsgBP2[wxT("visible")] = false;
        if(m_checkBoxBoundaryPointRangeRingsVisible->IsChecked()) jMsgBP2[wxT("ringsvisible")] = true;
        else jMsgBP2[wxT("ringsvisible")] = false;
        jMsgBP2[wxT("ringsnumber")] = m_choiceBoundaryBoundaryPointRingNumber->GetSelection();
        jMsgBP2[wxT("ringssteps")] = atof(m_textCtrlBoundaryBoundaryPointRingStep->GetValue().mb_str());
        jMsgBP2[wxT("ringsunits")] = m_choiceBoundaryBoundaryPointRingUnits->GetSelection();
        jMsgBP2[wxT("ringscolour")] = m_colourPickerBoundaryBoundaryPointRingColour->GetColour().GetAsString();
        jMsgB[wxT("BoundaryPoints")].Item(l_iPointNum++) = jMsgBP2;
        l_sname.Printf(_T("id %i"), l_iPointNum);
        jMsgBP3[wxT("Name")] = l_sname;
        jMsgBP3[wxT("Lat")] = fromDMM_Plugin(m_textCtrlCornerLat->GetValue()) + 0.0167;
        jMsgBP3[wxT("Lon")] = fromDMM_Plugin(m_textCtrlCornerLon->GetValue()) + 0.0167;
        jMsgBP3[wxT("BoundaryPointType")] = m_choiceBoundaryType->GetString(m_choiceBoundaryType->GetSelection());
        if(m_checkBoxBoundaryPointVisible->IsChecked()) jMsgBP3[wxT("visible")] = true;
        else jMsgBP3[wxT("visible")] = false;
        if(m_checkBoxBoundaryPointRangeRingsVisible->IsChecked()) jMsgBP3[wxT("ringsvisible")] = true;
        else jMsgBP3[wxT("ringsvisible")] = false;
        jMsgBP3[wxT("ringsnumber")] = m_choiceBoundaryBoundaryPointRingNumber->GetSelection();
        jMsgBP3[wxT("ringssteps")] = atof(m_textCtrlBoundaryBoundaryPointRingStep->GetValue().mb_str());
        jMsgBP3[wxT("ringsunits")] = m_choiceBoundaryBoundaryPointRingUnits->GetSelection();
        jMsgBP3[wxT("ringscolour")] = m_colourPickerBoundaryBoundaryPointRingColour->GetColour().GetAsString();
        jMsgB[wxT("BoundaryPoints")].Item(l_iPointNum++) = jMsgBP3;
        jMsgBP4[wxT("Name")] = l_sname;
        jMsgBP4[wxT("Lat")] = fromDMM_Plugin(m_textCtrlCornerLat->GetValue()) + 0.0167;
        jMsgBP4[wxT("Lon")] = fromDMM_Plugin(m_textCtrlCornerLon->GetValue());
        jMsgBP4[wxT("BoundaryPointType")] = m_choiceBoundaryType->GetString(m_choiceBoundaryType->GetSelection());
        if(m_checkBoxBoundaryBoundaryPointVisible->IsChecked()) jMsgBP4[wxT("visible")] = true;
        else jMsgBP4[wxT("visible")] = false;
        if(m_checkBoxBoundaryPointRangeRingsVisible->IsChecked()) jMsgBP4[wxT("ringsvisible")] = true;
        else jMsgBP4[wxT("ringsvisible")] = false;
        jMsgBP4[wxT("ringsnumber")] = m_choiceBoundaryBoundaryPointRingNumber->GetSelection();
        jMsgBP4[wxT("ringssteps")] = atof(m_textCtrlBoundaryBoundaryPointRingStep->GetValue().mb_str());
        jMsgBP4[wxT("ringsunits")] = m_choiceBoundaryBoundaryPointRingUnits->GetSelection();
        jMsgBP4[wxT("ringscolour")] = m_colourPickerBoundaryBoundaryPointRingColour->GetColour().GetAsString();
        jMsgB[wxT("BoundaryPoints")].Item(l_iPointNum++) = jMsgBP4;
    } else if(i == 6) {
        l_sname.Printf(_T("id %i"), l_iPointNum);
        jMsgBP1[wxT("Name")] = l_sname;
        jMsgBP1[wxT("Lat")] = fromDMM_Plugin(m_textCtrlCornerLat->GetValue());
        jMsgBP1[wxT("Lon")] = fromDMM_Plugin(m_textCtrlCornerLon->GetValue()) + 0.0167;
        jMsgBP1[wxT("BoundaryPointType")] = m_choiceBoundaryType->GetString(m_choiceBoundaryType->GetSelection());
        if(m_checkBoxBoundaryPointVisible->IsChecked()) jMsgBP1[wxT("visible")] = true;
        else jMsgBP1[wxT("visible")] = false;
        if(m_checkBoxBoundaryPointRangeRingsVisible->IsChecked()) jMsgBP1[wxT("ringsvisible")] = true;
        else jMsgBP1[wxT("ringsvisible")] = false;
        jMsgBP1[wxT("ringsnumber")] = m_choiceBoundaryBoundaryPointRingNumber->GetSelection();
        jMsgBP1[wxT("ringssteps")] = atof(m_textCtrlBoundaryBoundaryPointRingStep->GetValue().mb_str());
        jMsgBP1[wxT("ringsunits")] = m_choiceBoundaryBoundaryPointRingUnits->GetSelection();
        jMsgBP1[wxT("ringscolour")] = m_colourPickerBoundaryBoundaryPointRingColour->GetColour().GetAsString();
        jMsgB[wxT("BoundaryPoints")].Item(l_iPointNum++) = jMsgBP1;
        l_sname.Printf(_T("id %i"), l_iPointNum);
        jMsgBP2[wxT("Name")] = l_sname;
        jMsgBP2[wxT("Lat")] = fromDMM_Plugin(m_textCtrlCornerLat->GetValue()) + 0.00835;
        jMsgBP2[wxT("Lon")] = fromDMM_Plugin(m_textCtrlCornerLon->GetValue());
        jMsgBP2[wxT("BoundaryPointType")] = m_choiceBoundaryType->GetString(m_choiceBoundaryType->GetSelection());
        if(m_checkBoxBoundaryPointVisible->IsChecked()) jMsgBP2[wxT("visible")] = true;
        else jMsgBP2[wxT("visible")] = false;
        if(m_checkBoxBoundaryPointRangeRingsVisible->IsChecked()) jMsgBP2[wxT("ringsvisible")] = true;
        else jMsgBP2[wxT("ringsvisible")] = false;
        jMsgBP2[wxT("ringsnumber")] = m_choiceBoundaryBoundaryPointRingNumber->GetSelection();
        jMsgBP2[wxT("ringssteps")] = atof(m_textCtrlBoundaryBoundaryPointRingStep->GetValue().mb_str());
        jMsgBP2[wxT("ringsunits")] = m_choiceBoundaryBoundaryPointRingUnits->GetSelection();
        jMsgBP2[wxT("ringscolour")] = m_colourPickerBoundaryBoundaryPointRingColour->GetColour().GetAsString();
        jMsgB[wxT("BoundaryPoints")].Item(l_iPointNum++) = jMsgBP2;
        l_sname.Printf(_T("id %i"), l_iPointNum);
        jMsgBP3[wxT("Name")] = l_sname;
        jMsgBP3[wxT("Lat")] = fromDMM_Plugin(m_textCtrlCornerLat->GetValue()) + 0.0167;
        jMsgBP3[wxT("Lon")] = fromDMM_Plugin(m_textCtrlCornerLon->GetValue()) + 0.0167;
        jMsgBP3[wxT("BoundaryPointType")] = m_choiceBoundaryType->GetString(m_choiceBoundaryType->GetSelection());
        if(m_checkBoxBoundaryPointVisible->IsChecked()) jMsgBP3[wxT("visible")] = true;
        else jMsgBP3[wxT("visible")] = false;
        if(m_checkBoxBoundaryPointRangeRingsVisible->IsChecked()) jMsgBP3[wxT("ringsvisible")] = true;
        else jMsgBP3[wxT("ringsvisible")] = false;
        jMsgBP3[wxT("ringsnumber")] = m_choiceBoundaryBoundaryPointRingNumber->GetSelection();
        jMsgBP3[wxT("ringssteps")] = atof(m_textCtrlBoundaryBoundaryPointRingStep->GetValue().mb_str());
        jMsgBP3[wxT("ringsunits")] = m_choiceBoundaryBoundaryPointRingUnits->GetSelection();
        jMsgBP3[wxT("ringscolour")] = m_colourPickerBoundaryBoundaryPointRingColour->GetColour().GetAsString();
        jMsgB[wxT("BoundaryPoints")].Item(l_iPointNum++) = jMsgBP3;
        jMsgBP4[wxT("Name")] = l_sname;
        jMsgBP4[wxT("Lat")] = fromDMM_Plugin(m_textCtrlCornerLat->GetValue()) + 0.0167;
        jMsgBP4[wxT("Lon")] = fromDMM_Plugin(m_textCtrlCornerLon->GetValue());
        jMsgBP4[wxT("BoundaryPointType")] = m_choiceBoundaryType->GetString(m_choiceBoundaryType->GetSelection());
        if(m_checkBoxBoundaryPointVisible->IsChecked()) jMsgBP4[wxT("visible")] = true;
        else jMsgBP4[wxT("visible")] = false;
        if(m_checkBoxBoundaryPointRangeRingsVisible->IsChecked()) jMsgBP4[wxT("ringsvisible")] = true;
        else jMsgBP4[wxT("ringsvisible")] = false;
        jMsgBP4[wxT("ringsnumber")] = m_choiceBoundaryBoundaryPointRingNumber->GetSelection();
        jMsgBP4[wxT("ringssteps")] = atof(m_textCtrlBoundaryBoundaryPointRingStep->GetValue().mb_str());
        jMsgBP4[wxT("ringsunits")] = m_choiceBoundaryBoundaryPointRingUnits->GetSelection();
        jMsgBP4[wxT("ringscolour")] = m_colourPickerBoundaryBoundaryPointRingColour->GetColour().GetAsString();
        jMsgB[wxT("BoundaryPoints")].Item(l_iPointNum++) = jMsgBP4;
        jMsgBP5[wxT("Name")] = l_sname;
        jMsgBP5[wxT("Lat")] = fromDMM_Plugin(m_textCtrlCornerLat->GetValue()) + 0.00835;
        jMsgBP5[wxT("Lon")] = fromDMM_Plugin(m_textCtrlCornerLon->GetValue()) + 0.0167;;
        jMsgBP5[wxT("BoundaryPointType")] = m_choiceBoundaryType->GetString(m_choiceBoundaryType->GetSelection());
        if(m_checkBoxBoundaryPointVisible->IsChecked()) jMsgBP5[wxT("visible")] = true;
        else jMsgBP5[wxT("visible")] = false;
        if(m_checkBoxBoundaryPointRangeRingsVisible->IsChecked()) jMsgBP5[wxT("ringsvisible")] = true;
        else jMsgBP5[wxT("ringsvisible")] = false;
        jMsgBP5[wxT("ringsnumber")] = m_choiceBoundaryBoundaryPointRingNumber->GetSelection();
        jMsgBP5[wxT("ringssteps")] = atof(m_textCtrlBoundaryBoundaryPointRingStep->GetValue().mb_str());
        jMsgBP5[wxT("ringsunits")] = m_choiceBoundaryBoundaryPointRingUnits->GetSelection();
        jMsgBP5[wxT("ringscolour")] = m_colourPickerBoundaryBoundaryPointRingColour->GetColour().GetAsString();
        jMsgB[wxT("BoundaryPoints")].Item(l_iPointNum++) = jMsgBP5;
    }

    jMsg[wxT("Boundary")] = jMsgB;

    writer.Write( jMsg, MsgString );
    DEBUGSL("About to send message to OD");
    DEBUGSL(MsgString);
    SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
    if(g_ReceivedJSONMessage != wxEmptyString &&  g_ReceivedJSONJSONMsg[wxT("MsgId")].AsString() == wxS("CreateBoundaryPoint")) {
    }
    g_testplugin_pi->ToggleToolbarIcon();
    Show(false);
}

void tpControlDialogImpl::OnButtonClickCreateBoundaryPointJSON( wxCommandEvent& event )
{
    m_bOK = true;
    wxJSONWriter writer;
    wxJSONValue jMsg;
    wxJSONValue jMsgBP;
    wxString    MsgString;

    jMsg[wxT("Source")] = wxT("TESTPLUGIN_PI");
    jMsg[wxT("Type")] = wxT("Request");
    jMsg[wxT("Msg")] = wxS("CreateBoundaryPoint");
    srand((unsigned)time(0));
    int l_rand = (rand()%10000) + 1;
    jMsg[wxT("MsgId")] = wxString::Format(wxT("%i"), l_rand);

    wxString l_name = m_textCtrlBoundaryPointName->GetValue();
    if(l_name.Length() < 1) l_name = wxEmptyString;
    else jMsgBP[wxT("BoundaryPointName")] = l_name;
    if(m_textCtrlBoundaryPointIconName->GetValue().Length() > 0)
        jMsgBP[wxT("IconName")] = m_textCtrlBoundaryPointIconName->GetValue();
    jMsgBP[wxT("Lat")] = fromDMM_Plugin( m_textCtrlLatitude->GetValue() );
    jMsgBP[wxT("Lon")] = fromDMM_Plugin( m_textCtrlLongitude->GetValue() );
    jMsgBP[wxT("BoundaryPointType")] = m_choiceBoundaryPointType->GetString(m_choiceBoundaryPointType->GetSelection());
    jMsgBP[wxT("Visible")] = m_checkBoxBoundaryPointVisible->GetValue();
    jMsgBP[wxT("RingsVisible")] = m_checkBoxRingsVisible->GetValue();
    jMsgBP[wxT("RingsNumber")] = m_choiceBoundaryPointRingNumber->GetSelection();
    jMsgBP[wxT("RingsSteps")] = atof(m_textCtrlBoundaryPointRingStep->GetValue().mb_str());
    jMsgBP[wxT("RingsUnits")] = m_choiceBoundaryPointRingUnits->GetSelection();
    jMsgBP[wxT("RingsColour")] = m_colourPickerBoundaryPointRingColour->GetColour().GetAsString();

    jMsg[wxT("BoundaryPoint")] = jMsgBP;

    writer.Write( jMsg, MsgString );
    SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
    if(g_ReceivedJSONMessage != wxEmptyString &&  g_ReceivedJSONJSONMsg[wxT("MsgId")].AsString() == wxS("CreateBoundaryPoint")) {
    }

    g_testplugin_pi->ToggleToolbarIcon();
    Show(false);
}

void tpControlDialogImpl::OnButtonClickCreateTextPointJSON( wxCommandEvent& event )
{
    m_bOK = true;
    wxJSONValue jMsg;
    wxJSONValue jMsgTP;
    wxJSONValue jMsgTPHL;
    wxJSONWriter writer;
    wxString    MsgString;

    jMsg[wxT("Source")] = wxT("TESTPLUGIN_PI");
    jMsg[wxT("Type")] = wxT("Request");
    jMsg[wxT("Msg")] = wxS("CreateTextPoint");
    srand((unsigned)time(0));
    int l_rand = (rand()%10000) + 1;
    jMsg[wxT("MsgId")] = wxString::Format(wxT("%i"), l_rand);

    wxString l_name = m_textCtrlTextPointName->GetValue();
    if(l_name.Length() < 1) l_name = wxEmptyString;
    jMsgTP[wxT("TextPointName")] = l_name;
    jMsgTP[wxT("IconName")] = m_textCtrlBoundaryPointIconName->GetValue();
    jMsgTP[wxT("Lat")] = fromDMM_Plugin( m_textCtrlLatitude->GetValue() );
    jMsgTP[wxT("Lon")] = fromDMM_Plugin( m_textCtrlLongitude->GetValue() );
    jMsgTP[wxT("visible")] = m_checkBoxBoundaryPointVisible->GetValue();
    jMsgTP[wxT("TextToDisplay")] = m_textCtrlTextPointTextToDisplay->GetValue();
    jMsgTP[wxT("TextPosition")] = m_choiceTextPointTextPosition->GetSelection();
    jMsgTP[wxT("TextColour")] = m_colourPickerTextPointTextColour->GetColour().GetAsString();
    jMsgTP[wxT("BackgroundColour")] = m_colourPickerTextPointTextBackgroundColour->GetColour().GetAsString();
    jMsgTP[wxT("BackgroundTransparancy")] = m_sliderTextPointBackgroundTransparency->GetValue();
    jMsgTP[wxT("TextFont")] = m_staticTextTextPointTextFontExample->GetFont().GetNativeFontInfoDesc();
    jMsgTP[wxT("TextPointDisplayTextWhen")] = m_radioBoxTextPointTextDisplay->GetStringSelection();
    jMsgTP[wxT("defaultRangeRings")] = true;
    jMsgTP[wxT("ringsvisible")] = m_checkBoxRingsVisible->GetValue();
    jMsgTP[wxT("ringsnumber")] = m_choiceBoundaryPointRingNumber->GetSelection();
    jMsgTP[wxT("ringssteps")] = atof(m_textCtrlBoundaryPointRingStep->GetValue().mb_str());
    jMsgTP[wxT("ringsunits")] = m_choiceBoundaryPointRingUnits->GetSelection();
    jMsgTP[wxT("ringscolour")] = m_colourPickerBoundaryPointRingColour->GetColour().GetAsString();
    jMsgTPHL[wxT("LinkDescription")] = _("Test Link 1");
    jMsgTPHL[wxT("LinkURL")] = wxT("http://opencpn.org");
    jMsgTP[wxT("HyperLinks")].Item(0) = jMsgTPHL;
    jMsgTPHL[wxT("LinkDescription")] = _("Test Link 2");
    jMsgTPHL[wxT("LinkURL")] = wxT("http://google.com");
    jMsgTP[wxT("HyperLinks")].Item(1) = jMsgTPHL;

    jMsg[wxT("TextPoint")] = jMsgTP;
    writer.Write( jMsg, MsgString );
    SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
    if(g_ReceivedJSONMessage != wxEmptyString &&  g_ReceivedJSONJSONMsg[wxT("MsgId")].AsString() == wxS("CreateTextPoint")) {
    }

    g_testplugin_pi->ToggleToolbarIcon();
    Show(false);
}

void tpControlDialogImpl::OnButtonClickPointIconJSON(wxCommandEvent& event)
{
    m_bOK = true;
    wxJSONValue jMsg;
    wxJSONValue jMsgPI;
    wxJSONValue jMsgTPHL;
    wxJSONWriter writer;
    wxString    MsgString;

    if(m_radioBoxPointIcon->GetSelection() == 0) {
        wxMemoryBuffer mb;
        mb.AppendData("\211PNG\r\n\032\n\000\000\000\rIHDR\000\000\000 \000\000\000%\b\006\000\000\000#\267\353G\000\000\000\001sRGB\000\256\316\034\351\000\000\000\006bKGD\000\377\000\377\000\377\240\275\247\223\000\000\000\011pHYs\000\000\013\023\000\000\013\023\001\000\232\234\030\000\000\000\atIME\a\335\006\033\003\026\bz)\003\323\000\000\002\322IDATX\303\355\230=L\023a\034\306\177o\271J,\021\224:0\310\200\212`\342\000:p\261Q\373A\200\204Hbc\030pq a\201\311\004''\023\023H\210\203]X\214\023\0031\016\032\"\224\266\370\305@b\023\006\027\025\343\300|U \022\n\355\275\016\265'\005z\275;\212\014\362L\327\267\327\377\363\364\371\177\274\357\035\034\341\220!v.\250\252*+++q\273\335\306\232\224\322<\210\020\005\367J)\021B \204@\327u\204\020H)Q\024\205h4*\212\nhkk\223\311\315V\332\373\206\231\236\211\227\365\237vu\206\210M\214\0228\375\215\331\331Y\203\327\265\375&\217\307s \344\000\3233q\332\373\206\311f\263\005\353\005\002\024E9\020\362\355\"\266\247k\227\000]\327m\333\332\325\031\262Wt;\004(f_\232\021w\267\3240\020\252\006`\274%\314\324\342\212#\367\n\004H)\367\350\213\335\350n\251\241?TE\317\3302\000/\357\325\377\261\270\364owv\224\342$\227S\213+\014\204\252\rb-\273\302\324\342\252\243\024\270\234\026\323x<G\270\246\257\363\374\215p\\\274.'5\00004\362\002\200\023.\217q\355\004\266k\240\2533\304\203\233^\343\363\232\276\316|\244\027\200\207\257\264\222N\230\326\200\035\a.5V\030\0164\237_\377w\016L\317\304\231\236\3119\221/\302;\217\265\362\324\300a\340\377\023P\2269\260\037\330\356\202\371H/\256\232U>~\262\336\357O\356\207\271\\\237\013\355\033\234\334_\n|\203\223\250ug\214\300Vq\265\251zOr\323\024\024;zEb)\006\333k-\211X~\335\303@\250\232H,U\276A\224\263>\314m\277\244N\361\342k\b3\377\035\334B1v\307G\267\302\264\236\2535\004\027K\227\251\000\263A\224\027\00197.\236M\263\252\377\002\240?T\205\0337\221X\n_\003\266\366\006[\2438\037xh\0046\243w9.*\376\006\352xj\255\357]\256\3425\220\315f-\037\261\216u<\333\363\272\324F\226\311d\314\267\343\330\304\250e\021=c\313\306\251\310\352\261<\235N\233?\230\004\203A\231\267*\377\200!\245\344m\252\221\214\367\212\265\274jI\374\336\245\202\265\255\255-666XXX\020\246\002\212!\030\014\312w?.\224\024\241hIn\324~%\036\217[\212my\024'\022\011\341\367.\241h\311\262\221\333r\240\224\023\212\226\344\372\251/$\022\011[1m\013\000\360\373\375\362\303J\263!B\321\222\\;\371\231\271\2719\333\361\034\011\000\b\004\002\362\375\317&\000\307\344\373\206\252\252RUUy\364\226c?\370\r\006\316 \022WO\242\r\000\000\000\000IEND\256B`\202", 850);

        wxString l_sBlueAnchor(wxBase64Encode(mb));

        jMsg[wxT("Source")] = wxT("TESTPLUGIN_PI");
        jMsg[wxT("Type")] = wxT("Request");
        jMsg[wxT("Msg")] = wxS("AddPointIcon");
        srand((unsigned)time(0));
        int l_rand = (rand()%10000) + 1;
        jMsg[wxT("MsgId")] = wxString::Format(wxT("%i"), l_rand);

        jMsgPI[wxT("PointIcon")] = l_sBlueAnchor;
        jMsgPI[wxT("PointIconName")] = wxT("blue_anchor");
        jMsgPI[wxT("PointIconDescription")] = _("Blue Anchor");

        jMsg[wxT("AddPointIcon")] = jMsgPI;
        writer.Write( jMsg, MsgString );
        SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
        if(g_ReceivedJSONMessage != wxEmptyString &&  g_ReceivedJSONJSONMsg[wxT("MsgId")].AsString() == wxS("AddPointIcon")) {
        }

        g_testplugin_pi->ToggleToolbarIcon();
        Show(false);
    } else {
        jMsg[wxT("Source")] = wxT("TESTPLUGIN_PI");
        jMsg[wxT("Type")] = wxT("Request");
        jMsg[wxT("Msg")] = wxS("DeletePointIcon");
        srand((unsigned)time(0));
        int l_rand = (rand()%10000) + 1;
        jMsg[wxT("MsgId")] = wxString::Format(wxT("%i"), l_rand);

        jMsgPI[wxT("PointIconName")] = wxT("blue_anchor");

        jMsg[wxT("DeletePointIcon")] = jMsgPI;
        writer.Write( jMsg, MsgString );
        SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
        if(g_ReceivedJSONMessage != wxEmptyString &&  g_ReceivedJSONJSONMsg[wxT("MsgId")].AsString() == wxS("DeletePointIcon")) {
        }

        g_testplugin_pi->ToggleToolbarIcon();
        Show(false);
    }
}

void tpControlDialogImpl::OnCheckBoxSaveJSONOnStartup(wxCommandEvent& event)
{
    g_bSaveJSONOnStartup = m_checkBoxSaveJSONOnStartup->IsChecked();
}

void tpControlDialogImpl::tpControlCloseClick( wxCommandEvent& event )
{
    m_bOK = false;
    g_testplugin_pi->ToggleToolbarIcon();

    Show(false);
}

void tpControlDialogImpl::tpControlOnClickImportJSON( wxCommandEvent& event )
{

    if(m_checkBoxSaveJSON->IsChecked()) {
        if(!m_filePickerOutputJSON->GetPath()) {
            OCPNMessageBox_PlugIn(NULL, _("No file specified for output"), _("File not found"), wxICON_EXCLAMATION | wxCANCEL);
            return;
        }
        g_testplugin_pi->m_fnOutputJSON = m_filePickerOutputJSON->GetPath();
    } else {
        g_testplugin_pi->m_fnOutputJSON = wxEmptyString;
    }

    if(g_testplugin_pi->m_fnInputJSON.GetFullPath() == wxEmptyString) {
        OCPNMessageBox_PlugIn( NULL, _("No file specified for input"), _("File not found"), wxICON_EXCLAMATION | wxCANCEL );
        return;
    }
    g_testplugin_pi->m_fnInputJSON = m_filePickerInputJSON->GetPath();

    g_testplugin_pi->ImportJSONFile();
    g_testplugin_pi->ToggleToolbarIcon();
    Show(false);
}

void tpControlDialogImpl::SetLatLon( double lat, double lon )
{
    m_textCtrlCornerLat->SetValue( toSDMM_PlugIn(1, lat));
    m_textCtrlCornerLon->SetValue( toSDMM_PlugIn(2, lon));
    m_textCtrlLatitude->SetValue( toSDMM_PlugIn( 1, lat ) );
    m_textCtrlLongitude->SetValue( toSDMM_PlugIn( 2, lon ) );
    m_textCtrlTextPointLatitude->SetValue( toSDMM_PlugIn( 1, lat ) );
    m_textCtrlTextPointLongitude->SetValue( toSDMM_PlugIn( 2, lon ) );
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
    m_fgSizerGUIDS->Layout();
    m_SizerControl->Layout();
    this->GetSizer()->Fit( this );
    this->Layout();
}

void tpControlDialogImpl::SetPanels()
{
    if(g_testplugin_pi->m_bODCreateBoundary) m_buttonCreateBoundaryODAPI->Enable();
    else m_buttonCreateBoundaryODAPI->Disable();
    if(g_testplugin_pi->m_bODCreateBoundaryPoint) m_buttonCreateBoundaryPointODAPI->Enable();
    else m_buttonCreateBoundaryPointODAPI->Disable();
    if(g_testplugin_pi->m_bODCreateTextPoint) m_buttonCreateTextPointODAPI->Enable();
    else m_buttonCreateTextPointODAPI->Disable();
}

wxString tpControlDialogImpl::GetJSONSaveFile( void )
{
    wxString l_OutputFile = m_filePickerOutputJSON->GetPath();
    if(!m_filePickerOutputJSON->GetPath()) {
        return wxEmptyString;
    } else return m_filePickerOutputJSON->GetPath();
}

void tpControlDialogImpl::SetJSONSaveFile(wxString SaveFile)
{
    m_filePickerOutputJSON->SetPath( SaveFile );
    m_checkBoxSaveJSON->Enable();
    m_checkBoxSaveJSON->SetValue(true);
}

wxString tpControlDialogImpl::GetJSONInputFile()
{
    wxString l_InputFile = m_filePickerInputJSON->GetPath();
    if(!m_filePickerInputJSON->GetPath()) {
        return wxEmptyString;
    } else return m_filePickerInputJSON->GetPath();
}

void tpControlDialogImpl::SetJSONInputFile(wxString InputFile)
{
    m_filePickerInputJSON->SetPath( InputFile );
}

void tpControlDialogImpl::OnFileChangeInputJSON(wxFileDirPickerEvent& event)
{
    g_testplugin_pi->m_fnInputJSON = m_filePickerInputJSON->GetPath();
}

void tpControlDialogImpl::OnFileChangeOutputJSON(wxFileDirPickerEvent& event)
{
    g_testplugin_pi->m_fnOutputJSON = m_filePickerOutputJSON->GetPath();
    if(m_filePickerOutputJSON->GetPath() != wxEmptyString) m_checkBoxSaveJSON->Enable();
    else m_checkBoxSaveJSON->Disable();
}

void tpControlDialogImpl::OnCheckBoxSaveJSON(wxCommandEvent& event)
{
    if(m_checkBoxSaveJSON->IsChecked()) g_testplugin_pi->m_bSaveIncommingJSONMessages = true;
    else g_testplugin_pi->m_bSaveIncommingJSONMessages = false;
}

void tpControlDialogImpl::OnCheckBoxCloseSaveFileAfterEachWrite( wxCommandEvent& event )
{
    g_testplugin_pi->UpdateCloseAfterSave(m_checkBoxCloseAferWrite->IsChecked());
    if(m_checkBoxCloseAferWrite->IsChecked()) g_testplugin_pi->m_bCloseSaveFileAfterEachWrite = true;
    else g_testplugin_pi->m_bCloseSaveFileAfterEachWrite = false;
}

void tpControlDialogImpl::OnCheckBoxAppendToFile(wxCommandEvent& event)
{
    g_testplugin_pi->UpdateAppendToFile(m_checkBoxAppendToFile->IsChecked());
    if(m_checkBoxAppendToFile->IsChecked()) g_testplugin_pi->m_bAppendToSaveFile = true;
    else g_testplugin_pi->m_bAppendToSaveFile = false;
}

void tpControlDialogImpl::OnCheckBoxDeleteFromConfig(wxCommandEvent& event)
{
    g_testplugin_pi->m_bRecreateConfig = m_checkBoxRecreateConfig->IsChecked();
}

void tpControlDialogImpl::SetSaveJSONOnStartup(bool SaveJSONOnStartup)
{
    m_checkBoxSaveJSONOnStartup->SetValue(SaveJSONOnStartup);
}

void tpControlDialogImpl::SetAppendToSaveFile(bool AppendToSaveFile)
{
    m_checkBoxAppendToFile->SetValue(AppendToSaveFile);
}

void tpControlDialogImpl::SetCloseFileAfterEachWrite(bool CloseFileAfterEachWrite)
{
    m_checkBoxCloseAferWrite->SetValue(CloseFileAfterEachWrite);
}

void tpControlDialogImpl::SetIncommingJSONMessages(bool IncommingJSONMessages)
{
    m_checkBoxSaveJSON->SetValue(IncommingJSONMessages);
}
