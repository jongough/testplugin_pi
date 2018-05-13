///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 16 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "tpControlDialogDef.h"

///////////////////////////////////////////////////////////////////////////

tpControlDialogDef::tpControlDialogDef( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_SizerControl = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 2, 0, 0, 0 );
	fgSizer3->AddGrowableCol( 0 );
	fgSizer3->AddGrowableRow( 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_notebookControl = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelGeneral = new wxPanel( m_notebookControl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_notebookControl->AddPage( m_panelGeneral, _("General"), false );
	m_panelJSON = new wxPanel( m_notebookControl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_fgSizerJSON = new wxFlexGridSizer( 0, 0, 0, 0 );
	m_fgSizerJSON->SetFlexibleDirection( wxBOTH );
	m_fgSizerJSON->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextJSONFile = new wxStaticText( m_panelJSON, wxID_ANY, _("Select JSON file"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextJSONFile->Wrap( -1 );
	m_fgSizerJSON->Add( m_staticTextJSONFile, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_filePickerJSON = new wxFilePickerCtrl( m_panelJSON, wxID_ANY, wxEmptyString, _("Select a JSON file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	m_fgSizerJSON->Add( m_filePickerJSON, 0, wxALL, 5 );
	
	m_buttonProcessJSON = new wxButton( m_panelJSON, wxID_ANY, _("Process JSON"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fgSizerJSON->Add( m_buttonProcessJSON, 0, wxALL, 5 );
	
	
	m_fgSizerJSON->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	m_panelJSON->SetSizer( m_fgSizerJSON );
	m_panelJSON->Layout();
	m_fgSizerJSON->Fit( m_panelJSON );
	m_notebookControl->AddPage( m_panelJSON, _("JSON"), false );
	m_panelODAPI = new wxPanel( m_notebookControl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_fgSizerODAPI = new wxFlexGridSizer( 0, 0, 0, 0 );
	m_fgSizerODAPI->AddGrowableCol( 0 );
	m_fgSizerODAPI->AddGrowableRow( 0 );
	m_fgSizerODAPI->SetFlexibleDirection( wxBOTH );
	m_fgSizerODAPI->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_notebookODAPI = new wxNotebook( m_panelODAPI, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelODAPICreateBoundary = new wxPanel( m_notebookODAPI, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer5->AddGrowableCol( 1 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticBoundaryName = new wxStaticText( m_panelODAPICreateBoundary, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticBoundaryName->Wrap( -1 );
	fgSizer5->Add( m_staticBoundaryName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrlName = new wxTextCtrl( m_panelODAPICreateBoundary, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_textCtrlName, 1, wxALIGN_RIGHT|wxALL, 5 );
	
	m_staticTextType = new wxStaticText( m_panelODAPICreateBoundary, wxID_ANY, _("Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextType->Wrap( -1 );
	fgSizer5->Add( m_staticTextType, 0, wxALL, 5 );
	
	m_comboBoxType = new wxComboBox( m_panelODAPICreateBoundary, wxID_ANY, _("Exclusion\nInclusion\nNeither"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer5->Add( m_comboBoxType, 1, wxALIGN_RIGHT|wxALL, 5 );
	
	m_checkBoxPathActive = new wxCheckBox( m_panelODAPICreateBoundary, wxID_ANY, _("Active"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_checkBoxPathActive->SetValue(true); 
	fgSizer5->Add( m_checkBoxPathActive, 0, wxALL, 5 );
	
	m_checkBoxVisible = new wxCheckBox( m_panelODAPICreateBoundary, wxID_ANY, _("Visible"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxVisible->SetValue(true); 
	fgSizer5->Add( m_checkBoxVisible, 1, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextLineColour = new wxStaticText( m_panelODAPICreateBoundary, wxID_ANY, _("Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLineColour->Wrap( -1 );
	fgSizer6->Add( m_staticTextLineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_colourPickerLineColour = new wxColourPickerCtrl( m_panelODAPICreateBoundary, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer6->Add( m_colourPickerLineColour, 0, wxALL, 5 );
	
	
	fgSizer5->Add( fgSizer6, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextFillColour = new wxStaticText( m_panelODAPICreateBoundary, wxID_ANY, _("Fill Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFillColour->Wrap( -1 );
	fgSizer7->Add( m_staticTextFillColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_colourPickerFillColour = new wxColourPickerCtrl( m_panelODAPICreateBoundary, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer7->Add( m_colourPickerFillColour, 1, wxALIGN_RIGHT|wxALL, 5 );
	
	
	fgSizer5->Add( fgSizer7, 1, wxEXPAND, 5 );
	
	m_staticTextBoundaryPointsList = new wxStaticText( m_panelODAPICreateBoundary, wxID_ANY, _("Boundary Points"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBoundaryPointsList->Wrap( -1 );
	fgSizer5->Add( m_staticTextBoundaryPointsList, 0, wxALL, 5 );
	
	wxArrayString m_checkListBoundaryPointListChoices;
	m_checkListBoundaryPointList = new wxCheckListBox( m_panelODAPICreateBoundary, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_checkListBoundaryPointListChoices, 0 );
	fgSizer5->Add( m_checkListBoundaryPointList, 1, wxALL, 5 );
	
	
	m_panelODAPICreateBoundary->SetSizer( fgSizer5 );
	m_panelODAPICreateBoundary->Layout();
	fgSizer5->Fit( m_panelODAPICreateBoundary );
	m_notebookODAPI->AddPage( m_panelODAPICreateBoundary, _("Create Boundary"), true );
	m_panelODAPICreateBoundaryPoint = new wxPanel( m_notebookODAPI, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer8;
	fgSizer8 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer8->AddGrowableCol( 0 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer10->AddGrowableCol( 0 );
	fgSizer10->AddGrowableCol( 1 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextBPName = new wxStaticText( m_panelODAPICreateBoundaryPoint, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBPName->Wrap( -1 );
	fgSizer10->Add( m_staticTextBPName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrlBoundaryPointName = new wxTextCtrl( m_panelODAPICreateBoundaryPoint, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_textCtrlBoundaryPointName, 1, wxALIGN_RIGHT|wxALL, 5 );
	
	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer9->AddGrowableCol( 1 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextLat = new wxStaticText( m_panelODAPICreateBoundaryPoint, wxID_ANY, _("Lat"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLat->Wrap( -1 );
	fgSizer9->Add( m_staticTextLat, 0, wxALL, 5 );
	
	m_textCtrlLatitude = new wxTextCtrl( m_panelODAPICreateBoundaryPoint, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer9->Add( m_textCtrlLatitude, 1, wxALIGN_RIGHT|wxALL, 5 );
	
	
	fgSizer10->Add( fgSizer9, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer11->AddGrowableCol( 1 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextLon = new wxStaticText( m_panelODAPICreateBoundaryPoint, wxID_ANY, _("Lon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLon->Wrap( -1 );
	fgSizer11->Add( m_staticTextLon, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrlLongitude = new wxTextCtrl( m_panelODAPICreateBoundaryPoint, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer11->Add( m_textCtrlLongitude, 1, wxALIGN_RIGHT|wxALL, 5 );
	
	
	fgSizer10->Add( fgSizer11, 1, wxEXPAND, 5 );
	
	m_checkBoxBoundaryPointActive = new wxCheckBox( m_panelODAPICreateBoundaryPoint, wxID_ANY, _("Active"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_checkBoxBoundaryPointActive->SetValue(true); 
	fgSizer10->Add( m_checkBoxBoundaryPointActive, 0, wxALL, 5 );
	
	m_checkBoxBoundaryPointVisible = new wxCheckBox( m_panelODAPICreateBoundaryPoint, wxID_ANY, _("Visible"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_checkBoxBoundaryPointVisible->SetValue(true); 
	fgSizer10->Add( m_checkBoxBoundaryPointVisible, 1, wxALL, 5 );
	
	m_checkBoxRingsVisible = new wxCheckBox( m_panelODAPICreateBoundaryPoint, wxID_ANY, _("Rings Visible"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_checkBoxRingsVisible->SetValue(true); 
	fgSizer10->Add( m_checkBoxRingsVisible, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer13;
	fgSizer13 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer13->SetFlexibleDirection( wxBOTH );
	fgSizer13->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText10 = new wxStaticText( m_panelODAPICreateBoundaryPoint, wxID_ANY, _("Number of Rings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	fgSizer13->Add( m_staticText10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_choiceRingNumberChoices[] = { _("0"), _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7"), _("8"), _("9"), _("10") };
	int m_choiceRingNumberNChoices = sizeof( m_choiceRingNumberChoices ) / sizeof( wxString );
	m_choiceRingNumber = new wxChoice( m_panelODAPICreateBoundaryPoint, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceRingNumberNChoices, m_choiceRingNumberChoices, 0 );
	m_choiceRingNumber->SetSelection( 0 );
	fgSizer13->Add( m_choiceRingNumber, 0, wxALL, 5 );
	
	
	fgSizer10->Add( fgSizer13, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer131;
	fgSizer131 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer131->SetFlexibleDirection( wxBOTH );
	fgSizer131->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1RingStep = new wxStaticText( m_panelODAPICreateBoundaryPoint, wxID_ANY, _("Rings Step"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1RingStep->Wrap( -1 );
	fgSizer131->Add( m_staticText1RingStep, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrlRingStep = new wxTextCtrl( m_panelODAPICreateBoundaryPoint, wxID_ANY, _("0.5"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_textCtrlRingStep->HasFlag( wxTE_MULTILINE ) )
	{
	m_textCtrlRingStep->SetMaxLength( 4 );
	}
	#else
	m_textCtrlRingStep->SetMaxLength( 4 );
	#endif
	fgSizer131->Add( m_textCtrlRingStep, 0, wxALL, 5 );
	
	
	fgSizer10->Add( fgSizer131, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer14->SetFlexibleDirection( wxBOTH );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextRinUnits = new wxStaticText( m_panelODAPICreateBoundaryPoint, wxID_ANY, _("Step Units"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRinUnits->Wrap( -1 );
	fgSizer14->Add( m_staticTextRinUnits, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_choiceRingUnitsChoices[] = { _("Nautical Miles"), _("Kilometers") };
	int m_choiceRingUnitsNChoices = sizeof( m_choiceRingUnitsChoices ) / sizeof( wxString );
	m_choiceRingUnits = new wxChoice( m_panelODAPICreateBoundaryPoint, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceRingUnitsNChoices, m_choiceRingUnitsChoices, 0 );
	m_choiceRingUnits->SetSelection( 0 );
	fgSizer14->Add( m_choiceRingUnits, 0, wxALL, 5 );
	
	
	fgSizer10->Add( fgSizer14, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer15;
	fgSizer15 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer15->SetFlexibleDirection( wxBOTH );
	fgSizer15->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextRingColour = new wxStaticText( m_panelODAPICreateBoundaryPoint, wxID_ANY, _("Ring Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRingColour->Wrap( -1 );
	fgSizer15->Add( m_staticTextRingColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_colourPickerRingColour = new wxColourPickerCtrl( m_panelODAPICreateBoundaryPoint, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer15->Add( m_colourPickerRingColour, 0, wxALL, 5 );
	
	
	fgSizer10->Add( fgSizer15, 1, wxEXPAND, 5 );
	
	
	fgSizer8->Add( fgSizer10, 1, wxEXPAND, 5 );
	
	
	m_panelODAPICreateBoundaryPoint->SetSizer( fgSizer8 );
	m_panelODAPICreateBoundaryPoint->Layout();
	fgSizer8->Fit( m_panelODAPICreateBoundaryPoint );
	m_notebookODAPI->AddPage( m_panelODAPICreateBoundaryPoint, _("Create Boundary Point"), false );
	
	m_fgSizerODAPI->Add( m_notebookODAPI, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panelODAPI->SetSizer( m_fgSizerODAPI );
	m_panelODAPI->Layout();
	m_fgSizerODAPI->Fit( m_panelODAPI );
	m_notebookControl->AddPage( m_panelODAPI, _("ODAPI"), true );
	
	fgSizer3->Add( m_notebookControl, 0, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 0, 0, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizerOKCancel;
	bSizerOKCancel = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOK = new wxButton( this, wxID_ANY, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerOKCancel->Add( m_buttonOK, 0, wxALL, 5 );
	
	Cancel = new wxButton( this, wxID_ANY, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerOKCancel->Add( Cancel, 0, wxALL, 5 );
	
	
	fgSizer4->Add( bSizerOKCancel, 1, wxEXPAND, 5 );
	
	
	fgSizer3->Add( fgSizer4, 1, wxEXPAND, 5 );
	
	
	m_SizerControl->Add( fgSizer3, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( m_SizerControl );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_buttonProcessJSON->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tpControlDialogDef::tpControlOnClickProcessJSON ), NULL, this );
	m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tpControlDialogDef::tpControlOKClick ), NULL, this );
	Cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tpControlDialogDef::tpControlCancelClick ), NULL, this );
}

tpControlDialogDef::~tpControlDialogDef()
{
	// Disconnect Events
	m_buttonProcessJSON->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tpControlDialogDef::tpControlOnClickProcessJSON ), NULL, this );
	m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tpControlDialogDef::tpControlOKClick ), NULL, this );
	Cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( tpControlDialogDef::tpControlCancelClick ), NULL, this );
	
}
