idf_component_register(SRCS "src/LiquidScreen.cpp" "src/LiquidLine.cpp" "src/LiquidMenu.cpp" "src/recognizeType.cpp" "src/LiquidSystem.cpp"
                    REQUIRES DFRobot_RGBLCD1602 # component containing LiquidCrystal_I2C.h
                    INCLUDE_DIRS "./src")
                    