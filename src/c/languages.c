#include <pebble.h>
#include "languages.h"

/* day names in many different languages! */
const char dayNames[2][7][8] = {
	{"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"},
	{"DIM", "LUN", "MAR", "MER", "JEU", "VEN", "SAM"}
};

/* month names in many different languages! */
const char monthNames[2][12][8] = {
	{"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"},
	{"JAN", "FÉV", "MAR", "AVR", "MAI", "JUI", "JUL", "AOÛ", "SEP", "OCT", "NOV", "DÉC"}
};

// all of these are taken from:
// http://www.unicode.org/cldr/charts/28/by_type/date_&_time.fields.html#521165cf49647551
const char wordForWeek[2][12] = {
	"Wk",
	"Sem"
};
