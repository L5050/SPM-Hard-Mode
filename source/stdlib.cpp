#include "mod.h"

#include <msl/stdio.h>
#include <msl/string.h>

namespace mod {

  s32 roundi(f32 x) {
    if (!(x >= 0.0f))
        return -(s32) (0.5 - x);
    else
        return (s32) (0.5 + x);
  }

  const char * strrchr(const char * str, char c) {
    const char * lastOccurrence = nullptr;
    const char * currentPos = str;

    // Loop through the string and find the last occurrence of 'c'
    while ((currentPos = msl::string::strchr(currentPos, c)) != nullptr) {
      lastOccurrence = currentPos; // Store the last found position
      currentPos++; // Move to the next character to continue searching
    }

    return lastOccurrence; // Return the last occurrence (or NULL if not found)
  }

  int atoi(const char * str) {
    int result = 0;
    int sign = 1;
    int i = 0;

    // Handle negative numbers
    if (str[0] == '-') {
      sign = -1;
      i++; // Skip the negative sign
    }

    // Convert each digit to an integer
    for (; str[i] != '\0'; ++i) {
      if (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
      } else {
        // If a non-digit character is found, stop processing
        break;
      }
    }

    return result * sign;
  }

}
