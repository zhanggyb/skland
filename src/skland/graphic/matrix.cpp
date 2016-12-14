//
// Created by zhanggyb on 16-12-14.
//

#include <skland/graphic/matrix.hpp>

#include "internal/meta-matrix.hpp"

namespace skland {

Matrix::Matrix() {
}

Matrix::Matrix(graphic::MetaMatrix *matrix)
    : metadata_(matrix) {
}

Matrix::Matrix(const Matrix &other)
    : metadata_(other.metadata_) {
}

Matrix::~Matrix() {
}

Matrix &Matrix::operator=(const Matrix &other) {
  metadata_ = other.metadata_;
  return *this;
}

}