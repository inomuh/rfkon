/*
 * Timeout.cpp
 *
 * GLOBAL DEĞİŞKENLER:
 *  Yok
 *
 * ENUM KULLANIMI:
 *  Yok
 *
 * FONKSİYON PROTOTİPLERİ:
 *  time_point startTime() const;
 *  duration getMaxDuration() const;
 *  void setMaxDuration(duration);
 *  void setTimerToZero();
 *  bool isExpired() const;
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

#include "Timeout.h"

// 'startTime' fonksiyonu, sayıcıyı başlatır
std::chrono::steady_clock::time_point Timeout::startTime() const
{
  return startTime_;
}

// 'getMaxDuration' fonksiyonu, zaman aşımı değerini döndürür
std::chrono::steady_clock::duration Timeout::getMaxDuration() const
{
  return maxDuration_;
}

// 'setMaxDuration' fonksiyonu, aldığı parametreye bağlı olarak
// zaman aşımı değerini değiştirir
void Timeout::setMaxDuration(std::chrono::steady_clock::duration maxDuration)
{
  maxDuration_ = maxDuration;
}

// 'setTimerToZero' fonksiyonu, sayıcıyı sıfırlar
void Timeout::setTimerToZero()
{
  Timeout::startTime_ = clock_type::now();
}

// 'isExpired' fonksiyonu, belirlenen zaman aşılmışsa true
// aşılmamışsa false döner
bool Timeout::isExpired() const
{
  const auto endTime = clock_type::now();

  return (endTime - startTime()) > getMaxDuration();
}
