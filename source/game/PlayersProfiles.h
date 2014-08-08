#ifndef PLAYER_PROFILER_H
#define PLAYER_PROFILER_H

#include <map>

struct PlayerProfile
{
	std::string playerName;
	std::string gameStateFilename;

	float totalPlayingTime;
};

class ProfilesManager
{
public:
	static ProfilesManager& Instance(void);

	void LoadProfiles(void);
	void SaveProfiles(void);

	void AddNewProfile(const std::string &name);
	void RemoveProfile(const std::string &name);

	inline PlayerProfile* GetCurrentProfile(void) const {return _currentProfile;}

private:	
	typedef std::map<std::string, PlayerProfile> ProfileMap;
	typedef std::pair<std::string, PlayerProfile> ProfileMapPair;

private:
	ProfileMap _profilesMap;
	PlayerProfile *_currentProfile;

	ProfilesManager();
	~ProfilesManager();
	ProfilesManager(const ProfilesManager&);
	void operator=(const ProfilesManager&);
};

#endif