// Revlex6.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <gtkmm.h>
#include "Gui.h"

using namespace Gtk;
int main(int argc, char *argv[])
{
	auto app =
		Application::create(argc, argv,
			"org.gtkmm.examples.base");

	Gui window;


	return app->run(window);
}