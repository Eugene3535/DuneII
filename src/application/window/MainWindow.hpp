#pragma once


class MainWindow final
{
public:
	MainWindow() noexcept;
	~MainWindow();

	bool open(const char* title, int width, int height) noexcept;
	int run(class Game& game) noexcept;

private:
	bool createGLFWWindow(const char* title, int width, int height) noexcept;
	void initCallbacks() noexcept;

	struct GLFWwindow* m_window;
};