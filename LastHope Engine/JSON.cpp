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

JSONFile* JSON::CreateJSON(const char * path)
{
	JSON_Value* root_value = json_value_init_object();
	JSON_Object* root_object = json_value_get_object(root_value);

	JSONFile* jsonFile = new JSONFile(root_value, root_object, path);

	return jsonFile;
}

//JSONFile-------------------------------------------------------------------------

JSONFile::JSONFile()
{
}

JSONFile::JSONFile(JSON_Value * _value, JSON_Object * _object, const char* _path)
{
	value = _value;
	object = _object;
	root = _object;
	path = _path;
}

JSONFile::JSONFile(JSONFile & doc)
{
	value = doc.value;
	object = doc.object;
	path = doc.path;
	root = object;
}

JSONFile::~JSONFile()
{
}