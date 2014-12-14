#ifndef SPLINE_H
#define SPLINE_H

#include <vector>
#include "Log.h"

namespace Utils
{
	template <class T>
	T SplineInterpolation(T x1, T x2, T r1, T r2, float t)
	{
		T res;
		float t2 = t*t;
		float t3 = t2*t;

		res = x1*(2.0f*t3-3.0f*t2+1.0f)+r1*(t3-2.0f*t2+t)+x2*(-2.0f*t3+3.0f*t2)+r2*(t3-t2);
		return res;
	}

	template <class T>
	class Spline
	{
	public:
		typedef std::pair<T, T> KeyFrame;

		std::vector<KeyFrame> keys;

		void Clear()
		{
			keys.clear();
		}

		void AddKey(const T& key)
		{
			keys.push_back(KeyFrame(key, key));
		}

		T GetFrame(int sector, float t)
		{
			size_t i = static_cast<size_t>(sector);
			return SplineInterpolation<T>(keys[i].first, keys[i+1].first, keys[i].second, keys[i+1].second, t);
		}

		T GetGlobalFrame(float t)
		{
			float timeStep = (1.0f/GetSectors());
			int tessSector = static_cast<int>(t/timeStep);
			if (tessSector >= GetSectors()) {
				return keys.back().first;
			}
			float tessLocalTime = (t-tessSector*timeStep)/timeStep;
			return GetFrame(tessSector, tessLocalTime);
		}

		int GetSectors()
		{
			return (int)keys.size() - 1;
		}

		void CalculateGradient(bool cycled = false)
		{
			if (keys.empty()) {
				Log::Instance().writeWarning("Failed to create spine with no points");
				return;
			}
		
			if (cycled)
			{
				keys.push_back(keys[0]);
			}
		
			if (cycled)
			{
				if (keys.size() > 1) {
					keys[0].second = 0.5f*(keys[1].first - keys[keys.size() - 2].first);
					keys[keys.size() - 1].second = keys[0].second;
				} else {
					Log::Instance().writeWarning("Failed to create cycled spline with 1 point.");
				}
			}
			else
			{
				if (keys.size() > 1) {
					keys[0].second = keys[1].first-keys[0].first;
					keys[keys.size()-1].second = keys[keys.size()-1].first-keys[keys.size()-2].first;
				} else {
					Log::Instance().writeWarning("Failed to create spline with 1 point.");
				}
			}
		
			for (int i = 1; i < (int)keys.size() - 1; i++)
			{
				keys[i].second = 0.5f*(keys[i+1].first - keys[i-1].first);
			}
		}
	};
}

#endif