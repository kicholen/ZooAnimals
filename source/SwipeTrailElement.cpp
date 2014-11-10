#include "SwipeTrailElement.h"
#include "core/VideoDriver.h"
#include "RenderState.h"
#include "core/gl/VideoDriverGLES20.h"
#include "core/gl/ShaderProgramGL.h"

SwipeTrailElement::SwipeTrailElement(int maxInputPoints) {
	_initialDistance = 10;
	_minDistance = 20;
	_pressed = false;
	_touchPoints.setCapacity(maxInputPoints);
	_pointsSimplifier = new ResolverRadialChaikin();
	resolve();

	addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &SwipeTrailElement::onTouchEvent));
	addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &SwipeTrailElement::onTouchEvent));

	const char* vertexShaderData = "\
									uniform mediump mat4 projection;\
									attribute vec2 a_position;\
									void main() {\
									vec4 position = vec4(a_position, 0.0, 1.0);\
									gl_Position = projection * position;\
									}\
									";

	const char* fragmentShaderData = "\
									  uniform mediump vec4 color;\
									  void main() { \
									  gl_FragColor = color; \
									  } \
									  ";

	_program = new ShaderProgramGL();


	int vs = ShaderProgramGL::createShader(GL_VERTEX_SHADER, vertexShaderData, 0, 0);
	int fs = ShaderProgramGL::createShader(GL_FRAGMENT_SHADER, fragmentShaderData, 0, 0);

	int pr = ShaderProgramGL::createProgram(vs, fs, (VertexDeclarationGL*)IVideoDriver::instance->getVertexDeclaration(VERTEX_POSITION));
	_program->init(pr);
}


SwipeTrailElement::~SwipeTrailElement() {
	_pointsSimplifier = 0;
	delete _program;
}

void SwipeTrailElement::doUpdate(const UpdateState &us) {
	_triangles.clear();
	_texCoord.clear();
		
	if (_simplifiedPoints.length() < 2) {
		return;
	}

	_batchSize = generate(_simplifiedPoints, 1);
	int b = generate(_simplifiedPoints, -1);
}

void SwipeTrailElement::doRender(const RenderState &rs) {
	rs.renderer->drawBatch();	

	rs.renderer->getDriver()->setShaderProgram(_program);

	Matrix m = Matrix(rs.transform) * rs.renderer->getViewProjection();
	rs.renderer->getDriver()->setUniform("projection", &m);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	drawAllLines();

	rs.renderer->resetSettings();
}

void SwipeTrailElement::drawAllLines() {
	if (_triangles.length() >= 2) {
		for (int i = 0; i < _triangles.length() - 2; i++) {
			drawSegment(_triangles[i], _triangles[i + 1], true);
		}
	}
	
	if (_simplifiedPoints.length() >= 2) {
		for (int i = 0; i < _simplifiedPoints.length() - 2; i++) {
			drawSegment(_simplifiedPoints[i], _simplifiedPoints[i + 1], false);
		}
	}
	
	/*if (_touchPoints.length() >= 2) {
		for (int i = 0; i < _touchPoints.length() - 2; i++) {
			drawSegment(_touchPoints[i], _touchPoints[i + 1], true);
		}
	}*/
}

/// Draw a line segment.
void SwipeTrailElement::drawSegment(const Vector2& p1, const Vector2& p2, bool otherColor) {
	mVertices[0].x = p1.x;
	mVertices[0].y = p1.y;
	mVertices[1].x = p2.x;
	mVertices[1].y = p2.y;
	drawPrimitives(false, true, 2, otherColor);
}

void SwipeTrailElement::drawPrimitives(bool drawTriangles, bool drawLines, int count, bool otherColor) {
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLfloat*) mVertices);

	if (drawTriangles)
	{
		Vector4 c = otherColor ? Vector4(0.9f, 0.2f, 0.2f, 0.5f) : Vector4(0.5f, 0.5f, 0.5f, 0.5f);
		IVideoDriver::instance->setUniform("color", &c, 1);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, count);
	}

	if (drawLines)
	{
		Vector4 c = otherColor ? Vector4(0.9f, 0.2f, 0.2f, 1.0f) : Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		IVideoDriver::instance->setUniform("color", &c, 1);
		glDrawArrays(GL_LINE_LOOP, 0, count);
	}
	 
	glDisableVertexAttribArray(0);
}

