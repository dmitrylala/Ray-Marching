# Distance-Aided Ray Marching

Реализация алгоритма Distance-Aided Ray Marching для рендеринга треугольника Серпинского на CPU и GPU под Vulkan.

## Установка зависимостей и сборка

Список зависимостей:
- python (>=3.8,<3.11)
- cmake (>=3.16)
- пакетный менеджер poetry

Poetry можно поставить так:
```
pip install poetry
```

Создаем и активируем venv:
```
poetry install --no-root && poetry shell
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

## Кодстайл
C++ кодстайл - [Google Style Guide](https://google.github.io/styleguide/cppguide.html), python кодстайл - PEP8, исключения указаны в CPPLINT.cfg и в .pylintrc.

