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

#include <vector>
#include <string>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <sstream>

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif
#include "wx/thread.h"
#include "wx/button.h"
#include "wx/numdlg.h"
#include "wx/msgdlg.h"
#include "wx/progdlg.h"
#include "wx/dirdlg.h"
#include "wx/wfstream.h"
#include "wx/gdicmn.h"
#include "wx/combobox.h"
#include "wx/arrstr.h"
#include "wx/radiobox.h"
#include "wx/radiobut.h"
#include "wx/bmpbuttn.h"
#include "wx/stattext.h"
#include "wx/statline.h"
#include "wx/spinctrl.h"
#include "wx/aboutdlg.h"
#include "wx/choice.h"

#define SOCKET_BUFFER_SIZE 128
#define CHOICES_AVERAGES_0 "No averages"
#define CHOICES_AVERAGES_1 "4"
#define CHOICES_AVERAGES_2 "16"
#define CHOICES_AVERAGES_3 "32"
#define CHOICES_AVERAGES_4 "64"
#define CHOICES_AVERAGES_5 "128"
#define CHOICES_AVERAGES_0_NR 1
#define CHOICES_AVERAGES_1_NR 4
#define CHOICES_AVERAGES_2_NR 16
#define CHOICES_AVERAGES_3_NR 32
#define CHOICES_AVERAGES_4_NR 64
#define CHOICES_AVERAGES_5_NR 128

class MainApp;
class GuiFrame;
class WorkerThread;
class ContainerWorkspace;

enum
{
	APP_QUIT = wxID_EXIT,
	APP_ABOUT = wxID_ABOUT,
	EVENT_WORKER_STARTUP = wxID_HIGHEST + 1,
	EVENT_BUTTON_TDUP = wxID_HIGHEST + 2,
	EVENT_BUTTON_TDDW = wxID_HIGHEST + 3,
	EVENT_BUTTON_Y1UP = wxID_HIGHEST + 4,
	EVENT_BUTTON_Y1DW = wxID_HIGHEST + 5,
	EVENT_BUTTON_Y2UP = wxID_HIGHEST + 6,
	EVENT_BUTTON_Y2DW = wxID_HIGHEST + 7,
	EVENT_CHOSEN_TRIG_CHAN = wxID_HIGHEST + 8,
	EVENT_CHOSEN_TRIG_EDGE = wxID_HIGHEST + 9,
	EVENT_SPINNER_TRIG_LVL = wxID_HIGHEST + 10,
	EVENT_CHOICE_AVERAGES = wxID_HIGHEST + 11,
	EVENT_BUTTON_RUNPAUSE = wxID_HIGHEST + 12,
	EVENT_BUTTON_TOGGLEMODE = wxID_HIGHEST + 13,
	EVENT_BUTTON_SAVE = wxID_HIGHEST + 14,
	EVENT_CALIBRATION_CH1 = wxID_HIGHEST + 15,
	EVENT_CALIBRATION_CH2 = wxID_HIGHEST + 16
};

class MainApp : public wxApp
{
public:
	MainApp();
	virtual ~MainApp(){}
	virtual bool OnInit() ;
};

class GuiFrame : public wxFrame
{
public:
	GuiFrame(const wxString& title);
	void showAboutDialog(wxCommandEvent&);
	void onWorkerStart(wxCommandEvent&);
	void onChanged(wxCommandEvent&);
	void initializeConstants();
	void knobTdivUp(wxCommandEvent&);
	void knobTdivDw(wxCommandEvent&);
	void updateTdiv();
	void knobY1dvUp(wxCommandEvent&);
	void knobY1dvDw(wxCommandEvent&);
	void changedCalibrationCh1(wxCommandEvent&);
	void updateY1div();
	void knobY2dvUp(wxCommandEvent&);
	void knobY2dvDw(wxCommandEvent&);
	void changedCalibrationCh2(wxCommandEvent&);
	void updateY2div();
	void selectTrigChan(wxCommandEvent&);
	void selectTrigEdge(wxCommandEvent&);
	void selectTrigLevel(wxCommandEvent&);
	void setSpinnerTrigLevelExtrema();
	void selectChoiceAverages(wxCommandEvent&);
	void toggleMode(wxCommandEvent&);
	void selectFileToSave(wxCommandEvent&);
	void togglePauseRun(wxCommandEvent&);

	ContainerWorkspace	*scope_parameters;

private:
	void onFrameQuit(wxCommandEvent&);
	wxStaticText	*statictext_title_tdiv;
	wxStaticText	*statictext_value_tdiv;
	wxStaticLine	*staticline_title_tdiv;
	wxButton	*button_tdiv_up;
	wxButton	*button_tdiv_dw;