void SwipeTrailElement::onTouchEvent(Event *event) {
	TouchEvent *te = safeCast<TouchEvent*>(event);
	switch(te->type)
	{
	case TouchEvent::TOUCH_DOWN:
		{
			startDrag(te->localPosition);
		}
		break;
	case TouchEvent::TOUCH_UP:
		{
			_pressed = false;
			_lastPoint = Vector2();
			removeEventListener(TouchEvent::MOVE, CLOSURE(this, &SwipeTrailElement::onTouchEvent));
		}
		break;

	case TouchEvent::MOVE:
		{
			onMove(te->localPosition);
		}
		break;
	}
}

void SwipeTrailElement::startDrag(Vector2 localPosition) {
	_pressed = true;
	_touchPoints.clear();
	_lastPoint = localPosition;
	_touchPoints.push(_lastPoint);
	resolve();
	addEventListener(TouchEvent::MOVE, CLOSURE(this, &SwipeTrailElement::onTouchEvent));
}

void SwipeTrailElement::onMove(Vector2 localPosition) {
	// We push only points that exceeded < MIN_LENGTH
	if (!_pressed || _lastPoint.x == 0.0f && _lastPoint.y == 0.0f) {
		return;
	}
	float dx = localPosition.x - _lastPoint.x;
	float dy = localPosition.y - _lastPoint.y;
	float length = scalar::sqrt((dx * dx + dy * dy));

	if ((length < _minDistance) && (_touchPoints.length() > 1 || length < _initialDistance)) {
		return;
	}

	_touchPoints.push(localPosition);
	_lastPoint = localPosition;
		
	//simplify new line
	resolve();
}

void SwipeTrailElement::resolve() {
	_pointsSimplifier->resolve(_touchPoints, _simplifiedPoints);
}

int SwipeTrailElement::generate(DequeArray<Vector2>& input, int mult) {
	int c = _triangles.length();
	if (_endcap <= 0) {
		_triangles.push(input[0]);
	} 
	else {
		Vector2 p = input[0];
		Vector2 p2 = input[1];
		_perp = p;
		_perp -= p2;
		_perp *= _endcap;
		_triangles.push(Vector2(p.x + _perp.x, p.y + _perp.y));
	}
	_texCoord.push(Vector2(0.0f, 0.0f));
		
	for (int i = 1; i < input.length() - 1; i++) {
		Vector2 p = input[i];
		Vector2 p2 = input[i + 1];
			
		//get direction and normalize it
		_perp = p;
		_perp -= p2;
		_perp.normalize();
		
		//get perpendicular
		_perp = Vector2(-_perp.y, _perp.x);
		
		float thick = _thickness * (1.0f - ((i) / (float)(input.length())));
			
		//move outward by thickness
		_perp *= thick / 2.0f;
		
		//decide on which side we are using
		_perp *= mult;
		
		//add the tip of perpendicular
		_triangles.push(Vector2(p.x + _perp.x, p.y + _perp.y));
		//0.0 -> end, transparent
		_triangles.push(Vector2(0.0f, 0.0f));
			
		//add the center point
		_triangles.push(Vector2(p.x, p.y));
		//1.0 -> center, opaque
		_texCoord.push(Vector2(1.0f, 0.0f));
	}
		
	//final point
	if (_endcap <= 0) {
		_triangles.push(input[input.length() - 1]);
	} 
	else {
		Vector2 p = input[input.length() - 2];
		Vector2 p2 = input[input.length() - 1];
		_perp = p2;
		_perp -= p;
		_perp *= _endcap;
		_triangles.push(Vector2(p2.x + _perp.x, p2.y + _perp.y));
	}
	//end cap is transparent
	_texCoord.push(Vector2(0.0f, 0.0f));

	return _triangles.length() - c;
}
	