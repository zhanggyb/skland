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

#ifndef SKLAND_GRAPHIC_MATRIX_HPP_
#define SKLAND_GRAPHIC_MATRIX_HPP_

#include <memory>

class SkMatrix;

namespace skland {

class Matrix {

 public:

  Matrix();

  Matrix(const Matrix &other);

  ~Matrix();

  Matrix &operator=(const Matrix &other);

  SkMatrix *sk_matrix() const {
    return metadata_.get();
  }

 private:

  std::unique_ptr<SkMatrix> metadata_;

};

}

#endif // SKLAND_GRAPHIC_MATRIX_HPP_
