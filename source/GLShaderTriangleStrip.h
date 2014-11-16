#ifndef _GLSHADERTRIANGLESTRIP_
#define _GLSHADERTRIANGLESTRIP_

#include "oxygine-framework.h"
#include "FlashUtils.h"

using namespace FlashUtils;
using namespace oxygine;

namespace oxygine
{
	class ShaderProgramGL;
}

class GLShaderTriangleStrip : public Actor
{
public:
	GLShaderTriangleStrip(void);
	~GLShaderTriangleStrip(void);

protected:
	void doRender(const RenderState &rs);

	VectorArray<Vector2> _triangles;
	int _batchSize;
private:
	ShaderProgramGL *_program;

	void drawSwipe();
	void drawTriangleStrip(int count, Vector2 *vertices);

	static const int MAX_VERTICES = 256;
	Vector2 _swipeVertices[MAX_VERTICES];
};

#endif