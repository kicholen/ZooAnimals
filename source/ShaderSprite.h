#ifndef _SHADERSPRITE_
#define _SHADERSPRITE_

#include "core/UberShaderProgram.h"
#include "core/gl/VideoDriverGLES20.h"
#include "core/gl/oxgl.h"
#include "Sprite.h"

using namespace oxygine;

DECLARE_SMART(ShaderSprite, spShaderSprite);

class ShaderSprite: public Sprite
{
public:
	ShaderSprite();
	~ShaderSprite();

	const Vector4& getVal() const {
		return _val;
	}

	void setVal(const Vector4& v) {
		_val = v;
	}

	void setInvertShaderProgram();
	void setColorShaderProgram();
	void setMonoShaderProgram();

	typedef GetSet<Vector4, const Vector4&, ShaderSprite, &ShaderSprite::getVal, &ShaderSprite::setVal>	TweenVal;
private:
	Vector4 _val;
	UberShaderProgram* _program;

	UberShaderProgram *_shaderMono;
	UberShaderProgram *_shaderAddColor;
	UberShaderProgram *_shaderInvert;

	void setShaderProgram(UberShaderProgram* p);
	void setUniforms(IVideoDriver *driver, ShaderProgram *prog);
	void doRender(const RenderState &rs);
};


#endif