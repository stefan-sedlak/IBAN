//
//  NSString+IBAN.m
//  IBAN
//
//  Created by Sedlak, Stefan on 11/26/17.
//

#import "NSString+IBAN.h"

#import "iban.h"

@implementation NSString (IBAN)

- (BOOL)isValidIBAN {
    const char *iban = [self UTF8String];
    bool is_valid = iban_is_valid(iban);
    return is_valid;
}

- (BOOL)isValidSEPAIBAN {
    const char *iban = [self UTF8String];
    bool is_valid = iban_is_valid(iban) && iban_is_sepa(iban);
    return is_valid;
}

@end
