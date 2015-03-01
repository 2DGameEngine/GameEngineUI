#include "application.h"
#include "view.h"
#include "method_dispatcher.h"
#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>
#ifdef _WIN32
#include <Windows.h>
#endif

#include "FileManager.h"

using namespace Awesomium;

class GameEngine : public Application::Listener {
  Application* app_;
  View* view_;
  MethodDispatcher method_dispatcher_;
 public:
  GameEngine() 
    : app_(Application::Create()),
      view_(0) {
    app_->set_listener(this);
  }

  virtual ~GameEngine() {
    if (view_)
      app_->DestroyView(view_);
    if (app_)
      delete app_;
  }

  void Run() {
    app_->Run();
  }

  // Inherited from Application::Listener
  virtual void OnLoaded() {
    view_ = View::Create(500, 300);
    WebView* web_view = view_->web_view();

    BindMethods(web_view);

    WebURL url(WSLit("file:///./asset/index.html"));
    web_view->LoadURL(url);
  }

  // Inherited from Application::Listener
  virtual void OnUpdate() {
  }

  // Inherited from Application::Listener
  virtual void OnShutdown() {
  }

  void BindMethods(WebView* web_view) {
    // Create a global js object named 'app'
    JSValue result = web_view->CreateGlobalJavascriptObject(WSLit("Game"));
    if (result.IsObject()) {
      // Bind our custom method to it.
      JSObject& app_object = result.ToObject();
      method_dispatcher_.Bind(app_object,
        WSLit("sayHello"),
        JSDelegate(this, &GameEngine::OnSayHello));
	  method_dispatcher_.BindWithRetval(app_object,
        WSLit("copyStaticFiles"),
        JSDelegateWithRetval(this, &GameEngine::copyStaticFiles));
    }

    // Bind our method dispatcher to the WebView
    web_view->set_js_method_handler(&method_dispatcher_);
  }

  // Bound to Game.sayHello() in JavaScript
  void OnSayHello(WebView* caller,
                  const JSArray& args) {
    app_->ShowMessage("Hello!");
  }

  JSValue copyStaticFiles(WebView* caller,
                  const JSArray& args) {
	const char *src, *dest;
	// make src as const as same for all
	// make dest as relative to the workspace set at teh first time
    return JSValue(FileManager::Instance()->copyFolder(src, dest));
  }
};

#ifdef _WIN32
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, wchar_t*, 
  int nCmdShow) {
#else
int main() {
#endif

  GameEngine app;
  app.Run();

  return 0;
}
