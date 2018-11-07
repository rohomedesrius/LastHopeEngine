#pragma once

#include "Application.h"

enum ResType
{
	RSS_MATERIAL = 0,
	RSS_MESH,
	UNKNOWN
};

class Resources
{
public:
	Resources(uint uid, ResType type) : type(type), id(uid) {};
	virtual ~Resources() {};
	
	/*
	Resource::ResType GetType() const;
	UID GetUID() const;
	const char* GetFile() const;
	const char* GetExportedFile() const;
	bool IsLoadedToMemory() const;
	bool LoadToMemory();
	uint CountReferences() const;
	
	//virtual void Save(Config& config) const;
	//virtual void Load(const Config& config);
	virtual bool LoadInMemory() = 0;*/
	
public:

	uint id = NULL;
	ResType type = UNKNOWN;

};