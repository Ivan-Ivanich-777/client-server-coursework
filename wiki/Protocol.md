# Протокол обмена (разделитель &)
auth&login&pass -> auth+/auth-
reg&login&pass&email -> code_sent -> verify&email&code -> reg+
forgot&login -> code_sent -> reset&login&code&newpass -> reset+
check&task&x&y -> check+/check-
calc_table&a&b&c -> table_data&...
