#include "Window.h"

Window::Window(){}

Window::~Window(){}

bool Window::IsActive()
{
	return active;
}

void Window::SetActive(bool wActive)
{
	active = wActive;
}