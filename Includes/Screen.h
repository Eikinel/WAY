#pragma once

# include "Constants.h"

class IEvent;
class Button;
class IGameEvent;

enum				eGamestate
{
	EXIT = -1,
	MENU,
	OPTIONS,
	GAME
};

class				IScreen
{
public:
	IScreen(sf::RenderWindow& window, eGamestate state);
	virtual ~IScreen();

	//GETTERS
	virtual sf::RenderWindow&		getWindow();
	virtual std::vector<IEvent *>&	getEvents();
	virtual eGamestate				getState() const;
	virtual const unsigned int		getIndex() const;
	virtual const float				getFPS() const;
	virtual const sf::Text&			getFPSText() const;
	virtual const unsigned int		getFrameLimiter() const;
	virtual const sf::Font&			getMainFont() const;

	//SETTERS
	virtual void	updateFPS();
	virtual void	setFrameLimiter(const unsigned int frame_limiter);

	//METHODS
	virtual int		run();
	virtual void	draw(const sf::Drawable& object);

protected:
	sf::RenderWindow&		_window;
	std::vector<IEvent *>	_events;
	eGamestate				_state;
	unsigned int			_index;

	//FPS handling
	sf::Clock		_clock;
	float			_fps;
	unsigned int	_frame_limiter;
	sf::Text		_fps_text;
	sf::Font		_main_font;
};

class				MenuScreen : public IScreen
{
public:
	MenuScreen(sf::RenderWindow& window);
	virtual ~MenuScreen();

	virtual std::vector<Button *>&	getButtons();

protected:
	sf::Text				_title;
	std::vector<Button *>	_buttons;
};

class				GameScreen : public IScreen
{
public:
	GameScreen(sf::RenderWindow& window);
	virtual ~GameScreen();

	//METHODS
	virtual int		run();
	void			restart();
};