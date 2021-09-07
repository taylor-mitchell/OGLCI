#pragma once
#include "ShaderFactory.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "RendererFactory.h"
#include "Texture.h"
#include "TextureFactory.h"

enum class DrawMode
{
	BUFFERED,
	SINGLETON
};

class Drawable
{
protected:
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Texture> texture;

	VertexBuffer* vb;
	VertexArray* va;
	IndexBuffer* ib;
	
	DrawMode drawMode;
	
	float* vertices;
	unsigned int* indices;
	bool dirtyTextureFlag;
	bool isSolidColor;
	unsigned int batchIndex;
	int rendererId;

public:
	Drawable(DrawMode drawMode = DrawMode::BUFFERED, bool isSolidColor = false) : shader(nullptr), vb(nullptr), va(nullptr), ib(nullptr), texture(nullptr), drawMode(drawMode), vertices(nullptr), indices(nullptr), dirtyTextureFlag(true), isSolidColor(isSolidColor), batchIndex(0), rendererId(0) {};

	virtual void draw() = 0;
	virtual void update() = 0;

	static std::vector<unsigned int> getIndicesLayout() { return {}; };
	static unsigned int getVertexCount() { return 0; };
	static unsigned int getVertexSize() { return 0; };
	static std::shared_ptr<VertexBufferLayout> getBufferLayout() { return nullptr; };
	static std::shared_ptr<Shader> getShader() { return nullptr; };

	void setDrawMode(DrawMode dm) { drawMode = dm; };
    DrawMode getDrawMode() { return drawMode; };
	
	void setBatchIndex(unsigned int index) { batchIndex = index; };
	unsigned int getBatchIndex() { return batchIndex; };

	void setTexture(std::string fileName) { texture = TextureFactory::getInstance()->getTexture(fileName); };
	void setTexture(std::shared_ptr<Texture> tex) { texture = tex; };
	std::shared_ptr<Texture> getTexture() { return texture; };

	bool getIsSolidColor() { return isSolidColor; };

	void setRendererId(int rId) { rendererId = rId; };
	int getRendererId() { return rendererId; };

	float* getVertices() { return vertices; };
	unsigned int* getIndices() { return indices; };
};