#pragma once
#include <gtkmm.h>
#include <string>
class DataModelColumns : public Gtk::TreeModel::ColumnRecord
{
public:

	DataModelColumns();
	~DataModelColumns();

	Gtk::TreeModelColumn<std::string> col_guid;
	Gtk::TreeModelColumn<Glib::ustring> col_name;
	Gtk::TreeModelColumn<unsigned int> col_level;
	Gtk::TreeModelColumn<unsigned int> col_type;
	Gtk::TreeModelColumn<unsigned int> col_gameObjType;
	Gtk::TreeModelColumn<double> col_distance;
	Gtk::TreeModelColumn<unsigned int> col_healthPercent;
	Gtk::TreeModelColumn<unsigned int> col_health;
	Gtk::TreeModelColumn<unsigned int> col_factionTemplate;
	Gtk::TreeModelColumn<unsigned int> col_factionOffset;
	Gtk::TreeModelColumn<Glib::ustring> col_npcFlags;
	Gtk::TreeModelColumn<Glib::ustring> col_unitFlags;
	Gtk::TreeModelColumn<Glib::ustring> col_dynamicFlags;
};

