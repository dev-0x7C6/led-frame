#pragma once

#include <atomic>
#include <functional>

#include <QObject>
#include "core/types.h"

namespace Interface {

class INotify : public QObject {
	Q_OBJECT
public:
	explicit INotify(ci32 id) noexcept;
	virtual ~INotify();

	int id() const noexcept { return m_id; }

private:
	const int m_id = 0;

signals:
	void notify();
};
}
