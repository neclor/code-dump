#ifndef BIKE_STORAGE_H
#define BIKE_STORAGE_H

#include "../../core/bike_data/bike_data.h"

void bike_storage_init(void);
void bike_storage_load(BikeData* data);

void bike_storage_save_wheel(const BikeData* data);
void bike_storage_save_total_distance(const BikeData* data);
void bike_storage_save_total_distance_if_needed(const BikeData* data);

#endif // BIKE_STORAGE_H
