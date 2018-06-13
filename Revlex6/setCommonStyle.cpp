#include "stdafx.h"
#include "SetCommonStyle.h"

void SetCommonStyle::setMargin(Gtk::Button &_c, int _m)			{ _c.set_margin_left(_m); _c.set_margin_right(_m); _c.set_margin_top(_m); _c.set_margin_bottom(_m); }
void SetCommonStyle::setMargin(Gtk::Grid &_c, int _m)			{ _c.set_margin_left(_m); _c.set_margin_right(_m); _c.set_margin_top(_m); _c.set_margin_bottom(_m); }
void SetCommonStyle::setMargin(Gtk::TreeView &_c, int _m)		{ _c.set_margin_left(_m); _c.set_margin_right(_m); _c.set_margin_top(_m); _c.set_margin_bottom(_m); }
void SetCommonStyle::setMargin(Gtk::TextView &_c, int _m)		{ _c.set_margin_left(_m); _c.set_margin_right(_m); _c.set_margin_top(_m); _c.set_margin_bottom(_m); }
void SetCommonStyle::setMargin(Gtk::Window &_c, int _m)			{ _c.set_margin_left(_m); _c.set_margin_right(_m); _c.set_margin_top(_m); _c.set_margin_bottom(_m); }
void SetCommonStyle::setMargin(Gtk::ScrolledWindow &_c, int _m) { _c.set_margin_left(_m); _c.set_margin_right(_m); _c.set_margin_top(_m); _c.set_margin_bottom(_m); }
