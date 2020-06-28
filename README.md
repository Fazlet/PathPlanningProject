# PathPlanningProject

Программный проект "Path Planning" или "Нахождение кратчайших путей".

Представляет собой алгоритмы Dijkstra, AStar и ThetaStar поиска пути на двумерной карте-сетке.

Чтобы выбрать нужный алгоритм, необходимо указать во входном xml-файле тег searchtype: dijkstra, astar или theta.

### Входные данные

Первым аргументом на вход программе подаётся xml-файл. В нём содержится: 
- Карта
- Координаты начальной и конечной точек
- Опции алгоритма
- Уровни логирования

##### Теги входного xml-файла:
**\<root>** - глобальный тег

**\<map>** - обязательный тег, описывающий карту:
- **\<width>** и **\<height>** - ширина и высота карты
- **\<сellsize>** - масштаб карты (размер одной ячейки)
- **\<startx>** и **\<starty>** - координаты начальной точки
- **\<finishx>** и **\<finishy>** - координаты конечной точки
- **\<grid>** - тег, описывающий квадратную сетку карты; состоит из тегов **\<row>** - описания строки сетки, состоящего из 0 (свободных ячеек) и 1 (препятствий)

**\<algorithm>** - обязательный тег, описывающий опции алгоритма:
- **\<searchtype>** - тип алгоритма поиска (dijkstra, astar, theta)
- **\<metrictype>** - тип метрики пространства (diagonal, manhattan, euclidean or chebyshev)
- **\<hweight>** - вес эвристичиеской функции (больше нуля)
- **\<breakingties>** - тип сравнения вершин при равнестве F-значений (g-max или g-min)
- **\<allowdiagonal>** - возможность "ходить" по диагонали
- **\<cutcorners>** - возможность "срезать" углы
- **\<allowsqueeze>** -  возможность "ходить" по диагонали между двумя препятствиями

**\<options>** - необязательный тег, описывающий логирование:
- **\<loglevel>** - уровень логирования: 
"0" - без создания лог-файла; "0.5" - краткая информация (длина пути, количество шагов алгоритма, время работы); "1" - всё предыдущее и последовательность вершин, образующих путь, последовательность секций пути; "1.5" - всё предыдущее и информация о множествах вершин OPEN и CLOSE в конце работы алгоритма; "2" - всё предыдущее и и информация о множествах вершин OPEN и CLOSE после каждого шага работы алгоритма.
- **\<logpath>** - путь сохранения xml-файла с результатом
- **\<logfilename>** - имя xml-файла с результатом

### Выходные данные 
После работы программы получаем xml-файл с результатами. Он содержит в себе все теги из входного файла, а также:
- **\<mapfilename>** - имя лог-файла (?)
- **\<summary>** - информация о результате работы алгоритма (длина пути, количество шагов алгоритма, время работы)
- **\<path>** - карта с изображением пути из начальной точки до конечной (если он найден)
- **\<hplevel>** - последовательность секций пути
- **\<lplevel>** - последовательность вершин, образующих путь.

### Сборка и запуск

Сборку проекта возможно осуществить двумя способами:
- Используя QtCreator и qmake; 
- Используя CMake. 

В качестве первого аргумента командной строки нужно подать входной xml-файл (описан выше).

После запуска краткий лог работы можно увидеть в консоли. Также будет создан xml-файл с подробным результатом. 

#### Linux и Mac
Release сборка:
```bash
cd PathPlanningProject/Build/Release
cmake ../../ -DCMAKE_BUILD_TYPE="Release"
make
make install
```

Debug сборка:
```bash
cd PathPlanningProject/Build/Debug
cmake ../../ -DCMAKE_BUILD_TYPE="Debug"
make
make install
```

Запуск:
```bash
cd PathPlanningProject/Bin/{Debug|Release}
./PathPlanning ../../Examples/example.xml
```

#### Windows
Release сборка:
```cmd
cd PathPlanningProject/Build/Release
set PATH
cmake ../../ -DCMAKE_BUILD_TYPE="Release" -G "MinGW Makefiles"
mingw32-make
mingw32-make install
```

Debug сборка:
```cmd
cd PathPlanningProject/Build/Debug
set PATH
cmake ../../ -DCMAKE_BUILD_TYPE="Debug" -G "MinGW Makefiles"
mingw32-make
mingw32-make install
```

Запуск:
```cmd
cd PathPlanningProject/Bin/{Debug|Release}
PathPlanning.exe ../../Examples/example.xml
```

### Тестирование 
Linux test result:

[![Build Status](https://travis-ci.org/Fazlet/PathPlanningProject.svg?branch=master)](https://travis-ci.com/Fazlet/PathPlanningProject)

Windows test result:

[![Build status](https://ci.appveyor.com/api/projects/status/fq0ucckb0yu60ayy/branch/master?svg=true)](https://ci.appveyor.com/project/Fazlet/pathplanningproject/branch/master)
 
Локальный запуск тестов производится из директории `.../PathPlanningProject/Build/{Debug|Release}/` с помощью команды:
```
 ctest
```

либо (для более подробного вывода):
```
 ctest --output-on-failure
```

### Контакты
**Фазлетдинов Эдуард**
- ed.fazletdinov@gmail.com
- Telegram: @fazlet
