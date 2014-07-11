//
//  StoreManager.h
//  MKSync
//
//  Created by Mugunth Kumar on 17-Oct-09.
//  Copyright 2009 MK Inc. All rights reserved.
//  mugunthkumar.com

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
#import "MKStoreObserver.h"
#include "ClientGlobal-ios.h"


@interface MKStoreManager : NSObject<SKProductsRequestDelegate> {

	NSMutableArray *purchasableObjects;
	MKStoreObserver *storeObserver;	

    struct IPayDelegate* delegate;
}

@property struct IPayDelegate* delegate;
@property (nonatomic, retain) NSMutableArray *purchasableObjects;
@property (nonatomic, retain) MKStoreObserver *storeObserver;

- (void) requestProductData;

- (void) ReqbuyFeature:(int)Rid; // expose product buying functions, do not expose

// do not call this directly. This is like a private method
- (void) buyFeature:(NSString*) featureId;

-(void)paymentCanceled;

- (void) failedTransaction: (SKPaymentTransaction *)transaction;
-(void) provideContent: (SKPaymentTransaction *)transaction;

+ (MKStoreManager*)sharedManager;

+ (BOOL) featurePurchased;

+(void) loadPurchases;
+(void) updatePurchases;

@end
