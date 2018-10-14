#ifndef __MODULEHARDWARE_H__
#define __MODULEHARDWARE_H__

#include "Module.h"

class ModuleHardware : public Module
{
public:
	ModuleHardware(Application* app, bool start_enabled = true);

	bool Init();

	void DrawUI();

	void RefreshSpecs(bool all);

public:

	std::string a;

	char sdl_version[10] = "";
	int cpu_count = 0;
	int cpu_cache = 0;
	float ram_gb = 0.f;
	bool dddnow = false;
	bool avx = false;
	bool avx_2 = false;
	bool altivec = false;
	bool mmx = false;
	bool rdtsc = false;
	bool sse = false;
	bool sse2 = false;
	bool sse3 = false;
	bool sse41 = false;
	bool sse42 = false;
};

#endif 