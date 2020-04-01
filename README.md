# PROCESSES

Тестовое задание полученное от компании в которую устраивался. 
Задание ("NNSoft-Task-2020") лежит в корне в формате PDF.

Анотация:
"Необходимо создать программу  –  аналог вкладки “Services” с графическим интерфейсом  на  WPF  с применением  паттерна  MVVM. Основные функции приложения  по работе  со службами  должны  быть  реализованы  в  native  (не  .NET assembly)  DLL  на С++.  Список служб должен включать колонки  Name,  PID, Description,  Status  и Group. Необходимо  также  добавить колонку  “Image  path”, содержащую полный  путь  до  исполняемого  модуля  службы, которой нет в Task Manager’e."


В этом репозитории лежит версия программы организованная мреимущественно с помощью процедурного подхода в "класическом стиле".  По сравнению с версией "PROCESSES_DLL", в этой версии не учтены и не добавлены условия или обработчики исключений для закрытия дескриторов процессов, остановки процесса, перезапуска процесса,не происходит проверка на большинство возможных ошибок(Ибо их много,а необходимости в этом нету). Программа написана с использованием библиотеки Win32API. Данные исходники написаны исключительно в качестве демонстранции возможностей работы библиотеки для выполнения задания. 
