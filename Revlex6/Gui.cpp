#include "stdafx.h"

#include "Gui.h"
#include "sigc++\sigc++.h"
#include "SetCommonStyle.h"
#include <iomanip>  
#include "d3dStuff.h"
#include <iterator> 


/*
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
*/

using namespace std;

Gui::Gui()
	//: vBox(Gtk::ORIENTATION_VERTICAL)
	: wowHelper(L"World of Warcraft"),
	customRotation(wowHelper,wowInject)

{
	// füllt alle Keys mit codes
	RvxKeys::initKeys(rvxKey);
	
	// Creating Textbuffer for debug-window
	debugTextBuffer = Gtk::TextBuffer::create();
	debugTextBufferAlt = Gtk::TextBuffer::create();
	debugText.set_property("editable",0);
	

	createGui();
	

	show_all_children();

	hookAddress = d3dStuff::initD3D(wowHelper.wowMem.windowHandle); // determine endscene-address of dx9
	HINSTANCE hD3D9 = GetModuleHandle(TEXT("D3D9.DLL")); // necessary for determine endscene
	addDebugText("Endscene: ", WowHelpers::decToHexStr(hookAddress));

	// create objects for alle relevant keys
	//for (std::vector<unsigned int>::iterator it = acceptedKeys.begin(); it != acceptedKeys.end(); ++it)
	//{	
	//	int i = std::distance(acceptedKeys.begin(), it);

	//	rvxKey.push_back(RvxKeys());
	//	rvxKey[i].code = acceptedKeys[i];
	//	cout << rvxKey[i].code << " " << (unsigned int)rvxKey[i].code << " " << i << endl;
	//}




	//////////for (int i = 0;i < 256; i++)
	//////////{
	//////////	rvxKey.push_back(RvxKeys());
	//////////}
}

bool Gui::addDebugText(Glib::ustring string1, Glib::ustring string2, Glib::ustring string3, Glib::ustring string4, Glib::ustring string5, Glib::ustring string6, bool lineBreak)
{

	// get uccrent time
	Glib::ustring currentTime = WowHelpers::getDateTime();	
	// add per default line break (remove later if necessary)
	Glib::ustring lineBreakString = "\r\n";
	// get existing text from textbuffer
	Glib::ustring existingText = debugTextBuffer->get_text();
	// get position of \r\n, because the removing of lines which exceeding the buffer size will be deleted from start to findPos
	size_t findPos = 0;

	if (!lineBreak)	{ lineBreakString = "";	}
	string1 = currentTime.append("   ").append(string1).append(string2).append(string3).append(string4);
	cout << string1 << endl;
	existingText.append(string1).append(lineBreakString);

	if (existingText.length() > maxLengthDebugText)
	{
		findPos = existingText.find("\r\n");
		existingText = existingText.substr(findPos+2, existingText.length());
	}

	debugTextBuffer->set_text(existingText);
	debugText.set_buffer(debugTextBuffer);

	// scroll to the bottom
	const Gtk::TextIter endIter = debugTextBuffer->end();
	Glib::RefPtr<Gtk::TextMark> endMark = debugTextBuffer->create_mark(endIter);
	debugText.scroll_to(endMark);
	return true;
}




