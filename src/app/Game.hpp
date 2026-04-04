#ifndef GAME_HPP
#define GAME_HPP


class Game final
{
public:
	Game() noexcept;
	~Game();

	bool init(const char* title, int width, int height) noexcept;
	int run(class Engine& engine) noexcept;

private:
	bool initWindow(const char* title, int width, int height) noexcept;
	void checkOpenglTypes()  noexcept;
	void initCallbacks()     noexcept;

	struct GLFWwindow* m_window;
};

#endif // !GAME_HPP