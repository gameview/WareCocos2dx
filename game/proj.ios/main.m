//
//  main.m
//  Game
//
//  Created by 黄 崇海 on 13-8-3.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#import <UIKit/UIKit.h>

int main(int argc, char *argv[]) {
 
    //bool flg=  IOSRunPayRequest(1);

    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, @"AppController");
    [pool release];
    return retVal;
}
