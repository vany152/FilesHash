#Files hash

Программа принимает на вход путь к файлу или папке и высчитывает 
контрольную сумму CRC32 для объекта. 

Программа может работать в многопоточном режиме, но только если 
был введен путь к папке. Количество потоков не должно превышать 
количество папок в файле и количество логических ядер процессора.
Рекомендуется выбирать количество потоков больше одного в том 
случае, если проверяется папка, находящаяся на SSD диске. Если 
же папка находится на HDD диске, следует запускать программу 
в однопоточном режиме, иначе могут возникнуть серьезные просадки
в скорости обработки папки. Если был введен путь к файлу, его 
хеш высчитывается в однопоточном режиме. 

## Алгоритм поиска контрольной суммы каталога
Хеш файла высчитывается как контрольная сумма хешей вложенных 
файлов и каталогов (уровень вложенности не больше одного, 
папки и файлы в каталоге отсортированны в алфавитном порядке,
сначала к хешу добавляются хеши файлов, после - папок)

Сначала высчитываются контрольные суммы всех вложенных файлов
корневой папки и вложенных файлов её подкаталогов, после чего 
выполняется рекурсивный обход вложенных папок: сначала 
выполняется рекурсивный спуск, на подъеме высчитывается хеш. 