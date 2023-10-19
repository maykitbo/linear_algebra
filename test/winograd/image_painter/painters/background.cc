
#include "background.h"

using namespace Graph;

Background::Background(Parameters &params, const QColor &color,
                       const QPen &axes_pen, const QPen &grid_pen,
                       const QFont &text_font, QImage::Format format)
    : AbstractPainter(params, "Background", params.Size(), format),
      color_(color),
      axes_pen_(axes_pen),
      grid_pen_(grid_pen),
      text_font_(text_font),
      text_pen_(axes_pen) {}

void Background::Draw() {
  if (!p_.Valid()) {
    std::cerr << "Graph::Background::Draw: Parameters is not valid\n";
    return;
  }
  painter_.begin(&image_);
  image_.fill(color_);
  if (axes_ == true) {
    painter_.setPen(axes_pen_);
    // draw vertical axes
    painter_.drawLine(p_.Left(), p_.Top(), p_.Left(), p_.Bottom());
    // draw horizontal axes
    painter_.drawLine(p_.Left(), p_.Bottom(), p_.Right(), p_.Bottom());
  }
  unsigned min, max;
  painter_.setPen(grid_pen_);

  if (vertical_grid_) {
    min = p_.Left();
    max = p_.Right();
    for (unsigned i = min; i < max; i += vertical_grid_step_) {
      painter_.drawLine(i, p_.Top(), i, p_.Bottom());
    }
  }
  if (horizontal_grid_) {
    min = p_.Top();
    max = p_.Bottom();
    for (unsigned i = min; i < max; i += horizontal_grid_step_) {
      painter_.drawLine(p_.Left(), i, p_.Right(), i);
    }
  }
  if (show_text_) {
    painter_.setPen(text_pen_);
    painter_.setFont(text_font_);
    if (vertical_grid_) {
      min = p_.Left();
      max = p_.Right();
      for (unsigned i = min; i < max; i += vertical_grid_step_) {
        // TODO Why 30?
        painter_.drawText(i - 30, p_.Bottom() + bottom_text_indent_,
                          x_text_func_(p_.XToDataCords(i)));
      }
    }
    if (horizontal_grid_) {
      min = p_.Top();
      max = p_.Bottom();
      for (unsigned i = min; i < max; i += horizontal_grid_step_) {
        painter_.drawText(left_text_indent_, i + 5,
                          y_text_func_(p_.YToDataCords(i)));
      }
    }
  }
  painter_.end();
}

QString Background::NumberFormat(double n) {
  return QString::number(n, 'g', 3);
}

QString Background::DateFormatSec(double n) {
  return QDateTime::fromSecsSinceEpoch(n).toString("yyyy-MM-dd");
}

QString Background::DateFormatDay(double n) {
  return QDateTime::fromSecsSinceEpoch(n * 60 * 60 * 24).toString("yyyy-MM-dd");
}

void Background::SetAxes(bool axes) noexcept { Setter(axes_, axes); }

void Background::SetColor(const QColor &color) noexcept {
  Setter(color_, color);
}

void Background::SetHorizontalGrid(bool grid) noexcept {
  Setter(horizontal_grid_, grid);
}

void Background::SetVerticalGrid(bool grid) noexcept {
  Setter(vertical_grid_, grid);
}

void Background::SetGrid(bool grid) noexcept {
  if (horizontal_grid_ == grid && vertical_grid_ == grid) return;
  horizontal_grid_ = grid;
  vertical_grid_ = grid;
  Draw();
}

void Background::SetShowText(bool show) noexcept { Setter(show_text_, show); }

void Background::SetVerticalGridStep(qreal step) noexcept {
  Setter(vertical_grid_step_, p_.XStepToImageCords(step));
}

void Background::SetHorizontalGridStep(qreal step) noexcept {
  Setter(horizontal_grid_step_, p_.YStepToImageCords(step));
}

void Background::SetBottomTextIndent(qreal indent) noexcept {
  Setter(bottom_text_indent_, indent);
}

void Background::SetLeftTextIndent(qreal indent) noexcept {
  Setter(left_text_indent_, indent);
}

void Background::SetAxesPen(const QPen &pen) noexcept {
  Setter(axes_pen_, pen);
}

void Background::SetGridPen(const QPen &pen) noexcept {
  Setter(grid_pen_, pen);
}

void Background::SetTextFont(const QFont &font) noexcept {
  Setter(text_font_, font);
}

void Background::SetXTextFunc(const TextFunc &func) noexcept {
  x_text_func_ = func;
  // Draw();
}

void Background::SetYTextFunc(const TextFunc &func) noexcept {
  y_text_func_ = func;
  // Draw();
}

void Background::SetTextPen(const QPen &pen) noexcept {
  Setter(text_pen_, pen);
}

const QFont &Background::GetTextFont() const noexcept { return text_font_; }

const QColor &Background::GetColor() const noexcept { return color_; }

const QPen &Background::GetAxesPen() const noexcept { return axes_pen_; }

const QPen &Background::GetGridPen() const noexcept { return grid_pen_; }

const QPen &Background::GetTextPen() const noexcept { return text_pen_; }

qreal Background::GetHorizontalGridStep() const noexcept {
  return p_.YStepToDataCords(horizontal_grid_step_);
}

qreal Background::GetVerticalGridStep() const noexcept {
  return p_.XStepToDataCords(vertical_grid_step_);
}
