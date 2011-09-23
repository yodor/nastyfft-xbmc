ARCH=x86_64-linux
INCLUDES=-I. -I.. -I../../linux -I../../addons/include -I../../ -I ../../../guilib
DEFINES+=-DHAS_SDL_OPENGL -DHAS_SDL
CXXFLAGS=-fPIC
OBJS=nastyfft.o plugin.o

SLIB=../../../addons/visualization.nastyfft/nastyfft.vis

$(SLIB): $(OBJS)
ifeq ($(findstring osx,$(ARCH)), osx)
	@export MACOSX_DEPLOYMENT_TARGET=10.4
	$(CXX) $(CXXFLAGS) -bundle -shared -flat_namespace -undefined suppress -o $@ $(OBJS)
	../../../tools/Mach5/wrapper.rb $@;mv output.so $@
else
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared -g -o $(SLIB) $(OBJS)
endif

include ../../../Makefile.include

