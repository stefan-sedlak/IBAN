//
//  NSString+IBAN.h
//  IBAN
//
//  Created by Sedlak, Stefan on 11/26/17.
//

#import <Foundation/Foundation.h>

@interface NSString (IBAN)

- (BOOL)iban_isValid;
- (BOOL)iban_isValidSEPA;

- (NSArray<NSString *> *_Nullable)iban_list;
- (NSArray<NSString *> *_Nullable)iban_listSEPA;

@end
