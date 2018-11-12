#include "JSON.h"

JSON::JSON()
{
	
}

JSON::~JSON()
{
}

JSONFile* JSON::LoadJSON(const char * path)
{
	JSON_Value *user_data = json_parse_file(path);
	JSON_Object *root_object = json_value_get_object(user_data);

	JSONFile* jsonFile = new JSONFile(user_data, root_object, path);

	return jsonFile;
}

//JSONFile-------------------------------------------------------------------------

JSONFile::JSONFile()
{
}

JSONFile::JSONFile(JSON_Value * _value, JSON_Object * _object, const char* _path)
{
	value = _value;
	info = _object;
	root = _object;
	path = _path;
}

JSONFile::JSONFile(JSONFile & doc)
{
	value = doc.value;
	info = doc.info;
	path = doc.path;
	root = info;
}

JSONFile::~JSONFile()
{
}

//Strings------------------------------------------
void JSONFile::SetInfoString(const char* name, const char* n_info)
{
	json_object_dotset_string(info, name, n_info);
}

const char* JSONFile::GetInfoString(const char* n_info)
{
	return (json_object_dotget_string(info, n_info));
}

//Numbers------------------------------------------
void JSONFile::SetInfoNum(const char* name, float num) 
{
	json_object_dotset_number(info, name, num);
}

float JSONFile::GetInfoNum(const char* name)
{
	return(json_object_dotget_number(info, name));
}

//Bools--------------------------------------------
void JSONFile::SetInfoBool(const char* name, bool value)
{
	json_object_dotset_boolean(info, name, value);
}

bool JSONFile::GetInfoBool(const char* name)
{
	return (json_object_dotget_boolean(info, name));
}

//Save Information----------------------------------
void JSONFile::SaveInfo()
{
	json_serialize_to_file_pretty(value, path);
}

//Management----------------------------------------
bool JSONFile::Exists()
{
	if (value == nullptr)
		return false;
	else
		return true;
}

//Creation-----------------------------------------
JSONFile* JSONFile::Create(const char* path)
{
	JSONFile* ret;

	JSON_Value* init_value = json_value_init_object();

	if (value == nullptr)
	{
		JSON_Object* root_object = json_value_get_object(init_value);
	
		JSONFile* new_json = new JSONFile(init_value, root_object, path);
	
		json_serialize_to_file(value, path);
	
		return new_json;
	}

	return ret;
}