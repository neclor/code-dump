#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

static inline double mm_to_m(uint64_t mm) { return mm / 1000.0; }

static inline double mm_to_km(uint64_t mm) { return mm / 1e6; }

static inline double mm_to_mi(uint64_t mm) { return mm / 1609344.0; }

static inline float m_s_to_kmh(float m_s) { return m_s * 3.6f; }

static inline float m_s_to_mph(float m_s) { return m_s * 2.236936f; }

#endif // UTILS_H
