#ifdef PBL_HEALTH
#include <pebble.h>
#include "health.h"
#include "debug.h"

#define SECONDS_AFTER_WAKE_UP 1800 // Half hour
#define NIGHT_SECONDS_BEFORE_MIDNIGHT SECONDS_PER_HOUR * 4 /* Night start at 8 pm */


static bool s_sleeping;
static bool s_restfulSleeping;
static time_t s_endSleepTime;
static HealthValue s_sleep_seconds;
static HealthValue s_restful_sleep_seconds;
static HealthValue s_distance_walked;
static HealthValue s_steps;
static HealthValue s_heart_rate;

static inline bool is_health_metric_accessible(HealthMetric metric, time_t time_start, time_t time_end) {
    HealthServiceAccessibilityMask mask = health_service_metric_accessible(metric, time_start, time_end);
    return mask & HealthServiceAccessibilityMaskAvailable;
}

static inline HealthValue get_health_value_sum_today(HealthMetric metric) {
    time_t start = time_start_of_today();
    time_t end = time(NULL);

    return is_health_metric_accessible(metric, start, end) ? health_service_sum_today(metric) : 0;
}

static inline HealthValue get_health_value_sum_from_nigth_start(HealthMetric metric) {
    time_t start = time_start_of_today() - NIGHT_SECONDS_BEFORE_MIDNIGHT;
    time_t end = time(NULL);

    return is_health_metric_accessible(metric, start, end) ? health_service_sum(metric, start, end) : 0;
}

void Health_update(void) {
    HealthActivityMask mask = health_service_peek_current_activities();

    // Sleep
    s_sleeping = (mask & HealthActivitySleep) || (mask & HealthActivityRestfulSleep);
    s_restfulSleeping = (mask & HealthActivityRestfulSleep);

    s_sleep_seconds = get_health_value_sum_today(HealthMetricSleepSeconds);
    s_restful_sleep_seconds = get_health_value_sum_today(HealthMetricSleepRestfulSeconds);

    // Last screen update before midnight
    if(time(NULL) - time_start_of_today() > SECONDS_PER_DAY - 90) {
        s_sleep_seconds_before_midnight = get_health_value_sum_from_nigth_start(HealthMetricSleepSeconds);
        s_sleep_seconds_before_midnight_time = time(NULL);
    }

    // Add first part of night if exist
    if(s_sleep_seconds_before_midnight_time > time_start_of_today() - NIGHT_SECONDS_BEFORE_MIDNIGHT &&
       s_sleep_seconds_before_midnight_time < time_start_of_today()) {
        s_sleep_seconds += s_sleep_seconds_before_midnight;
    }


    if(s_sleeping) {
        s_endSleepTime = time(NULL);
/* Debug */ Debug_healthSleepCall++;
    }
/* Debug */     if(s_restfulSleeping) {
/* Debug */         Debug_restfulSleeping++;
/* Debug */     }

    // Steps
    s_distance_walked = get_health_value_sum_today(HealthMetricWalkedDistanceMeters);
    s_steps = get_health_value_sum_today(HealthMetricStepCount);

    // Heart rate
    time_t now = time(NULL);
    if (is_health_metric_accessible(HealthMetricHeartRateBPM, now, now)) {
        s_heart_rate = health_service_peek_current_value(HealthMetricHeartRateBPM);
    }
}

bool Health_isUserSleeping(void) {
    return s_sleeping;
}

bool Health_isUserRestfulSleeping(void) {
    return s_restfulSleeping;
}

bool Health_sleepingToBeDisplayed(void) {
    // Sleep should be display during an half hour after wake up
    return s_sleeping || (s_endSleepTime + SECONDS_AFTER_WAKE_UP > time(NULL));
}

HealthValue Health_getSleepSeconds(void) {
    return s_sleep_seconds;
}

HealthValue Health_getRestfulSleepSeconds(void) {
    return s_restful_sleep_seconds;
}

HealthValue Health_getDistanceWalked(void) {
    return s_distance_walked;
}

HealthValue Health_getSteps(void) {
    return s_steps;
}

HealthValue Health_getHeartRate(void) {
    return s_heart_rate;
}

#endif // PBL_HEALTH
