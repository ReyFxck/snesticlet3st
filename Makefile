PS2DEV ?= /opt/ps2dev
PS2SDK ?= $(PS2DEV)/ps2sdk
GSKIT  ?= $(PS2DEV)/gsKit

SRC_DIR := $(CURDIR)/src
OBJ_DIR := $(CURDIR)/build
PKG_DIR := $(OBJ_DIR)/pkg
TARGET  := $(OBJ_DIR)/SNESticle.elf

EE_CC    ?= ee-gcc
EE_CXX   ?= ee-g++
EE_STRIP ?= ee-strip

LEGACY_ROOT ?= /root/SNESticle-Beta
IRX_DIR     ?= $(PS2SDK)/iop/irx

CFLAGS := -G0 -O2 -Wall \
	-D_EE -DPS2 -DLSB_FIRST -DALIGN_DWORD -DCODE_PLATFORM=3

CXXFLAGS := -G0 -O2 -Wall -fno-exceptions -fno-rtti \
	-D_EE -DPS2 -DLSB_FIRST -DALIGN_DWORD -DCODE_PLATFORM=3

INCS := \
	-I$(CURDIR)/src/common/media \
	-I$(CURDIR)/src/platform/ps2/cdvd \
	-I$(CURDIR)/src/platform/ps2/gs \
	-I$(CURDIR)/src/platform/ps2/lowlevel \
	-I$(CURDIR)/src/platform/ps2 \
	-I$(CURDIR)/src/platform/ps2/ui \
	-I$(CURDIR)/src/platform/ps2/system \
	-I$(CURDIR)/src/platform/ps2/input \
	-I$(CURDIR)/src/platform/ps2/memcard \
	-I$(CURDIR)/src \
	-I$(LEGACY_ROOT)/Gep/Include/common \
	-I$(LEGACY_ROOT)/Gep/Include/ps2 \
	-I$(LEGACY_ROOT)/Gep/Source/common \
	-I$(LEGACY_ROOT)/Gep/Source/common/zlib \
	-I$(LEGACY_ROOT)/Gep/Include/common/zlib \
	-I$(LEGACY_ROOT)/Gep/Source/common/unzip \
	-I$(LEGACY_ROOT)/Gep/Include/common/unzip \
	-I$(LEGACY_ROOT)/SNESticle/Source/common \
	-I$(LEGACY_ROOT)/SNESticle/Source/ps2 \
	-I$(LEGACY_ROOT)/SNESticle/Modules/mcsave/ee \
	-I$(LEGACY_ROOT)/SNESticle/Modules/sjpcm/ee \
	-I$(LEGACY_ROOT)/SNESticle/Modules/netplay/Source/common \
	-I$(LEGACY_ROOT)/SNESticle/Modules/netplay/Source/ps2/common \
	-I$(LEGACY_ROOT)/SNESticle/Modules/netplay/Source/ps2/ee \
	-I$(LEGACY_ROOT)/SNESticle/Modules/libcdvd/common \
	-I$(LEGACY_ROOT)/SNESticle/Modules/libcdvd/ee \
	-I$(CURDIR)/compat \
	-I$(PS2SDK)/common/include \
	-I$(PS2SDK)/ee/include \
	-I$(PS2SDK)/ports/include

LIBDIRS := \
	-L$(PS2SDK)/ee/lib \
	-L$(PS2SDK)/ports/lib

LIBS := \
	-lmc -lpad -lps2ip \
	-lkernel -lfileXio -lc -lm -ldebug -lstdc++ -lgcc

SRCS := $(shell tr '\n' ' ' < ok-files.txt)

OBJS := \
	$(patsubst src/%.c,$(OBJ_DIR)/%.o,$(filter %.c,$(SRCS))) \
	$(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(filter %.cpp,$(SRCS))) \
	$(patsubst src/%.s,$(OBJ_DIR)/%.o,$(filter %.s,$(SRCS))) \
	$(patsubst src/%.S,$(OBJ_DIR)/%.o,$(filter %.S,$(SRCS)))

SDK_NET_IRX := ps2dev9.irx netman.irx ps2ip-nm.irx smap.irx
SDK_COMPAT_NET_IRX := ps2ip.irx ps2ips.irx smap-ps2ip.irx
SDK_MC_IRX := mcman.irx mcserv.irx
SDK_EXTRA_IRX := ioptrap.irx poweroff.irx

