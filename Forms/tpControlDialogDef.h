///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 16 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __TPCONTROLDIALOGDEF_H__
#define __TPCONTROLDIALOGDEF_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/stattext.h>
#include <wx/filepicker.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/clrpicker.h>
#include <wx/slider.h>
#include <wx/radiobox.h>
#include <wx/notebook.h>
#include <wx/dialog.h>

#include "wxWTranslateCatalog.h"

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class tpControlDialogDef
///////////////////////////////////////////////////////////////////////////////
class tpControlDialogDef : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_SizerControl;
		wxNotebook* m_notebookControl;
		wxPanel* m_panelGeneral;
		wxPanel* m_panelJSON;
		wxFlexGridSizer* m_fgSizerJSON;
		wxStaticText* m_staticTextJSONFile;
		wxFilePickerCtrl* m_filePickerJSON;
		wxButton* m_buttonProcessJSON;
		wxPanel* m_panelODAPI;
		wxFlexGridSizer* m_fgSizerODAPI;
		wxNotebook* m_notebookODAPI;
		wxPanel* m_panelODAPICreateBoundary;
		wxStaticText* m_staticBoundaryName;
		wxTextCtrl* m_textCtrlBoundaryName;
		wxStaticText* m_staticTextType;
		wxChoice* m_choiceBoundaryType;
		wxCheckBox* m_checkBoxBoundaryActive;
		wxCheckBox* m_checkBoxBoundaryVisible;
		wxStaticText* m_staticTextLineColour;
		wxColourPickerCtrl* m_colourPickerBoundaryLineColour;
		wxStaticText* m_staticTextFillColour;
		wxColourPickerCtrl* m_colourPickerBoundaryFillColour;
		wxStaticText* m_staticTextBoundaryPointsList;
		wxStaticText* m_staticTextCornetLat;
		wxTextCtrl* m_textCtrlCornerLat;
		wxStaticText* m_staticTextCornerLon;
		wxTextCtrl* m_textCtrlCornerLon;
		wxStaticText* m_staticTextNumPoints;
		wxChoice* m_choiceNumberOfPoints;
		wxButton* m_buttonCreateBoundary;
		wxPanel* m_panelODAPICreateBoundaryPoint;
		wxStaticText* m_staticTextBPName;
		wxTextCtrl* m_textCtrlBoundaryPointName;
		wxStaticText* m_staticTextBoundaryPointType;
		wxChoice* m_choiceBoundaryPointType;
		wxStaticText* m_staticTextIconName;
		wxTextCtrl* m_textCtrlBoundaryPointIconName;
		wxStaticText* m_staticTextLat;
		wxTextCtrl* m_textCtrlLatitude;
		wxStaticText* m_staticTextLon;
		wxTextCtrl* m_textCtrlLongitude;
		wxCheckBox* m_checkBoxBoundaryPointVisible;
		wxCheckBox* m_checkBoxRingsVisible;
		wxStaticText* m_staticText10;
		wxChoice* m_choiceBoundaryPointRingNumber;
		wxStaticText* m_staticText1RingStep;
		wxTextCtrl* m_textCtrlBoundaryPointRingStep;
		wxStaticText* m_staticTextRinUnits;
		wxChoice* m_choiceBoundaryPointRingUnits;
		wxStaticText* m_staticTextRingColour;
		wxColourPickerCtrl* m_colourPickerBoundaryPointRingColour;
		wxButton* m_buttonCreateBoundaryPoint;
		wxPanel* m_panelODAPICreateTextPoint;
		wxFlexGridSizer* m_fgSizerTextPoint;
		wxStaticText* m_staticTextTextPointName;
		wxTextCtrl* m_textCtrlTextPointName;
		wxStaticText* m_staticTextTextPointIconName;
		wxTextCtrl* m_textCtrlTextPointIconName;
		wxStaticText* m_staticTextTextPointLat;
		wxTextCtrl* m_textCtrlTextPointLatitude;
		wxStaticText* m_staticTextTextPointLon;
		wxTextCtrl* m_textCtrlTextPointLongitude;
		wxCheckBox* m_checkBoxTextPointVisible;
		wxStaticText* m_staticTextTextPointText;
		wxTextCtrl* m_textCtrlTextPointTextToDisplay;
		wxStaticText* m_staticTextTextPointTextPosition;
		wxChoice* m_choiceTextPointTextPosition;
		wxStaticText* m_staticTextTextPointTextColour;
		wxColourPickerCtrl* m_colourPickerTextPointTextColour;
		wxStaticText* m_staticTextTextPointBackgroundColour;
		wxColourPickerCtrl* m_colourPickerTextPointTextBackgroundColour;
		wxStaticText* m_staticTextTextPointBackgroundTransparancy;
		wxSlider* m_sliderTextPointBackgroundTransparency;
		wxFlexGridSizer* m_fgSizerDisplayText;
		wxStaticText* m_staticTextTextPointFont;
		wxStaticText* m_staticTextTextPointTextFontExample;
		wxButton* m_buttonTextPointFonts;
		wxRadioBox* m_radioBoxTextPointTextDisplay;
		wxButton* m_buttonCreateTextPoint;
		wxButton* Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void tpControlOnClickProcessJSON( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickCreateBoundary( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickCreateBoundaryPoint( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickFonts( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickCreateTextPoint( wxCommandEvent& event ) { event.Skip(); }
		virtual void tpControlCancelClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		tpControlDialogDef( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Test Plugin Control Dialog"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 399,716 ), long style = wxCAPTION|wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP ); 
		~tpControlDialogDef();
	
};

#endif //__TPCONTROLDIALOGDEF_H__
