#pragma once

// Conversión entre píxeles y metros
#define PIXELS_PER_METER 50.0f          // 1 metro = 50 píxeles
#define METER_PER_PIXEL (1.0f / PIXELS_PER_METER)

#define PIXEL_TO_METERS(p) ((p) * METER_PER_PIXEL)  // pasar de píxeles a metros
#define METERS_TO_PIXELS(m) ((m) * PIXELS_PER_METER) // pasar de metros a píxeles