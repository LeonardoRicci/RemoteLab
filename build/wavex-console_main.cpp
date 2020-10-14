// --------------------------------------------------------------------------
//
// This file is part of the RemoteLab software package.
//
// Version 1.0 - September 2020
//
//
// The RemoteLab package is free software; you can use it, redistribute it,
// and/or modify it under the terms of the GNU General Public License
// version 3 as published by the Free Software Foundation. The full text
// of the license can be found in the file LICENSE.txt at the top level of
// the package distribution.
//
// Authors:
//		Alessio Perinelli and Leonardo Ricci
//		Department of Physics, University of Trento
//		I-38123 Trento, Italy
//		alessio.perinelli@unitn.it
//		leonardo.ricci@unitn.it
//		https://github.com/LeonardoRicci/RemoteLab
//
// --------------------------------------------------------------------------

#ifndef INCLUDED_MAINAPP
	#include "wavex-console_main.hpp"
	#define INCLUDED_MAINAPP
#endif

wxIMPLEMENT_APP(MainApp);

MainApp::MainApp(){}

bool MainApp::OnInit()
{
	if (!wxApp::OnInit()) {
		return false;
	}

	wxImage::AddHandler(new wxPNGHandler);

	new GuiFrame("Waveform generator console");

	return true;
}
