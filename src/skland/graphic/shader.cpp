/*
 * Copyright 2016 Freeman Zhang <zhanggyb@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <skland/graphic/shader.hpp>

#include "internal/meta-shader.hpp"

namespace skland {

Shader::Shader() {

}

Shader::Shader(graphic::MetaShader *metadata)
    : metadata_(metadata) {
}

Shader::Shader(const Shader &other)
    : metadata_(other.metadata_) {
}

Shader::~Shader() {
}

Shader &Shader::operator=(const Shader &other) {
  metadata_ = other.metadata_;
  return *this;
}

void Shader::SetMetadata(graphic::MetaShader *metashader) {
  metadata_.reset(metashader);
}

}
