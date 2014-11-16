#include "ResolverRadialChaikin.h"


ResolverRadialChaikin::ResolverRadialChaikin() {
	_iterations = 2;
	_simplifyTolerance = 35.0f;
}


ResolverRadialChaikin::~ResolverRadialChaikin() {
	_tempArray.clear();
}

void ResolverRadialChaikin::resolve(DequeArray<Vector2>& input, DequeArray<Vector2>& output) {
	output.clear();

	if (input.length() <= 2) {
		output.addAll(input);
		return;
	}

	//simplify with squared tolerance
	if (_simplifyTolerance > 0 && input.length() > 3) {
		simplify(input, _simplifyTolerance * _simplifyTolerance, _tempArray);
		input.clear();
		input.addAll(_tempArray);
		//input = _tempArray;
	}

	// perform smooth operations
	if (_iterations <= 0) { //no smooth, just copy input to output
		output.addAll(input);
	} 
	else if (_iterations == 1) { // 1 iteration, smooth to output
		smooth(input, output);
	} 
	else { //multiple iterations.. ping-pong between arrays
		int iters = _iterations;
		//subsequent iterations
		do {
			smooth(input, output);
			_tempArray.clear();
			_tempArray.addAll(output);
			DequeArray<Vector2> old = output;
			input.clear();
			input.addAll(_tempArray);
			output.clear();
			output.addAll(old);
		} while (--iters > 0);
	}
}

void ResolverRadialChaikin::smooth(DequeArray<Vector2>& input, DequeArray<Vector2>& output) {
	//expected size
	output.clear();
	//output.ensureCapacity(input.size*2);
	//first element
	output.push(input[0]);

	//average elements
	for (int i = 0; i < input.length() - 1; i++) {
		Vector2 p0 = input[i];
		Vector2 p1 = input[i + 1];
	
		Vector2 Q = Vector2(0.75f * p0.x + 0.25f * p1.x, 0.75f * p0.y + 0.25f * p1.y);
		Vector2 R = Vector2(0.25f * p0.x + 0.75f * p1.x, 0.25f * p0.y + 0.75f * p1.y);
		output.push(Q);
		output.push(R);
	}
		
	//last element
	output.push(input[input.length() - 1]);
}

/*
* simple distance-based simplification
* adapted from simplify.js
*/
void ResolverRadialChaikin::simplify(DequeArray<Vector2>& points, float sqTolerance, DequeArray<Vector2>& out) {
	int len = points.length();

	Vector2 point = Vector2();
	Vector2 prevPoint = points[0];
	
	out.clear();
	out.push(prevPoint);
		
	for (int i = 1; i < len; i++) {
		point = points[i];
		if (distSq(point, prevPoint) > sqTolerance) {
			out.push(point);
			prevPoint = point;
		}
	}
	if (prevPoint != point) {
		out.push(point);
	}
}
	
float ResolverRadialChaikin::distSq(Vector2 p1, Vector2 p2) {
	float dx = p1.x - p2.x;
	float dy = p1.y - p2.y;
	return dx * dx + dy * dy;
}