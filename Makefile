# Makefile for the Final Project. Keep it in your project folder.
#
# 1. After SRCS, list every .c file of your program; after HDRS, every .h file.
#    Do not list checker.c: it is the test checker, not part of your program.
# 2. Build by running this command in your project folder:
#        make             (macOS / Linux)
#        mingw32-make     (Windows)
#    It creates the program "inventory" (inventory.exe on Windows).
#
# Do not change CC, CFLAGS or the command line below: the TA team builds
# every project on Linux with exactly this command.
# The command line under "inventory:" must start with a TAB, not with spaces.

CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra
SRCS   = work/FinalProject/main.c work/FinalProject/import.c work/FinalProject/checkout.c work/FinalProject/product_manager.c work/FinalProject/recording_system.c work/FinalProject/small_operations.c
HDRS   = work/FinalProject/import.h work/FinalProject/checkout.h work/FinalProject/product_manager.h work/FinalProject/recording_system.h work/FinalProject/small_operations.h work/FinalProject/product.h work/FinalProject/promotion.h

inventory: $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(SRCS) -o inventory -lm
