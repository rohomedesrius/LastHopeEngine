#pragma once
#include "Parson\parson.h"

class JSONFile
{
public:
	JSONFile();
	JSONFile(JSON_Value* value, JSON_Object* object, const char* path);
	JSONFile(JSONFile& doc);
	~JSONFile();

public:

	//Strings
	void SetInfoString(const char* name, const char* n_info);
	const char* GetInfoString(const char* n_info);

	//Numbers
	void SetInfoNum(const char* name, float num);
	float GetInfoNum(const char* num);

	//Bools
	void SetInfoBool(const char* name, bool value);
	bool GetInfoBool(const char* name);

	//Save Information
	void SaveInfo();

	//Management
	bool Exists();

private:
	JSON_Value*  value = nullptr;
	JSON_Object* info = nullptr;
	JSON_Object* root = nullptr;
	const char* path = nullptr;
};

class JSON
{
public:
	JSON();
	~JSON();

	JSONFile* LoadJSON(const char* path);
};