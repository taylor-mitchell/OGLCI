#pragma once
#include <unordered_map>
#include "Renderer.h"

class RendererFactory
{
private:
	static RendererFactory* instance;
	std::unordered_map<int, std::shared_ptr<Renderer>> m_renderers;

	RendererFactory()
		:m_renderers()
	{
	}

public:
	static RendererFactory* getInstance()
	{
		if (!instance)
		{
			instance = new RendererFactory;
		}
		return instance;
	}

	std::shared_ptr<Renderer> getRenderer(int id);
};