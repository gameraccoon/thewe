#ifndef MISC_UTILS_H
#define MISC_UTILS_H

#include <string>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <ctime>
#include <limits>

#include "Spline.h"

namespace Utils
{
	typedef float GameTime;

	std::string GetResourcesPath();
	std::string GetDocumentsPath();
	std::string GetWritablePath();

	GameTime GetGameTime(void);
	std::string TimeToString(GameTime time);
	GameTime StringToTime(const std::string& stringTime);

	bool IsPlatformDesktop();

	class VariablesSet
	{
	private:
		std::map<std::string, int> _intSet;
		std::map<std::string, float> _floatSet;
		std::map<std::string, Utils::GameTime> _timeSet;
		std::map<std::string, std::string> _strSet;

	public:
		VariablesSet(void);

		void ClearAll(void);

		void SetInt(const std::string &name, int value);
		void SetFloat(const std::string &name, float value);
		void SetBool(const std::string &name, bool value);
		void SetTime(const std::string &name, Utils::GameTime value);
		void SetString(const std::string &name, const std::string &value);

		int GetInt(const std::string &name, int def = 0) const;
		float GetFloat(const std::string &name, float def = 0.0f) const;
		bool GetBool(const std::string &name, bool def = false) const;
		Utils::GameTime GetTime(const std::string &name, Utils::GameTime def = 0) const;
		std::string GetString(const std::string &name, std::string def = " ") const;
	};

	const float PI = 3.14159265359f;

	template <class T>
	T Lerp(T a, T b, float time) {
		return a+time*(b-a);
	}

	template <class T>
	T Clamp(T a, T b, T Value) {
		if (Value < a) return b;
		if (Value > b) return a;
		return Value;
	}
}

#endif // MISC_UTILS_H
