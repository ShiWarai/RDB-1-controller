# Прошивка для контроллера на робособаке RDB-1
Прошивка контроллера кинематических узлов создана специально под разрабатываемого внутри института ИИТ университета РТУ МИРЭА робота-квадрупеда. Контроллер выполняет связь между основным компьютером или устройством удалённого управления и двигателями робота.

![image](https://user-images.githubusercontent.com/40498648/230801189-ff013034-4c64-4cb9-a5fd-aa42576cb8f1.png)
## О прошлом проекта
Прошлый репозиторий доступен по [ссылке](https://github.com/AlexWanderman/RobotDogAlpha "ссылке"). Разработка была начата ещё в августе 2022, но так как код множество раз претерпевал изменения и сейчас вышел в относительную стабильность на уровне его ядра (а именно контроллера двигателей), было решено перенести данный репозиторий.
## О будущем развитии
Предлагается переход на архитектуру с 4 контроллерами STM32F4, которые будут поддерживать каждый свою конечность робота. Из-за смены контроллера с ESP32 на STM32 отпадёт возможность удалённого управления и просмотра данных, однако получится углубиться в лучшую инкапсуляцию задач контроллера, а именно промежуточную обработку мат. модели каждой ноги.
До перехода на STM32 есть желание испытать двухядерную версию прошивки, где будет поток ввода/вывода высокоуровневых команд и поток ввода/вывода низкоуровневых команд на двигатели.
## Сборка и настройка проекта
Проект разработан для работы с PlatformIO, поэтому данный плагин обязателен для работы в VS Code. Представлен ряд макросов, которые могут изменить функционал прошивки:
- ENABLE_RESTRICTIONS - включает ограничение на отправку команд управления по CAN-шине, согласно проставленным значениям максимального и минимального угла поворота двигателей;
- SERIAL_OUTPUT - включает консольный вывод данных;
- SERIAL_INPUT - включает консольный ввод команд;
- SERIAL_DEBUG - включает консольный вывод ряда дополнительных, отладочных данных;
- DISABLE_CAN - выключает инициализацию CAN-шины
- BT_CONTROL_TYPE - при состоянии =0 включает управление джойстиком по Bluetooth, при состоянии =1 включает управление через BLE-характеристики;

Для указания пин-кода сопряжения с удалённым устройством можно использовать файл конфигурации secret_values.ini и макрос BLE_PINCODE.
