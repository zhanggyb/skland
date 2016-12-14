//
// Created by zhanggyb on 16-12-14.
//

#ifndef SKLAND_MATRIX_HPP
#define SKLAND_MATRIX_HPP

#include <memory>

namespace skland {

namespace graphic {
struct MetaMatrix;
}

class Matrix {

 public:

  Matrix();

  Matrix(graphic::MetaMatrix *matrix);

  Matrix(const Matrix &other);

  ~Matrix();

  Matrix &operator=(const Matrix &other);

  const graphic::MetaMatrix *metadata() const {
    return metadata_.get();
  }

 private:

  std::shared_ptr<graphic::MetaMatrix> metadata_;

};

}

#endif //SKLAND_MATRIX_HPP
