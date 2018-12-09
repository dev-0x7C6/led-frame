#include <core/emitters/concretes/off-emitter.h>
#include <core/containers/color-scanline-container.h>

#include <QColor>

using namespace Emitter::Concrete;
using namespace Enum;

OffEmitter::OffEmitter(const i32 id)
		: AbstractEmitter(id)

{
	Container::Scanline scanline;
	scanline.clear();
	commit(scanline);
}
