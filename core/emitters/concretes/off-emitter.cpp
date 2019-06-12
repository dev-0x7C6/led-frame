#include <core/emitters/concretes/off-emitter.h>
#include <core/containers/color-scanline-container.h>

#include <QColor>

using namespace Emitter::Concrete;
using namespace Enum;

OffEmitter::OffEmitter() {
	commit(Container::Scanline(0));
}
