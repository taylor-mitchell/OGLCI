#include "RendererFactory.h"

RendererFactory* RendererFactory::instance = new RendererFactory;

std::shared_ptr<Renderer> RendererFactory::getRenderer(int id)
{
	if (m_renderers.find(id) == m_renderers.end())
	{
		m_renderers.insert(std::pair<int, std::shared_ptr<Renderer>>(id, std::make_shared<Renderer>()));
	}
	return m_renderers.at(id);
}
