#include "ShaderSprite.h"
#include "RenderState.h"

ShaderSprite::ShaderSprite() : _program(0), _val(0,0,0,0) {
	_shaderMono = new UberShaderProgram();
	_shaderMono->init(Renderer::uberShaderBody, 
		"#define MODIFY_BASE\n"
		"uniform lowp vec4 userValue;"
		"lowp vec4 modify_base(lowp vec4 base)\n"
		"{\n"
		"lowp float c = (base.r + base.g + base.b)/3.0;\n"
		"return mix(vec4(c, c, c, base.a), base, userValue.r);\n"
		"}\n");

	_shaderAddColor = new UberShaderProgram();
	_shaderAddColor->init(Renderer::uberShaderBody, 
		"#define MODIFY_BASE\n"
		"uniform lowp vec4 userValue;"
		"lowp vec4 modify_base(lowp vec4 base)\n"
		"{\n"
		"return base + userValue;\n"
		"}\n");

	_shaderInvert = new UberShaderProgram();
	_shaderInvert->init(Renderer::uberShaderBody, 
		"#define MODIFY_BASE\n"
		"uniform lowp vec4 userValue;"
		"lowp vec4 modify_base(lowp vec4 base)\n"
		"{\n"
		"\n"
		"return vec4(mix(vec4(1.0, 1.0, 1.0, 1.0) - base, base, userValue.r).rgb, base.a);\n"
		"}\n");
}


ShaderSprite::~ShaderSprite() {
	delete _shaderInvert;
	delete _shaderMono;
	delete _shaderAddColor;
}


void ShaderSprite::setShaderProgram(UberShaderProgram* p) {
	_program = p;
}

void ShaderSprite::setColorShaderProgram() {
	_program = _shaderAddColor;
	//setVal(Vector4(1, 1, 1, 1));
}

void ShaderSprite::setMonoShaderProgram() {
	_program = _shaderMono;
	setVal(Vector4(0.1f, 0.1f, 0.1f, 0.0f));
}

void ShaderSprite::setInvertShaderProgram() {
	_program = _shaderInvert;
	//setVal(Vector4(1, 1, 1, 1));
}

void ShaderSprite::setUniforms(IVideoDriver *driver, ShaderProgram *prog) {
	driver->setUniform("userValue", &_val, 1);
}

void ShaderSprite::doRender(const RenderState &rs) {
	_program->setShaderUniformsCallback(CLOSURE(this, &ShaderSprite::setUniforms));
	rs.renderer->setUberShaderProgram(_program);
	Sprite::doRender(rs);
	rs.renderer->setUberShaderProgram(&Renderer::uberShader);

	_program->setShaderUniformsCallback(UberShaderProgram::ShaderUniformsCallback());
}