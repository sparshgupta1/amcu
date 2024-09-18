import os
#'build/sharedObjs/cy_m0p_image.bin'
st = os.stat('build/sharedObjs/cy_m0p_image.bin')
with open('build/sharedObjs/cy_m0p_image.bin','r') as f:
    hex_list = ["{:02x}".format(ord(c)) for c in f.read()]
with open('build/CY8CKIT-062S2-43012/Debug/cy_m0p_image.c','w') as result_file:
    result_file.write(
                      '''
/******************************************************************************* 
    * Auto-generated from ../app_cm0p/build/CY8CKIT-062S2-43012/Debug/cy_m0p_image.bin 
*******************************************************************************/ 
#include <stdint.h> 

#if defined(__APPLE__) && defined(__clang__) 
__attribute__ ((__section__("__CY_M0P_IMAGE,__cy_m0p_image"), used)) 
#elif defined(__GNUC__) || defined(__ARMCC_VERSION) 
__attribute__ ((__section__(".cy_m0p_image"), used)) 
#elif defined(__ICCARM__) 
#pragma  location=".cy_m0p_image" 
#else 
#error "An unsupported toolchain" 
#endif 

    const uint8_t cy_m0p_image[
                    ''')
    result_file.write(str(st.st_size))
    result_file.write("] = {")
    for line in hex_list:
        result_file.write("0x{},\n".format(line))
    result_file.write("};")
f.close()