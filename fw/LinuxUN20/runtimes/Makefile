
install:
	@echo "copying c/c++ runtimes..."
	adb push ld-uClibc.so.0 /usr/lib
	adb push libc.so.0 /usr/lib
	adb push libm.so.0 /usr/lib
	adb push libstdc++.so.6 /usr/lib
	@echo "copying runtimes done."

