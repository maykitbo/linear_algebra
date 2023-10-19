#pragma once

#include <QPainterPath>

#include "abstract.h"

namespace Graph {

class Painter : public AbstractPainter {
 public:
  Painter(Parameters &params, const QString &name,
          const QPen &graph_pen = QPen(Qt::black, 2),
          QImage::Format format = QImage::Format_ARGB32);

  Painter(Parameters &params, const QString &name, const data_t &data,
          const QPen &graph_pen = QPen(Qt::black, 2),
          QImage::Format format = QImage::Format_ARGB32);

  Painter(Parameters &params, const QString &name, data_t &&data,
          const QPen &graph_pen = QPen(Qt::black, 2),
          QImage::Format format = QImage::Format_ARGB32);

  void AddData(const data_t &data);
  void AddData(data_t &&data);
  void AddData(const std::pair<double, double> &data);
  void AddData(std::pair<double, double> &&data);

  Painter(const Painter &other) = default;
  Painter(Painter &&other) = default;
  virtual ~Painter() = default;

  void Draw() override;

  virtual void DrawingPoints(bool draw_points);
  virtual void DrawingLines(bool draw_lines);
  void SetPointsPen(QPen pen);
  void SetGraphPen(QPen pen);
  void SetMaxPointOnOneGraph(unsigned max_point_on_one_graph);

  const QPen &GetGraphPen() const noexcept;
  const QPen &GetPointsPen() const noexcept;

  qreal MinX() const noexcept;
  qreal MaxX() const noexcept;
  qreal MinY() const noexcept;
  qreal MaxY() const noexcept;

  static QColor ColorInvert(const QColor &color);

 protected:
  std::vector<std::pair<double, double>> data_;
  bool draw_points_{false};
  bool draw_lines_{true};
  QPen points_pen_;
  QPen graph_pen_;
  unsigned max_point_on_one_graph_{30000};

  qreal min_x_;
  qreal max_x_;
  qreal min_y_;
  qreal max_y_;

  void MinMax();

  static QString NameFormat(const QString &name, unsigned size);
};

}  // namespace Graph
