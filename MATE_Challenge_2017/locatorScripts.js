// Include Math library

takeOff_deg = 184;
ascent_v = 93; // meters per second
ascent_r = 10;
failure_t = 43; // seconds
descent_v = 64;
descent_r = 6;
wind_deg = 270;

ascent_alt = ascent_r * failure_t;
ascent_dist = ascent_v * failure_t;
crashTime = ascent_alt / descent_r;
descent_dist = descent_v * crashTime;
wind_dist = -(1/720*3) * failure_t^3 + 25 * failure_t;

ascent_x = ascent_dist * Math.cos( takeOff_deg );
ascent_y = ascent_dist * Math.sin( takeOff_deg );

descent_x = descent_dist * Math.cos( takeOff_deg );
descent_y = descent_dist * Math.sin( takeOff_deg );

wind_x = wind_dist * Math.cos( 0 ) = 0;
wind_y = wind_dist * Math.sin( wind_deg );

total_x = ascent_x + descent_x + wind_x;
total_y = ascent_y + descent_y + wind_y;

total_dist = { total_x^2 + total_y^2 }^{1/2};
total_deg = tan^{-1}( total_y/total_x );
  // Need to delegate based off quadrant may need to add 90