bool Gui::createGui()
{

	// setting up main window
	set_title("Revlex2");
	set_resizable(false);
	set_border_width(0);
	set_default_size(windowWidth, windowHeight);

	// positioniert das GUI unten links
	int winWidth, winHeight;
	get_default_size(winWidth, winHeight);	
	GdkRectangle myRectangle;
	GdkRectangle *r = &myRectangle;
	auto display = gdk_display_get_default(); 
	auto monitor = gdk_display_get_monitor(display, 0); 
	gdk_monitor_get_geometry(monitor, r);
	move(0, r->height - winHeight);


	//setting up buttons
	btnAttach.add_label("Attach");
	btnUiUpdate.add_label("Ui Update [off]");
	btnInject.add_label("Inject");
	SetCommonStyle::setMargin(btnAttach, 0);
	SetCommonStyle::setMargin(btnUiUpdate, 0);
	SetCommonStyle::setMargin(btnInject, 0);
	btnAttach.signal_clicked().connect(sigc::mem_fun(*this, &Gui::onButtonAttached));
	btnUiUpdate.signal_clicked().connect(sigc::mem_fun(*this, &Gui::onButtonUiUpdate));
	btnInject.signal_clicked().connect(sigc::mem_fun(*this, &Gui::onButtonInject));



	//setting up TreeView/wowObjData
	scrolledObjList.set_size_request(windowWidth - elementBorderWidth * 2, 180);
	scrolledObjList.add(wowObjData);
	//Only show the scrollbars when they are necessary:
	scrolledObjList.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	dataTreeModel = Gtk::ListStore::create(wowDataColums);
	wowObjData.set_model(dataTreeModel);

	Gtk::TreeModel::Row row;
	row = *(dataTreeModel->append());

	wowObjData.append_column("ID", wowDataColums.col_guid);
	wowObjData.append_column("Name", wowDataColums.col_name);
	wowObjData.append_column("Level", wowDataColums.col_level);
	wowObjData.append_column("Type", wowDataColums.col_type);
	wowObjData.append_column("GameIbjType", wowDataColums.col_gameObjType);
	wowObjData.append_column("Distance", wowDataColums.col_distance);
	wowObjData.append_column("Health %", wowDataColums.col_healthPercent);
	wowObjData.append_column("CurHealth", wowDataColums.col_health);
	wowObjData.append_column("FactTempl", wowDataColums.col_factionTemplate);
	wowObjData.append_column("FactOffs", wowDataColums.col_factionOffset);
	wowObjData.append_column("npcFlags", wowDataColums.col_npcFlags);
	wowObjData.append_column("unitFlags", wowDataColums.col_unitFlags);
	wowObjData.append_column("dynamicFlags", wowDataColums.col_dynamicFlags);

	//Make all the columns reorderable
	for (guint i = 0; i < 10; i++)
	{
		auto column = wowObjData.get_column(i);
		column->set_reorderable();
		column->set_resizable();
		column->set_sort_column(2);
	}



	//setting up Debug Window
	debugText.set_size_request(windowWidth - elementBorderWidth * 2, 70);
	scrolledDebugText.set_size_request(windowWidth - elementBorderWidth * 2, 70);
	scrolledDebugText.add(debugText);
	//Only show the scrollbars when they are necessary:
	scrolledDebugText.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);



	// Launch timer every second
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &Gui::onMyTimeOut), intervall);

	// setting up Grid
	add(baseLayout);
	SetCommonStyle::setMargin(baseLayout, elementBorderWidth);
	baseLayout.set_column_spacing(elementBorderWidth);
	baseLayout.set_row_spacing(elementBorderWidth);
	baseLayout.attach(btnAttach, 0, 0, 1, 1);
	baseLayout.attach(btnUiUpdate, 1, 0, 1, 1);
	baseLayout.attach(btnInject, 2, 0, 1, 1);
	baseLayout.attach(scrolledObjList, 0, 1, 8, 5);
	baseLayout.attach(scrolledDebugText, 0, 6, 8, 3);

	return true;
}





void Gui::onButtonInject()
{
	wowInject = InjectAsm(wowHelper);
	if (wowInject.initialized && wowInject.buildCodeCave(hookAddress) && wowInject.changeEndScene(hookAddress) && wowInject.buildActionButtonCave(hookAddress) && wowInject.buildPetActionCave(hookAddress))
	{
		isInjected = true;
	}
	
}

