#pragma once

class Window 
{
private:
	bool active;
public:
	Window();
	~Window();

	bool IsActive();
	void SetActive(bool wActive);

	virtual void DrawOnEditor() {};
};