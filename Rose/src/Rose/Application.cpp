#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Log.h"

#include "GLFW/glfw3.h"
namespace Rose
{
    Application::Application()
    {
        window = std::unique_ptr<Window>(Window::Create());  
        window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    }

    Application::~Application()
    {
    }    

    void Application::PushLayer(Layer* layer)
    {
        layerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* layer)
    {
        layerStack.PushLayer(layer);
    }   

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

        ROSE_CORE_TRACE("{0}", e);

        for(auto it = layerStack.end(); it != layerStack.begin(); )
        {
            (*--it)->OnEvent(e);
            if(e.Handled)
                break;
        }
    }

    void Application::Run()
    {
        while(running)
        {
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            for(Layer* layer : layerStack)
                layer->OnUpdate();
                
            window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& event)
    {
        running = false;
        return true;
    }
} 
