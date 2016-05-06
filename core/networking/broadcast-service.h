#pragma once

#include <QObject>

class QUdpSocket;

namespace Network {

	class BroadcastService final : public QObject {
		Q_OBJECT
	public:
		explicit BroadcastService(QObject *parent = nullptr);
		virtual ~BroadcastService() = default;

		uint16_t servicePort() const;
		void setServicePort(const uint16_t &servicePort);

	protected:
		void timeout();

	private:
		QUdpSocket *m_socket;
		uint16_t m_servicePort;

	};

}
