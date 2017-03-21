#include <PID_v1.h>

double pot_esq, pot_dir;
double gyro;
double Kep = 1, Kei = 0, Ked = 0;
double Kdp = 1, Kdi = 0, Kdd = 0;


PID PID_ESQ(&gyro, &pot_esq, 0, Kep, Kei, Ked, REVERSE);
PID PID_DIR(&gyro, &pot_dir, 0, Kdp, Kdi, Kdd, DIRECT);

void Setup_PID()
{
  PID_ESQ.SetOutputLimits(110, 255);
  PID_DIR.SetOutputLimits(110, 255);
  PID_ESQ.SetSampleTime(100);
  PID_DIR.SetSampleTime(100);
  PID_ESQ.SetMode(AUTOMATIC);
  PID_DIR.SetMode(AUTOMATIC);
}

void update_pid()
{
  update_gyro();
  gyro = get_gyro();
  PID_ESQ.Compute();
  PID_DIR.Compute();
}

double potesq()
{
  return pot_esq;
}

double potdir()
{
  return pot_dir;
}


