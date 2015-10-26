/*
 * Timeout.h
 *
 * AMAÇ:
 *  Timeout, BT ve Wifi tarama sonucunda oluşturulan mesajların ne
 *  sıklıkta Publish edileceğini belirleyen nesnedir. Bu yapı sayesinde,
 *  belirlenen zaman limiti aşıldığında mesajlar Publish edilir, fakat zaman
 *  limiti aşılmadığı sürece de diğer işlemler yapılır. Böylece diğer işlemler
 *  askıya alınmamış olur
 *
 * NOTLAR:
 *  Yazar: Uğur Bolat
 *  Tarih: 15.10.2015
 *
 *   Versiyon: v_1.0
 *    Güncelleme Tarihi: 15.10.2015
 *
 */

#ifndef TIMEOUT_H
#define TIMEOUT_H

#include <iostream>
#include <future>
#include <chrono>

class Timeout
{
public:
  typedef std::chrono::steady_clock clock_type;
  typedef clock_type::time_point time_point;
  typedef clock_type::duration duration;

  explicit Timeout(duration maxDuration) :
    startTime_(clock_type::now()),
    maxDuration_(maxDuration)
  {}

  time_point startTime() const;
  duration getMaxDuration() const;
  void setMaxDuration(duration);
  void setTimerToZero();
  bool isExpired() const;


private:
  time_point startTime_;
  duration maxDuration_;
};

#endif // TIMEOUT_H

