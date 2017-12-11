/*
 * Simplified BSD License
 *
 * Copyright (c) 2017, Stefan Sedlak <stefan@sedlak.eu>
 * All rights reserved.
 *
 * Redistribution and  use in  source and binary  forms, with  or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * o Redistributions  of  source  code must  retain  the  above copyright
 *   notice, this list of conditions and the following disclaimer.
 * o Redistributions  in  binary form  must reproduce the above copyright
 *   notice, this list of conditions and  the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE  IS PROVIDED BY  THE COPYRIGHT HOLDERS  AND CONTRIBUTORS
 * "AS  IS" AND  ANY EXPRESS  OR IMPLIED  WARRANTIES, INCLUDING,  BUT NOT
 * LIMITED TO, THE IMPLIED  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE  ARE DISCLAIMED. IN NO EVENT  SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL,  EXEMPLARY,  OR  CONSEQUENTIAL  DAMAGES (INCLUDING,  BUT  NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS  INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF  LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY,  OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING  IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bignum/bigz.h"

typedef struct iban_info {
    char country_code[2];
    uint8_t length;
    uint8_t is_sepa;
} iban_info;

static const size_t IBAN_INFOS_COUNT = 93;
static const iban_info IBAN_INFOS[] = {
    {.country_code = {'A', 'D'}, .length = 24, .is_sepa = false},
    {.country_code = {'A', 'E'}, .length = 23, .is_sepa = false},
    {.country_code = {'A', 'L'}, .length = 28, .is_sepa = false},
    {.country_code = {'A', 'O'}, .length = 25, .is_sepa = false},
    {.country_code = {'A', 'T'}, .length = 20, .is_sepa = true },
    {.country_code = {'A', 'Z'}, .length = 28, .is_sepa = false},
    {.country_code = {'B', 'A'}, .length = 20, .is_sepa = false},
    {.country_code = {'B', 'E'}, .length = 16, .is_sepa = true },
    {.country_code = {'B', 'F'}, .length = 27, .is_sepa = false},
    {.country_code = {'B', 'G'}, .length = 22, .is_sepa = true },
    {.country_code = {'B', 'H'}, .length = 22, .is_sepa = false},
    {.country_code = {'B', 'I'}, .length = 16, .is_sepa = false},
    {.country_code = {'B', 'J'}, .length = 28, .is_sepa = false},
    {.country_code = {'B', 'R'}, .length = 29, .is_sepa = false},
    {.country_code = {'B', 'Y'}, .length = 28, .is_sepa = false},
    {.country_code = {'C', 'G'}, .length = 27, .is_sepa = false},
    {.country_code = {'C', 'H'}, .length = 21, .is_sepa = true },
    {.country_code = {'C', 'I'}, .length = 28, .is_sepa = false},
    {.country_code = {'C', 'M'}, .length = 27, .is_sepa = false},
    {.country_code = {'C', 'R'}, .length = 22, .is_sepa = false},
    {.country_code = {'C', 'V'}, .length = 25, .is_sepa = false},
    {.country_code = {'C', 'Y'}, .length = 28, .is_sepa = true },
    {.country_code = {'C', 'Z'}, .length = 24, .is_sepa = true },
    {.country_code = {'D', 'E'}, .length = 22, .is_sepa = true },
    {.country_code = {'D', 'K'}, .length = 18, .is_sepa = true },
    {.country_code = {'D', 'O'}, .length = 28, .is_sepa = false},
    {.country_code = {'D', 'Z'}, .length = 24, .is_sepa = false},
    {.country_code = {'E', 'E'}, .length = 20, .is_sepa = true },
    {.country_code = {'E', 'G'}, .length = 27, .is_sepa = false},
    {.country_code = {'E', 'S'}, .length = 24, .is_sepa = true },
    {.country_code = {'F', 'I'}, .length = 18, .is_sepa = true },
    {.country_code = {'F', 'O'}, .length = 18, .is_sepa = false},
    {.country_code = {'F', 'R'}, .length = 27, .is_sepa = true },
    {.country_code = {'G', 'A'}, .length = 27, .is_sepa = false},
    {.country_code = {'G', 'B'}, .length = 22, .is_sepa = true },
    {.country_code = {'G', 'E'}, .length = 22, .is_sepa = false},
    {.country_code = {'G', 'I'}, .length = 23, .is_sepa = false},
    {.country_code = {'G', 'L'}, .length = 18, .is_sepa = false},
    {.country_code = {'G', 'R'}, .length = 27, .is_sepa = true },
    {.country_code = {'G', 'T'}, .length = 28, .is_sepa = false},
    {.country_code = {'H', 'N'}, .length = 28, .is_sepa = false},
    {.country_code = {'H', 'R'}, .length = 21, .is_sepa = true },
    {.country_code = {'H', 'U'}, .length = 28, .is_sepa = true },
    {.country_code = {'I', 'E'}, .length = 22, .is_sepa = true },
    {.country_code = {'I', 'L'}, .length = 23, .is_sepa = false},
    {.country_code = {'I', 'R'}, .length = 26, .is_sepa = false},
    {.country_code = {'I', 'S'}, .length = 26, .is_sepa = true },
    {.country_code = {'I', 'T'}, .length = 27, .is_sepa = true },
    {.country_code = {'J', 'O'}, .length = 30, .is_sepa = false},
    {.country_code = {'K', 'M'}, .length = 27, .is_sepa = false},
    {.country_code = {'K', 'W'}, .length = 30, .is_sepa = false},
    {.country_code = {'K', 'Z'}, .length = 20, .is_sepa = false},
    {.country_code = {'L', 'B'}, .length = 28, .is_sepa = false},
    {.country_code = {'L', 'I'}, .length = 21, .is_sepa = true },
    {.country_code = {'L', 'T'}, .length = 20, .is_sepa = true },
    {.country_code = {'L', 'U'}, .length = 20, .is_sepa = true },
    {.country_code = {'L', 'V'}, .length = 21, .is_sepa = true },
    {.country_code = {'M', 'A'}, .length = 28, .is_sepa = false},
    {.country_code = {'M', 'C'}, .length = 27, .is_sepa = true },
    {.country_code = {'M', 'D'}, .length = 24, .is_sepa = false},
    {.country_code = {'M', 'E'}, .length = 22, .is_sepa = true },
    {.country_code = {'M', 'G'}, .length = 27, .is_sepa = false},
    {.country_code = {'M', 'K'}, .length = 19, .is_sepa = false},
    {.country_code = {'M', 'L'}, .length = 28, .is_sepa = false},
    {.country_code = {'M', 'R'}, .length = 27, .is_sepa = false},
    {.country_code = {'M', 'T'}, .length = 31, .is_sepa = true },
    {.country_code = {'M', 'U'}, .length = 30, .is_sepa = false},
    {.country_code = {'M', 'Z'}, .length = 25, .is_sepa = false},
    {.country_code = {'N', 'E'}, .length = 28, .is_sepa = false},
    {.country_code = {'N', 'I'}, .length = 32, .is_sepa = false},
    {.country_code = {'N', 'L'}, .length = 18, .is_sepa = false},
    {.country_code = {'N', 'O'}, .length = 15, .is_sepa = true },
    {.country_code = {'P', 'K'}, .length = 24, .is_sepa = false},
    {.country_code = {'P', 'L'}, .length = 28, .is_sepa = true },
    {.country_code = {'P', 'S'}, .length = 29, .is_sepa = false},
    {.country_code = {'P', 'T'}, .length = 25, .is_sepa = true },
    {.country_code = {'Q', 'A'}, .length = 29, .is_sepa = false},
    {.country_code = {'R', 'O'}, .length = 24, .is_sepa = true },
    {.country_code = {'R', 'S'}, .length = 22, .is_sepa = false},
    {.country_code = {'S', 'A'}, .length = 24, .is_sepa = false},
    {.country_code = {'S', 'E'}, .length = 24, .is_sepa = true },
    {.country_code = {'S', 'I'}, .length = 19, .is_sepa = true },
    {.country_code = {'S', 'K'}, .length = 24, .is_sepa = true },
    {.country_code = {'S', 'M'}, .length = 27, .is_sepa = true },
    {.country_code = {'S', 'N'}, .length = 28, .is_sepa = false},
    {.country_code = {'T', 'D'}, .length = 27, .is_sepa = false},
    {.country_code = {'T', 'G'}, .length = 28, .is_sepa = false},
    {.country_code = {'T', 'L'}, .length = 23, .is_sepa = false},
    {.country_code = {'T', 'N'}, .length = 24, .is_sepa = false},
    {.country_code = {'T', 'R'}, .length = 26, .is_sepa = false},
    {.country_code = {'U', 'A'}, .length = 29, .is_sepa = false},
    {.country_code = {'V', 'G'}, .length = 24, .is_sepa = false},
    {.country_code = {'X', 'K'}, .length = 20, .is_sepa = false}
};

char *str_trim_spaces(const char *str) {
    if (!str) {
        return 0;
    }

    char *s = strdup(str);
    char *l = s, *r = s;
    while (*r) {
        char c = *r;
        if (!isspace(c)) {
            *l++ = *r;
        }
        r++;
    }
    *l = '\0';

    return s;
}

BigZ BzAddIbanChar(const BigZ a, char c) {
    BigZ result = BZNULL;
    if (!a) {
        return result;
    }

    if (isdigit(c)) {
        BigZ bz10, t1, bzc;

        bz10 = BzFromUnsignedInteger(10);
        t1 = BzMultiply(a, bz10);

        c = c - '0';
        bzc = BzFromInteger(c);
        result = BzAdd(t1, bzc);

        BzFree(bz10);
        BzFree(t1);
        BzFree(bzc);

    } else if (isupper(c)) {
        BigZ b10, t1, t2, t3, bz1, bz2;

        b10 = BzFromUnsignedInteger(10);

        c = c - 'A' + 10;
        BzInt first = c / 10;
        bz1 = BzFromInteger(first);

        BzInt second = c % 10;
        bz2 = BzFromInteger(second);

        t1 = BzMultiply(a, b10);
        t2 = BzAdd(t1, bz1);

        t3 = BzMultiply(t2, b10);
        result = BzAdd(t3, bz2);

        BzFree(b10);
        BzFree(t1);
        BzFree(t2);
        BzFree(t3);
        BzFree(bz1);
        BzFree(bz2);
    }

    return result;
}

BigZ BzFromIbanString(const char *iban) {
    if (!iban) {
        return BZNULL;
    }

    unsigned long iban_length = strlen(iban);
    if (iban_length < 15) {
        return BZNULL;
    }

    BigZ result = BzFromUnsignedInteger(0);

    const char *p = iban + 4;
    while (*p) {
        BigZ t = BzAddIbanChar(result, *p);
        BzFree(result);
        if (!t) {
           return BZNULL;
        }
        result = t;
        p++;
    }

    p = iban;
    for (int i = 0; i < 4; ++i, ++p) {
        BigZ t = BzAddIbanChar(result, *p);
        BzFree(result);
        if (!t) {
           return BZNULL;
        }
        result = t;
    }
    return result;
}

int iban_compare_country_code(const void *str, const void *info) {
    int result = strncmp((const char *)str, ((const iban_info *)info)->country_code, 2);
    return result;
}

const iban_info *iban_find_info(const char *str) {
    if (!str) {
        return 0;
    }
    if (str[0] == '\0' || str[1] == '\0') { // if length < 2
        return 0;
    }
    const iban_info *info = bsearch(str, IBAN_INFOS, IBAN_INFOS_COUNT, sizeof(iban_info), iban_compare_country_code);
    return info;
}

bool iban_is_valid(const char *str) {
    if (!str) {
        return false;
    }

    char *iban = str_trim_spaces(str);
    const iban_info *info = iban_find_info(iban);
    if (!info) { // ivalid country code
        free(iban); iban = 0;
        return false;
    }
    unsigned long iban_length = strlen(iban);
    if (iban_length != info->length) { // invalid length
        free(iban); iban = 0;
        return false;
    }

    BigZ iban_number = BzFromIbanString(iban);
    if (!iban_number) {
        return false;
    }
    free(iban); iban = 0;

    BigZ b97 = BzFromUnsignedInteger(97);
    BigZ reminder = BzMod(iban_number, b97);
    BzFree(b97);
    BzFree(iban_number);

    BigZ b1 = BzFromUnsignedInteger(1);
    bool is_valid_checksum = BzCompare(reminder, b1) == BZ_EQ; // iban is valid when (iban_number % 97 == 1)
    BzFree(b1);
    BzFree(reminder);

    return is_valid_checksum;
}

bool iban_is_sepa(const char *str) {
    if (!str) {
        return false;
    }

    char country_code[2];
    int i = 0;
    const char *p = str;
    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }
        if (!isupper(*p)) {
            return false;
        }
        country_code[i++] = *p++;
        if (i == 2) {
            break;
        }
    }
    if (i < 2) {
        return false;
    }

    const iban_info *info = iban_find_info(country_code);
    if (!info) { // ivalid country code
        return false;
    }

    bool is_sepa = info->is_sepa;
    return is_sepa;
}
