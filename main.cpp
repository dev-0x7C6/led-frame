#include <core/containers/application-info-container.h>
#include <core/functionals/debug-notification.h>
#include <core/functionals/file-collection.h>
#include <core/functionals/raii-call-on-return.h>
#include <core/functionals/remote-controller.h>
#include <core/managers/main-manager.h>
#include <core/managers/session-manager.h>
#include <core/networking/web-socket-server.h>

#include <QApplication>
#include <QSettings>
#include <memory>
#include <QTimer>

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

#include <chrono>

using namespace std::chrono_literals;

void catchUnixSignals(const std::vector<int> &quitSignals,
	const std::vector<int> &ignoreSignals = std::vector<int>()) {

	auto handler = [](int) -> void { QCoreApplication::quit(); };

	for (int sig : ignoreSignals)
		signal(sig, SIG_IGN);

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

	QApplication application(argc, argv);
	application.setApplicationName(applicationName);
	application.setApplicationVersion(applicationVersion);
#ifdef __unix__
	catchUnixSignals({SIGQUIT, SIGINT, SIGTERM, SIGHUP});
#endif

	QSettings settings(applicationName, applicationName);
	MainManager manager(settings);
	SessionManager session(settings, manager);
	FileCollection imageCollection;
	RemoteController controller(manager);
	WebSocketServer webSocketServer(manager, controller);

	QTimer::singleShot(3s, [&application]() { application.quit(); });

	manager.run();
	return application.exec();
}
