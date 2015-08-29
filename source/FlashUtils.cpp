#include "FlashUtils.h"

namespace FlashUtils 
{
	using namespace std;

	const int SIZE  = 300;

	void *first_free = NULL;

	float CMath::random() {
		return (float(rand()%RAND_MAX))/RAND_MAX;
	}

	int CMath::random(int min, int max) {
		return rand() % (max - min) + min;
	}

	//Random float between two values!
	float CMath::Rand(float low, float high) { 
		return random() *(high-low)+low;
	}

	#define	ABS(a)			(((a) > 0) ? (a) : -(a))

	bool CMath::VecEqual(const Vector2 &p1, const Vector2 &p2, float dist) {
		if((ABS(p1.x - p2.x) <= dist) && (ABS(p1.y - p2.y) <= dist)) {
			return true;
		};
		return false;
	}

	Vector2 CMath::RotatePoint(Vector2 p, float ang) {
		float xang = ang * (float)M_PI / 180.0f;

		float s = sinf(xang);
		float c = cosf(xang);

		float x1 = p.x*c - p.y*s;
		float y1 = p.y*c + p.x*s;

		return Vector2(x1, y1);
	}

	unsigned int CMath::GetHueFromARGB(unsigned int col) {
		Color32 cola = HexToARGB(col);
		return (unsigned int)((180.0f / M_PI) * atan2(sqrtf(3.0f) * (cola[2] - cola[3]), 2*cola[1] - cola[2] - cola[3]));
	}

	bool CMath::ColorEqual2(unsigned int col1, unsigned int col2, float low, float buffer) {
		Color32 col1a = HexToARGB(col1);
		Color32 col2a = HexToARGB(col2);
		if((static_cast<unsigned int>(col1a[0]) < low) || (static_cast<unsigned int>(col2a[0]) < low)) {
			return true;
		}


		//Hue calc:
		unsigned int hue1 = GetHueFromARGB(col1);
		unsigned int hue2 = GetHueFromARGB(col2);

		if(scalar::abs((hue1-hue2)/360.0) <= buffer) {
			return true;
		}

		return false;
	}

	Color32 CMath::HexToARGB(unsigned int hex) {
		unsigned int a = (hex >> 24) & 0xFF;
		unsigned int r = (hex >> 16) & 0xFF;
		unsigned int g = (hex >> 8) & 0xFF;
		unsigned int b = hex & 0xFF;

		return Color32(r, g, b, a);
	}

	Color24 CMath::HexToRGB(unsigned int hex) {
		unsigned int r = (hex >> 16) & 0xFF;
		unsigned int g = (hex >> 8) & 0xFF;
		unsigned int b = hex & 0xFF;

		return Color24(r,g,b);
	}

	Color32 CMath::HexRGBToColor32(unsigned int hex) {
		Color24 q = HexToRGB(hex);
		return Color32(q.r, q.g, q.b, 255);
	}

	Color32 CMath::HexARGBToColor32(unsigned int hex) {
		Color32 q = HexToARGB(hex);
		return q;
	}

	unsigned int CMath::Color32ToHex(Color32 c) {
		return ARGBToHex(c.r, c.g, c.b, c.a);
	}



	unsigned int CMath::ARGBToHex(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
		unsigned int hex1 = (__min(r, 255) << 16 | __min(g,255) << 8 | __min(b,255));
		return (hex1 | (__min(a,255) << 24));
	}

	unsigned int CMath::RGBToHex(unsigned int r, unsigned int g, unsigned int b) {
		unsigned int hex = (r << 16 | g << 8 | b);
		return hex;
	}

	float CMath::Clamp(float num, float lbnd, float hbnd) {
		return __min(__max(num, lbnd), hbnd);
	}

	float CMath::DegToRad(float degree) {
		return degree * MATH_PI / 180;
	}

	float CMath::RadToDeg(float rad) {
		return rad * 180 / MATH_PI;
	}

	std::string CMath::getFormattedCooldown(int cooldown) {
		int hours = (int)std::floor((double)cooldown / 3600.0f);
		int minutes = (int)std::floor((double)(cooldown % 3600) / 60.0f);
		std::string result = "";

		if (hours > 0) {
			std::string prefix = hours < 10 ? "0" : "";
			std::string between = minutes < 10 ? ":0" : ":";
			result = prefix + intToString(hours) + between + intToString(minutes);
		}
		else {
			int seconds = cooldown % 60;
			std::string between = seconds < 10 ? ":0" : ":";
			std::string prefix = minutes < 10 ? "0" : "";
			result = prefix + intToString(minutes) + between + intToString(seconds);
		}

		return result;
	}

	std::string CMath::hexToString(int hexIn) {
		char hexString[4*sizeof(int)+1];
		// returns decimal value of hex
		sprintf(hexString,"%i", hexIn); 
		std::string input = string(hexString);
		const char* hex_str = input.c_str();

		std::string resultString;
		unsigned int ch;
		for(; std::sscanf( hex_str, "%2x", &ch ) == 1 ; hex_str += 2) {
			resultString += ch ;
		}

		return resultString;
		//return output;
	}

	int CMath::stringToHex(std::string str) {
		int num = strtol(str.c_str(), NULL, 16);
		return num;
	}

	std::string CMath::doubleToString(double x) {
		stringstream ss (stringstream::in | stringstream::out);
		ss << x;
		return ss.str();
	}

	std::string CMath::intToString(int x) {
		stringstream ss (stringstream::in | stringstream::out);
		ss << x;
		return ss.str();
	}

	double CMath::stringToDouble(const char *input) {
		const size_t bytesInDouble = 8;

		union {
			double value;
			unsigned char bytes[bytesInDouble];
		} u;

		unsigned char *output = u.bytes;

		for(uint i = 0; i < bytesInDouble; ++i) {
			sscanf(input, "%02hhX", output);

			input += 2;
			++output;
		}

		return u.value;
	}

	std::string CMath::stringFormat(const std::string fmt, ...) {
		int size = 100;
		std::string str;
		va_list ap;
		while (1) {
			str.resize(size);
			va_start(ap, fmt);
			int n = vsnprintf((char *)str.c_str(), size, fmt.c_str(), ap);
			va_end(ap);
			if (n > -1 && n < size) {
				str.resize(n);
				return str;
			}
			if (n > -1)
				size = n + 1;
			else
				size *= 2;
		}
		return str;
	}

	float CMath::calculateDistance(Vector2 from, Vector2 to) {
		return sqrt((to.x - from.x) * (to.x - from.x) + (to.y - from.y) * (to.y - from.y));
	}

	float CMath::calculateAngle(Vector2 to, Vector2 from) {
		return atan2(from.y - to.y, from.x - to.x);
	}
}
