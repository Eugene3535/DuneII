#pragma once

#include <memory>

#include <cglm/struct/ivec2.h>



class MainWindow final
{
public:
	explicit MainWindow(struct WindowData& data) noexcept;
	~MainWindow();

	bool open(const char* title, int width, int height) noexcept;
    void close() const noexcept;

    void pollEvents() const noexcept;
    void display() const noexcept;

    float getElapsedTime() const noexcept;
    ivec2s getSize() const noexcept;

    static bool isKeyPressed(int key) noexcept;

    bool isOpen() const noexcept;

private:
	bool createGLFWWindow(const char* title, int width, int height) noexcept;
    void initializeDebug() noexcept;
	void initCallbacks() noexcept;

    struct WindowData& m_windowData;
	struct GLFWwindow* m_glfwWindow;

    std::unique_ptr<class OrthoMatrix> m_camera;
};