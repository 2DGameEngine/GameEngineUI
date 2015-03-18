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
  std::string workspace, prName, srcPath;
 public:
  GameEngine() 
    : app_(Application::Create()),
      view_(0) {
    app_->set_listener(this);
	workspace = "\.\.\\Games\\";
	prName = "";
	srcPath = "\.\.\\game_backend";
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
        WSLit("readJSON"),
        JSDelegateWithRetval(this, &GameEngine::readJSON));
	  method_dispatcher_.BindWithRetval(app_object,
        WSLit("writeJSON"),
        JSDelegateWithRetval(this, &GameEngine::writeJSON));
	  method_dispatcher_.BindWithRetval(app_object,
        WSLit("setNewGame"),
        JSDelegateWithRetval(this, &GameEngine::setNewGame));
	  method_dispatcher_.BindWithRetval(app_object,
        WSLit("gamePresent"),
        JSDelegateWithRetval(this, &GameEngine::gamePresent));
    }

    // Bind our method dispatcher to the WebView
    web_view->set_js_method_handler(&method_dispatcher_);
  }

  // Bound to Game.sayHello() in JavaScript
  void OnSayHello(WebView* caller,
                  const JSArray& args) {
	std::string str = ToString(args[0].ToString());
	app_->ShowMessage(str.c_str());
  }

  JSValue readJSON(WebView* caller,
                  const JSArray& args) {
	std::string filename = workspace + prName + "\\json\\" + ToString(args[1].ToString()) + "\\" + ToString(args[0].ToString());
	std::cout<<"Reading "<<filename<<"\n";
	return JSValue(FileManager::Instance()->readJSON(filename.c_str()));
  }

  JSValue writeJSON(WebView* caller,
                  const JSArray& args) {
	std::string filename = workspace + prName + "\\json\\" + ToString(args[2].ToString()) + "\\" + ToString(args[0].ToString());
	std::cout<<"Reading "<<filename<<"\n";
	std::string jsonStr = ToString(args[1].ToString());
	return JSValue(FileManager::Instance()->writeJSON(filename.c_str(), jsonStr.c_str()));
  }

  JSValue setNewGame(WebView* caller,
                  const JSArray& args) {
	std::string gameName = workspace + ToString(args[0].ToString());
	std::string jsonStr = ToString(args[1].ToString());
	if(FileManager::Instance()->copyFolder(srcPath.c_str(), gameName.c_str()) && FileManager::Instance()->renameGame(gameName.c_str(), "game_backend",ToString(args[0].ToString()).c_str())){
		gameName += "\\Game.json";
		std::cout<<"GameName = "<<gameName<<"\n";
		prName = ToString(args[0].ToString());
		return JSValue(FileManager::Instance()->writeJSON(gameName.c_str(), jsonStr.c_str()));
	}
	FileManager::Instance()->removeDir(gameName.c_str());
	return JSValue(false);
  }

  JSValue gamePresent(WebView* caller,
                  const JSArray& args) {
	std::string gameName = ToString(args[0].ToString());
	std::string source = workspace;
	return JSValue(FileManager::Instance()->checkGame(source.c_str(), gameName.c_str()));
  }

};

#ifdef _WIN32
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, wchar_t*, 
  int nCmdShow) {
#else
int main() {
#endif
  AllocConsole();
  freopen("CON","w",stdout);
  GameEngine app;
  app.Run();

  return 0;
}
