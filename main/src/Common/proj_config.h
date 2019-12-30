#ifndef CONFIG_H_
#define CONFIG_H_

/* GPIO */
#define GPIO_LED_MATRIX      (5U)
#define GPIO_BUZZER          (27U)

/* LOOP CONFIG */
#define DISP_LOOP_INTERVAL 	500
#define UATUBE_SYNC_LOOP_INTERVAL  30000 //ms = 30s

/* MATRIX CONFIG */
#define MAT_BRIGHTNESS          (150U)
#define MAT_WIDTH               (32)
#define MAT_HEIGHT              (8)
#define MAT_FIRST_PIXEL_POS     (NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG)
#define MAT_NEO_PIXEL_TYPE      (NEO_GRB + NEO_KHZ800)
/*
----
MAT_FIRST_PIXEL_POS = matrix layout flags, add together as needed:
  NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
    Position of the FIRST LED in the FIRST MATRIX; pick two, e.g.
    NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
  NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs WITHIN EACH MATRIX are
    arranged in horizontal rows or in vertical columns, respectively;
    pick one or the other.
  NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns WITHIN
    EACH MATRIX proceed in the same order, or alternate lines reverse
    direction; pick one.
  NEO_TILE_TOP, NEO_TILE_BOTTOM, NEO_TILE_LEFT, NEO_TILE_RIGHT:
    Position of the FIRST MATRIX (tile) in the OVERALL DISPLAY; pick
    two, e.g. NEO_TILE_TOP + NEO_TILE_LEFT for the top-left corner.
  NEO_TILE_ROWS, NEO_TILE_COLUMNS: the matrices in the OVERALL DISPLAY
    are arranged in horizontal rows or in vertical columns, respectively;
    pick one or the other.
  NEO_TILE_PROGRESSIVE, NEO_TILE_ZIGZAG: the ROWS/COLUMS OF MATRICES
    (tiles) in the OVERALL DISPLAY proceed in the same order for every
    line, or alternate lines reverse direction; pick one.  When using
    zig-zag order, the orientation of the matrices in alternate rows
    will be rotated 180 degrees (this is normal -- simplifies wiring).
  See example below for these values in action.
-----
MAT_NEO_PIXEL_TYPE = pixel type flags, add together as needed:
  NEO_RGB     Pixels are wired for RGB bitstream (v1 pixels)
  NEO_GRB     Pixels are wired for GRB bitstream (v2 pixels)
  NEO_KHZ400  400 KHz bitstream (e.g. FLORA v1 pixels)
  NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
*/

#endif //CONFIG_H_
