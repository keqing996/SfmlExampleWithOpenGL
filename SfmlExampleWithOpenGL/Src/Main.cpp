#include <glad/gl.h>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>

int main()
{
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;

    // ����һ��window
    sf::Window window(sf::VideoMode(640, 480), "TestTest", sf::Style::Default, contextSettings);

    // �����
    window.setActive();

    // ������ѭ��
    while (window.isOpen())
    {
        // �����¼�
        sf::Event event;
        while (window.pollEvent(event))
        {
            // do something
        }

        //...

        // ˢ��ʾ
        window.display();
    }
}