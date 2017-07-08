#include "Screen.h"
#include "Event.h"
#include "Button.h"


//CONSTRUCTORS
IScreen::IScreen(sf::RenderWindow& window, eGamestate state) : _window(window), _state(state)
{
	sf::Vector2u	win_size(window.getSize());

	this->_index = all_screens.size();
	this->_frame_limiter = 120;
	this->_fps = 0.f;
	if (!this->_main_font.loadFromFile(FONTS_DIR"/Madness Hyperactive.otf"))
		std::cerr << "Failed to load font " << FONTS_DIR"/Madness Hyperactive.otf" << std::endl;
	this->_window.setFramerateLimit(this->_frame_limiter);
	this->_fps_text.setFont(this->_main_font);
	this->_fps_text.setCharacterSize(win_size.y / 25.f);
	this->_fps_text.setString(std::to_string(this->_fps) + " / " + std::to_string(this->_frame_limiter));
	this->_fps_text.setPosition(sf::Vector2f(
		win_size.x / 1.07f,
		win_size.y / 1.05f));
	this->_clock.restart().asSeconds();
}

MenuScreen::MenuScreen(sf::RenderWindow& window) : IScreen(window, MENU)
{
	std::cout << std::endl << "Creating menu screen" << std::endl;
	this->_events.push_back(new WindowDefaultEvent); // Event handler for options, close window, etc.
	this->_events.push_back(new MenuEvent); // Update menu, draw it and react in terms of user inputs.

	sf::Vector2f	win_size(window.getSize());

	this->_buttons.push_back(new Button(GAME_NAME, win_size.y / 4.f, this->_main_font, sf::Vector2f(
		win_size.x / 2.f,
		win_size.y / 20.f), CENTER));
	this->_buttons.push_back(new Button("Play", win_size.y / 6.f, this->_main_font, sf::Vector2f(
		win_size.x / 2.f,
		win_size.y / 2.5f), CENTER));
	this->_buttons.push_back(new Button("Options", win_size.y / 6.f, this->_main_font, sf::Vector2f(
		win_size.x / 2.f,
		win_size.y / 1.6f), CENTER));
	this->_buttons.push_back(new Button("Exit", win_size.y / 12.f, this->_main_font, sf::Vector2f(
		win_size.x / 2.f,
		win_size.y / 1.15f), CENTER));
	this->_buttons[1]->onClick(&IEvent::changeScreen, this->_events[1], GAME, static_cast<IScreen *>(this));
	this->_buttons[2]->onClick(&IEvent::changeScreen, this->_events[1], OPTIONS, static_cast<IScreen *>(this));
	this->_buttons[3]->onClick(&IEvent::changeScreen, this->_events[1], EXIT, static_cast<IScreen *>(this));
}

GameScreen::GameScreen(sf::RenderWindow& window) :  IScreen(window, GAME)
{
	std::cout << std::endl << "Creating game screen" << std::endl;
	this->_events.push_back(new WindowDefaultEvent); // Event handler for options, close window, etc.
	this->_events.push_back(new GameEvent); // Update game, draw it and react in terms of user inputs.
	this->_window.setKeyRepeatEnabled(true);

	sf::Vector2f		win_size(window.getSize());

	this->restart();
}

IScreen::~IScreen()
{
	std::cout << "Deleting window events" << std::endl;
	for (auto it : this->_events)
		delete(it);
}

MenuScreen::~MenuScreen()
{
	std::cout << "Deleting menu screen" << std::endl;
	for (auto it : this->_buttons)
		delete (it);
}

GameScreen::~GameScreen()
{
	std::cout << "Deleting game screen" << std::endl;
}


//GETTERS
sf::RenderWindow&		IScreen::getWindow()
{
	return (this->_window);
}

std::vector<IEvent *>&	IScreen::getEvents()
{
	return (this->_events);
}

eGamestate				IScreen::getState() const
{
	return (this->_state);
}

const unsigned int		IScreen::getIndex() const
{
	return (this->_index);
}

const float	IScreen::getFPS() const
{
	return (this->_fps);
}

const sf::Text&	IScreen::getFPSText() const
{
	return (this->_fps_text);
}

const unsigned int	IScreen::getFrameLimiter() const
{
	return (this->_frame_limiter);
}

const sf::Font&	IScreen::getMainFont() const
{
	return (this->_main_font);
}


std::vector<Button *>&	MenuScreen::getButtons()
{
	return (this->_buttons);
}


//SETTERS
void	IScreen::updateFPS()
{
	float			current_time;
	static sf::Time	freeze = sf::seconds(1.f);

	freeze += this->_clock.getElapsedTime();
	current_time = this->_clock.restart().asSeconds();

	this->_fps = 1.f / current_time;
	if (freeze.asSeconds() >= 1.f)
	{
		this->_fps_text.setString(sf::String(std::to_string((int)std::round(this->_fps)) + " / " + std::to_string(this->_frame_limiter)));
		freeze = sf::Time::Zero;
	}
}

void	IScreen::setFrameLimiter(const unsigned int frame_limiter)
{
	this->_frame_limiter = frame_limiter;
}


//METHODS
int		IScreen::run()
{
	int					status;
	sf::Event			event;

	while (this->_window.pollEvent(event))
	{
		for (auto it : this->_events)
		{
			if ((status = it->update(*this, event)) != this->_index)
				return (status);
		}
	}

	this->_window.clear();
	for (auto it : this->_events)
		it->draw(*this);
	this->_window.display();

	this->updateFPS();
	return (this->_index);
}

void	IScreen::draw(const sf::Drawable& object)
{
	this->_window.draw(object);
}

int		GameScreen::run()
{
	int				status;
	sf::Event		event;
	bool			user_input;

	while (user_input = this->_window.pollEvent(event))
	{
		for (auto it : this->_events)
		{
			if ((status = it->update(*this, event)) != this->_index)
				return (status);
		}
	}

	// We need to update the window even if the user doesn't do anything.
	// The game has to continue ;)
	if (!user_input)
		this->_events[1]->update(*this, sf::Event());

	this->_window.clear();
	for (auto it : this->_events)
		it->draw(*this);
	this->_window.display();

	this->updateFPS();
	return (this->_index);
}

void	GameScreen::restart()
{
	std::cout << "Restarting game" << std::endl;
}