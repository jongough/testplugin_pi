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

extern testplugin_pi  *g_testplugin_pi;

tpControlDialogImpl::tpControlDialogImpl( wxWindow* parent ) : tpControlDialogDef( parent )
{
}
 
 void tpControlDialogImpl::tpControlOKClick( wxCommandEvent& event )
{
    m_bOK = true;
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
