//
//  IBANTests.m
//  IBANTests
//
//  Created by Stefan Sedlak on 11/26/2017.
//  Copyright (c) 2017 Stefan Sedlak. All rights reserved.
//

@import XCTest;

#include <IBAN/NSString+IBAN.h>

@interface Tests : XCTestCase
@end

@implementation Tests

- (void)testIsValid {
    NSDictionary<NSString *, NSNumber *> *IBANs = @{
        @"DE89370400440532013000" : @(YES),
        @"  MT84 MALT 0110 0001 2345 MTLC AST0 01S" : @(YES),
        @"SK31 1200 0000 1987 4263 7541  " : @(YES),
        @"MIN_LENGTH" : @(NO),
        @"MAX_LENGTH_MAX_LENGTH_MAX_LENGTH_": @(NO),
        @"INVALID_COUNTRY_CODE": @(NO),
        @"DE85370400440532013000": @(NO) /* invalid checksum */
    };
    [IBANs enumerateKeysAndObjectsUsingBlock:^(NSString *IBAN, NSNumber *expectedResultObject, BOOL* stop) {
        BOOL expectedResult = [expectedResultObject boolValue];
        BOOL result = [IBAN iban_isValid];
        XCTAssertEqual(result, expectedResult, @"%@", IBAN);
    }];
}

- (void)testIsValidSEPA {
    NSDictionary<NSString *, NSNumber *> *IBANs = @{
        @"DE89370400440532013000" : @(YES),
        @"SK31 1200 0000 1987 4263 7541  " : @(YES),
        @"  MT84 MALT 0110 0001 2345 MTLC AST0 01S" : @(YES),
        @"SA03 8000 0000 6080 1016 7519": @(NO)
    };
    [IBANs enumerateKeysAndObjectsUsingBlock:^(NSString *IBAN, NSNumber *expectedResultObject, BOOL* stop) {
        BOOL expectedResult = [expectedResultObject boolValue];
        BOOL result = [IBAN iban_isValidSEPA];
        XCTAssertEqual(result, expectedResult, @"%@", IBAN);
    }];
}

- (void)testListSEPA {
    NSDictionary<NSString *, NSArray<NSString *> *> *tests = @{
        @"DE89370400440532013000DE89370400440532013000": (NSArray<NSString *> *)[NSNull null], /* nil can not be stored*/
        @"DE89370400440532013000" : @[@"DE89370400440532013000"],
        @"DE89 3704 0044 0532 0130 00" : @[@"DE89370400440532013000"],
        @"IBAN: DE89370400440532013000" : @[@"DE89370400440532013000"],
        @"IBAN: DE89 3704 0044 0532 0130 00" : @[@"DE89370400440532013000"],
        @"IBAN: DE89370400440532013000 Amount" : @[@"DE89370400440532013000"],
        @"IBAN: DE89 3704 0044 0532 0130 00 Amount" : @[@"DE89370400440532013000"],
        @"DE89370400440532013000 Amount" : @[@"DE89370400440532013000"],
        @"DE89 3704 0044 0532 0130 00 Amount" : @[@"DE89370400440532013000"],

        @"DE89370400440532013000 SK3112000000198742637541" : @[@"DE89370400440532013000", @"SK3112000000198742637541"],
        @"DE89370400440532013000, SK3112000000198742637541" : @[@"DE89370400440532013000", @"SK3112000000198742637541"],
        @"IBAN: DE89370400440532013000 IBAN: SK3112000000198742637541" : @[@"DE89370400440532013000", @"SK3112000000198742637541"],
        @"IBAN: DE89370400440532013000, IBAN: SK3112000000198742637541" : @[@"DE89370400440532013000", @"SK3112000000198742637541"],
        @"IBAN: DE89370400440532013000, IBAN: SK3112000000198742637541, IBAN: INVALID_IBAN" : @[@"DE89370400440532013000", @"SK3112000000198742637541"],

        @"DE89370400440532013000 SA0380000000608010167519" : @[@"DE89370400440532013000"],
        @"DE89370400440532013000, SA03 8000 0000 6080 1016 7519" : @[@"DE89370400440532013000"],
        @"IBAN: DE89370400440532013000 IBAN: SA0380000000608010167519" : @[@"DE89370400440532013000"],
        @"IBAN: DE89370400440532013000, IBAN: SA03 8000 0000 6080 1016 7519" : @[@"DE89370400440532013000"]
    };
    [tests enumerateKeysAndObjectsUsingBlock:^(NSString *text, NSArray<NSString *> *expectedList, BOOL* stop) {
        if ([expectedList isKindOfClass:[NSNull class]]) {
            expectedList = nil;
        }
        NSArray<NSString *> *list = [text iban_listSEPA];
        XCTAssertEqualObjects(list, expectedList, @"text: %@", text);
    }];
}
@end

