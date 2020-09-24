ifeq ($(OS), win)
	TOOLCHAIN := x86_64-w64-mingw32.static-
	APP := app.exe
else
	TOOLCHAIN :=
	APP := app
endif

CXX=$(TOOLCHAIN)g++
LD=$(TOOLCHAIN)ld
AR=$(TOOLCHAIN)ar
PKG_CONFIG=$(TOOLCHAIN)pkg-config

OBJS = main.o toolbar.o file.o window.o user_list.o user_menu.o user_search.o user_assistant.o warn_list.o warn_menu.o warn_assistant.o birthday_window.o birthday_list.o dialog.o months.o
FLAGS = -std=c++11 -MMD -ffunction-sections -fdata-sections	#add -mwindows before -std=c++11 to hide the terminal


$(APP): $(OBJS)
	$(CXX) $(FLAGS) `$(PKG_CONFIG) --cflags gtkmm-3.0` -o $(APP) $(OBJS) `$(PKG_CONFIG) --libs gtkmm-3.0` -Wl,-gc-sections

%.o : %.c++
	$(CXX) $(FLAGS) `$(PKG_CONFIG) --cflags gtkmm-3.0` -o $@ -c $< `$(PKG_CONFIG) --libs gtkmm-3.0`

clean:
	rm *.o
	rm -f *.d 

-include $(OBJS:.o=.d)

