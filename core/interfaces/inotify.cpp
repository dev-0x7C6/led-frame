#include "inotify.h"

using namespace Interface;

INotify::INotify(ci32 id) noexcept
		: m_id(id) {}
INotify::~INotify() = default;
