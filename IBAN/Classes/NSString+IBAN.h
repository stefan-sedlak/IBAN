//
//  NSString+IBAN.h
//  IBAN
//
//  Created by Sedlak, Stefan on 11/26/17.
//

#import <Foundation/Foundation.h>

@interface NSString (IBAN)

- (BOOL)isValidIBAN;
- (BOOL)isValidSEPAIBAN;

@end
