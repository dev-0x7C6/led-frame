/**********************************************************************************
 * AmbientLedDriver - https://gitorious.org/ambientleddriver -                    *
 * Copyright (C) 2014  Bartłomiej Burdukiewicz                                    *
 * Contact: bartlomiej.burdukiewicz@gmail.com                                     *
 *                                                                                *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU Lesser General Public                     *
 * License as published by the Free Software Foundation; either                   *
 * version 2.1 of the License, or (at your option) any later version.             *
 *                                                                                *
 * This program is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU              *
 * Lesser General Public License for more details.                                *
 *                                                                                *
 * You should have received a copy of the GNU Lesser General Public               *
 * License along with this program; if not, see <http://www.gnu.org/licences/>.   *
 **********************************************************************************/

#ifndef ALCANIMATIONEMITTER_H
#define ALCANIMATIONEMITTER_H

#include <QColor>
#include <QObject>
#include <QRgb>
#include <QThread>
#include <QVariantAnimation>

#include <atomic>

#include "emitters/alc-emitter.h"
#include "classes/alc-runtime-sync.h"

namespace Emitters {

  class ALCAnimationEmitter : public QThread, public ALCEmitter {
    Q_OBJECT
  public:
    enum class Animation {
      None,
      Glow,
      Rotate,
      Shift
    };

    enum class Effect {
      None,
      Flicker
    };

    explicit ALCAnimationEmitter();
    virtual ~ALCAnimationEmitter();

    bool open();
    bool configure();

    void setup(Animation animation, Effect effect, int cycle = 10000);

    void setFlickerValue(int flicker = 0);
    int flickerValue();

    void setQuitState(bool state = true);

  protected:
    void run();
    void init();
    void done();

  private:
    std::atomic <Animation> m_animation;
    std::atomic <Effect> m_effect;
    std::atomic <quint32> m_rgb;
    std::atomic <quint64> m_flicker;
    std::atomic <bool> m_quit;

    QVariantAnimation m_variantAnimation;

    void process(const QVariant &value);
  };

}

#endif // ALCANIMATIONEMITTER_H
