#include <core/emitters/concretes/off-emitter.h>
#include <core/containers/color-scanline-container.h>

#include <QColor>

using namespace Emitter::Concrete;
using namespace Enum;

OffEmitter::OffEmitter(ci32 id)
		: AbstractEmitter(id)

{
	Container::Scanline scanline;
	scanline.clear();
	commit(scanline);
}

EmitterType OffEmitter::type() const { return EmitterType::Off; }