bool Gui::attachToWow()
{
	bool staticConnection;
	bool wowWindowFound;

	// read cfg
	wowHelper.configLoad();


	// Find wow.exe
	wowWindowFound = wowHelper.wowMem.Append();
	addDebugText("wowMem.Append(): ", to_string(wowWindowFound));
	if (wowWindowFound)
	{

		processBaseAddress = wowHelper.wowMem.GetBaseAddress();
		

		

		// Find BaseAdress
		staticConnection = wowHelper.GetStaticClientConnection();
		addDebugText("GetStaticClientConnection()", to_string(staticConnection));
		if (staticConnection)
		{			
			// Retrieve first and basic data of wow-world
			isWowConnected = wowHelper.GetBasicWowData();
			addDebugText("isWowConnected: ", to_string(isWowConnected));
			if (isWowConnected)
			{
				//wowHelper.getPlayerSpells(); // liest alle Spells aus dem Spellbook in einen Vector
				wowHelper.spellsAndRank(); // macht mir eine Map/Dic mit Spellname,Spellrank (es steht immer nur der höchste Rank drin)
				btnAttach.override_color(Gdk::RGBA("green"));
				btnAttach.set_label("Detach");
				// trivial data
				playerBaseAddress = (processBaseAddress + 0x173D390);
				playerGuidAddress = (processBaseAddress + 0x741e30);
				playerBase = wowHelper.wowMem.MemReadUInt(playerBaseAddress);
				playerGuid = wowHelper.wowMem.MemReadUInt(playerGuidAddress);
				/*
				addDebugText("processBaseAddress: ", WowHelpers::decToHexStr(processBaseAddress));
				addDebugText("playerBaseAddress: ", WowHelpers::decToHexStr(playerBaseAddress), "\t", "playerBase: ", WowHelpers::decToHexStr(playerBase));
				addDebugText("playerGuidAddress: ", WowHelpers::decToHexStr(playerGuidAddress), "\t", "playerGuid: ", WowHelpers::decToHexStr(playerGuid));
				*/
				cout << hex << endl;
				cout << "processBaseAddress: " << processBaseAddress << endl;
				cout << "playerBaseAddress: " << playerBaseAddress << "\t playerBase: " << playerBase << endl;
				cout << "playerGuidAddress: " << playerGuidAddress << "\t playerGuid: " << playerGuid << endl;


				return true;
			}
		}
	}
	btnAttach.override_color(Gdk::RGBA("black"));
	return false;
	
	
}


//bool Gui::on_title_press_event(GdkEventButton* button_event)
//{
//	debugTextBufferAlt->set_text("test");
//
//
//	Glib::ustring existingText = debugTextBuffer->get_text();
//	cout << "zzz" << endl;
//	existingText.append("zzz\r\n");
//	debugTextBuffer->set_text(existingText);
//	debugText.set_buffer(debugTextBuffer);
//	return true;
//}


