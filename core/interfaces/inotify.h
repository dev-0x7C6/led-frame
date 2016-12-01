#pragma once

#include <atomic>
#include <functional>

#include <QObject>

namespace Interface {

class INotify : public QObject {
	Q_OBJECT
public:
	explicit INotify(int id) noexcept
			: m_id(id) {}
	virtual ~INotify() noexcept override = default;

	int id() const noexcept { return m_id; }

private:
	const int m_id = 0;

signals:
	void notify();
};
}
