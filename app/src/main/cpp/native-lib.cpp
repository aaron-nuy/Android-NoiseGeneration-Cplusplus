#include <jni.h>
#include <string>
#include <stdlib.h>
#include <filesystem>
#include <thread>
#include <unistd.h>
#include "BMPImage.h"
#include "PerlinNoise.h"


#define function(returnType, name) extern "C" JNIEXPORT returnType JNICALL Java_dz_tp_noisegen_MainActivity_##name



function(jstring ,stringFromJNI) (JNIEnv* env, jobject) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

function(jbyteArray , generateNoise) (JNIEnv* env, jobject MainActivity,
        const jint seed,
        const jdouble frequency,
        const jint oct
        ){
    int imageWidth = 720;
    int imageHeight = 720;



    int pixelCount = imageWidth * imageHeight;

    std::vector<Pixel> bitArray(pixelCount,Pixel(0,0,0));

    const double fy = (frequency / imageWidth);
    const double fx = (frequency / imageHeight);

    siv::PerlinNoise perlin{(uint32_t)seed};

    std::thread t1([&]() {
        for (int i = 1; i < pixelCount/4; i++) {
            double noise = perlin.normalizedOctave2D((i % imageWidth * fx), (i / imageHeight * fy), oct) * 255;
            bitArray[i-1] = Pixel(noise*0.8, noise*0.6, noise*0.7);
        }
    });

    std::thread t2([&]() {
        for (int i = (pixelCount+1)/4; i < (pixelCount+1)/2; i++) {
            double noise = perlin.normalizedOctave2D((i % imageWidth * fx), (i / imageHeight * fy), oct) * 255;
            bitArray[i-1] = Pixel(noise*0.8, noise*0.6, noise*0.7);
        }
    });

    std::thread t3([&]() {
        for (int i = (pixelCount+1)/2; i < 3*(pixelCount+1)/4; i++) {
            double noise = perlin.normalizedOctave2D((i % imageWidth * fx), (i / imageHeight * fy), oct) * 255;
            bitArray[i-1] = Pixel(noise*0.8, noise*0.6, noise*0.7);
        }
    });

    std::thread t4([&]() {
        for (int i = 3*(pixelCount+1)/4; i < pixelCount+1; i++) {
            double noise = perlin.normalizedOctave2D((i % imageWidth * fx), (i / imageHeight * fy), oct) * 255;
            bitArray[i-1] = Pixel(noise*0.8, noise*0.6, noise*0.7);
        }
    });

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    BMPImage image(bitArray, imageWidth, imageHeight);

    std::vector<uint8_t> buffer = image.format();

    jbyteArray arr = env->NewByteArray(buffer.size());
    env->SetByteArrayRegion(arr,0,buffer.size(),(jbyte*)buffer.data());

    return arr;

}