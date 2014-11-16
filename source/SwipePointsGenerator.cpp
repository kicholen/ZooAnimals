#include "SwipePointsGenerator.h"


SwipePointsGenerator::SwipePointsGenerator(float thickness, float endcap) {
	_thickness = thickness;
	_endcap = endcap;
}

SwipePointsGenerator::~SwipePointsGenerator() {
}

int SwipePointsGenerator::generate(DequeArray<Vector2>& input, VectorArray<Vector2> &output, int mult) {
	int c = output.length();
	if (_endcap <= 0) {
		output.push(input[0]);
	} 
	else {
		Vector2 p = input[0];
		Vector2 p2 = input[1];
		_perp = p;
		_perp -= p2;
		_perp *= _endcap;
		output.push(Vector2(p.x + _perp.x, p.y + _perp.y));
	}

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
		output.push(Vector2(p.x + _perp.x, p.y + _perp.y));
		//0.0 -> end, transparent
			
		//add the center point
		output.push(Vector2(p.x, p.y));
		//1.0 -> center, opaque
	}

	//final point
	if (_endcap <= 0) {
		output.push(input[input.length() - 1]);
	} 
	else {
		Vector2 p = input[input.length() - 2];
		Vector2 p2 = input[input.length() - 1];
		_perp = p2;
		_perp -= p;
		_perp *= _endcap;
		output.push(Vector2(p2.x + _perp.x, p2.y + _perp.y));
	}

	return output.length() - c;
}