bool Gui::onMyTimeOut()
{
	RvxKeys::processKeyState(rvxKey);


	//if (rvxKey[0x41].getKeyState() > 0 && rvxKey[0xA0].getKeyState() > 0)
	//{
	//	cout << "Shift + A" << endl;
	//}
	
	//test get width and set width
	/*
	Gtk::Allocation rect;
	int baseline;
	baseLayout.get_allocated_size(rect, baseline);
	int tempWidth = rect.get_width();
	scrolledDebugText.set_min_content_width(tempWidth-10);
	std::cout << "width: " << tempWidth << std::endl;
	*/



	// Prepare message
	if (isWowConnected)
	{
		// mal zum test wo es sich denn aufhängt
		if (!wowHelper.GetStaticClientConnection())
		{
			cout << "GetStaticClientConnection() FALSE!" << endl;
		}
		if (!wowHelper.GetBasicWowData())
		{
			cout << "GetBasicWowData() FALSE!" << endl;
		}

		// scans objects of wow
		wowHelper.scanObjLight();
		//wowHelper.getNearEnemies(15, 5);

		// checks if any key is pressed then do a general rotation 
		HWND activeApp = GetForegroundWindow();
		for (RvxKeys k : rvxKey)
		{
			if (k.getKeyState() > 1 && wowHelper.wowMem.windowHandle == activeApp && isInjected && !wowHelper.textCaretActive)
			{
				customRotation.baseRotation(wowInject);
				// checks if key is pressed when hooked wow-app is active
				customRotation.mainRotation(wowInject, rvxKey);
			}
		}

		if (wowHelper.wowMem.windowHandle == activeApp && isInjected && !wowHelper.textCaretActive)
		{
			customRotation.baseRotation(wowInject);
			// checks if key is pressed when hooked wow-app is active
			customRotation.autoRotation(wowInject, rvxKey);
		}


		// checks every 15min all player spells in spell book
		if (wowHelper.lastSpellBookUpdate + wowHelper.spellBookUpdateIntervall < WowHelpers::getTime())
		{
			wowHelper.lastSpellBookUpdate = WowHelpers::getTime();
			wowHelper.getPlayerSpells();
		}


		// fills gtkmm-table with wow-data
		if (isUiUpdateEnabled && wowHelper.lastUiUpdate + wowHelper.uiUpdateIntervall < WowHelpers::getTime())
		{
			dataTreeModel->clear();
			//cout << "wow connected" << endl;
			cout << "-----------------------------------------------------" << endl;

			try
			{
				for (WowObject listObj : wowHelper.cachedUnitlist)
				{
					Gtk::TreeModel::Row row;
					const gchar *gCharName = (listObj.name).c_str();
					Glib::ustring gName = static_cast<Glib::ustring>(listObj.name);
					gsize gLength = sizeof(gName);
					bitset<32> npcFlags_b(listObj.npcFlags);
					bitset<32> unitFlags_b(listObj.unitFlags);
					bitset<32> dynamicFlags_b(listObj.dynamicFlags);
					row = *(dataTreeModel->append());
					
					row[wowDataColums.col_guid] = WowHelpers::decToHexStr(listObj.guid);
					row[wowDataColums.col_name] = g_convert(gCharName, gLength, "UTF-8", "ISO-8859-1", NULL, NULL, NULL);
					row[wowDataColums.col_level] = listObj.level;
					row[wowDataColums.col_type] = (unsigned int)listObj.type;
					row[wowDataColums.col_gameObjType] = listObj.gameObjectType;
					row[wowDataColums.col_distance] = listObj.distance;
					row[wowDataColums.col_healthPercent] = listObj.healthPercent;
					row[wowDataColums.col_health] = listObj.health;
					row[wowDataColums.col_factionTemplate] = listObj.factionTemplate;
					row[wowDataColums.col_factionOffset] = listObj.factionOffset;
					row[wowDataColums.col_npcFlags] = npcFlags_b.to_string();
					row[wowDataColums.col_unitFlags] = unitFlags_b.to_string();
					row[wowDataColums.col_dynamicFlags] = dynamicFlags_b.to_string();
					//std::cout << listObj.name << "\t Health: " << listObj.distance << std::endl;
				}
			}
			catch (int e)
			{
				std::cout << "Error: Ausgabe " << e << std::endl;
			}
			wowHelper.lastUiUpdate = WowHelpers::getTime();
		}
	}

	return true;
}



Gui::~Gui()
{
}

void Gui::onButtonQuit()
{
	hide();
}
void Gui::onButtonAttached()
{
	if (!isWowConnected)
	{

		attachToWow();
	}
	else
	{
		isWowConnected = false;
		btnAttach.set_label("Attach");
		btnAttach.override_color(Gdk::RGBA("black"));
	}

}
void Gui::onButtonUiUpdate()
{
	if (!isUiUpdateEnabled)
	{
		btnUiUpdate.set_label("Ui Update [on]");
		btnUiUpdate.override_color(Gdk::RGBA("green"));
		isUiUpdateEnabled = true;
	}
	else
	{
		isUiUpdateEnabled = false;
		btnUiUpdate.set_label("Ui Update [off]");
		btnUiUpdate.override_color(Gdk::RGBA("black"));
	}

}


//Gtk::Allocation rect1;
//int baseline1;
//get_allocated_size(rect1, baseline1);
//int tempWidth1 = rect1.get_width();
//std::cout << "windowwidth: " << tempWidth1 << std::endl;

