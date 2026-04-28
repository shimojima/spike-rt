#
# Makefile for a workspace of SPIKE-RT Platform.
#

# Configuration
SRCLANG := c
KERNEL := asp
TARGET := primehub_gcc

#
# Paths
#
SPIKE_RT_DIR := $(abspath ../..)
SDKDIR       := $(abspath ..)
WSPDIR       := $(abspath .)

SRCDIR       := $(SPIKE_RT_DIR)/asp3
COMMONDIR    := $(SDKDIR)/common
KERNEL_LIB   := $(SPIKE_RT_DIR)/build/obj-primehub_kernel
TARGETDIR    := $(SRCDIR)/target/$(TARGET)
# Object files
OBJFILENAME := $(KERNEL)
OBJBINARY := $(KERNEL).bin

#
# Determine Makefile for application
# OUTPUT:
# $(APPLDIRS):        Absolute path of application folder
# $(MKFILENAME):      File name of Makefile
# $(MKFILE_DIR):      Absolute path of the folder holding Makefile
#

ifdef img

default: img

#APPLNAME := $(img)
APPLNAME := app

APPLDIRS   := $(WSPDIR)/$(img)
MKFILENAME := app.mk
MKFILE_DIR := $(COMMONDIR)

BUILDDIR := $(APPLDIRS)/build

#
# Target for an application (static)
#
JOB_NUM = ${JOB_NUM:-$(nproc)} 

include $(APPLDIRS)/Makefile.inc

CDLFILE := $(APPLNAME).cdl
CDL_DIR := $(APPLDIRS)
ifeq (,$(wildcard $(CDL_DIR)/$(CDLFILE)))
CDL_DIR := $(COMMONDIR)
CDLFILE := app.cdl
endif

ifdef KERNEL_LIB
DEPKERNEL=$(KERNEL_LIB)/libkernel.a
OPTKERNEL=-L $(KERNEL_LIB)
PYBRICKS_LIB = $(KERNEL_LIB)/../obj-primehub_pybricks
else
DEPKERNEL=
OPTKERNEL=
PYBRICKS_LIB =
endif

img: $(APPLDIRS) $(BUILDDIR)/Makefile $(DEPKERNEL)
	echo "$(APPLDIRS)" > $(WSPDIR)/appdir && \
	make -j $(JOB_NUM) -C $(BUILDDIR) && \
	cp $(BUILDDIR)/$(OBJBINARY) $(WSPDIR)
ifdef KERNEL_LIB
	@echo "$(abspath $(KERNEL_LIB))" > $(BUILDDIR)/.KERNEL_LIB
	@echo "$(abspath $(PYBRICKS_LIB))" > $(BUILDDIR)/.PYBRICKS_LIB
endif

$(BUILDDIR)/Makefile:	Makefile $(MKFILE_DIR)/$(MKFILENAME)
	@mkdir -p $(BUILDDIR)
	@cd $(BUILDDIR) && \
	$(SRCDIR)/configure.rb -T $(TARGET) $(OPTKERNEL) \
          -a $(APPLDIRS) \
	  -A $(APPLNAME) \
	  -l $(SRCLANG) \
	  -d $(BUILDDIR)/dep \
	  -C $(CDL_DIR)/$(CDLFILE) \
	  -m $(MKFILE_DIR)/$(MKFILENAME)

ifdef KERNEL_LIB
$(KERNEL_LIB)/Makefile:	Makefile $(COMMONDIR)/kernel.mk
	@mkdir -p $(KERNEL_LIB)
	@cd $(KERNEL_LIB) && \
	$(SRCDIR)/configure.rb -T $(TARGET) -f -m $(COMMONDIR)/kernel.mk

$(DEPKERNEL): $(KERNEL_LIB)/Makefile
	@make libkernel.a -j $(JOB_NUM) -C $(KERNEL_LIB)
endif

endif

usage:
	@echo "Usage:"
	@echo "  make img=\"<folder>\""
	@echo "  make upload"
	@echo "  make clean"
	@echo "  make realclean"

upload: $(OBJBINARY)
	@echo "Upload $(OBJBINARY) file"
	@$(COMMONDIR)/deploy-dfu.sh $(OBJBINARY) && \
	echo $(OBJBINARY) has been successfully uploaded.

ifneq (,$(wildcard appdir))
#ifneq ($(shell test -f "appdir" && echo 1 || echo 0), 0)
clean:	cleanup
	@make -C $(shell cat appdir)/build clean

realclean: clean
	@make -C $(shell cat appdir)/build cleankernel
	@make -C $(shell cat appdir)/build realclean
ifneq (,$(wildcard $(shell cat appdir)/build/.KERNEL_LIB))
	rm -rf $(shell cat $(shell cat appdir)/build/.PYBRICKS_LIB)
	rm -rf $(shell cat $(shell cat appdir)/build/.KERNEL_LIB)
endif
else
clean:	cleanup
realclean: clean
endif

cleanup:	
	@rm -rf $(notdir $(OBJFILENAME)) $(OBJBINARY) firmware.dfu $(DEPKERNEL)

distclean:
	@$(foreach app,$(wildcard */build),make -C $(app) realclean;)
	@$(foreach app,$(wildcard */build),rm -rf $(app);)
	@rm appdir asp.bin
	@rm -rf ../../build

.PHONY: usage img upload clean cleanup realclean distclean
	
