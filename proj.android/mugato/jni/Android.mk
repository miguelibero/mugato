LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := mugato

BASE_PATH := $(LOCAL_PATH)/../../..
PLATFORM_DIR_FILTER := ! \( -path \*/platform/\* ! -regex .\*/platform/android/\*.\* \)

#Recursive include all .cpp files in the SRC directory
FILE_LIST := $(shell find $(BASE_PATH)/src -type f -iname \*.cpp $(PLATFORM_DIR_FILTER) )
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

INCLUDES += $(BASE_PATH)/src
INCLUDES += $(BASE_PATH)/lib

LOCAL_C_INCLUDES += $(INCLUDES)
LOCAL_EXPORT_C_INCLUDES += $(INCLUDES)

LOCAL_WHOLE_STATIC_LIBRARIES := gorn

include $(BUILD_STATIC_LIBRARY)

$(call import-add-path,$(BASE_PATH)/lib/gorn/proj.android)
$(call import-module,gorn/jni)
