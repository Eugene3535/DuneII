#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP


class MainWindow final
{
public:
	MainWindow() noexcept;
	~MainWindow();

	bool create(const char* title, int width, int height) noexcept;
	int run(class Engine& engine) noexcept;

private:
	bool createGLFWWindow(const char* title, int width, int height) noexcept;
	void checkOpenglTypes()  noexcept;
	void initCallbacks()     noexcept;

	struct GLFWwindow* m_window;
};

#endif // !MAIN_WINDOW_HPP