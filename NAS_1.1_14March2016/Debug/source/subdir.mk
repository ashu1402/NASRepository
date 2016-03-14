################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/onvifclientdevice.cpp \
../source/onvifclientevent.cpp \
../source/onvifclientimaging.cpp \
../source/onvifclientmedia.cpp \
../source/onvifclientptz.cpp \
../source/onvifclientreceiver.cpp \
../source/onvifclientrecording.cpp \
../source/onvifclientreplay.cpp \
../source/onvifclientsearch.cpp \
../source/onvifserver.cpp \
../source/onvifserverdevice.cpp 

OBJS += \
./source/onvifclientdevice.o \
./source/onvifclientevent.o \
./source/onvifclientimaging.o \
./source/onvifclientmedia.o \
./source/onvifclientptz.o \
./source/onvifclientreceiver.o \
./source/onvifclientrecording.o \
./source/onvifclientreplay.o \
./source/onvifclientsearch.o \
./source/onvifserver.o \
./source/onvifserverdevice.o 

CPP_DEPS += \
./source/onvifclientdevice.d \
./source/onvifclientevent.d \
./source/onvifclientimaging.d \
./source/onvifclientmedia.d \
./source/onvifclientptz.d \
./source/onvifclientreceiver.d \
./source/onvifclientrecording.d \
./source/onvifclientreplay.d \
./source/onvifclientsearch.d \
./source/onvifserver.d \
./source/onvifserverdevice.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include -I../cpgf/include/cpgf -I../cpgf/include -I../src -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/gdk-pixbuf-2.0 -I/usr/local/lib/gstreamer-1.0/include -I/usr/local/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/local/include/opencv -I/usr/include/mysql -I../gsoap/include -I../include -I../source -I../onvifgen/include -I../inc -I/usr/local/include -I/usr/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


