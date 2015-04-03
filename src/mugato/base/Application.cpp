
#include <mugato/base/Application.hpp>
#include <gorn/base/Context.hpp>

namespace mugato
{
    Application::Application()
    {
    }

    gorn::Context& Application::getGorn()
    {
        return getMugato().getGorn();
    }

    const gorn::Context& Application::getGorn() const
    {
        return getMugato().getGorn();
    }

    mugato::Context& Application::getMugato()
    {
        return _mugato;
    }

    const mugato::Context& Application::getMugato() const
    {
        return _mugato;
    }

    void Application::load()
    {
        gorn::Application::load();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void Application::reload()
    {
        gorn::Application::reload();
        getGorn().reload();
    }

    void Application::unload()
    {
        gorn::Application::unload();
    }

    void Application::background()
    {
        gorn::Application::background();
    }

    void Application::foreground()
    {
        gorn::Application::foreground();
    }

    void Application::update(double dt)
    {
        gorn::Application::update(dt);
        getMugato().update(dt);
    }

    void Application::draw()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        getMugato().draw();
    }

    void Application::touch(const glm::vec2& p)
    {
    }

}
