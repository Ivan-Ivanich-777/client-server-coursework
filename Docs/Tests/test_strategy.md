# Стратегия тестирования

## Тест-кейсы
| ID | Тест | Ожидаемый результат |
|----|------|---------------------|
| TC01 | reg&test&123&t@mail.ru | reg+&test |
| TC02 | reg&test&123&t@mail.ru (повтор) | reg- |
| TC03 | auth&test&123 | auth+&test |
| TC04 | auth&test&wrong | auth- |
| TC05 | stat&test | stat&0&0&0 |
| TC06 | check&1&0&0 | check+ |
| TC07 | check&1&5&5 | check- |
| TC08 | check&2&-0.57735&-0.3849 | check+ |
| TC09 | check&3&1&1 | check+ |
| TC10 | unknown_command | error |
