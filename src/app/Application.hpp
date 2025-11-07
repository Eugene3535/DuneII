#ifndef APPLICATION_HPP
#define APPLICATION_HPP


class Application final
{
public:
	Application() noexcept;
	~Application();

	bool init(const char* title, int width, int height) noexcept;
	int run(class DuneII& game) noexcept;

private:
	bool initWindow(const char* title, int width, int height) noexcept;
	void checkOpenglTypes() noexcept;
	void initCallbacks() noexcept;
	void loadGame(class DuneII* game) noexcept;

	struct GLFWwindow* m_window;
	class DuneII* m_game;
};

#endif // !APPLICATION_HPP