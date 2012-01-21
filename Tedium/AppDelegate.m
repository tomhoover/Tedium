//
//  AppDelegate.m
//  Tedium
//
//  Created by Dustin Rue on 1/16/12.
//  Copyright (c) 2012. All rights reserved.
//

#import "AppDelegate.h"
#import "AppDelegate+HelperTool.h"
#import "TediumHelpertoolCommon.h"


@implementation AppDelegate

@synthesize destinationsController;
@synthesize window = _window;
@synthesize currentDestination;
@synthesize destinations;
@synthesize activeSheet;



- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
    menuBarImage = [self prepareImageForMenubar:@"awesomeclock"];
    [self showInStatusBar:nil];
    [self setMenuBarImage:menuBarImage];
    

    
   	[GrowlApplicationBridge setGrowlDelegate: self];
    processInfo = [NSProcessInfo processInfo];
    [processInfo enableSuddenTermination];
    
    [self setDestinations:[[NSUserDefaults standardUserDefaults] objectForKey:@"destinations"]];
    [[NSNotificationCenter defaultCenter] 
                               addObserver:self 
                               selector:@selector(saveSettings) 
                               name:NSWindowWillCloseNotification
                               object:nil];

}

// Helper: Load a named image, and scale it to be suitable for menu bar use.
- (NSImage *)prepareImageForMenubar:(NSString *)name
{
	NSImage *img = [NSImage imageNamed:name];
	[img setScalesWhenResized:YES];
	[img setSize:NSMakeSize(16, 16)];
    
	return img;
}

- (void)setMenuBarImage:(NSImage *)imageName 
{
    
    // if the menu bar item has been hidden menuBarStatusItem will have been released
    // and we should not attempt to update the image
    if (!menuBarStatusItem)
        return;
    

    [menuBarStatusItem setImage:imageName];

}

- (void)showInStatusBar:(id)sender
{
	if (menuBarStatusItem) {
		// Already there? Rebuild it anyway.
		[[NSStatusBar systemStatusBar] removeStatusItem:menuBarStatusItem];
	}
    
	menuBarStatusItem = [[NSStatusBar systemStatusBar] statusItemWithLength:NSVariableStatusItemLength];
	[menuBarStatusItem setHighlightMode:YES];
    

	
	[menuBarStatusItem setMenu:menuBarMenu];
}

- (void) growlMessage:(NSString *)title message:(NSString *)message 
{
    const int pri = 0;
    NSLog(@"growl");
    
    [GrowlApplicationBridge notifyWithTitle:title
								description:message
						   notificationName:@"TediumGrowl"
								   iconData:nil
								   priority:pri
								   isSticky:NO
							   clickContext:nil];
}



- (void)saveSettings {
    NSLog(@"saving settings");
    [[NSUserDefaults standardUserDefaults] setObject:[self destinations] forKey:@"destinations"];
   	[[NSUserDefaults standardUserDefaults] synchronize];
}

- (void)addNewDestination:(NSString *)newDestination {

    // test to see if the destination is already inserted
    // there has to be a more efficient way of handling this...
    for (NSDictionary * aDictionary in [self destinations] )
    {
        NSString *aDestination = [aDictionary objectForKey:@"destination"];
        if ([aDestination isEqualToString:newDestination])
            return;
    }
    NSMutableArray *tmp = [[NSMutableArray alloc] init];
    if ([self destinations])
        tmp = [[self destinations] mutableCopy];
    

    [tmp addObject:[NSDictionary dictionaryWithObject:newDestination forKey:@"destination"]];
    
    [self setDestinations:tmp];
    NSLog(@"%@", [self destinations]);
    [self saveSettings];
}

- (void) setCurrentDestination:(NSString *)newVal {
    currentDestination = newVal;
    [self growlMessage:@"Updating Destination" message:[NSString stringWithFormat:@"Changing Time Machine destination to %@", newVal]];
    
    [self addNewDestination:newVal];
}

#pragma mark Growl Delegates

- (void) growlIsReady
{
    NSLog(@"growl reports it is ready");
}


- (void) growlNotificationWasClicked:(id)clickContext
{
    NSLog(@"growl reports the notification was clicked");
}

- (void) growlNotificationTimedOut:(id)clickContext
{
    NSLog(@"growl reports the notification timed out");
}

- (NSDictionary *) registrationDictionaryForGrowl 
{
    NSDictionary *tmp = [NSDictionary dictionaryWithObjectsAndKeys:
                         [NSNumber numberWithInt:1],
                         @"TicketVersion",
                         [NSArray arrayWithObject:@"TediumGrowl"], 
                          @"AllNotifications",
                         [NSArray arrayWithObject:@"TediumGrowl"], 
                          @"DefaultNotifications",
                         nil];
    return tmp;
}

#pragma mark GUI Routines
- (IBAction)openPreferences:(id)sender 
{
    [NSApp activateIgnoringOtherApps:YES];
    [prefsWindow makeKeyAndOrderFront:self];
}

- (IBAction)addExternalDrive:(id)sender 
{
    [self setActiveSheet:addExternalDriveSheet];
    [NSApp beginSheet:addExternalDriveSheet
	   modalForWindow:prefsWindow
	    modalDelegate:self
	   didEndSelector:@selector(addExternalDriveSheetDidEnd:returnCode:contextInfo:)
	      contextInfo:nil];
}

- (IBAction)addNetworkShare:(id)sender 
{
    [self setActiveSheet:addNetworkShareSheet];
    [NSApp beginSheet:addNetworkShareSheet
	   modalForWindow:prefsWindow
	    modalDelegate:self
	   didEndSelector:@selector(addExternalDriveSheetDidEnd:returnCode:contextInfo:)
	      contextInfo:nil];
}

- (IBAction)addCurrentDrive:(id)sender 
{


    NSDictionary *tmp = [NSDictionary dictionaryWithContentsOfFile:@"/Library/Preferences/com.apple.TimeMachine.plist"];

    [self addNewDestination:[NSString stringWithFormat:@"/Volumes/%@",[tmp valueForKey:@"LocalizedDiskImageVolumeName"]]];
}

- (IBAction)closeSheetWithOK:(id)sender
{
	[NSApp endSheet:[self activeSheet] returnCode:NSOKButton];
	[[self activeSheet] orderOut:nil];
    [self setActiveSheet:nil];
}

- (IBAction)closeSheetWithCancel:(id)sender
{
	[NSApp endSheet:[self activeSheet] returnCode:NSCancelButton];
	[[self activeSheet] orderOut:nil];
    [self setActiveSheet:nil];
}


- (IBAction)closePreferences:(id)sender {
    [prefsWindow close];
}

- (IBAction)applyNewDestination:(id)sender {
    [prefsWindow close];
    
    NSDictionary *newDestination = [[self destinations] objectAtIndex:[[self destinationsController] selectionIndex]]; 
    
    NSLog(@"new destination will be set to %@", [newDestination valueForKey:@"destination"]);

    
    NSString *command = @kTediumHelperToolSetDestinationCommand;
    if([self helperToolPerformAction: command])
        [self growlMessage:@"Failure" message:@"Failed to set new destination"];
}

- (void)addExternalDriveSheetDidEnd:(NSWindow *)sheet returnCode:(int)returnCode contextInfo:(void *)contextInfo
{
	if (returnCode != NSOKButton)
		return;
    
	NSLog(@"got new data!");
}

@end
