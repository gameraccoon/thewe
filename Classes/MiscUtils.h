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

	void CachePaths();
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
		void SetString(const std::string &name, const std::string& value);

		bool HaveInt(const std::string &name) const;
		bool HaveFloat(const std::string &name) const;
		bool HaveBool(const std::string &name) const;
		bool HaveTime(const std::string &name) const;
		bool HaveString(const std::string &name) const;

		int GetInt(const std::string &name) const;
		float GetFloat(const std::string &name) const;
		bool GetBool(const std::string &name) const;
		Utils::GameTime GetTime(const std::string &name) const;
		std::string GetString(const std::string &name) const;
	};
}

namespace Math
{
	const float PI = 3.14159265359f;

	template <class T>
	T Lerp(T a, T b, float time) {
		return a+time*(b-a);
	}

	template <class T>
	T Clamp(T a, T b, T Value) {
		if (Value > a) return a;
		if (Value < b) return b;
		return Value;
	}
}

#endif // MISC_UTILS_H
