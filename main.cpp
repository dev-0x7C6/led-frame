#include <core/containers/application-info-container.h>
#include <core/functionals/raii-call-on-return.h>
#include <core/functionals/remote-controller.h>
#include <core/managers/main-manager.h>
#include <core/managers/session-manager.h>
#include <core/networking/web-socket-server.h>
#include <gui/dialogs/about-dialog.h>
#include <gui/tray/system-tray.h>

#ifdef GUI
#include <QApplication>
#else
#include <QCoreApplication>
#endif
#include <QSettings>

#ifdef RPI
#include <bcm_host.h>
#endif

using namespace Container;
using namespace Enum;
using namespace Functional;
using namespace Manager;
using namespace Network;

using namespace Corrector::Concrete;
using namespace Emitter::Concrete;
using namespace Receiver::Concrete;

int main(int argc, char *argv[]) {
#ifdef RPI
	bcm_host_init();
	RaiiCallOnReturn deinit = {[]() { bcm_host_deinit(); }};
#endif
	auto applicationName = QString(ApplicationInfo::name());
	auto applicationVersion = QString::fromStdString(ApplicationInfo::versionToString());

#ifdef GUI
	QApplication application(argc, argv);
	application.setQuitOnLastWindowClosed(false);
	application.setApplicationDisplayName(QString("%1 %2").arg(applicationName, applicationVersion));
#else
	QCoreApplication application(argc, argv);
#endif

	application.setApplicationName(applicationName);
	application.setApplicationVersion(applicationVersion);

	QSettings settings(applicationName, applicationName);
	MainManager manager(settings);
	SessionManager session(settings, manager);
	RemoteController controller(manager);
	WebSocketConnectionManager webSocketServer(manager, controller);

#ifdef GUI
	Tray::SystemTray tray;
	manager.attach(tray);

	tray.setAboutRequestCallback([] {
		static bool dialogGuardVisible = false;

		if (dialogGuardVisible)
			return;

		dialogGuardVisible = true;
		Widget::AboutDialog dialog;
		dialog.exec();
		dialogGuardVisible = false;
	});

	tray.setCloseRequestCallback([&application] { application.quit(); });
#endif

	manager.run();
	return application.exec();
}
