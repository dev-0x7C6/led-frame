#pragma once

#include <QThread>
#include <functional>

namespace Functional {

class LambdaThread : public QThread {
public:
	explicit LambdaThread(const std::function<void()> &lambda, QObject *parent = nullptr);
	~LambdaThread() override = default;

protected:
	virtual void run() override;

private:
	std::function<void()> m_lambda;
};
} // namespace Functional
