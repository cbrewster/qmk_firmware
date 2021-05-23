RGB_MATRIX_EFFECT(better_splash)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#define EFFECT_MAX 100

uint16_t effect_func(uint8_t dist, uint16_t tick) {
    uint16_t effect = tick - dist;
    if (effect > EFFECT_MAX) effect = EFFECT_MAX;
    return EFFECT_MAX - effect;
}

// lol pretty sure this exists somewhere
RGB rgb_color(uint8_t r, uint8_t g, uint8_t b) {
    RGB res;
    res.r = r;
    res.g = g;
    res.b = b;
    return res;
}

static bool better_splash(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    HSV hsv = rgb_matrix_config.hsv;
    RGB rgb1 = rgb_matrix_hsv_to_rgb(hsv);
    RGB rgb2 = rgb_color(RGB_RED);

    uint8_t count = g_last_hit_tracker.count;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        uint16_t effect = 0;
        for (uint8_t j = 0; j < count; j++) {
            int16_t  dx   = g_led_config.point[i].x - g_last_hit_tracker.x[j];
            int16_t  dy   = g_led_config.point[i].y - g_last_hit_tracker.y[j];
            uint8_t  dist = sqrt16(dx * dx + dy * dy);
            uint16_t tick = scale16by8(g_last_hit_tracker.tick[j], rgb_matrix_config.speed);
            effect        = qadd8(effect, effect_func(dist, tick));
        }
        if (effect > 255) effect = 255;

        RGB res;
        res.r = lerp8by8(rgb1.r, rgb2.r, effect);
        res.g = lerp8by8(rgb1.g, rgb2.b, effect);
        res.b = lerp8by8(rgb1.b, rgb2.b, effect);

        rgb_matrix_set_color(i, res.r, res.g, res.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}

#endif
