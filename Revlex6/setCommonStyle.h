#pragma once
#include <gtkmm.h>
#include <gtkmm/box.h>
#include <gtkmm/window.h>
class SetCommonStyle : public Gtk::Container
{
public:
	static void SetCommonStyle::setMargin(Gtk::Button &_c, int _m);
	static void SetCommonStyle::setMargin(Gtk::Grid &_c, int _m);
	static void SetCommonStyle::setMargin(Gtk::TreeView &_c, int _m);
	static void SetCommonStyle::setMargin(Gtk::TextView &_c, int _m);
	static void SetCommonStyle::setMargin(Gtk::Window &_c, int _m);
	static void SetCommonStyle::setMargin(Gtk::ScrolledWindow &_c, int _m);
};

