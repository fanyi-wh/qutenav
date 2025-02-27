/* -*- coding: utf-8-unix -*-
 *
 * File: src/settings.cpp
 *
 * Copyright (C) 2021 Jukka Sirkka
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "settings.h"
#include <QDebug>
#include "platform.h"

bool TextGroup::enabled() const {
  return Conf::MarinerParams::TextGrouping().contains(m_group);
}

void TextGroup::setEnabled(bool v) {
  auto values = Conf::MarinerParams::TextGrouping();
  if (values.contains(m_group) && v) return;
  if (!values.contains(m_group) && !v) return;

  if (values.contains(m_group)) {
    values.removeOne(m_group);
  } else {
    values.append(m_group);
  }
  Conf::MarinerParams::setTextGrouping(values);
  emit enabledChanged();
}

Settings* Settings::instance() {
  static Settings* s = new Settings();
  return s;
}

Settings::Settings(QObject *parent)
  : QObject(parent)
{
  m_textGroups << new TextGroup(10, true, "Important text");
  m_textGroups << new TextGroup(11, true, "Vertical clearances etc.",
                                "Vertical clearance of bridges, overhead cable, pipe or conveyor, "
                                "bearing of navline, recommended route, deep water route centreline, "
                                "name and communications channel of radio calling-in point.");
  m_textGroups << new TextGroup(12, true, "Soundings");
  m_textGroups << new TextGroup(20, true, "Other text");
  m_textGroups << new TextGroup(21, true, "Position names",
                                "Names for position reporting: "
                                "name or number of buoys, beacons, daymarks, "
                                "light vessel, light float, offshore platform.");
  m_textGroups << new TextGroup(23, true, "Light descriptions");
  m_textGroups << new TextGroup(24, true, "Notes", "Note on chart data or nautical publication.");
  m_textGroups << new TextGroup(25, true, "Nature of seabed");
  m_textGroups << new TextGroup(26, true, "Geographic names");
  m_textGroups << new TextGroup(27, true, "Other values", "Value of magnetic variation or swept depth.");
  m_textGroups << new TextGroup(28, true, "Other heights", "Height of islet or land feature");
  m_textGroups << new TextGroup(29, true, "Berth numbers");
  m_textGroups << new TextGroup(31, true, "National language text");

  for (auto obj: m_textGroups) {
    auto group = qobject_cast<TextGroup*>(obj);
    connect(group, &TextGroup::enabledChanged, this, &Settings::settingsChanged);
  }
}


float Settings::displayLengthScaling() const {
  const float y0 = 6.2 / nominal_dpmm;
  const float y1 = 9.0 / nominal_dpmm;
  const float t = (dots_per_mm_y() - dpmm0) / delta_dpmm1;
  return y1 * t + y0 * (1 - t);
}

float Settings::displayTextSizeScaling() const {
  const float y0 = 3.5;
  const float y1 = 4.2;
  const float t = (dots_per_mm_y() - dpmm0) / delta_dpmm1;
  return (y1 * t + y0 * (1 - t));
}

float Settings::displayLineWidthScaling() const {
  const float y0 = .7;
  const float y1 = .3;
  const float t = (dots_per_mm_y() - dpmm0) / delta_dpmm1;
  return y1 * t + y0 * (1 - t);
}

float Settings::displayRasterSymbolScaling() const {
  const float y0 = 1.;
  const float y1 = .5;
  const float t = (dots_per_mm_y() - dpmm0) / delta_dpmm1;
  return y1 * t + y0 * (1 - t);
}

Settings::~Settings() {
  qDeleteAll(m_textGroups);
}
