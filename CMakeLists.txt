# דרישת מינימום לגרסת CMake
cmake_minimum_required(VERSION 3.10)

# שם הפרויקט והשפה
project(MyShell VERSION 1.0 LANGUAGES CXX)

# הגדרת סטנדרט C++ (גרסה 17 מומלצת, מודרנית ונוחה)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# הוספת דגלי קומפילציה לאזהרות (Best Practice)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# הגדרת תיקיית ה-includes כדי שהקומפיילר ימצא את קובצי ה-.h
include_directories(include)

# איסוף כל קובצי המקור מתיקיית src
# (גישה נוחה כדי לא להוסיף כל קובץ ידנית, אם כי בפרויקטים ענקיים נהוג לפרט)
file(GLOB SOURCES "src/*.cpp")

# יצירת קובץ ההרצה הסופי (Executable) מהמקורות
add_executable(my_shell ${SOURCES})

# הוספת קובצי הבדיקה (אם יש) לקומפילציה 
add_executable(run_tests
    tests/test__parser_tokenize.cpp
    src/parser.cpp
)

target_include_directories(run_tests PRIVATE include)

