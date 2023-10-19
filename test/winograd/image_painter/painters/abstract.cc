#include "abstract.h"

using namespace Graph;

AbstractPainter::AbstractPainter(Parameters &params, const QString &name,
                                 QSize size, QImage::Format format)
    : image_(size, format), p_(params), name_(name) {}

AbstractPainter::AbstractPainter(const AbstractPainter &other)
    : p_(other.p_), image_(other.image_), name_(other.name_) {}

AbstractPainter::AbstractPainter(AbstractPainter &&other)
    : p_(other.p_),
      image_(std::move(other.image_)),
      name_(std::move(other.name_)) {}

void AbstractPainter::Resize(QSize size) {
  image_ = QImage(size, image_.format());
  Draw();
}

void AbstractPainter::SetName(const QString &name) { name_ = name; }

QSize AbstractPainter::GetSize() const noexcept { return image_.size(); }

const QString &AbstractPainter::GetName() const noexcept { return name_; }

const QImage &AbstractPainter::GetImage() const noexcept { return image_; }

void AbstractPainter::SavePng(const QString &path) const {
  image_.save(path.endsWith(".png") ? path : path + ".png");
  // if (path.endsWith(".png"))
  //     image_.save(path);
  // else
  //     image_.save(path + ".png");
}
