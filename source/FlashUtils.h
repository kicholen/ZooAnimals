#pragma once

#define _USE_MATH_DEFINES
#include <string>
#include <cmath>
#include <vector>
#include "Sprite.h"
#include "RootActor.h"
#include <sstream>
#include <deque>

using namespace oxygine;

#define __min(a, b) (a > b ? b : a)
#define __max(a, b) (a > b ? b : a)

namespace FlashUtils
{

	typedef Color Color32;


	class Color24 {
	public:
		Color24(unsigned char r_, unsigned char g_, unsigned char b_) {
			r = r_;
			g = g_;
			b = b_;
		}

		~Color24() {

		}

		unsigned char operator [](int i) const {
			return colors[i];
		}

		union {
			struct {
				unsigned char r, g, b;
			};
			
			unsigned char colors[3];
		};


	};
	
	template <class T>
	class VectorArray
	{
	public:
		void push(T v) {
			_vector.push_back(v);
		}
		
		void addAll(VectorArray<T> array) {
			for(int i = array.length() - 1; i >= 0; i--) {
				push(array[i]);
			}
		}

		int length() {
			return _vector.size();
		}

		T operator [](int i) {
			return _vector[i];
		}

		void clear() {
			_vector.resize(0);
		}

		void ensureCapacity(int reserveCapacity) {
			_vector.reserve(reserveCapacity);
		}

		void splice(int start, bool swap = false) {
			if (swap) {
				_vector[start] = _vector[_vector.size() - 1];
				_vector.pop_back();
			}
			else {
				_vector.erase(_vector.begin() + start);
			}

		}
		
		std::vector<T> _vector;
	};

	template <class T>
	class DequeArray
	{
	public:
		virtual void push(T v) {
			_deque.push_back(v);
		}

		void addAll(DequeArray<T> array) {
			for(int i = array.length() - 1; i >= 0; i--) {
				push(array[i]);
			}
		}

		int length() {
			return _deque.size();
		}

		T operator [](int i) {
			return _deque[i];
		}

		void clear() {
			_deque.resize(0);
		}
	protected:
		std::deque<T> _deque;
	};

	template <class T>
	class CapacityDeque : public DequeArray<T>
	{
	public:
		virtual void push(T v) {
			if (this->length() == capacity()) {
				this->_deque.pop_back();
			}
			this->_deque.push_front(v);
		}

		void setCapacity(int capacity) {
			_capacity = capacity;
		}

		int capacity() {
			return _capacity;
		}
		
		int _capacity;
	};

	class CMath
	{
	private:
	public:
		static float random();

		static int random(int min, int max);

		static float Rand(float low, float high);

		static bool VecEqual(const Vector2 &p1, const Vector2 &p2, float dist);

		static Vector2 RotatePoint(Vector2 p, float ang);

		static unsigned int GetHueFromARGB(unsigned int col);

		static bool ColorEqual2(unsigned int col1, unsigned int col2, float low, float buffer);

		static Color32 HexToARGB(unsigned int hex);

		static Color24 HexToRGB(unsigned int hex);

		static Color32 HexRGBToColor32(unsigned int hex);

		static Color32 HexARGBToColor32(unsigned int hex);


		static unsigned int Color32ToHex(Color32 c);

		static unsigned int ARGBToHex(unsigned int r, unsigned int g, unsigned int b, unsigned int a);


		static unsigned int RGBToHex(unsigned int r, unsigned int g, unsigned int b);

		static float Clamp(float num, float lbnd, float hbnd);

		static float DegToRad(float degree);

		static float RadToDeg(float rad);

		struct SweptCircleIntersect_data {
			float nu;
			Vector2 nrm, cpt, f1, f2;

		};

		static bool SweptCircleIntersect(CMath::SweptCircleIntersect_data &data, const Vector2 &p1, const Vector2 &v1, float r1, const Vector2 &p2, const Vector2 &v2, float r2);
	
		static string hexToString(int hexIn);
		static int stringToHex(string str);

		static string doubleToString(double x);
		static string intToString(int x);
		static double stringToDouble(const char *input);
		static string stringFormat(const string fmt, ...);

		static float calculateDistance(Vector2 from, Vector2 to);
		static float calculateAngle(Vector2 to, Vector2 from);
	};
}

