//
//	HelperTool.c
//	Tedium
//
//	Created by Dustin Rue on 1/16/2012.
//	Copyright 2012. All rights reserved.
//

#import <netinet/in.h>
#import <sys/socket.h>
#import <stdio.h>
#import <unistd.h>
#import <CoreServices/CoreServices.h>
#import <syslog.h>

#import "AuthorizationLib/BetterAuthorizationSampleLib.h"
#import "TediumHelpertoolCommon.h"

extern const BASCommandSpec kTediumHelperToolCommandSet[];


// Implements the GetVersionCommand. Returns the version number of the helper tool.
static OSStatus DoGetVersion(AuthorizationRef			auth,
							 const void *				userData,
							 CFDictionaryRef			request,
							 CFMutableDictionaryRef		response,
							 aslclient					asl,
							 aslmsg						aslMsg) {
	
	OSStatus retval = noErr;
	CFNumberRef value;
	static const unsigned int kCurrentVersion = kTediumHelperToolVersionNumber;
	
	assert(auth     != NULL);
	assert(request  != NULL);
	assert(response != NULL);
	
    syslog(LOG_DEBUG,"Getting Helper Tool Version");
	// Add to the response.
	value = CFNumberCreate(NULL, kCFNumberIntType, &kCurrentVersion);
	if (!value)
		retval = coreFoundationUnknownErr;
	else {
		CFDictionaryAddValue(response, CFSTR(kTediumHelperToolGetVersionResponse), value);
		CFRelease(value);
	}
	
	return retval;
}


// Implements the SetDestination. Returns the version number of the helper tool.
static OSStatus DoSetAFPDestination(AuthorizationRef		auth,
                                 const void *				userData,
                                 CFDictionaryRef			request,
                                 CFMutableDictionaryRef		response,
                                 aslclient					asl,
                                 aslmsg						aslMsg) {
	
	OSStatus retval = noErr;
	
	
	
	assert(auth     != NULL);
	assert(request  != NULL);
	assert(response != NULL);
    
    CFDictionaryRef parameters = (CFDictionaryRef) CFDictionaryGetValue(request, CFSTR("param"));
    CFStringRef uname     = (CFStringRef) CFDictionaryGetValue(parameters, CFSTR("username"));
    CFStringRef pwd       = (CFStringRef) CFDictionaryGetValue(parameters, CFSTR("password"));
    CFStringRef host      = (CFStringRef) CFDictionaryGetValue(parameters, CFSTR("hostname"));
    CFStringRef url       = (CFStringRef) CFDictionaryGetValue(parameters, CFSTR("url"));
    

    if (uname == NULL) 
        syslog(LOG_LOCAL0, "uname is null");
    
    if (pwd == NULL)
        syslog(LOG_LOCAL0, "pwd is null");
    
    if (!host)
        syslog(LOG_LOCAL0, "host is null");
    
    if (!url)
        syslog(LOG_LOCAL0, "url is null");
    
    char command[128];
    char hostname[128];
    char username[128];
    char password[128];
    char theUrl[128];
    syslog(LOG_LOCAL0, "starting the thing");
    Boolean success;

    success = CFStringGetCString(uname, username, 128, kCFStringEncodingUTF8);
    
    if (!success) {
        syslog(LOG_EMERG, "big fail");
    }
    success = CFStringGetCString(pwd, password, 1024, kCFStringEncodingUTF8);
    
    if (!success) {
        syslog(LOG_EMERG, "password failed");
    }
    
    success = CFStringGetCString(host, hostname, 1024, kCFStringEncodingUTF8);
    
    if (!success)
        syslog(LOG_EMERG, "hostname failed");
    
    success = CFStringGetCString(url, theUrl, 1024, kCFStringEncodingUTF8);
    
    if (!success)
        syslog(LOG_EMERG, "url failed");
            
    sprintf(command, "/usr/bin/tmutil setdestination afp://\"%s\":\"%s\"@%s%s", username, password, hostname, theUrl);

    retval = system(command);
	return retval;
    
}

