#pragma once

#include <core/containers/color-scanline-container.h>
#include <core/receivers/receiver-type.hpp>

#include <QString>

#include <memory>
#include <core/generic/irepresentable.h>
#include <core/generic/atom-aggregator.h>

class IEmitter;

class IReceiver : public IRepresentable {
public:
	explicit IReceiver() = default;
	~IReceiver() override = default;

	auto category() const noexcept -> Category final;
	auto properties() const noexcept -> Properties final;

	virtual auto name() const noexcept -> std::string = 0;
	virtual auto type() const noexcept -> receiver_type = 0;
	virtual auto framerate() const noexcept -> u32;

	virtual void connectEmitter(const std::shared_ptr<IEmitter> &emitter) = 0;
	virtual bool isEmitterConnected() const = 0;
	virtual int connectedEmitterId() const = 0;

	virtual std::shared_ptr<IEmitter> connectedEmitter() const = 0;

	virtual void setName(const std::string &name) = 0;

	virtual auto correctors() noexcept -> AtomAggregator & = 0;
};
