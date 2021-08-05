#ifndef MATH_TOOLS_H
#define MATH_TOOLS_H

#include <cmath>

using namespace std;

double round_nplaces(double value, int decimal_places) {
    double places = pow(10.0, decimal_places);
    return round(value * places) / places;
}

double round_up(double value, int decimal_places) {
    const double multiplier = pow(10.0, decimal_places);
    return ceil(value * multiplier) / multiplier;
}

double round_down(double value, int decimal_places) {
    const double multiplier = pow(10.0, decimal_places);
    return floor(value * multiplier) / multiplier;
}

int trunc_double(double value) {
    return trunc(value);
}

/**
 * (1) convert double to string, e.g. "10.00"
 * (2) find the "." in the created string and get the index (position) of it
 * (3) create string that displays the part before the dot, e.g. "10"
 * (4) create string that displays the part beginning at the dot, e.g. ".00"
 * 
 * (5.1.) [decimal_places == 0] just return the prePoint string (literally just end
 *        the substring after the length of the string --> last index+1 --> substring does
 *        not take this char anymore, but doesn't matter because the last index+1 has no value, does practically nothing)
 * 
 * (5.2.) [else] return value in front of the dot followed by the dot and
 *        decimal places (add 1 to the to-param of substring() so this index
 *        will also be included and the string is not cut of before)
 * 
 * @param value double to be truncated
 * @param decimal_places decimal places to cut to
 * @returns string as truncated version of the input cut to n decimals
 */
String removeDecimalPlaces(double value, int decimal_places) {
    String convertedString = String(value); // (1)
    int decimalPointIndex = convertedString.indexOf("."); // (2)

    String prePoint = convertedString.substring(0, decimalPointIndex); // (3)
    String afterPoint = convertedString.substring(decimalPointIndex, convertedString.length()); // (4)

    return decimal_places == 0 ? prePoint.substring(0, prePoint.length()) : prePoint + afterPoint.substring(0, decimal_places+1); // (5)
}

#endif