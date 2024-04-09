/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "velox/expression/ReverseSignatureBinder.h"

namespace facebook::velox::exec {

bool ReverseSignatureBinder::hasConstrainedIntegerVariable(
    const TypeSignature& type) const {
  if (type.parameters().empty()) {
    auto it = variables().find(type.baseName());
    return it != variables().end() && it->second.isIntegerParameter() &&
        it->second.constraint() != "";
  }

  const auto& parameters = type.parameters();
  for (const auto& parameter : parameters) {
    if (hasConstrainedIntegerVariable(parameter)) {
      return true;
    }
  }
  return false;
}

bool ReverseSignatureBinder::tryBind() {
  if (hasConstrainedIntegerVariable(signature_.returnType())) {
    return false;
  }
  tryBindSucceeded_ =
      SignatureBinderBase::tryBind(signature_.returnType(), returnType_);
  return tryBindSucceeded_;
}

} // namespace facebook::velox::exec
