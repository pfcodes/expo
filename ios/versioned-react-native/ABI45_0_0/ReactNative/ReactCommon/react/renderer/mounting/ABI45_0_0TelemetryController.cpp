/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ABI45_0_0TelemetryController.h"

#include <ABI45_0_0React/ABI45_0_0renderer/mounting/MountingCoordinator.h>

namespace ABI45_0_0facebook {
namespace ABI45_0_0React {

TelemetryController::TelemetryController(
    MountingCoordinator const &mountingCoordinator) noexcept
    : mountingCoordinator_(mountingCoordinator) {}

bool TelemetryController::pullTransaction(
    std::function<void(MountingTransactionMetadata metadata)> const &willMount,
    std::function<void(ShadowViewMutationList const &mutations)> const &doMount,
    std::function<void(MountingTransactionMetadata metadata)> const &didMount)
    const {
  auto optional = mountingCoordinator_.pullTransaction();
  if (!optional.has_value()) {
    return false;
  }

  auto transaction = std::move(*optional);

  auto surfaceId = transaction.getSurfaceId();
  auto number = transaction.getNumber();
  auto telemetry = transaction.getTelemetry();
  auto numberOfMutations = static_cast<int>(transaction.getMutations().size());

  mutex_.lock();
  auto compoundTelemetry = compoundTelemetry_;
  mutex_.unlock();

  willMount({surfaceId, number, telemetry, compoundTelemetry});

  telemetry.willMount();
  doMount(transaction.getMutations());
  telemetry.didMount();

  compoundTelemetry.incorporate(telemetry, numberOfMutations);

  didMount({surfaceId, number, telemetry, compoundTelemetry});

  mutex_.lock();
  compoundTelemetry_ = compoundTelemetry;
  mutex_.unlock();

  return true;
}

} // namespace ABI45_0_0React
} // namespace ABI45_0_0facebook
