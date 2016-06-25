#include <core/functionals/lambda-thread.h>

using namespace Functional;

LambdaThread::LambdaThread(const std::function<void()> &lambda, QObject *parent)
		: QThread(parent)
		, m_lambda(lambda) {
}

void LambdaThread::run() {
	if (m_lambda)
		m_lambda();
}
