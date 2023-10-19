#include "painter.h"

using namespace Graph;

Painter::Painter(Parameters &params, const QString &name, const QPen &graph_pen,
                 QImage::Format format)
    : AbstractPainter(params, NameFormat(name, 0), params.GraphSize(), format),
      points_pen_(ColorInvert(graph_pen.color()), graph_pen.width() + 1),
      graph_pen_(graph_pen) {}

Painter::Painter(Parameters &params, const QString &name, const data_t &data,
                 const QPen &graph_pen, QImage::Format format)
    : AbstractPainter(params, NameFormat(name, data.size()), params.GraphSize(),
                      format),
      points_pen_(ColorInvert(graph_pen.color()), graph_pen.width() + 1),
      graph_pen_(graph_pen),
      data_(data) {
  MinMax();
}

Painter::Painter(Parameters &params, const QString &name, data_t &&data,
                 const QPen &graph_pen, QImage::Format format)
    : AbstractPainter(params, NameFormat(name, data.size()), params.GraphSize(),
                      format),
      points_pen_(ColorInvert(graph_pen.color()), graph_pen.width() + 1),
      graph_pen_(graph_pen),
      data_(data) {
  MinMax();
}

void Painter::MinMax() {
  if (data_.empty()) return;
  min_x_ = data_.front().first;
  max_x_ = data_.back().first;
  auto yminmax = std::minmax_element(data_.begin(), data_.end(),
                                     [](const std::pair<double, double> &p1,
                                        const std::pair<double, double> &p2) {
                                       return p1.second < p2.second;
                                     });
  min_y_ = yminmax.first->second;
  max_y_ = yminmax.second->second;

  p_.AddMinMax(min_x_, max_x_, min_y_, max_y_);
}

void Painter::Draw() {
  if (!p_.Valid()) {
    std::cerr << "Graph::Painter::Draw: Parameters is not valid\n";
    return;
  }
  image_.fill(Qt::transparent);

  std::vector<QPointF> points;

  if (data_.empty()) return;

  unsigned step = 1u + data_.size() / max_point_on_one_graph_;
  points.reserve(data_.size() / step);
  for (unsigned i = 0; i < data_.size(); i += step) {
    points.emplace_back(p_.XToImageCords(data_[i].first),
                        p_.YToImageCords(data_[i].second));
  }

  painter_.begin(&image_);
  if (draw_lines_) {
    painter_.setPen(graph_pen_);
    painter_.drawPolyline(points.data(), points.size());
  }
  if (draw_points_) {
    painter_.setPen(points_pen_);
    painter_.drawPoints(points.data(), points.size());
  }
  painter_.end();
}

void Painter::DrawingPoints(bool draw_points) { draw_points_ = draw_points; }

void Painter::DrawingLines(bool draw_lines) { draw_lines_ = draw_lines; }

void Painter::SetPointsPen(QPen pen) { points_pen_ = pen; }

void Painter::SetGraphPen(QPen pen) { graph_pen_ = pen; }

void Painter::SetMaxPointOnOneGraph(unsigned max_point_on_one_graph) {
  max_point_on_one_graph_ = max_point_on_one_graph;
}

const QPen &Painter::GetGraphPen() const noexcept { return graph_pen_; }

const QPen &Painter::GetPointsPen() const noexcept { return points_pen_; }

qreal Painter::MinX() const noexcept { return min_x_; }

qreal Painter::MaxX() const noexcept { return max_x_; }

qreal Painter::MinY() const noexcept { return min_y_; }

qreal Painter::MaxY() const noexcept { return max_y_; }

QColor Painter::ColorInvert(const QColor &color) {
  QColor c = color.toHsv();
  c.setHsl(c.hslHue() > 180 ? c.hslHue() - 180 : c.hslHue() + 180,
           c.hslSaturation(), c.lightness());
  return c;
}

QString Painter::NameFormat(const QString &name, unsigned size) {
  return name + " [" + QString::number(size) + "]";
}
