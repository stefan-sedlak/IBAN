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

- (void)testValid {
    NSArray<NSString *> *IBANs = @[
        @"DE89370400440532013000",
        @"  MT84 MALT 0110 0001 2345 MTLC AST0 01S",
        @"SK31 1200 0000 1987 4263 7541  "
    ];
    for (NSString *IBAN in IBANs) {
        NSLog(@"validate IBAN: %@", IBAN);
        BOOL isValidIBAN = [IBAN isValidIBAN];
        XCTAssertTrue(isValidIBAN);
    }
}

@end

