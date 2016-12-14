//
// Created by zhanggyb on 16-12-14.
//

#ifndef SKLAND_META_MATRIX_HPP
#define SKLAND_META_MATRIX_HPP

#include "SkMatrix.h"

namespace skland {
namespace graphic {

struct MetaMatrix {

  MetaMatrix() {}

  MetaMatrix(const MetaMatrix &other)
      : sk_matrix(other.sk_matrix) {}

  MetaMatrix &operator=(const MetaMatrix &other) {
    sk_matrix = other.sk_matrix;
    return *this;
  }

  SkMatrix sk_matrix;

};

}
}

#endif //SKLAND_META_MATRIX_HPP
