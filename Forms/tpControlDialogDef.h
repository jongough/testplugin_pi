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
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/clrpicker.h>
#include <wx/checklst.h>
#include <wx/choice.h>
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
		wxTextCtrl* m_textCtrlName;
		wxStaticText* m_staticTextType;
		wxComboBox* m_comboBoxType;
		wxCheckBox* m_checkBoxPathActive;
		wxCheckBox* m_checkBoxVisible;
		wxStaticText* m_staticTextLineColour;
		wxColourPickerCtrl* m_colourPickerLineColour;
		wxStaticText* m_staticTextFillColour;
		wxColourPickerCtrl* m_colourPickerFillColour;
		wxStaticText* m_staticTextBoundaryPointsList;
		wxCheckListBox* m_checkListBoundaryPointList;
		wxPanel* m_panelODAPICreateBoundaryPoint;
		wxStaticText* m_staticTextBPName;
		wxTextCtrl* m_textCtrlBoundaryPointName;
		wxStaticText* m_staticTextLat;
		wxTextCtrl* m_textCtrlLatitude;
		wxStaticText* m_staticTextLon;
		wxTextCtrl* m_textCtrlLongitude;
		wxCheckBox* m_checkBoxBoundaryPointActive;
		wxCheckBox* m_checkBoxBoundaryPointVisible;
		wxCheckBox* m_checkBoxRingsVisible;
		wxStaticText* m_staticText10;
		wxChoice* m_choiceRingNumber;
		wxStaticText* m_staticText1RingStep;
		wxTextCtrl* m_textCtrlRingStep;
		wxStaticText* m_staticTextRinUnits;
		wxChoice* m_choiceRingUnits;
		wxStaticText* m_staticTextRingColour;
		wxColourPickerCtrl* m_colourPickerRingColour;
		wxButton* m_buttonOK;
		wxButton* Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void tpControlOnClickProcessJSON( wxCommandEvent& event ) { event.Skip(); }
		virtual void tpControlOKClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void tpControlCancelClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		tpControlDialogDef( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Test Plugin Control Dialog"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 403,410 ), long style = wxCAPTION|wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP ); 
		~tpControlDialogDef();
	
};

#endif //__TPCONTROLDIALOGDEF_H__
