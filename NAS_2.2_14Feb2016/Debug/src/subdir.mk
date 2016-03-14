################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AbsoluteMove.cpp \
../src/BootLoader.cpp \
../src/CameraControl.cpp \
../src/Channel.cpp \
../src/CommonClass.cpp \
../src/ContinuousMove.cpp \
../src/DataBaseAccess.cpp \
../src/Delete.cpp \
../src/DetectMotion.cpp \
../src/Download.cpp \
../src/Dump.cpp \
../src/Failover.cpp \
../src/GetPTZNodes.cpp \
../src/GetPTZStatus.cpp \
../src/GetPTZUrl.cpp \
../src/GetRTSPUrl.cpp \
../src/GetServiceCap.cpp \
../src/Interaction.cpp \
../src/Markup.cpp \
../src/Monitoring.cpp \
../src/Notification.cpp \
../src/NotificationInitiate.cpp \
../src/OnvifMain.cpp \
../src/ReStream.cpp \
../src/RelativeMove.cpp \
../src/RelativeMoveEvent.cpp \
../src/Retrieve.cpp \
../src/StopContinuous.cpp \
../src/TCPConnection.cpp \
../src/VOD.cpp \
../src/XMLCreator.cpp \
../src/XMLParser.cpp \
../src/main.cpp \
../src/motionxml.cpp 

OBJS += \
./src/AbsoluteMove.o \
./src/BootLoader.o \
./src/CameraControl.o \
./src/Channel.o \
./src/CommonClass.o \
./src/ContinuousMove.o \
./src/DataBaseAccess.o \
./src/Delete.o \
./src/DetectMotion.o \
./src/Download.o \
./src/Dump.o \
./src/Failover.o \
./src/GetPTZNodes.o \
./src/GetPTZStatus.o \
./src/GetPTZUrl.o \
./src/GetRTSPUrl.o \
./src/GetServiceCap.o \
./src/Interaction.o \
./src/Markup.o \
./src/Monitoring.o \
./src/Notification.o \
./src/NotificationInitiate.o \
./src/OnvifMain.o \
./src/ReStream.o \
./src/RelativeMove.o \
./src/RelativeMoveEvent.o \
./src/Retrieve.o \
./src/StopContinuous.o \
./src/TCPConnection.o \
./src/VOD.o \
./src/XMLCreator.o \
./src/XMLParser.o \
./src/main.o \
./src/motionxml.o 

CPP_DEPS += \
./src/AbsoluteMove.d \
./src/BootLoader.d \
./src/CameraControl.d \
./src/Channel.d \
./src/CommonClass.d \
./src/ContinuousMove.d \
./src/DataBaseAccess.d \
./src/Delete.d \
./src/DetectMotion.d \
./src/Download.d \
./src/Dump.d \
./src/Failover.d \
./src/GetPTZNodes.d \
./src/GetPTZStatus.d \
./src/GetPTZUrl.d \
./src/GetRTSPUrl.d \
./src/GetServiceCap.d \
./src/Interaction.d \
./src/Markup.d \
./src/Monitoring.d \
./src/Notification.d \
./src/NotificationInitiate.d \
./src/OnvifMain.d \
./src/ReStream.d \
./src/RelativeMove.d \
./src/RelativeMoveEvent.d \
./src/Retrieve.d \
./src/StopContinuous.d \
./src/TCPConnection.d \
./src/VOD.d \
./src/XMLCreator.d \
./src/XMLParser.d \
./src/main.d \
./src/motionxml.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include -I../cpgf/include/cpgf -I../cpgf/include -I../src -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/gdk-pixbuf-2.0 -I/usr/local/lib/gstreamer-1.0/include -I/usr/local/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/local/include/opencv -I/usr/include/mysql -I../gsoap/include -I../include -I../source -I../onvifgen/include -I../inc -I/usr/local/include -I/usr/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


