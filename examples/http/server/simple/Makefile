EXE_NAME := example/http/server/simple

CPP_SRC_FILES := simple.cpp

CXXFLAGS := -DLOG_MODULE_ID='"$(EXE_NAME)"' $(CXXFLAGS)
LDFLAGS += \
	-ltbox_http \
	-ltbox_network \
	-ltbox_eventx \
	-ltbox_event \
	-ltbox_log \
	-ltbox_util \
	-ltbox_base \

include ${TOP_DIR}/tools/exe_common.mk
