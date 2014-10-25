//
//  vdachecker.cpp
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

//This array of "avcC Data" comes directly from an sample MP4 File.
const UInt8 avcC[]        = {   0x00, 0x00, 0x00, 0x31, 0x61, 0x76, 0x63, 0x43, 0x01, 0x4D,
                                0x40, 0x29, 0xFF, 0xE1, 0x00, 0x1A, 0x67, 0x4D, 0x40, 0x29,
                                0x9A, 0x72, 0x80, 0xF0, 0x04, 0x2D, 0x80, 0x88, 0x00, 0x00,
                                0x03, 0x00, 0x08, 0x00, 0x00, 0x03, 0x01, 0x94, 0x78, 0xC1,
                                0x88, 0xB0, 0x01, 0x00, 0x04, 0x68, 0xEE, 0xBC, 0x80        };

//This array of "avcC Data" is parsed and optimized by Subler, it comes from the same MP4 File.
const UInt8 avcC_subler[] = {   0x01, 0x4d, 0x40, 0x29, 0x03, 0x01, 0x00, 0x1a, 0x67, 0x4d,
                                0x40, 0x29, 0x9a, 0x72, 0x80, 0xf0, 0x04, 0x2d, 0x80, 0x88,
                                0x00, 0x00, 0x03, 0x00, 0x08, 0x00, 0x00, 0x03, 0x01, 0x94,
                                0x78, 0xc1, 0x88, 0xb0, 0x01, 0x00, 0x04, 0x68, 0xee, 0xbc,
                                0x80                                                        };

//Needed for a valid configuration
void myDecoderOutputCallback();
void myDecoderOutputCallback(){}

int main (int argc, const char * argv[])
{
    
    OSStatus status;
    CFDataRef inAVCCData;
    VDADecoder decoderOut = NULL;
    
    OSType inSourceFormat='avc1';
    int inHeight = 1920;
    int inWidth = 1080;
    
    CFMutableDictionaryRef decoderConfiguration = NULL;
    CFNumberRef height = NULL;
    CFNumberRef width= NULL;
    CFNumberRef sourceFormat = NULL;

    
    inAVCCData = CFDataCreate(kCFAllocatorDefault, avcC_subler, sizeof(avcC_subler)*sizeof(UInt8));
    

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


    
    status = VDADecoderCreate(decoderConfiguration,NULL, (VDADecoderOutputCallback*) myDecoderOutputCallback, NULL, &decoderOut);
    switch (status) {
        case kVDADecoderNoErr:
            std::cout << "Hardware acceleration is fully supported" << "\n";
            break;
        case kVDADecoderHardwareNotSupportedErr:
            std::cout << "The hardware does not support accelerated video services required for hardware decode." << "\n";
            break;
        case kVDADecoderConfigurationError:
            std::cout << "Invalid or unsupported configuration parameters were specified in VDADecoderCreate." << "\n";
            break;
        case kVDADecoderDecoderFailedErr:
            std::cout << "An error was returned by the decoder layer. This may happen for example because of bitstream/data errors during a decode operation. This error may also be returned from VDADecoderCreate when hardware decoder resources are available on the system but currently in use by another process." << "\n";
            break;
        case -50:
            std::cout << "Parameter error" << "\n";
            break;
            
        default:
            std::cout << "Unknown Status: " << status << "\n";
            break;
    }
    
    if(kVDADecoderNoErr != status) fprintf(stderr, "VDADecoderCreate failed. err: %d\n", status);
    if(decoderOut) VDADecoderDestroy(decoderOut);
    if(decoderConfiguration) CFRelease(decoderConfiguration);
    if(inAVCCData) CFRelease(inAVCCData);
    return 0; 
}





