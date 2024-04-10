# Лабораторная работа №2 по разработке драйвера блочных устройств

Цель работы: получить знания и навыки разработки драйверов блочных устройств
для операционной системы Linux. 

Вариант, судя по всему:
<table>
<thead>
  <tr>
    <td>№ варианта </td>
    <td> Структура разделов создаваемого виртуального диска </td>
  </tr>
</thead>
<tbody>
  <tr>
    <td> 8 </td>
    <td>
      Один первичный раздел размером 20Мбайт и один расширенный раздел,
      содержащий два логических раздела размером 17Мбайт и 13Мбайт. Каждый
      последующий байт должен быть результатом умножения предыдущих.|
    </td>
  </tr>
</tbody>
</table>


## Задачи: 

1. Написать драйвер блочного устройства, удовлетворяющий требованиям:
    1. Драйвер должен создавать виртуальный жесткий диск в оперативной памяти с
       размером 50 Мбайт. 
    2. Созданный диск должен быть разбит на разделы в соответствии с вариантом
       задания.
    3. Измерить скорость передачи данных при копировании файлов между разделами
       созданного виртуального диска. 
    4. Измерить скорость передачи данных при копировании файлов между разделами
       виртуального и реального жестких дисков. 

2. Для подготовки отчета выполнить:
    1. Склонировать на github.com структуру репозитория (должен быть
       склонирован при выполнении первой лабораторной работы):
       https://gitlab.se.ifmo.ru/io-systems/report-skeleton
    2. Выложить исходные файлы и скрипты для сборки драйвера в папку lab2
       репозитория
    3. Написать отчет о проделанной работе в файл lab2/README.md репозитория по
       предложенному шаблону, размещенному в файле.

## Полезные материалы

1. Corbet J., Rubini A., Kroah-Hartman G. Linux Device Drivers, Third Edition.
   Электронный ресурс. Ссылка для доступа:
   [https://lwn.net/Kernel/LDD3/](https://lwn.net/Kernel/LDD3/)
2. Ромоданов Н. Драйверы устройств в Linux. Электронный ресурс. Ссылка:
   [http://rus-linux.net/MyLDP/BOOKS/drivers/linux-device-drivers-00.html](http://rus-linux.net/MyLDP/BOOKS/drivers/linux-device-drivers-00.html)
3. О блочных устройствах (eng):
   [https://linux-kernellabs.github.io/refs/heads/master/labs/block_device_drivers.html](https://linux-kernellabs.github.io/refs/heads/master/labs/block_device_drivers.html)
4. О логической структуре жёсткого диска:
   [https://www.opennet.ru/base/dev/hdd_struct.txt.html](https://www.opennet.ru/base/dev/hdd_struct.txt.html)
5. О цилиндрах, головках и секторах (CHS):
   [https://ru.wikipedia.org/wiki/CHS](https://ru.wikipedia.org/wiki/CHS)

- - -

## Комментарии