CUSTOM_IRX_DIR ?= $(CURDIR)/irx
CUSTOM_IRX := NETPLAY.IRX MCSAVE.IRX SJPCM2.IRX

.PHONY: all clean strip list count package package-irx check-env

all: check-env $(TARGET)

check-env:
	@test -d "$(PS2SDK)" || (echo "ERRO: PS2SDK nao encontrado em $(PS2SDK)"; exit 1)
	@test -d "$(IRX_DIR)" || (echo "ERRO: pasta de IRX nao encontrada em $(IRX_DIR)"; exit 1)

$(OBJ_DIR):
	@mkdir -p "$(OBJ_DIR)"

$(PKG_DIR):
	@mkdir -p "$(PKG_DIR)"

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	@mkdir -p "$(dir $@)"
	@echo "CC  $<"
	@$(EE_CC) $(CFLAGS) $(INCS) -c $< -o $@

$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)
	@mkdir -p "$(dir $@)"
	@echo "CXX $<"
	@$(EE_CXX) $(CXXFLAGS) $(INCS) -c $< -o $@

$(OBJ_DIR)/%.o: src/%.s | $(OBJ_DIR)
	@mkdir -p "$(dir $@)"
	@echo "AS  $<"
	@$(EE_CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: src/%.S | $(OBJ_DIR)
	@mkdir -p "$(dir $@)"
	@echo "AS  $<"
	@$(EE_CC) $(CFLAGS) $(INCS) -c $< -o $@

# input.c esta sendo tratado como C++ no seu build atual
$(OBJ_DIR)/platform/ps2/input/input.o: src/platform/ps2/input/input.c | $(OBJ_DIR)
	@mkdir -p "$(dir $@)"
	@echo "CXX $<"
	@$(EE_CXX) -x c++ $(CXXFLAGS) $(INCS) -c $< -o $@

$(TARGET): $(OBJS) | $(OBJ_DIR)
	@echo "LD  $@"
	@$(EE_CXX) -o $@ $(OBJS) $(LIBDIRS) $(LIBS)

strip: $(TARGET)
	@echo "STRIP $<"
	@$(EE_STRIP) "$(TARGET)"

package: check-env $(TARGET) package-irx

package-irx: | $(PKG_DIR)
	@set -e; \
	echo "PKG $(PKG_DIR)"; \
	cp "$(TARGET)" "$(PKG_DIR)/SNESticle.elf"; \
	copy_sdk() { \
		f="$$1"; found=""; \
		for cand in "$$f" "$$(printf '%s' "$$f" | tr '[:upper:]' '[:lower:]')" "$$(printf '%s' "$$f" | tr '[:lower:]' '[:upper:]')"; do \
			if [ -f "$(IRX_DIR)/$$cand" ]; then \
				cp "$(IRX_DIR)/$$cand" "$(PKG_DIR)/"; \
				echo "  + $$cand"; found=1; break; \
			fi; \
		done; \
		if [ -z "$$found" ]; then echo "  ! faltando $$f em $(IRX_DIR)"; fi; \
	}; \
	copy_custom() { \
		f="$$1"; \
		if [ -f "$(CUSTOM_IRX_DIR)/$$f" ]; then \
			cp "$(CUSTOM_IRX_DIR)/$$f" "$(PKG_DIR)/"; \
			echo "  + $$(basename "$(CUSTOM_IRX_DIR)")/$$f"; \
		else \
			echo "  ! faltando custom IRX $$f (procurei em $(CUSTOM_IRX_DIR))"; \
		fi; \
	}; \
	echo "== SDK network IRX =="; \
	for f in $(SDK_NET_IRX); do copy_sdk "$$f"; done; \
	echo "== SDK compat network IRX =="; \
	for f in $(SDK_COMPAT_NET_IRX); do copy_sdk "$$f"; done; \
	echo "== SDK memory card IRX =="; \
	for f in $(SDK_MC_IRX); do copy_sdk "$$f"; done; \
	echo "== SDK extra IRX =="; \
	for f in $(SDK_EXTRA_IRX); do copy_sdk "$$f"; done; \
	echo "== Project custom IRX =="; \
	for f in $(CUSTOM_IRX); do copy_custom "$$f"; done; \
	echo "Pronto: $(PKG_DIR)"
clean:
	rm -rf "$(OBJ_DIR)"

list:
	@printf '%s\n' $(SRCS)

count:
	@printf 'sources: %s\n' "$(words $(SRCS))"
	@printf 'objects: %s\n' "$(words $(OBJS))"
