KPQC_NATIVE_BUILD_DIR = $(builddir)/.pie-native
KPQC_NATIVE_LIBRARY = $(KPQC_NATIVE_BUILD_DIR)/libkpqc.a

$(builddir)/kpqc.la: $(KPQC_NATIVE_LIBRARY)

$(KPQC_NATIVE_LIBRARY):
	$(CMAKE) -S $(srcdir)/native/kpqc -B $(KPQC_NATIVE_BUILD_DIR) \
		-DCMAKE_BUILD_TYPE=Release \
		-DKPQC_BUILD_TESTS=OFF \
		-DKPQC_INSTALL=OFF
	$(CMAKE) --build $(KPQC_NATIVE_BUILD_DIR) --target kpqc --parallel

clean: clean-kpqc-native

clean-kpqc-native:
	$(RM) -r $(KPQC_NATIVE_BUILD_DIR)
