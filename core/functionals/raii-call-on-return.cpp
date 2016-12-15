#include "raii-call-on-return.h"

using namespace Functional;

RaiiCallOnReturn::RaiiCallOnReturn(std::function<void()> destroy)
		: m_destroy(destroy) {}

RaiiCallOnReturn::~RaiiCallOnReturn() { m_destroy(); }
