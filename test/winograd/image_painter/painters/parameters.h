#pragma once

#include <math.h>

#include <QDateTime>
#include <QPointF>
#include <QSize>
#include <iostream>
#include <limits>

namespace Graph {

typedef std::vector<std::pair<double, double>> data_t;

class Parameters {
 public:
  Parameters() = default;
  Parameters(QSize size);

  QSize GraphSize() const noexcept;

  qreal XToImageCords(double x) const noexcept;
  qreal YToImageCords(double y) const noexcept;
  double XToDataCords(qreal x) const noexcept;
  double YToDataCords(qreal y) const noexcept;
  qreal XStepToImageCords(double step) const noexcept;
  double XStepToDataCords(qreal step) const noexcept;
  qreal YStepToImageCords(double step) const noexcept;
  double YStepToDataCords(qreal step) const noexcept;
  qreal GetXDistance() const noexcept;
  qreal GetYDistance() const noexcept;

  bool Valid();

  void SetTopIndent(qreal indent) noexcept;
  void SetBottomIndent(qreal indent) noexcept;
  void SetLeftIndent(qreal indent) noexcept;
  void SetRightIndent(qreal indent) noexcept;
  void SetIndents(qreal t_indent, qreal b_indent, qreal l_indent,
                  qreal r_indent) noexcept;

  void SetMinMax(qreal min_x, qreal max_x, qreal min_y, qreal max_y);
  void AddMinMax(qreal min_x, qreal max_x, qreal min_y, qreal max_y);

  void Resize(qreal w, qreal h);
  void SetFactors();

  qreal Bottom() const noexcept;
  qreal Top() const noexcept;
  qreal Left() const noexcept;
  qreal Right() const noexcept;

  QSize Size() const noexcept;

  void Move(qreal x, qreal y);
  bool WheelScale(qreal scale_factor, QPointF cursor_pos);

 private:
  qreal dx{0};
  qreal dy{0};

  qreal x_min{0};
  qreal x_max{0};
  qreal y_min{0};
  qreal y_max{0};

  qreal x_factor;
  qreal y_factor;

  qreal top_indent{10};
  qreal bottom_indent{30};
  qreal left_indent{70};
  qreal right_indent{10};

  qreal width{1};
  qreal height{1};
  qreal graph_width{1};
  qreal graph_height{1};

  int scale{0};
  qreal wheel_scale_factor_{1.1};

  void SetDx(qreal dx);
  void SetDy(qreal dy);

  void SetYMinMax(qreal min_y, qreal max_y);
  void SetXMinMax(qreal min_x, qreal max_x);
};

}  // namespace Graph
