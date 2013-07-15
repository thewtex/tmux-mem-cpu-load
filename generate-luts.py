#!/usr/bin/env python

"""Create the color code lookup table header."""

import matplotlib.cm

def write_table(fp, colormap_name, stat_name, first_half_foreground, second_half_foreground):
    fp.write('static const char ')
    fp.write(stat_name)
    fp.write('_lut[][32] = {\n')
    colormap = matplotlib.cm.get_cmap(colormap_name)
    for ii in range(101):
        fp.write('"#[fg=')
        if ii < 50:
            fp.write(first_half_foreground)
        else:
            fp.write(second_half_foreground)
        fp.write(',bg=colour')
        rgba = colormap(ii * 0.01)
        red = int(round(rgba[0] * 5))
        green = int(round(rgba[1] * 5))
        blue = int(round(rgba[2] * 5))
        color = 16 + 36*red + 6*green + blue;
        fp.write(str(color))
        fp.write(']"')
        if ii != 100:
            fp.write(',')
        fp.write('\n')
    fp.write('}; // end ')
    fp.write(stat_name)
    fp.write('_lut\n\n')

with open('luts.h', 'w') as fp:
    fp.write('#ifndef _luts_h\n')
    fp.write('#define _luts_h\n\n')

    # hot colormap with white fg for the first half
    # and black fg for the second half
    write_table(fp, 'hot', 'cpu_percentage', 'brightwhite', 'black')
    write_table(fp, 'gist_earth', 'mem', 'brightwhite', 'black')
    write_table(fp, 'bone', 'load', 'brightwhite', 'black')

    fp.write('#endif\n')
