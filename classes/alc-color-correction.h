#ifndef ALCCOLORCORRECTION_H
#define ALCCOLORCORRECTION_H

enum Color {
  Red = 0x00,
  Blue,
  Green
};

enum Format {
  RGB = 0x00,
  RBG,
  GRB,
  BRG,
  GBR,
  BGR
};

class QMutex;

class ALCColorCorrection {
protected:
  double m_colorCorrection[3];
  double m_brightness;
  Format m_format;
  QMutex *m_mutex;

public:
  explicit ALCColorCorrection();

  void setColorFormat(Format format);
  void setBrightness(double value);
  void setBlueCorrection(double value);
  void setGreenCorrection(double value);
  void setRedCorrection(double value);

  Format colorFormat();
  double brightness(bool global = false);
  double blueCorrection(bool global = false);
  double greenCorrection(bool global = false);
  double redCorrection(bool global = false);

  static ALCColorCorrection *instance();
};

#endif // ALCCOLORCORRECTION_H
