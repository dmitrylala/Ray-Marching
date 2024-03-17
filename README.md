# Distance-Aided Ray Marching


## Кодстайл
C++ кодстайл - [Google Style Guide](https://google.github.io/styleguide/cppguide.html), python кодстайл - PEP8, исключения указаны в CPPLINT.cfg и в .pylintrc.

## Установка зависимостей и сборка
Из зависимостей понадобится cmake, clang10 (либо другая версия, нужно ее позже указать conan) и poetry:
```
pip install poetry
```

Сборку и запуск тестов нужно выполнять под venv, который устанавливается и активируется с помощью poetry:
```
poetry install --no-root && poetry shell
conan detect profile
mkdir -p build
conan install . --build=missing --output-folder=build
cmake . -B build -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
cmake --build build --target all
```
Имеются следующие таргеты (помещаются в build/):
- hashmap - демонстрационная программа, интерфейс взаимодействия доступен по help
- hashmap_test - юнит тесты

Запуск интеграционных тестов:
```
pytest test/integr/hashmap.py
```

Запуск линтеров:
```
cpplint include/* bin/* test/unit/*
pylint test/integr/hashmap.py
```
