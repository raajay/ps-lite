APP_DIR="./app"
APP_SRC = $(wildcard tests/*.cc)
APP_LIBS = -L./deps/lib -L./build -lps -lzmq -lprotobuf-lite

psrun: ps $(APP_SRC)
	@echo "Building a simple app to use parameter server."
	$(CXX) $(INCPATH) -std=c++0x $(APP_LIBS) -o $(APP_DIR)/psrun $(APP_DIR)/*.cc

appclean:
	rm -rf  ${APP_DIR}/psrun
