#include <glad/gl.h>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>

int main()
{
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;

    // 创建一个window
    sf::Window window(sf::VideoMode(640, 480), "TestTest", sf::Style::Default, contextSettings);

    // 激活窗口
    window.setActive();

    // 窗口主循环
    while (window.isOpen())
    {
        // 窗口事件
        sf::Event event;
        while (window.pollEvent(event))
        {
            // do something
        }

        //...

        // 刷显示
        window.display();
    }
}