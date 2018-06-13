#pragma once

#include "stdafx.h"
#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/application.h>
#include <gtkmm/window.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <windows.h>
#include "wowObject.h"
#include <psapi.h>
#include "MemMagic.h"
#include "WowPointers.h"
#include <math.h>
#include "WowHelpers.h"
#include "DataModelColumns.h"
#include "InjectAsm.h"
#include "RvxKeys.h"
#include <vector>
#include "CustomRotation.h"
#include <bitset>
#include "Logging.h"



class Gui : public Gtk::Window
{
public:
	Gui();
	~Gui();
	InjectAsm wowInject;
protected:

	
	
	// Called every timeout
	bool onMyTimeOut();

	//Signal handlers:
	void onButtonQuit();
	void onButtonAttached();
	void onButtonUiUpdate();
	void onButtonInject();
	bool attachToWow();
	bool createGui();
	bool addDebugText(Glib::ustring string1, Glib::ustring string2 = "", Glib::ustring string3 = "", Glib::ustring string4 = "", Glib::ustring string5 = "", Glib::ustring string6 = "", bool lineBreak = true);
	//void on_button_buffer1();
	//void on_button_buffer2();
	//bool on_title_press_event(GdkEventButton* button_event);

	WowHelpers wowHelper;
	Gtk::ActionBar windowTitle;

	Gtk::ScrolledWindow scrolledDebugText;
	Gtk::ScrolledWindow scrolledObjList;
	Gtk::TextView debugText;

	Glib::RefPtr<Gtk::TextBuffer> debugTextBuffer;
	Glib::RefPtr<Gtk::TextBuffer> debugTextBufferAlt;


	Gtk::Grid baseLayout;
	Gtk::TreeView wowObjData;
	DataModelColumns wowDataColums;

	Glib::RefPtr<Gtk::ListStore> dataTreeModel;
	Gtk::Button btnAttach;
	Gtk::Button btnUiUpdate;
	Gtk::Button btnInject;

	//std::stringstream debugTextStream;
	UINT_PTR playerBaseAddress;
	UINT_PTR playerGuidAddress;
	UINT_PTR processBaseAddress;
	UINT_PTR playerBase;
	UINT_PTR playerGuid;
	unsigned int maxLengthDebugText = 500;
	bool isWowConnected = false;
	bool isInjected = false;
	bool isUiUpdateEnabled = false;


	//bool wowWindowFound = false;

	


private: 
	bool isRvxAttached = false;
	DWORD hookAddress;
	int seconds = 0;
	int intervall = 20;
	int windowWidth = 354;
	int windowHeight = 303;
	int elementBorderWidth = 5;
	std::vector<RvxKeys> rvxKey;
	CustomRotation customRotation;



};

