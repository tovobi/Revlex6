#include "stdafx.h"
#include "DataModelColumns.h"


DataModelColumns::DataModelColumns()
{
	add(col_guid);
	add(col_name);
	add(col_level);
	add(col_type);
	add(col_gameObjType);
	add(col_distance);
	add(col_healthPercent);
	add(col_health);
	add(col_factionTemplate);
	add(col_factionOffset);
	add(col_npcFlags);
	add(col_unitFlags);
	add(col_dynamicFlags);
}


DataModelColumns::~DataModelColumns()
{
}
