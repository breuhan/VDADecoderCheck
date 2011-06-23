//
//  main.cpp
//  VDADecoderChecker
//
//  Created by Andy Breuhan on 23.06.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;
#include <CoreFoundation/CoreFoundation.h>
#include <CoreVideo/CoreVideo.h>
#include <VideoDecodeAcceleration/VDADecoder.h>
typedef void (*VDADecoderOutputCallback)(void             *decompressionOutputRefCon,
                                         CFDictionaryRef  frameInfo,
                                         OSStatus         status,
                                         uint32_t         infoFlags,
                                         CVImageBufferRef imageBuffer);


void myDecoderOutputCallback(void               *decompressionOutputRefCon,
                             CFDictionaryRef    frameInfo,
                             OSStatus           status, 
                             uint32_t           infoFlags,
                             CVImageBufferRef   imageBuffer);


int main (int argc, const char * argv[])
{

    
    ifstream::pos_type size;
    UInt8 *data;
    ifstream file ("avcC", ios::in|ios::binary|ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        data = new UInt8 [size];
        file.seekg (0, ios::beg);
        file.read ((char*) data, size);
        file.close();
        
        cout << "Read " << size << " bytes\n";
    }
    else cout << "Unable to open file";
    
    OSStatus status;
    OSType inSourceFormat;
    SInt32 inHeight,inWidth;
    CFDataRef inAVCCData;
    VDADecoder *decoderOut;
    
    inSourceFormat='avc1';
    inHeight = 1920;
    inWidth = 1056;
    
    CFMutableDictionaryRef decoderConfiguration = NULL;
    CFNumberRef height = NULL;
    CFNumberRef width= NULL;
    CFNumberRef sourceFormat = NULL;

    
    inAVCCData = CFDataCreate(kCFAllocatorDefault, (UInt8*) data, size);
    

    // create a CFDictionary describing the source material for decoder configuration
    decoderConfiguration = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                     4,
                                                     &kCFTypeDictionaryKeyCallBacks,
                                                     &kCFTypeDictionaryValueCallBacks);
    
    height = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &inHeight);
    width = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &inWidth);
    sourceFormat = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &inSourceFormat);
    

    CFDictionarySetValue(decoderConfiguration, kVDADecoderConfiguration_Height, height);
    CFDictionarySetValue(decoderConfiguration, kVDADecoderConfiguration_Width, width);
    CFDictionarySetValue(decoderConfiguration, kVDADecoderConfiguration_SourceFormat, sourceFormat);
    CFDictionarySetValue(decoderConfiguration, kVDADecoderConfiguration_avcCData, inAVCCData);


    
    status = VDADecoderCreate(decoderConfiguration,NULL, (VDADecoderOutputCallback*) myDecoderOutputCallback, NULL, decoderOut);
    if (kVDADecoderNoErr != status) {
        fprintf(stderr, "VDADecoderCreate failed. err: %d\n", status);
    }
    
    std::cout << status;
    
    if (decoderConfiguration) CFRelease(decoderConfiguration);

    return 0;
}

void myDecoderOutputCallback(void               *decompressionOutputRefCon,
                             CFDictionaryRef    frameInfo,
                             OSStatus           status, 
                             uint32_t           infoFlags,
                             CVImageBufferRef   imageBuffer){}




