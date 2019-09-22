#include <core/containers/application-info-container.h>
#include <core/functionals/debug-notification.h>
#include <core/functionals/remote-controller.h>
#include <core/managers/main-manager.h>
#include <core/managers/session-manager.h>
#include <core/networking/web-socket-server.h>
#include <externals/common/logger/logger.hpp>

#include <QApplication>
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

#include <csignal>
#include <chrono>

using namespace std::chrono_literals;

void catchUnixSignals(std::initializer_list<int> &&catch_those) {
	auto handler = [](int) -> void {
		logger<>::error("quiting application...");
		QCoreApplication::quit(); };

	for (int sig : catch_those)
		signal(sig, handler);
}

#endif

#include <externals/common/logger/logger.hpp>

int main(int argc, char *argv[]) {
#ifdef RPI
	bcm_host_init();
#endif
	auto applicationName = QString(ApplicationInfo::name());
	auto applicationVersion = QString::fromStdString(ApplicationInfo::versionToString());

	QApplication application(argc, argv);
	QApplication::setApplicationName(applicationName);
	QApplication::setApplicationVersion(applicationVersion);
#ifdef __unix__
	catchUnixSignals({SIGQUIT, SIGINT, SIGTERM, SIGHUP});
#endif

	QSettings settings(applicationName, applicationName);
	MainManager manager(settings);
	SessionManager session(settings, manager);
	RemoteController controller(manager);
	WebSocketServer webSocketServer(manager, controller);

	manager.run();
	return QApplication::exec();

#ifdef RPI
	bcm_host_deinit();
#endif
}
