#include "GLShaderTriangleStrip.h"
#include "core/VideoDriver.h"
#include "RenderState.h"
#include "core/gl/VideoDriverGLES20.h"
#include "core/gl/ShaderProgramGL.h"

GLShaderTriangleStrip::GLShaderTriangleStrip()
{

	const char* vertexShaderData = "\
									uniform mediump mat4 projection;\
									attribute vec2 a_position;\
									void main() {\
									vec4 position = vec4(a_position, 0.0, 1.0);\
									gl_Position = projection * position;\
									}\
				";

	const char* fragmentShaderData = "\
									precision mediump float;\
									uniform float time;\
									uniform vec4 resolution;\
									void main() {\
										vec2 sp = vec2(.4, .7);\
										vec2 p = sp*5.0 - vec2(10.0);\
										vec2 i = p;\
										float c = 0.6;\
										float inten = .1;\
										float t=0.0;\
										for (int n = 0; n < 4; n++){\
											t += (100.+time * 0.05) * (1.0 - (3.0 / float(n+1)));\
											i = p + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + cos(t + i.x));\
											c += 1.0/length(vec2(p.x / (sin(i.x+t)/inten),p.y / (cos(i.y+t)/inten)));\
										}\
										c /= float(3);\
										c = 1.5-sqrt(c);\
										gl_FragColor = vec4(vec3(c*c*c*c), 333.0) + vec4(0.0, 0.3, 0.5, 1.0);\
									}\
				";
	_program = new ShaderProgramGL();


	int vs = ShaderProgramGL::createShader(GL_VERTEX_SHADER, vertexShaderData, 0, 0);
	int fs = ShaderProgramGL::createShader(GL_FRAGMENT_SHADER, fragmentShaderData, 0, 0);

	int pr = ShaderProgramGL::createProgram(vs, fs, (VertexDeclarationGL*)IVideoDriver::instance->getVertexDeclaration(VERTEX_POSITION));
	_program->init(pr);
}


GLShaderTriangleStrip::~GLShaderTriangleStrip()
{
	delete _program;
	delete[] _swipeVertices;
}


void GLShaderTriangleStrip::doRender(const RenderState &rs) {
	rs.renderer->drawBatch();	

	rs.renderer->getDriver()->setShaderProgram(_program);

	Matrix m = Matrix(rs.transform) * rs.renderer->getViewProjection();
	rs.renderer->getDriver()->setUniform("projection", &m);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	drawSwipe();

	rs.renderer->resetSettings();
}

void GLShaderTriangleStrip::drawSwipe() {
	if (_triangles.length() <= 0) {
		return;
	}

	int counter = 0;
	for (int i = 0; i < _triangles.length(); i++) {
		if (counter == _batchSize) {
			drawTriangleStrip(_batchSize, _swipeVertices);
			counter = 0;
		}
		_swipeVertices[counter].x = _triangles[i].x;
		_swipeVertices[counter].y = _triangles[i].y;
		counter++;
	}
	drawTriangleStrip(_batchSize, _swipeVertices);
}

void GLShaderTriangleStrip::drawTriangleStrip(int count, Vector2 *vertices) {
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLfloat*) vertices);

	float time = 0.0f;
	Vector4 resolution = Vector4(getRoot()->getWidth(), getRoot()->getHeight(), 0.0f, 0.0f);
	IVideoDriver::instance->setUniform("resolution", &resolution, 1);
	IVideoDriver::instance->setUniform("time", time);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, count);

	glDisableVertexAttribArray(0);
}
