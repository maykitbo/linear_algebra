#include "parameters.h"

using namespace Graph;

Parameters::Parameters(QSize size)
    : width(size.width()),
      height(size.height()),
      graph_width(size.width() - left_indent - right_indent),
      graph_height(size.height() - top_indent - bottom_indent) {}

qreal Parameters::XToImageCords(double x) const noexcept {
  return (x * x_factor) + dx;
}

qreal Parameters::YToImageCords(double y) const noexcept {
  return (y * y_factor) + dy;
}

double Parameters::XToDataCords(qreal x) const noexcept {
  return (x - left_indent - dx) / x_factor;
}

double Parameters::YToDataCords(qreal y) const noexcept {
  return (y - top_indent - dy) / y_factor;
}

qreal Parameters::XStepToImageCords(double step) const noexcept {
  return step * x_factor;
}

double Parameters::XStepToDataCords(qreal step) const noexcept {
  return step / x_factor;
}

qreal Parameters::YStepToImageCords(double step) const noexcept {
  return -1 * step * y_factor;
}

double Parameters::YStepToDataCords(qreal step) const noexcept {
  return -1 * step / y_factor;
}

qreal Parameters::GetXDistance() const noexcept { return x_max - x_min; }

qreal Parameters::GetYDistance() const noexcept { return y_max - y_min; }

bool Parameters::Valid() {
  return !(isnan(x_min) || isnan(x_max) || isnan(y_min) || isnan(y_max) ||
               isinf(x_min) || isinf(x_max) || isinf(y_min) || isinf(y_max) ||
               x_min >= x_max || y_min >= y_max || isnan(dx),
           isnan(dy) || isinf(dx) || isinf(dy) || isnan(x_factor) ||
               isnan(y_factor) || isinf(x_factor) || isinf(y_factor));
}

void Parameters::Resize(qreal w, qreal h) {
  height = h;
  width = w;

  qreal gw = w - left_indent - right_indent;
  qreal gh = h - top_indent - bottom_indent;

  qreal dw = graph_width / gw;
  qreal dh = graph_height / gh;

  x_factor /= dw;
  y_factor /= dh;
  dx /= dw;
  dy = (dy - graph_height) / dh + gh;

  graph_height = gh;
  graph_width = gw;
}

void Parameters::SetMinMax(qreal min_x, qreal max_x, qreal min_y, qreal max_y) {
  if (isnan(min_x) || isnan(max_x) || isnan(min_y) || isnan(max_y) ||
      isinf(min_x) || isinf(max_x) || isinf(min_y) || isinf(max_y)) {
    std::cerr << "Graph::Parameters::SetMinMax: Incorrect input data\n";
    return;
  }
  SetYMinMax(min_y, max_y);
  SetXMinMax(min_x, max_x);
}

void Parameters::SetYMinMax(qreal min_y, qreal max_y) {
  qreal qy = y_max - y_min;
  y_min = min_y;
  y_max = max_y;
  qreal fy = qy / (y_max - y_min);
  y_factor *= fy;
  dy = (dy - graph_height) * fy + graph_height;
  SetDy(dy);
}

void Parameters::SetXMinMax(qreal min_x, qreal max_x) {
  qreal qx = x_max - x_min;
  x_min = min_x;
  x_max = max_x;
  qreal fx = qx / (x_max - x_min);
  x_factor *= fx;
  dx *= fx;
  SetDx(dx);
}

void Parameters::AddMinMax(qreal min_x, qreal max_x, qreal min_y, qreal max_y) {
  if (isnan(min_x) || isnan(max_x) || isnan(min_y) || isnan(max_y) ||
      isinf(min_x) || isinf(max_x) || isinf(min_y) || isinf(max_y)) {
    std::cerr << "Graph::Parameters::AddMinMax: Incorrect input data\n";
    return;
  }
  if (x_max - x_min <= 0) {
    x_max = max_x;
    x_min = min_x;
  } else if (max_x > x_max || min_x < x_min) {
    SetXMinMax(min_x, max_x);
  }
  if (y_max - y_min <= 0) {
    y_max = max_y;
    y_min = min_y;
  } else if (max_y > y_max || min_y < y_min) {
    SetYMinMax(min_y, max_y);
  }
}

void Parameters::SetDx(qreal candidate) {
  dx = std::max(std::min(candidate, -1.0 * x_min * x_factor),
                graph_width - x_max * x_factor);
}

void Parameters::SetDy(qreal candidate) {
  dy = std::min(std::max(candidate, graph_height - y_min * y_factor),
                -1.0 * y_max * y_factor);
}

void Parameters::SetFactors() {
  x_factor = graph_width / (x_max - x_min);
  dx = -1.0 * x_min * x_factor;

  y_factor = graph_height / (y_min - y_max);
  dy = graph_height - y_min * y_factor;

  scale = 0;
}

QSize Parameters::GraphSize() const noexcept {
  return QSize(graph_width, graph_height);
}

qreal Parameters::Bottom() const noexcept { return height - bottom_indent; }

qreal Parameters::Top() const noexcept { return top_indent; }

qreal Parameters::Left() const noexcept { return left_indent; }

qreal Parameters::Right() const noexcept { return width - right_indent; }

QSize Parameters::Size() const noexcept { return QSize(width, height); }

void Parameters::Move(qreal x, qreal y) {
  SetDx(dx + x);
  SetDy(dy + y);
}

bool Parameters::WheelScale(qreal scale_factor, QPointF cursor_pos) {
  qreal f = 1.0;
  if (scale_factor > 0) {
    ++scale;
    f = wheel_scale_factor_;
  } else {
    if (scale <= 0) {
      return false;
    }
    --scale;
    f = 1.0 / wheel_scale_factor_;
  }

  qreal pos_x = cursor_pos.x() - left_indent;
  qreal pos_y = cursor_pos.y() - top_indent;

  x_factor *= f;
  y_factor *= f;

  SetDx(pos_x - f * (pos_x - dx));
  SetDy(pos_y - f * (pos_y - dy));

  return true;
}

void Parameters::SetTopIndent(qreal indent) noexcept {
  graph_height += top_indent - indent;
  top_indent = indent;
}

void Parameters::SetBottomIndent(qreal indent) noexcept {
  graph_height += bottom_indent - indent;
  bottom_indent = indent;
}

void Parameters::SetLeftIndent(qreal indent) noexcept {
  graph_width += left_indent - indent;
  left_indent = indent;
}

void Parameters::SetRightIndent(qreal indent) noexcept {
  graph_width += right_indent - indent;
  right_indent = indent;
}

void Parameters::SetIndents(qreal t_indent, qreal b_indent, qreal l_indent,
                            qreal r_indent) noexcept {
  graph_height += top_indent + bottom_indent - t_indent - b_indent;
  graph_width += left_indent + right_indent - l_indent - r_indent;

  top_indent = t_indent;
  bottom_indent = b_indent;
  left_indent = l_indent;
  right_indent = r_indent;
}
