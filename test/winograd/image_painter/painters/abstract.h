#pragma once

#include <QImage>
#include <QPainter>
#include <QPen>

#include "parameters.h"

namespace Graph {

class AbstractPainter {
 public:
  AbstractPainter(Parameters &params, const QString &name, QSize size,
                  QImage::Format format = QImage::Format_ARGB32);

  virtual void Draw() = 0;
  virtual void Resize(QSize size);
  void SetName(const QString &name);

  QSize GetSize() const noexcept;
  const QString &GetName() const noexcept;
  const QImage &GetImage() const noexcept;

  void SavePng(const QString &path) const;

  virtual ~AbstractPainter() = default;

  AbstractPainter(const AbstractPainter &other);
  AbstractPainter(AbstractPainter &&other);

 protected:
  Parameters &p_;
  QImage image_;
  QPainter painter_;

  QString name_;
};

}  // namespace Graph
