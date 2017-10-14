#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_
#include "Graphics/TextWindow.h"
#include "Graphics/ScreenRoot.h"
#include "Graphics/GameBoard.h"
#include "Graphics/MoveInputWindow.h"
#include "KeyboardListener.h"
#include "GameMatch.h"
#include "PlayerTypes.h"
#include <list>
/**
 * @brief Game engine singleton
 *
 * Runs game loop and creates Players and GameMatches as requested.
 */
class GameEngine
{

public:
	static GameEngine& GetInstance()
	{
		static GameEngine instance;
		return instance;
	}
	enum GameState
	{
		STATE_INIT,
		STATE_MENU,
		STATE_GAME,
		STATE_POSTGAME,
		STATE_QUIT,
	};

	GameState GetGameState() const
	{
		return currentState;
	}

	///Initialize ncurses, color pairs and show menu
	void Initialize();
	///Main game loop
	void Think();
	///Update terminal window graphics if necessary
	void Draw();

	///Registers KeyboardListener interface
	void AddKeyboardListener(KeyboardListener* listener);
	void RemoveKeyboardListener(KeyboardListener* listener);

	///Called by GameMatch on every turn start, updates HUD
	void OnStartTurn();

	///Create a GameMatch with players of specified type
	void StartGame(PlayerType p1Type, PlayerType p2Type);

	///Create a RemoteGameMatch with local player of specified type
	void JoinGame(PlayerType p1Type, std::string address);

	///Displays a text window that disappears on next key press
	void DisplayPopup(const char* text);

	void HidePopup();

	///Displays the main menu
	void ShowMainMenu();
private:
	GameEngine();
	~GameEngine();

	///Delete all player instances
	void DeletePlayers();

	//do not implement (prevents creating more instances)
	void operator=(const GameEngine) = delete;
	GameEngine(const GameEngine& other) = delete;

	///Handle GameEngine keybinds and pass pressed key to all listeners
	void HandleKeyPress();

	///Displays the winner on screen
	void DisplayPostGameScreen();

	///Creates specified player instance
	unique_ptr<Player> CreatePlayerByType(PlayerType type) const;

	///Frees all resources used by game match
	void CleanupGame();

	///Update HUD display of player on turn
	void UpdateTurnInfoBox();

	///Set input window for local player
	void SetLocalPlayerInputWin(class LocalPlayer* player);

	/// Build UI for displaying game info
	void BuildGameUI();

	ScreenRoot screenRoot; ///< Root of all HUD components
	GameState currentState; ///< Current game state
	GameMatch* currentMatch; ///< Current game match
	GameBoard* boardDisplay; ///< HUD element displaying the board. Memory handled by screen.
	TextWindow* turnInfoBox; ///< HUD element displaying player on turn. Memory handled by screen.
	MoveInputWindow* inputWin; ///< HUD element displaying currently typed turn by LocalPlayer.

	TextWindow* popupWin; ///< HUD Popup window used for information and error messages

	bool breakKL; ///< Flag used to break KeyboardListener loop. @see HandleKeyPress()

	std::list<KeyboardListener*> keyboardListeners; ///< List of all registered KeyboardListeners
	std::vector<unique_ptr<Player>> players; ///< List of players in match

	static const int KEY_TERMINATE = 27; ///< KEY_ESCAPE (missing in ncurses)
};
#endif /* GAMEENGINE_H_ */