	wxStaticText	*statictext_title_y1dv;
	wxStaticText	*statictext_value_y1dv;
	wxStaticLine	*staticline_title_y1dv;
	wxButton	*button_y1dv_up;
	wxButton	*button_y1dv_dw;
	wxStaticLine	*staticline_calibr_ch1;
	wxStaticText	*statictext_title_calibration_ch1;
	wxStaticText	*statictext_calibration_ch1;
	wxButton	*button_calibrate_ch1;

	wxStaticText	*statictext_title_y2dv;
	wxStaticText	*statictext_value_y2dv;
	wxStaticLine	*staticline_title_y2dv;
	wxButton	*button_y2dv_up;
	wxButton	*button_y2dv_dw;
	wxStaticLine	*staticline_calibr_ch2;
	wxStaticText	*statictext_title_calibration_ch2;
	wxStaticText	*statictext_calibration_ch2;
	wxButton	*button_calibrate_ch2;

	wxStaticText	*statictext_title_trig;
	wxStaticText	*statictext_label_trig;
	wxStaticLine	*staticline_title_trig;
	wxRadioBox	*radiobox_trig_chan;
	wxRadioBox	*radiobox_trig_edge;
	wxSpinCtrlDouble *spinner_trig_level;

	wxStaticText	*statictext_title_misc;
	wxStaticLine	*staticline_title_misc;
	wxButton	*button_runpause;
	wxButton	*button_togglemode;
	wxButton	*button_save;
	wxChoice	*choice_averages;

	wxDECLARE_EVENT_TABLE();
};

class WorkerThread : public wxThread
{
public:
	WorkerThread(GuiFrame *frame);

	virtual void *Entry();
	virtual void OnExit();

	ContainerWorkspace	*data_container;
	GuiFrame		*parent_frame;
};

class ContainerWorkspace
{
public:
	bool	send_changes;

	std::vector<std::string>	list_tdiv;
	std::vector<std::string>	list_ydiv_samples;
	std::vector<std::string>	list_ydiv_volts;
	std::vector<std::string>	list_tdiv_txt;
	std::vector<std::string>	list_ydiv_samples_txt;
	std::vector<std::string>	list_ydiv_volts_txt;

	int	tdiv_idx;
	int	y1div_idx;
	int	y2div_idx;
	int	ydiv_size;

	int	trig_edge;
	int	trig_channel;
	double	trig_level;

	double	y1_vps;
	double	y2_vps;
	unsigned int navg;

	bool	change_mode;
	char	mode;

	bool	pause_command;
	bool	save_command;
	std::string	output_file;
};

