#ifndef _GAME_H_
#define _GAME_H_

#include"common\common.h"

class Map;
class Player;
class App;
class InputEvent;
class Savegame;
class LuaContext;
/**
*	\brief Game�����ǰ��ͼ�����е���Ϸ����
*/
class Game
{
public:
	Game(App* app);	// test,no savegame
	Game(App* app, const std::shared_ptr<Savegame>& savegame);
	
	Game(const Game& game) = delete;
	Game& operator=(const Game& game) = delete;
	
	/** System */
	void Start();
	void Stop();
	void Restart();
	void Update();
	void Draw();
	
	bool IsSuspended()const;
	Savegame& GetSavegame();

	/** Map */
	bool HasCurrentMap()const;
	Map& GetCurrentMap();
	void SetCurrentMap(const string& mapID);
	void UpdateTransitoin();

	/** Notify */
	bool NotifyInput(const InputEvent & ent);

	/** Lua */
	LuaContext& GetLuaContext();


private:
	bool mStarted;
	bool mSuspended;

	App& mApp;
	std::shared_ptr<Savegame> mSavegame;
	std::shared_ptr<Player> mPlayer;
	std::shared_ptr<Map> mCurrentMap;
	std::shared_ptr<Map> mNextMap;
};


#endif