// Implements the SetDestination. Returns the version number of the helper tool.
static OSStatus DoSetDestination(AuthorizationRef		auth,
                                    const void *				userData,
                                    CFDictionaryRef			request,
                                    CFMutableDictionaryRef		response,
                                    aslclient					asl,
                                    aslmsg						aslMsg) {
	
	OSStatus retval = noErr;
	
	
	
	assert(auth     != NULL);
	assert(request  != NULL);
	assert(response != NULL);
    
    CFDictionaryRef parameters = (CFDictionaryRef) CFDictionaryGetValue(request, CFSTR("param"));
    CFStringRef uname     = (CFStringRef) CFDictionaryGetValue(parameters, CFSTR("username"));
    CFStringRef pwd       = (CFStringRef) CFDictionaryGetValue(parameters, CFSTR("password"));
    CFStringRef host      = (CFStringRef) CFDictionaryGetValue(parameters, CFSTR("hostname"));
    CFStringRef url       = (CFStringRef) CFDictionaryGetValue(parameters, CFSTR("url"));
    
    
    if (uname == NULL) 
        syslog(LOG_LOCAL0, "uname is null");
    
    if (pwd == NULL)
        syslog(LOG_LOCAL0, "pwd is null");
    
    if (!host)
        syslog(LOG_LOCAL0, "host is null");
    
    if (!url)
        syslog(LOG_LOCAL0, "url is null");
    
    char command[128];
    char hostname[128];
    char username[128];
    char password[128];
    char theUrl[128];
    syslog(LOG_LOCAL0, "starting the thing");
    Boolean success;
    
    success = CFStringGetCString(uname, username, 128, kCFStringEncodingUTF8);
    
    if (!success) {
        syslog(LOG_EMERG, "big fail");
    }
    success = CFStringGetCString(pwd, password, 1024, kCFStringEncodingUTF8);
    
    if (!success) {
        syslog(LOG_EMERG, "password failed");
    }
    
    success = CFStringGetCString(host, hostname, 1024, kCFStringEncodingUTF8);
    
    if (!success)
        syslog(LOG_EMERG, "hostname failed");
    
    success = CFStringGetCString(url, theUrl, 1024, kCFStringEncodingUTF8);
    
    if (!success)
        syslog(LOG_EMERG, "url failed");
    
    sprintf(command, "/usr/bin/tmutil setdestination afp://\"%s\":\"%s\"@%s%s", username, password, hostname, theUrl);
    
    retval = system(command);
	return retval;
    
}

// Implements the SetMobileBackup. Returns the version number of the helper tool.
static OSStatus DoSetMobileBackup(AuthorizationRef			auth,
                                 const void *				userData,
                                 CFDictionaryRef			request,
                                 CFMutableDictionaryRef		response,
                                 aslclient					asl,
                                 aslmsg						aslMsg) {
	
	OSStatus retval = noErr;
	int value;
	
	
	assert(auth     != NULL);
	assert(request  != NULL);
	assert(response != NULL);

    CFNumberRef parameter = (CFNumberRef) CFDictionaryGetValue(request, CFSTR("param"));
    
    char command[256];
    
    CFNumberGetValue(parameter, kCFNumberSInt32Type, &value);
    
    
    if (value) 
        sprintf(command, "/usr/bin/tmutil enablelocal");
    else
        sprintf(command, "/usr/bin/tmutil disablelocal");
    
    retval = system(command);
    //syslog(LOG_EMERG, "command finished");
	return retval;
    
}

// Implements the SetMobileBackup. Returns the version number of the helper tool.
static OSStatus DoMobileBackupNow(AuthorizationRef			auth,
                                  const void *				userData,
                                  CFDictionaryRef			request,
                                  CFMutableDictionaryRef	response,
                                  aslclient					asl,
                                  aslmsg					aslMsg) {
	
	OSStatus retval = noErr;
	
	assert(auth     != NULL);
	assert(request  != NULL);
	assert(response != NULL);
    
    
    char command[256];
    
    sprintf(command, "/usr/bin/tmutil snapshot");
    
    retval = system(command);
    //syslog(LOG_EMERG, "command finished");
	return retval;
    
}

// Implements the SetMobileBackup. Returns the version number of the helper tool.
static OSStatus DoBackupNow(AuthorizationRef                auth,
                                  const void *				userData,
                                  CFDictionaryRef			request,
                                  CFMutableDictionaryRef	response,
                                  aslclient					asl,
                                  aslmsg					aslMsg) {
	
	OSStatus retval = noErr;


	assert(auth     != NULL);
	assert(request  != NULL);
	assert(response != NULL);
    
    char command[256];
    
    

    sprintf(command, "/usr/bin/tmutil startbackup");

    
    retval = system(command);
    //syslog(LOG_EMERG, "command finished");
	return retval;
    
}

#pragma mark -
#pragma mark Tool Infrastructure

// the list defined here must match (same order) the list in CPHelperToolCommon.c
static const BASCommandProc kHelperToolCommandProcs[] = {
	DoGetVersion,
    DoSetAFPDestination,
    DoSetDestination,
    DoSetMobileBackup,
    DoMobileBackupNow,
    DoBackupNow,
	NULL
};

int main(int argc, char **argv) {
	// Go directly into BetterAuthorizationSampleLib code.
	
	// IMPORTANT
	// BASHelperToolMain doesn't clean up after itself, so once it returns 
	// we must quit.
	
	return BASHelperToolMain(kTediumHelperToolCommandSet, kHelperToolCommandProcs);
}
