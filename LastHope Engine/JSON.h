#pragma once
#include "Parson\parson.h"

class JSONFile
{
public:
	JSONFile();
	JSONFile(JSON_Value* value, JSON_Object* object, const char* path);
	JSONFile(JSONFile& doc);
	~JSONFile();

private:
	JSON_Value*  value = nullptr;
	JSON_Object* object = nullptr;
	JSON_Object* root = nullptr;
	const char* path = nullptr;
};

class JSON
{
public:
	JSON();
	~JSON();

	JSONFile* LoadJSON(const char* path);
	JSONFile* CreateJSON(const char * path);
};