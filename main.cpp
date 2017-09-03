#include <core/containers/application-info-container.h>
#include <core/functionals/debug-notification.h>
#include <core/functionals/file-collection.h>
#include <core/functionals/raii-call-on-return.h>
#include <core/functionals/remote-controller.h>
#include <core/managers/main-manager.h>
#include <core/managers/session-manager.h>
#include <core/networking/web-socket-server.h>

#include <QCoreApplication>
#include <QSettings>
#include <memory>

#ifdef RPI
#include <bcm_host.h>
#endif

using namespace Container;
using namespace Enum;
using namespace Functional;
using namespace Manager;
using namespace Network;

#ifdef __unix__

#include <signal.h>
#include <unistd.h>

void catchUnixSignals(const std::vector<int> &quitSignals,
	const std::vector<int> & = std::vector<int>()) {

	auto handler = [](int) -> void { QCoreApplication::quit(); };

	//#pragma
	//	for (int sig : ignoreSignals)
	//		signal(sig, SIG_IGN);

	for (int sig : quitSignals)
		signal(sig, handler);
}

#endif

int main(int argc, char *argv[]) {
#ifdef RPI
	bcm_host_init();
	RaiiCallOnReturn deinit = {[]() { bcm_host_deinit(); }};
#endif
	auto applicationName = QString(ApplicationInfo::name());
	auto applicationVersion = QString::fromStdString(ApplicationInfo::versionToString());

	QCoreApplication application(argc, argv);
#ifdef __unix__
	catchUnixSignals({SIGQUIT, SIGINT, SIGTERM, SIGHUP});
#endif

	application.setApplicationName(applicationName);
	application.setApplicationVersion(applicationVersion);

	QSettings settings(applicationName, applicationName);
	MainManager manager(settings);
	SessionManager session(settings, manager);
	FileCollection imageCollection;
	RemoteController controller(manager);
	WebSocketServer webSocketServer(manager, controller);

	manager.run();
	return application.exec();
}
