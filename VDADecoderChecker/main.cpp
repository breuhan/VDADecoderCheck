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


void myDecoderOutputCallback();


int main (int argc, const char * argv[])
{

    
    ifstream::pos_type size;
    UInt8 *data;
    ifstream file ("subler_avcC", ios::in|ios::binary|ios::ate);
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
    CFDataRef inAVCCData;
    VDADecoder decoderOut = NULL;
    
    OSType inSourceFormat='avc1';
    int inHeight = 1920;
    int inWidth = 1080;
    
    CFMutableDictionaryRef decoderConfiguration = NULL;
    CFNumberRef height = NULL;
    CFNumberRef width= NULL;
    CFNumberRef sourceFormat = NULL;

    
    inAVCCData = CFDataCreate(kCFAllocatorDefault, (const uint8_t*) data, (int)size);
    

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
    if (kVDADecoderNoErr != status) {
        fprintf(stderr, "VDADecoderCreate failed. err: %d\n", status);
    }
    switch (status) {
        case kVDADecoderNoErr:
            std::cout << "Hardware acceleration full supported" << "\n";
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
            std::cout << "Unknown Status: " << status;
            break;
    }
    
    if (decoderConfiguration) CFRelease(decoderConfiguration);
    //if(decoderOut!=NULL) VDADecoderDestroy(*decoderOut);
    return 0;
}

void myDecoderOutputCallback(){}




