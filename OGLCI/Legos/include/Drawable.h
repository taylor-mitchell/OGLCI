#pragma once
#include "ShaderFactory.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Renderer.h"

enum class DrawMode
{
	BUFFERED,
	SINGLETON
};

class Drawable
{
protected:
	std::shared_ptr<Shader> shader;
	VertexBuffer* vb;
	VertexArray* va;
	IndexBuffer* ib;
	DrawMode drawMode;

public:
	Drawable() : shader(nullptr), vb(nullptr), va(nullptr), ib(nullptr), drawMode(DrawMode::BUFFERED) {};
	Drawable(DrawMode drawMode) : shader(nullptr), vb(nullptr), va(nullptr), ib(nullptr), drawMode(drawMode) {};


	virtual void draw(Renderer& renderer) = 0;
	virtual void update() = 0;
	static std::vector<unsigned int> getIndicesLayout() { return {}; };
	static unsigned int getVertexCount() { return 0; };
	static unsigned int getVertexSize() { return 0; };
	static std::shared_ptr<VertexBufferLayout> getBufferLayout() { return nullptr; };
	static std::shared_ptr<Shader> getShader() { return nullptr; };
};