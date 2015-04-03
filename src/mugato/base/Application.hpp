#ifndef __mugato__Application__
#define __mugato__Application__

#include <mugato/base/Context.hpp>
#include <gorn/base/Application.hpp>

namespace mugato
{
    class Application : public gorn::Application
    {
    private:
        mugato::Context _mugato;

    protected:
	    virtual void load() override;
	    virtual void reload() override;
	    virtual void unload() override;
	    virtual void background() override;
	    virtual void foreground() override;
	    virtual void update(double dt) override;
	    virtual void draw() override;
        virtual void touch(const glm::vec2& p) override;

        gorn::Context& getGorn();
        const gorn::Context& getGorn() const;

        mugato::Context& getMugato();
        const mugato::Context& getMugato() const;

    public:

        Application();
    };
}

#endif
