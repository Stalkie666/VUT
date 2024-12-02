/**
 * convoySettings.hpp
 * IMS, T2: Logistika
 * Author: Jakub Hamadej, xhamad03, FIT VUT, 28. 11. 2024
 */

#ifndef __CONVOY_SETTINGS_HPP__
#define __CONVOY_SETTINGS_HPP__

/**
 * Basic matrics for defining steps in simulation
 */
#define HOUR 60.0
#define DAY  (HOUR * 24)
#define MONTH (DAY * 28)
#define YEAR (DAY * 365)


#define SIMULATION_RUN_TIME (YEAR * 5)
/**
 * Standard convoy trafics
 * when you divide, then 1 or more convoys per day
 * when multiply, then 1 or less convoys per day
 */
#define NORTH_CONVOYS_PER_DAY (DAY / 2)
#define SOUTH_CONVOYS_PER_DAY (DAY / 1)

/**
 * When 1, accidents in simulation are allowed to hapened
 */
#define ALLOW_ACCIDENTS 0
#define EVER_GIVEN_ACCIDENTS_PER_MONTH (MONTH / 1)
#define EVER_GIVEN_ACCIDENTS_RECOVER_TIME (DAY * 6)

/**
 * When 1, more convoys are allowed
 */
#define ALLOW_SPECIAL_CONVOYS 0
#define SPECIAL_CONVOYS_FROM_NORTH_PER_MONTH (MONTH / 32)
#define SPECIAL_CONVOYS_FROM_SOUTH_PER_MONTH (DAY / 1)

#endif //__CONVOY_SETTINGS_HPP__