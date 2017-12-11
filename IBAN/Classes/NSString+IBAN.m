//
//  NSString+IBAN.m
//  IBAN
//
//  Created by Sedlak, Stefan on 11/26/17.
//

#import "NSString+IBAN.h"

#import "iban.h"

@implementation NSString (IBAN)

- (BOOL)iban_isValid {
    const char *iban = [self UTF8String];
    bool is_valid = iban_is_valid(iban);
    return is_valid;
}

- (BOOL)iban_isValidSEPA {
    const char *iban = [self UTF8String];
    bool is_valid = iban_is_valid(iban) && iban_is_sepa(iban);
    return is_valid;
}

// canPerformSelector need to be called to expose method publicly
- (NSArray<NSString *> *)iban_listWithValidator:(SEL)validationSelector {
    NSMutableArray *result = [NSMutableArray new];

    NSMutableCharacterSet *separationCharacters = [NSMutableCharacterSet whitespaceAndNewlineCharacterSet];
    [separationCharacters formUnionWithCharacterSet:[NSCharacterSet punctuationCharacterSet]];

    NSArray<NSString *> *substrings = [self componentsSeparatedByCharactersInSet:[separationCharacters copy]];
    NSUInteger count = substrings.count;
    for (NSUInteger start = 0; start < count; start++) {
        NSUInteger candidateLength = 0;
        for (NSUInteger end = start; end < count; end++) {
            NSUInteger substringLength = substrings[end].length;
            if (substringLength == 0) {
                continue;
            }
            candidateLength += substringLength;
            if (candidateLength < IBAN_MIN_LENGTH) {
                continue;
            }
            if (candidateLength > IBAN_MAX_LENGTH) {
                break;
            }
            NSUInteger rangeLength = end - start + 1;
            NSString *candidate = [[substrings subarrayWithRange:NSMakeRange(start, rangeLength)] componentsJoinedByString:@""];

            BOOL (*validation_function)(id, SEL) = (void *)[candidate methodForSelector:validationSelector];
            BOOL isValid = validation_function(candidate, validationSelector);
            if (isValid) {
                [result addObject:candidate];
                start = end;
            }
        }
    }

    return result.count ? [result copy] : nil;
}

- (NSArray<NSString *> *)iban_list {
    NSArray *result = [self iban_listWithValidator:@selector(iban_isValid)];
    return result;
}

- (NSArray<NSString *> *)iban_listSEPA {
    NSArray *result = [self iban_listWithValidator:@selector(iban_isValidSEPA)];
    return result;
}


@end