static const unsigned char icon_64_xoscope_png[4429] = {0211,'P','N','G',015,012,032,012,0,0,0,015,'I','H','D','R',0,0,0,'@',0,0,0,'@',010,06,0,0,0,0252,'i','q',0336,0,0,0,011,'p','H','Y','s',0,0,035,0207,0,0,035,0207,01,0217,0345,0361,'e',0,0,0,031,'t','E','X','t','S','o','f','t','w','a','r','e',0,'w','w','w','.','i','n','k','s','c','a','p','e','.','o','r','g',0233,0356,'<',032,0,0,020,0332,'I','D','A','T','x',0234,0325,0233,0327,'s',034,'W','v',0306,0177,0335,023,0220,047,0,'`','B',06,010,020,' ','@','D','b',031,0304,04,0212,'a',')','r','%','r',0223,0254,']','U','y','w',']','%',0225,'_',0326,0377,0214,0253,0374,'`','o',0271,0312,'r',0331,'k',0312,0253,0325,0212,'J',014,022,023,'H',0202,'$',010,021,0201,010,'C',0244,'A',016,0203,0301,' ','L',0354,0351,0351,0366,0303,0200,'C','4','f',06,0300,0210,0240,'v',0375,'U',0241,'P','}',0373,0364,0271,'}',0317,0234,'{',0316,'=',0337,0275,'-','t',0134,0277,0254,0262,05,'p',0314,0315,0260,'-','{',0307,0226,0313,0276,'n',0350,023,021,036,0233,030,'e','q',0331,025,0325,036,0222,'e',034,'s',0263,0234,'n','z','+',0322,'6','<','2',0210,0307,0347,0216,0222,015,'J',022,'K',0356,'%','N',0374,0177,'4',0,'@','G','W',033,0305,0205,0245,0232,0266,'`','P','B',0222,'%','M',0233,'$',05,0350,0351,0355,'"','/',0267,'@',0323,036,0360,0373,'7',0335,'W',0377,0220,0215,0356,0276,016,0214,0306,'$','D',04,0315,'=','U',0205,'@',0320,0217,0305,'d',0245,0351,0350,0231,0227,0375,06,'%',0276,0276,0376,031,':',0275,016,0275,'N',';',0274,0240,034,0304,'j',0311,0342,0350,0241,0246,'H','[','B',06,0310,0317,'-',0240,0274,0254,0212,0306,0272,0203,'Q',0367,0206,0354,0375,0232,0353,'=',0245,'{','q','{',0335,0354,0257,'=',020,'%',';','8',0334,037,0325,026,013,01,0277,0217,0361,0311,'1','T','E',0211,'y','_',020,04,'D','A',0324,0264,')',0212,0302,0334,0374,',','^',0237,'7','J','^',024,'E',0364,':',0203,'V',0307,'V',0305,0200,0327,0205,0220,',',0323,0333,0337,0315,0203,'G','w',011,'H','/',0275,'g',0327,0216,034,'.',']','x',027,0275,0336,020,0363,'9',0333,'@',017,0327,'o','~',011,0200,0331,'d',0341,0310,0241,'&',0212,0362,0213,0321,0255,0361,0212,0204,0247,'@',0242,'p',0316,';','X',0134,'Z','D',024,'E',0254,'f','+','f',0263,'5',0241,0347,'u','z','=',0373,0366,0326,0222,0273,'+',0217,'O',0277,0270,0214,0307,0353,01,0302,0201,0324,0355,'u','c','1',0305,0326,'7','9','5',016,0300,0266,0354,035,0374,0354,047,0177,0207,0321,0230,024,'S','n',0313,014,'`',0353,0357,0241,0274,0254,'2','r',0275,0260,0350,0342,'f',0363,'5',0374,'~','?',031,0351,'&','B',0212,'L','@',012,0220,0226,0222,0306,0351,0246,0363,'$',047,'%',047,0244,0337,'j',0311,0342,0334,0351,'w',0370,0364,0213,0313,'(',0212,0202,034,012,'q',0363,0316,'U','~',0372,0366,'{',010,'k',0342,0303,0330,0370,010,0335,0275,0235,'d',0244,0233,0270,0370,0326,0317,0343,016,036,022,'4','@','W',0317,'S',06,0355,0321,0363,'W',0226,'C','$',031,0215,032,03,0330,06,'z','x',0373,0307,'?',0303,'`','0','j','d',0375,'~',037,0335,'}',0235,'1','c',0303,'F',0310,0331,0231,'G','c',0375,'!',036,0267,'=',0,'`','b','j',0234,0366,0256,'6',0352,0253,033,'#','2',01,0311,0317,0267,'w',0276,'F',0247,023,'9',0177,0366,'"',')',')','i',0353,0352,'L',0310,0,'E',05,0273,031,031,0263,'s','`',0377,'a','M','{','0','(',0363,'|',0260,'W',0323,'&',010,02,036,0257,07,0213,'Y','k',0200,'@',0300,0217,022,0222,023,0351,'V',0203,03,0365,0207,031,031,035,'f',0306,'1',05,'@',0313,0343,'f',012,0363,0212,0310,0264,'f',03,'p',0353,0356,'u',0226,'=',0313,0234,':','q',0216,0355,0331,';','7',0324,0227,'p',020,034,031,033,0246,'0',0277,'8',0252,'}',0336,'5',027,'y',011,0,0177,0300,0317,'7',0267,0277,'b','n','n',0226,0244,0344,024,'t',0242,016,0177,0300,0207,0325,0222,0305,0251,023,0347,'H','M','I','M',0244,'[',015,0134,013,'N',0376,0370,0347,0217,010,0311,'a','C','n',0317,0336,0311,'/','/',0275,'O',0217,0255,0213,'[',0315,0327,0251,0256,0254,0247,0351,0350,0351,'M',0351,'z',0355,'Y',0300,0353,0363,0262,0270,'8',017,0202,0210,0305,'l','%','%','9','e','K',0364,0266,'w',0265,0322,0334,'r',';','r',']',0230,'_',0314,0304,0324,'8','Y',0231,0331,0374,0374,0355,0367,0242,0242,'}','<','$',034,04,0335,0236,'e','z','m',']',',',',','.',0240,0242,'b','6','Y','(','/',0253,0214,033,0215,'S','S','R',0243,'~','m',')','(','a',0134,023,033,022,'E','m','u','#','C',0366,'A','&',0246,0306,0200,0260,'g','&',031,0223,'9',0177,0346,0342,0246,07,017,' ','n',',',0362,022,'c',0343,'#',0334,0275,0377,'-',0252,012,0271,'9',0371,0344,0345,0344,'c','4','&',0361,0350,0311,'}',0206,0354,03,032,'Y',0177,0300,'O','@',0212,0376,'[','^','^',0344,0273,0216,0326,'D',0272,0215,011,01,0201,0323,'M','o','i',0203,0254,'@','T','F',0330,010,011,'y',0200,0323,'5',0313,0371,0263,0227,'b',0336,'[',';',0250,0357,':','Z',0351,'|',0326,'F','j',0252,'6',012,07,0244,0,'%',0205,'e',011,0275,'d','<','d',0244,0233,0260,0230,0255,'8',0346,'f',0302,0272,03,'~','n',0334,0376,0222,0213,027,0336,0335,0264,'!',022,'2','@','j','J',':','-',0217,0357,'R','X','P','B','j','J',032,':',0235,016,0177,' ',0300,0344,0324,'(',0301,'5',0265,'@','}','M','#',':','Q',0344,'`',0343,021,'M',0273,0212,0312,0223,0247,017,023,0351,'6','.',0332,0332,037,0341,0230,0233,0301,'h','L','B',0222,02,'@',0270,'`','k',0357,'|','B','}',0315,0217,'6',0245,'#',0341,' ','8','<','2','@','g','O',';',0256,0371,'9','B',0252,0212,0325,'l','e','w','q',031,'5','U',015,010,0202,0326,0352,0313,0236,'e','2',0322,'2',0242,'t',0204,'B','r','B',0363,'4',026,0306,0306,'G',0370,0354,0353,0377,'e',0347,0216,0134,'.',0276,0365,'s',0376,'t',0345,0217,0314,'9','g',01,0320,0351,0364,0274,'{',0351,'}',0262,0263,0266,'o',0250,0347,'o',0276,026,0210,0205,'e',0317,'2',0227,'?',0371,010,'Y',016,0362,0336,'/','~',0203,0305,'d','e','a',0311,0305,0345,'O','>','B',012,0206,'=','1',0323,0232,0305,'{','?',0375,'{','t',0372,0365,015,0235,'P',020,0134,017,'/',0346,0341,'V',0313,0256,0205,0242,'(',0134,0373,0366,'s','|','~','/',0307,0217,0234,0212,'d',037,0213,0311,0312,0261,'7','N','F',0344,0346,']','N',0356,'?',0272,0263,0241,0276,0204,0374,'p','t',0334,0316,0222,'{','1',0252,']',016,0312,0314,'9',0265,0204,0310,0220,'}',0,0257,0337,023,'%','+',05,'$',0226,'=',0337,0237,020,0271,0363,0340,'[',0246,0246,047,'(','.',0334,'M','e','y',0265,0346,'^','e','y',015,'#',0343,'v',06,06,'m',0,'t',0366,'<','%','?',0257,0210,0342,0302,0335,'q',0365,'%',0344,01,0242,'(',0320,0321,0325,0306,0322,0322,0342,0232,0277,0205,'(','B','$','(',07,0351,0351,0353,0212,0222,']',0134,'r',0241,0310,0241,'D',0272,0215,0240,0255,0343,'1',0317,'z',0332,'I','I','N',0341,0315,0343,0347,'b',0312,'4',035,'9','M',0222,'1',0134,'h',0251,0252,0312,0215,'[','_',0261,0270,0264,020,'W','g','B',036,0220,0227,'S',0270,'y','B','d','w',05,036,0357,'2',015,'5',0321,'E',0317,'Z',0331,0215,0240,'(',012,0217,0333,036,0360,0244,'=',0234,'=','j',0366,'5',0304,']','J',0247,'$',0247,0262,0257,0262,0216,0266,025,0331,0200,0344,0347,'/','_','~',0314,'O','~',0374,'3',0262,'2',0263,0243,0344,0377,0246,0203,0340,0220,0275,0237,0201,'!',033,'c',0223,'c','x',0275,'/',0371,0305,0264,0264,'t',0212,0362,'K','x',0343,0300,'q',0222,'W','-',0255,0245,0240,0304,0275,0207,0267,030,032,0356,0307,0347,0367,'i','t',011,0202,'@',0316,0256,'<',0254,0226,',',016,0355,0177,'#','R','%','~',0257,0134,0344,'Z','p',0262,0260,030,'&','G',0315,'&',0213,0246,010,'Z',013,0207,0323,0301,0322,0362,02,':','Q',0304,'j',0316,'L',0210,020,031,0262,017,'`',033,010,'W',0231,'z',0235,016,'U',020,010,0311,'2',036,0217,0233,0356,0276,'N',0252,'+',0353,'5',06,0360,'y','=','t',0367,'v','j','t',030,0215,'I',0204,'d',0231,0220,022,'b','b','r',0214,0211,0311,'1',0252,0366,'T','G',014,0220,0220,07,'x',0275,'n',0276,0271,'s',0225,'e',0367,022,031,0351,'&',' ','L','~',0352,0365,'z','N',035,'?','G','F',0206,'9','"',0273,0260,0350,0342,0346,0335,'k',04,'$','?',0351,'i','&',024,'U','!',020,0360,0223,0222,0234,0302,0231,0223,027,'6','E',0210,0310,0241,020,':','Q',0214,'Z','_','@',0230,0210,0325,0353,015,'Q',0367,'$',')',0260,'.',01,0262,0366,0271,0204,'<',0240,0347,'y','7','g','N','^',0210,0252,0350,0202,'A',0211,0247,']','O','8',0320,0360,'F',0244,0315,'6',0320,0303,0333,0347,0242,011,021,'_',02,0204,0210,'^',0247,0213,'{','o',0255,0336,027,'X','o',0360,0261,0236,'K','(',013,0350,'D',021,0267,'{',')',0252,0335,'/',05,0220,0327,'D','v','A',020,'p','{',0243,0367,05,'^',0225,020,0331,'j','$',0344,01,0325,0225,'u',0334,'j',0276,0301,0370,0344,'(',0311,0311,0311,0350,'D',021,0277,'$',0221,0236,0226,0316,0251,'5','i',0251,0246,0252,0201,'o','n',0177,0315,0334,0334,0314,012,'!','"',0340,017,04,0260,'Z','2','9','}','"','v',012,0373,'k',' ',0241,030,0340,0230,'s',0240,'(',012,'&',0223,0211,0305,'E',027,0252,0252,'`','6','[','Y',0134,'X','$','3','3',0223,0244,'U',0363,':','$',0313,'L',0317,'L','a',0261,'X','Y','Z','^','D',020,04,',',0226,'L',0224,0220,0214,0333,0343,'f',0373,0266,0215,0351,0252,037,02,0233,0366,0,0227,0313,0311,0345,0377,0371,017,020,0340,'W',0357,0375,0226,0235,';','r',0,'x',0370,0350,'>',0255,0255,017,'H','J','J',0346,037,'~',0367,0217,021,0236,0376,0223,'?',0377,'7','3',0263,'3',0224,0224,0224,'r',0341,0374,'O',0201,'0',0231,0362,0321,0177,0376,0201,0220,',','s',0362,0344,'Y',0366,'U',0325,'&',0374,0302,0222,'$','1',0347,0234,0305,'l',0262,0220,0226,0226,0236,0360,0363,'k',0261,'i',03,0314,0317,0317,0243,0242,0202,012,0363,0363,'s','d','e',0205,'S',0237,0307,0275,014,0204,0347,'v','P',0226,'#',06,'p','{','<','+',0377,'_',0306,01,0277,0317,037,0341,0361,0334,0356,0350,0370,0260,021,0332,0333,'[','i','y',0330,0214,',',0207,020,'D',0221,0272,0352,06,0216,034,'m',0212,0231,'%','6',0213,'u',015,0340,'p',0314,'2','6','6',0202,'$',05,0230,'s',':','"',0355,'}',0266,036,034,'s',0341,0322,'s','f','v',':',0322,0376,0270,0365,01,0206,025,03,0274,0250,0317,0335,0313,'K','<','h',0271,013,0200,0327,0373,0262,'6',030,037,0267,0323,0242,0204,'0',032,0223,0330,0276,'}',047,0371,0371,0205,0353,0276,'h','{','{','+',0315,0367,'n','G',0256,'U','E',0341,'i',0307,023,024,'U',0341,0370,0361,'S',0233,031,'k','L',0304,0215,01,0243,'c','v',0256,'|',0376,'I',0334,'}',0271,0255,0306,0361,'c','o','R','[',0273,'?',0346,0275,0271,'9',07,0227,'?',0376,010,'E','Q',0330,']',0272,0207,0323,0247,0336,0342,0341,0303,'{','t','t',0264,'!',010,02,0357,0274,0375,013,012,012,0212,0276,'W',0277,'q','=',0340,0331,0263,'v','2',0322,'3',0370,0365,0257,'~',033,'7',0347,'n',05,024,'E',0341,0363,'/','>',0241,0243,0353,'i',0134,03,0334,0277,0177,033,'E','Q',0260,'X',0254,0234,'9','u',036,0203,0301,0300,0261,'c',047,0231,'u',0314,'0','5','9',0316,0275,0373,0267,0371,'U',0301,'o',022,0346,03,'a',0235,'u','@','x','K','+',0343,0265,016,036,0302,';',0266,'f',0263,0205,'@',' ',0366,0266,0371,0324,0364,'$',0243,'c','v',0,0216,0274,0321,0204,0301,020,0236,'b',02,02,0307,0216,0276,0211,' ',010,'8',0235,016,'F',0354,'C',0337,0257,0377,0265,015,0222,'$',0255,04,0264,0340,'+',0323,'V',0233,0205,'N','g',' ','$',0313,04,02,0376,'(','C','t',0264,0267,01,0220,0235,0275,0235,0222,022,0355,0271,0204,035,0333,'w',0220,0227,0227,017,0300,0263,'n','m',015,0260,'Y','h','F','8','>','1',0312,'_','>',0375,'8',034,0355,0201,0302,0302,0350,035,0240,0327,01,0235,'(',020,014,06,0371,0327,0177,0373,'g',0,'N',034,';','E','M','m',03,'~',0237,0217,0301,0241,0347,0,0324,0326,'6',0304,'|',0266,0252,0252,0226,0261,0261,'Q',0354,0366,'A',0334,0236,'e',0322,'c','p',0220,0353,'A','c',0200,0351,0251,0311,0310,0340,01,'T','E',033,037,'%','I',0342,0323,'O','/',047,0324,'A',',','4',0235,'<',0313,0216,0355,'/',031,'!','E',0325,0366,'3','9','5','A','M','m',03,0375,03,'}','(',0212,0202,0301,'`','d','O','Y','E','L',']','%',0305,0245,0244,0244,0244,0342,0363,'y',0351,0353,0355,0246,0261,0361,'P','B',0357,0242,'1','@','u','u',035,036,0217,033,'E','Q',030,034,032,0210,022,'V','T',0205,'Y',0307,'4',026,0213,0225,0214,014,'S','B',035,'A','8',0256,'8',034,'3',0310,'A',')',0352,0236,' ',0212,'T',0355,015,'S',0134,'u',0365,0341,0335,0336,0347,0317,0373,0,'(',')',')',0215,'{',020,'B',0247,0323,'S','V','Z','N','g',0327,'S',06,0207,0373,'_',0315,0,'I','I',0311,0234,'8',021,0336,'T','|',0221,0347,'c',0241,'z','_','-','u','u',0233,0343,0335,'W','c','l','l',0204,0277,'|',0366,'q',0314,'{',0242,'(','r',0362,0344,0331,0310,0265,0327,0353,'e','j','j',02,0200,'=','{',0366,0256,0253,0267,0270,0270,0224,0316,0256,0247,'8','f','g',0360,'z',0335,0244,0246,'n','~',0205,0270,'e',0254,0360,'V',0303,'>','2',0204,0212,0212,0301,'`',' ','/','w',0375,'E','R','n','^','>','F',0243,01,'U','U',0261,0331,0372,'6',0335,0307,0274,0313,0251,'5',0200,0252,0252,014,015,0365,0323,'?','`',0303,037,047,'-',0275,012,'V',0307,0227,'(','(','*',0375,03,'6',0372,07,'l','H',0222,024,'I','k',0371,0371,0205,0350,0365,'/','y',01,025,0225,0345,0245,'E','B',0312,0313,0362,'[',047,0352,'"','d','L','G',0307,0223,'M',0277,0217,0335,'>',0250,0235,02,0335,'=',0235,0334,0272,'u','=','r','m','^',0305,0360,'l',')','b',0254,0335,'C','J',0210,0253,'W',0257,0,'P','S',']',037,0331,';','(','Z','E','i',0367,017,0330,'h','n',0276,0211,0307,0343,0306,'h','0','r',0360,0340,'Q',0352,0352,0302,0213,0247,0322,0335,0345,'8',0235,'s','x','}','^','d','9',0244,'1','Z','<',0364,0364,'t','i','=','@',024,'7','~',0350,0207,0200,' ',0212,034,'=','r',0222,0375,0373,017,'R','Q',036,'>','v','c','{',0336,0313,0265,'k',0237,0343,'Y',')',0256,0244,0240,'D',0363,0275,0233,0264,'>','i',01,0302,01,0134,'@',' ',024,012,'1','1','1',0272,'a',037,'~',0237,0217,05,0327,0274,0326,03,0366,'V','T',0221,0234,0224,'L','H',011,0321,0362,0260,'y',0253,0307,0305,'z','3','@',047,0352,'8','s',0366,02,0,0305,'E','%',0350,0365,0206,0310,0302,'g','i','i',0221,'[',0267,0256,0243,0252,'*',0271,'9','y','4','5',0235,0341,'Q',0353,03,06,0372,'m','<','z','t',0237,0202,0202,'"','v','l',0337,0305,0216,0235,0273,0230,0236,0236,'d','h','x','`',0303,'5',0314,0340,'P','?','*','k',0202,0240,' ',010,0224,0224,0224,'R','V','Z',0236,0360,')',0256,'W',0206,'(','P','V','Z','N','Y','i','y','T',0312,0273,0337,'r',0207,'`','P','"','-','-',0235,0267,0316,'_','"','3','3',0233,0263,0247,0317,0223,0225,0265,015,'U','U','i','n',0276,05,0300,0356,0335,0341,'m',0367,0201,0376,'>','B',033,0320,'n',0375,'+','l',0363,017,0234,05,0302,'.',0220,'H',0371,0356,'t',':',030,034,010,0257,06,017,037,'>',026,'!','d','u',':','=',0307,0216,0276,011,0300,0324,0324,04,0343,023,0243,'T',0224,'W','"',010,02,0376,0200,0237,'a',0373,0340,0272,':',0307,0307,0303,047,'K','4',06,0360,0373,'|',0134,0277,0361,'%','W',0257,'^','a','q','!',0372,'P',0364,'V','A',0210,'1',025,0224,'P','8',010,'^',0275,'z',05,0247,'s','.',0322,0336,0321,0371,035,0252,0252,'b',0266,'X',0251,0250,0250,0322,'<',0223,0237,'_',0300,0216,0355,0273,0,0350,0356,0356,'$','5','5',0235,0302,0202,0260,0353,0267,'?',0215,0237,015,0332,0276,'{',0214,0252,0252,'X',',','V',0255,01,0272,0272,0333,0261,0331,'z','^','_',032,0134,047,06,0250,0352,0313,'4',0330,0332,032,'>',07,'(','I',022,'6','[',0330,'U','k',0253,0353,'c',0226,0273,'U',0373,'j',0,030,034,0354,0307,0357,0363,'Q',0337,020,'^',0240,0255,0256,'"','W','c','f','v',0212,0347,0375,0341,0265,'B','C',0303,01,0255,01,0362,'r',0362,'_',0211,'^',0332,'4','6',0350,'#','o',0205,035,032,032,0352,'G','^',0251,'J',0327,0376,0372,'/',0260,0247,0254,02,0243,0301,'H','(','$','c','{',0336,'C','^','n',01,0273,'r',0362,0,0270,'}',0347,033,'d','9',030,0221,0225,0345,' ','7',0256,0177,0205,0252,'(','d','e','e','S','Q','Q',0245,'5',0300,0256,0234,'<','>',0374,0340,0237,0370,0360,0203,0337,0277,'^',0326,'6',0206,01,0364,'z',035,037,'~',0360,'{','>',0374,0340,0367,021,0262,0364,'y',0177,0370,0327,'/','*','*',0326,'0',0316,0253,'a','0',030,0331,']',0272,07,010,'G','v',0200,0246,0343,0247,0321,0211,':',026,027,0134,'|','}',0365,'s','$',')',0200,0317,0357,0343,0313,0257,'>',0303,0265,'0',0217,'N',0324,'q',0366,0314,05,'t',0242,'.',0232,021,'2',032,0303,04,0210,' ','n',0275,047,0254,0267,022,'T',021,'4',0203,014,04,0374,0214,0215,0215,0,0233,0253,05,'z','{',0237,'1','9','5',0201,'?',0340,047,';','{',033,'G',0217,'4','q',0247,0371,'[',0354,0366,'A',0376,0360,0357,0377,0202,0200,032,0331,0274,'9','z',0244,0211,0354,0354,0360,0361,0231,0357,0305,'x',04,'W',0310,0213,'D','!','K',0301,0215,0205,'V','0',':','j','G','Q',024,'t',':','=',05,05,0353,0347,0364,0302,0202,'"',0364,'z',03,0262,034,'d','h',0260,0237,0312,0312,'j','j','j',033,020,'u','"',0367,037,0334,0215,020,0264,'I','I',0311,034,';','z',0222,0275,'{',0367,01,0260,0274,0274,0244,'5',0300,0334,0234,0203,0317,0256,'|','L','(',0244,' ',05,'%',0362,0327,'|',0355,0361,02,017,037,0336,0343,0341,0303,'{',0233,036,'L',024,0342,'d',0201,027,0204,0310,0251,'7',0317,'a',037,011,0327,02,0271,0271,'y',033,036,0252,0324,0353,015,'d','d',0230,'p',0271,0234,'<','i','{','H','e','e',0270,0254,0336,0267,0257,0216,0322,0262,012,0246,0247,047,'A','U',0311,0311,0311,0217,'x','8',0300,0360,0320,0200,0326,0,0203,'C',0317,0361,'z','W','}','i',0261,'f',0256,0352,'u',06,0336,'8','|','|',0363,03,0215,03,0223,'I',0313,'%',010,010,0250,0252,032,0361,0252,'>','[','7',0316,025,032,0276,0250,0250,'d','S',':',013,012,0213,'p',0271,0234,'x','<',036,024,'E','A',024,0303,0341,'-','9',')',0231,0242,0302,0330,':',0372,'l',0335,'Z',03,'T',0356,0255,'f','f','f',012,'E','Q',0230,0231,0231,0216,0212,'U','z',0275,0216,0375,0373,0243,'O',0207,'l',05,04,'A',' ','/','/',0354,'q',015,'u',0215,'8',']','N',0306,0307,'G',0251,'(',0217,035,0375,0327,0242,0276,0356,'G','t','v','|',0207,',',07,0231,0232,0232,' ','7','7',0177,']','y',')','(','1',';',';',0255,'5','@','F',0206,0211,'w',0336,0376,05,0,0177,0372,0344,0277,'P','~',0240,'=',01,'U','Q','0',0350,015,0134,0272,0370,'n',0244,'m','W','N','^','B','[','g',031,0351,031,'d','g','e',0343,0230,'s','0','l',037,0334,0320,0,0303,0303,0203,0321,0265,0300,'j',0210,0242,0356,07,'3',0200,0254,0310,0350,0326,'9',013,0260,'Y',024,0227,0204,'k',0201,0347,0317,'{','Q',0327,'[','u',01,0375,'+',0213,0241,0270,'Y',0300,'d','2','3','8',0330,'O','o',0337,0263,0270,'|',0334,0226,'@','U',031,035,033,0321,0234,'.',0371,0276,0250,'(',0257,0242,0265,0265,05,0217,0307,0315,0350,0250,'=','n','E',0270,0270,0270,020,011,0262,'q',015,'p',0360,0340,'Q',0246,0247,'&',0371,0346,0233,0257,'_',0371,0305,'6','B','j','j','*','g','N',0235,0177,'e','=','f',0263,0205,0234,']',0271,'L','L',0216,0323,0321,0331,026,0327,0,'O',0333,'[','Q',025,0205,0364,0264,0214,0370,06,0310,'H',0317,0340,0375,'_',0377,0216,0245,0345,'%','d','9',0210,0335,'>',0310,0203,0226,'0','G','p',0350,0340,021,012,'W',0242,0363,0343,'G',017,'"',0225,0327,0245,0213,0357,0222,0224,034,'>',0242,0362,0331,0225,'?',0341,0367,0371,0310,0264,'f','q',0346,'l','x','p',013,'.',027,0327,0256,0177,01,'@','e','e',015,0265,'5',0365,030,014,'F',0322,0322,0322,0266,0314,0313,'j',0353,032,0231,0230,034,'g','d','d',0230,0331,0231,'i',0266,0357,0320,0256,'h',']',013,0363,0364,0366,'v',01,'P','_',0277,0177,0375,0205,0220,0260,0262,'m',05,'a','R',0342,05,'2',0255,0331,0221,0245,'r',0372,'*','z','|',0347,0316,']',0221,0255,'4',0243,0301,0210,0337,0347,'#',')','%','%','"',0273,'z',0233,0315,'d','2','E','V','c','[',0211,0222,0342,'R',0262,0262,0266,0341,'t',':',0270,'u',0373,06,0357,0376,0362,'}',0204,0225,0224,0250,'*',012,'7',0256,0177,0205,',',0207,0310,'0',0231,0251,0252,0252,0335,'<',037,0220,0227,'W','@','n','n','>',0273,'r',0362,0310,'/','x',0311,0322,0356,0255,0250,0302,'j',0315,0242,0266,'f',0277,'f',0200,'u','u',0215,0230,0315,026,'j','k',0352,'#','m',026,0263,0225,0262,0262,012,0262,0262,0266,'Q','V',032,'{',0243,0343,'U','!',010,02,047,'N',0234,'F','@','`',0326,'1',0315,0315,0333,'7','P',025,0205,'P','H',0346,0333,0233,0327,0230,0231,0235,'B','@',0340,0314,0251,0360,0307,026,0177,0323,07,'%','_',05,'-','-','w','y',0322,0366,010,010,'O',0347,0220,0242,'F',016,'[','6',0356,'?',0310,0341,0225,05,0335,017,0263,0373,0371,'W',0300,0241,'C',0307,'@',020,'h',0373,0356,'1',0313,'+',0247,'X','D','Q',0344,0300,0217,016,0323,0330,0370,0362,0263,0277,0377,03,024,0344,'<','`',0224,'s',0321,0177,0,0,0,0,'I','E','N','D',0256,'B','`',0202,};
