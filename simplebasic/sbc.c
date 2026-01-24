#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256
#define MAX_CONSTANTS 100
#define MAX_LABELS 100
#define MAX_TOKENS 50
#define MAX_RPN_TOKENS 100

// Стек
typedef struct {
    char* items[MAX_TOKENS];
    int top;
} Stack;

void push(Stack* s, char* item)
{
    if (s->top >= MAX_TOKENS - 1) {
        fprintf(stderr, "Ошибка: Переполнение стека.\n");
        exit(1);
    }
    s->items[++(s->top)] = item;
}

char* pop(Stack* s)
{
    if (s->top < 0) {
        return NULL;
    }
    return s->items[(s->top)--];
}

char* peek(const Stack* s)
{
    if (s->top < 0) {
        return NULL;
    }
    return s->items[s->top];
}

bool is_empty(const Stack* s)
{
    return s->top < 0;
}

// Глобальные переменные
bool used_vars[26] = {false};
int var_addresses[26]; // A-Z
int constants[MAX_CONSTANTS];
int constant_count = 0;
int const_addresses[MAX_CONSTANTS];
int label_addresses[MAX_LABELS * 2];
int label_count = 0;
int temp_storage_address = -1; // Временная переменная для RPN

int get_precedence(char op)
{
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

bool is_operator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int add_constant(int value)
{
    for (int i = 0; i < constant_count; i++) {
        if (constants[i] == value)
            return i;
    }
    if (constant_count >= MAX_CONSTANTS) {
        fprintf(stderr,
                "Ошибка: Слишком много констант (макс. %d)\n",
                MAX_CONSTANTS);
        exit(1);
    }
    constants[constant_count] = value;
    return constant_count++;
}

int find_constant_index(int value)
{
    for (int i = 0; i < constant_count; i++) {
        if (constants[i] == value)
            return i;
    }
    fprintf(stderr,
            "Ошибка: Внутренняя - Значение константы %d не найдено в таблице "
            "символов.\n",
            value);
    exit(1);
}

void add_label(int line_number, int address)
{
    for (int i = 0; i < label_count; i++) {
        if (label_addresses[i * 2] == line_number) {
            if (label_addresses[i * 2 + 1] == -1) {
                label_addresses[i * 2 + 1] = address;
            }
            return;
        }
    }
    if (label_count >= MAX_LABELS) {
        fprintf(stderr, "Ошибка: Слишком много меток (макс. %d)\n", MAX_LABELS);
        exit(1);
    }
    label_addresses[label_count * 2] = line_number;
    label_addresses[label_count * 2 + 1] = address;
    label_count++;
}

int get_label_address(int line_number)
{
    for (int i = 0; i < label_count; i++) {
        if (label_addresses[i * 2] == line_number) {
            if (label_addresses[i * 2 + 1] != -1) {
                return label_addresses[i * 2 + 1];
            } else {
                fprintf(stderr,
                        "Ошибка: Метка %d используется, но не связана с "
                        "инструкцией.\n",
                        line_number);
                exit(1);
            }
        }
    }
    fprintf(stderr, "Ошибка: Неопределенная метка: %d\n", line_number);
    exit(1);
}

typedef enum { OPERAND_VAR, OPERAND_CONST, OPERAND_ERROR } operand_type;

operand_type
parse_operand(const char* token, bool is_first_pass, int* value_out)
{
    if (!token || token[0] == '\0')
        return OPERAND_ERROR;

    if (strlen(token) == 1 && isupper(token[0])) {
        if (is_first_pass) {
            used_vars[token[0] - 'A'] = true;
        }
        return OPERAND_VAR;
    }

    char* endptr;
    long val = strtol(token, &endptr, 10);
    if (*endptr == '\0') {
        if (val > 32767 || val < -32768) {
            fprintf(stderr,
                    "Ошибка: Значение константы %s выходит за допустимый "
                    "диапазон (-32768 до 32767).\n",
                    token);
            exit(1);
        }
        if (is_first_pass) {
            *value_out = (int)val;
            add_constant((int)val);
        }
        return OPERAND_CONST;
    }
    return OPERAND_ERROR;
}

void generate_code(FILE* out, const char* op, int operand, int* addr)
{
    if (strcmp(op, "READ") == 0 || strcmp(op, "WRITE") == 0
        || strcmp(op, "LOAD") == 0 || strcmp(op, "STORE") == 0
        || strcmp(op, "ADD") == 0 || strcmp(op, "SUB") == 0
        || strcmp(op, "MUL") == 0 || strcmp(op, "DIVIDE") == 0
        || strcmp(op, "JUMP") == 0 || strcmp(op, "JNEG") == 0
        || strcmp(op, "JP") == 0 || strcmp(op, "JZ") == 0) {
        fprintf(out, "%02d %s %02d\n", (*addr)++, op, operand);
    } else if (strcmp(op, "HALT") == 0) {
        fprintf(out, "%02d HALT 00\n", (*addr)++);
    } else {
        fprintf(stderr,
                "Ошибка: Внутренняя - Неизвестная инструкция '%s'.\n",
                op);
        exit(1);
    }
}

// Разделяет выражение на токены (операнды, операторы, скобки)
char** tokenize_expression(const char* expression, int* num_tokens)
{
    char** tokens = (char**)malloc(MAX_TOKENS * sizeof(char*));
    if (!tokens) {
        fprintf(stderr, "Ошибка выделения памяти для токенов.\n");
        exit(1);
    }
    *num_tokens = 0;

    char temp_token[MAX_LINE];
    int temp_idx = 0;

    for (int i = 0; expression[i] != '\0'; i++) {
        char c = expression[i];

        if (isspace(c)) {
            if (temp_idx > 0) {
                temp_token[temp_idx] = '\0';
                tokens[*num_tokens] = strdup(temp_token);
                if (!tokens[*num_tokens]) {
                    exit(1);
                }
                (*num_tokens)++;
                temp_idx = 0;
            }
            continue;
        }

        // Обработка чисел, включая унарный минус
        if (isdigit(c)
            || (c == '-'
                && (i == 0 || is_operator(expression[i - 1])
                    || expression[i - 1] == '('
                    || isspace(expression[i - 1])))) {
            if (temp_idx > 0) {
                temp_token[temp_idx] = '\0';
                tokens[*num_tokens] = strdup(temp_token);
                if (!tokens[*num_tokens]) {
                    exit(1);
                }
                (*num_tokens)++;
                temp_idx = 0;
            }
            temp_token[temp_idx++] = c;
            while (isdigit(expression[i + 1])) {
                i++;
                temp_token[temp_idx++] = expression[i];
            }
            temp_token[temp_idx] = '\0';
            tokens[*num_tokens] = strdup(temp_token);
            if (!tokens[*num_tokens]) {
                exit(1);
            }
            (*num_tokens)++;
            temp_idx = 0;
        } else if (isupper(c)) { // Переменная
            if (temp_idx > 0) {
                temp_token[temp_idx] = '\0';
                tokens[*num_tokens] = strdup(temp_token);
                if (!tokens[*num_tokens]) {
                    exit(1);
                }
                (*num_tokens)++;
                temp_idx = 0;
            }
            temp_token[0] = c;
            temp_token[1] = '\0';
            tokens[*num_tokens] = strdup(temp_token);
            if (!tokens[*num_tokens]) {
                exit(1);
            }
            (*num_tokens)++;
        } else if (is_operator(c) || c == '(' || c == ')') { // Оператор или
                                                             // скобка
            if (temp_idx > 0) {
                temp_token[temp_idx] = '\0';
                tokens[*num_tokens] = strdup(temp_token);
                if (!tokens[*num_tokens]) {
                    exit(1);
                }
                (*num_tokens)++;
                temp_idx = 0;
            }
            temp_token[0] = c;
            temp_token[1] = '\0';
            tokens[*num_tokens] = strdup(temp_token);
            if (!tokens[*num_tokens]) {
                exit(1);
            }
            (*num_tokens)++;
        } else {
            fprintf(stderr,
                    "Ошибка: Недопустимый символ в выражении: '%c'\n",
                    c);
            for (int j = 0; j < *num_tokens; ++j)
                free(tokens[j]);
            free(tokens);
            exit(1);
        }
    }
    if (temp_idx > 0) {
        temp_token[temp_idx] = '\0';
        tokens[*num_tokens] = strdup(temp_token);
        if (!tokens[*num_tokens]) {
            exit(1);
        }
        (*num_tokens)++;
    }
    return tokens;
}

// Преобразует инфиксное выражение в RPN
char**
infix_to_rpn(const char* expression, int* rpn_token_count, bool is_first_pass)
{
    Stack op_stack;
    op_stack.top = -1;

    char** rpn_output = (char**)malloc(MAX_RPN_TOKENS * sizeof(char*));
    if (!rpn_output) {
        fprintf(stderr, "Ошибка выделения памяти для RPN.\n");
        exit(1);
    }
    *rpn_token_count = 0;

    int num_infix_tokens;
    char** infix_tokens = tokenize_expression(expression, &num_infix_tokens);

    for (int i = 0; i < num_infix_tokens; i++) {
        char* token = infix_tokens[i];
        char first_char = token[0];

        if (isdigit(first_char)
            || (strlen(token) > 1 && first_char == '-'
                && isdigit(token[1]))) { // Число
            int val;
            parse_operand(token, is_first_pass, &val);
            rpn_output[(*rpn_token_count)++] = strdup(token);
            if (!rpn_output[(*rpn_token_count) - 1]) {
                exit(1);
            }
        } else if (isupper(first_char) && strlen(token) == 1) { // Переменная
            int val;
            parse_operand(token, is_first_pass, &val);
            rpn_output[(*rpn_token_count)++] = strdup(token);
            if (!rpn_output[(*rpn_token_count) - 1]) {
                exit(1);
            }
        } else if (first_char == '(') {
            push(&op_stack, strdup(token));
            if (!peek(&op_stack)) {
                exit(1);
            }
        } else if (first_char == ')') {
            while (!is_empty(&op_stack) && peek(&op_stack)[0] != '(') {
                rpn_output[(*rpn_token_count)++] = pop(&op_stack);
                if (!rpn_output[(*rpn_token_count) - 1]) {
                    exit(1);
                }
            }
            if (!is_empty(&op_stack) && peek(&op_stack)[0] == '(') {
                free(pop(&op_stack));
            } else {
                fprintf(stderr,
                        "Ошибка: Несогласованные скобки в выражении.\n");
                for (int j = 0; j < *rpn_token_count; ++j)
                    free(rpn_output[j]);
                free(rpn_output);
                for (int j = 0; j < num_infix_tokens; ++j)
                    free(infix_tokens[j]);
                free(infix_tokens);
                exit(1);
            }
        } else if (is_operator(first_char)) {
            while (!is_empty(&op_stack) && peek(&op_stack)[0] != '('
                   && get_precedence(peek(&op_stack)[0])
                           >= get_precedence(first_char)) {
                rpn_output[(*rpn_token_count)++] = pop(&op_stack);
                if (!rpn_output[(*rpn_token_count) - 1]) {
                    exit(1);
                }
            }
            push(&op_stack, strdup(token));
            if (!peek(&op_stack)) {
                exit(1);
            }
        } else {
            fprintf(stderr,
                    "Ошибка: Неизвестный токен в выражении: %s\n",
                    token);
            for (int j = 0; j < *rpn_token_count; ++j)
                free(rpn_output[j]);
            free(rpn_output);
            for (int j = 0; j < num_infix_tokens; ++j)
                free(infix_tokens[j]);
            free(infix_tokens);
            exit(1);
        }
    }

    while (!is_empty(&op_stack)) {
        if (peek(&op_stack)[0] == '(' || peek(&op_stack)[0] == ')') {
            fprintf(stderr, "Ошибка: Несогласованные скобки в выражении.\n");
            for (int j = 0; j < *rpn_token_count; ++j)
                free(rpn_output[j]);
            free(rpn_output);
            for (int j = 0; j < num_infix_tokens; ++j)
                free(infix_tokens[j]);
            free(infix_tokens);
            exit(1);
        }
        rpn_output[(*rpn_token_count)++] = pop(&op_stack);
        if (!rpn_output[(*rpn_token_count) - 1]) {
            exit(1);
        }
    }

    for (int j = 0; j < num_infix_tokens; ++j)
        free(infix_tokens[j]);
    free(infix_tokens);

    return rpn_output;
}

// Генерирует код Simple Assembler из RPN-последовательности
int generate_rpn_code(
        FILE* out,
        const char** rpn_tokens,
        int rpn_token_count,
        int* current_address)
{
    Stack eval_stack;
    eval_stack.top = -1;

    for (int i = 0; i < rpn_token_count; i++) {
        const char* token = rpn_tokens[i];
        char first_char = token[0];

        if (isdigit(first_char)
            || (strlen(token) > 1 && first_char == '-'
                && isdigit(token[1]))) { // Число
            int val = atoi(token);
            int const_idx = find_constant_index(val);
            push(&eval_stack, (char*)(long)const_addresses[const_idx]);
        } else if (isupper(first_char) && strlen(token) == 1) { // Переменная
            push(&eval_stack, (char*)(long)var_addresses[first_char - 'A']);
        } else if (is_operator(first_char)) {
            char* op2_ptr = pop(&eval_stack);
            char* op1_ptr = pop(&eval_stack);

            if (!op1_ptr || !op2_ptr) {
                fprintf(stderr,
                        "Ошибка: Недостаточно операндов для оператора '%c' в "
                        "RPN.\n",
                        first_char);
                exit(1);
            }
            int op2_addr = (int)(long)op2_ptr;
            int op1_addr = (int)(long)op1_ptr;

            generate_code(out, "LOAD", op1_addr, current_address);

            switch (first_char) {
            case '+':
                generate_code(out, "ADD", op2_addr, current_address);
                break;
            case '-':
                generate_code(out, "SUB", op2_addr, current_address);
                break;
            case '*':
                generate_code(out, "MUL", op2_addr, current_address);
                break;
            case '/':
                generate_code(out, "DIVIDE", op2_addr, current_address);
                break;
            }

            if (i < rpn_token_count - 1) {
                generate_code(
                        out, "STORE", temp_storage_address, current_address);
                push(&eval_stack, (char*)(long)temp_storage_address);
            }
        } else {
            fprintf(stderr,
                    "Ошибка: Внутренняя - Неизвестный токен '%s' в RPN.\n",
                    token);
            exit(1);
        }
    }
    return -1;
}

int main(int argc, const char* const argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Использование: %s <input.sb> <output.sa>\n", argv[0]);
        return 1;
    }

    FILE* in = fopen(argv[1], "r");
    FILE* out = fopen(argv[2], "w");
    if (!in || !out) {
        fprintf(stderr, "Ошибка: Не удалось открыть файлы.\n");
        perror("");
        return 1;
    }

    char line[MAX_LINE];
    int current_address = 0;

    // Проход 1: Сбор меток, переменных, констант и расчет адресов
    for (int i = 0; i < MAX_LABELS * 2; ++i)
        label_addresses[i] = -1;

    while (fgets(line, sizeof(line), in)) {
        char line_copy[MAX_LINE];
        strcpy(line_copy, line);

        int line_number;
        char operator_str[10];
        char* rest_of_line;

        char* token = strtok(line_copy, " \t\n");
        if (!token)
            continue;

        if (sscanf(token, "%d", &line_number) != 1) {
            fprintf(stderr,
                    "Ошибка на строке '%s': Недопустимый номер строки или "
                    "формат.\n",
                    line);
            exit(1);
        }

        token = strtok(NULL, " \t\n");
        if (!token) {
            fprintf(stderr,
                    "Ошибка на строке %d: Отсутствует оператор.\n",
                    line_number);
            exit(1);
        }
        strcpy(operator_str, token);

        rest_of_line = strtok(NULL, "\n");
        char params[MAX_LINE] = {0};
        if (rest_of_line)
            strcpy(params, rest_of_line);

        add_label(line_number, current_address);

        if (strcmp(operator_str, "REM") == 0) {
            // Комментарий не генерирует код
        } else if (
                strcmp(operator_str, "INPUT") == 0
                || strcmp(operator_str, "OUTPUT") == 0) {
            char var_name[MAX_LINE];
            sscanf(params, "%s", var_name);
            if (strlen(var_name) != 1 || !isupper(var_name[0])) {
                fprintf(stderr,
                        "Ошибка на строке %d: %s ожидает одну заглавную "
                        "переменную (A-Z).\n",
                        line_number,
                        operator_str);
                exit(1);
            }
            used_vars[var_name[0] - 'A'] = true;
            current_address++;

        } else if (strcmp(operator_str, "GOTO") == 0) {
            int target_line;
            if (sscanf(params, "%d", &target_line) != 1) {
                fprintf(stderr,
                        "Ошибка на строке %d: GOTO ожидает номер строки.\n",
                        line_number);
                exit(1);
            }
            add_label(target_line, -1);
            current_address++;

        } else if (strcmp(operator_str, "END") == 0) {
            current_address++;

        } else if (strcmp(operator_str, "LET") == 0) {
            char dest_var_name_str[2];
            char* expr_start;

            char* eq_pos = strchr(params, '=');
            if (!eq_pos) {
                fprintf(stderr,
                        "Ошибка на строке %d: В LET отсутствует '='.\n",
                        line_number);
                exit(1);
            }

            char var_part[MAX_LINE];
            strncpy(var_part, params, eq_pos - params);
            var_part[eq_pos - params] = '\0';

            int len = strlen(var_part);
            while (len > 0 && isspace(var_part[len - 1])) {
                len--;
            }
            var_part[len] = '\0';

            if (strlen(var_part) != 1 || !isupper(var_part[0])) {
                fprintf(stderr,
                        "Ошибка на строке %d: Переменная назначения в LET "
                        "должна быть одной заглавной буквой.\n",
                        line_number);
                exit(1);
            }
            dest_var_name_str[0] = var_part[0];
            dest_var_name_str[1] = '\0';
            used_vars[dest_var_name_str[0] - 'A'] = true;

            expr_start = eq_pos + 1;
            while (isspace(*expr_start))
                expr_start++;

            int rpn_count;
            char** rpn_tokens = infix_to_rpn(expr_start, &rpn_count, true);

            int op_count = 0;
            for (int j = 0; j < rpn_count; ++j) {
                if (strlen(rpn_tokens[j]) == 1
                    && is_operator(rpn_tokens[j][0])) {
                    op_count++;
                }
            }

            if (op_count == 0) {
                current_address += 2;
            } else {
                current_address += (op_count * 3);
            }

            for (int j = 0; j < rpn_count; ++j)
                free(rpn_tokens[j]);
            free(rpn_tokens);

        } else if (strcmp(operator_str, "IF") == 0) {
            char if_params_copy[MAX_LINE];
            strcpy(if_params_copy, params);

            char* tokens[MAX_TOKENS];
            int num_tokens = 0;
            char* param_token = strtok(if_params_copy, " \t");
            while (param_token && num_tokens < MAX_TOKENS) {
                tokens[num_tokens++] = param_token;
                param_token = strtok(NULL, " \t");
            }

            if (num_tokens != 5) {
                fprintf(stderr,
                        "Ошибка на строке %d: Недопустимое количество токенов "
                        "для оператора IF.\n",
                        line_number);
                exit(1);
            }

            add_label(atoi(tokens[4]), -1);

            int val_dummy;
            parse_operand(tokens[0], true, &val_dummy);
            parse_operand(tokens[2], true, &val_dummy);

            current_address += 2;
            char* op_str = tokens[1];
            if (strcmp(op_str, "<") == 0 || strcmp(op_str, "=") == 0) {
                current_address += 1;
            } else if (strcmp(op_str, ">") == 0) {
                current_address += 3;
            } else {
                fprintf(stderr,
                        "Ошибка на строке %d: Неподдерживаемый оператор "
                        "сравнения '%s' в IF.\n",
                        line_number,
                        op_str);
                exit(1);
            }
        } else {
            fprintf(stderr,
                    "Ошибка на строке %d: Неизвестный оператор '%s'.\n",
                    line_number,
                    operator_str);
            exit(1);
        }
    }

    // Назначение адресов для переменных и констант
    int data_address = current_address;
    for (int i = 0; i < 26; i++) {
        if (used_vars[i])
            var_addresses[i] = data_address++;
    }
    for (int i = 0; i < constant_count; i++) {
        const_addresses[i] = data_address++;
    }
    temp_storage_address = data_address++;

    // Проверка, что все метки были определены
    for (int i = 0; i < label_count; ++i) {
        if (label_addresses[i * 2 + 1] == -1) {
            fprintf(stderr,
                    "Ошибка: Метка %d используется, но не определена.\n",
                    label_addresses[i * 2]);
            exit(1);
        }
    }

    // Проход 2: Генерация ассемблерного кода
    rewind(in);
    current_address = 0;

    while (fgets(line, sizeof(line), in)) {
        char line_copy[MAX_LINE];
        strcpy(line_copy, line);

        int line_number;
        char operator_str[10];
        char* rest_of_line;

        char* token = strtok(line_copy, " \t\n");
        if (!token)
            continue;

        sscanf(token, "%d", &line_number);

        token = strtok(NULL, " \t\n");
        strcpy(operator_str, token);

        rest_of_line = strtok(NULL, "\n");
        char params[MAX_LINE] = {0};
        if (rest_of_line)
            strcpy(params, rest_of_line);

        if (strcmp(operator_str, "REM") == 0) {
            fprintf(out, "; %s\n", params);
        } else if (strcmp(operator_str, "INPUT") == 0) {
            char var_name[MAX_LINE];
            sscanf(params, "%s", var_name);
            generate_code(
                    out,
                    "READ",
                    var_addresses[var_name[0] - 'A'],
                    &current_address);

        } else if (strcmp(operator_str, "OUTPUT") == 0) {
            char var_name[MAX_LINE];
            sscanf(params, "%s", var_name);
            generate_code(
                    out,
                    "WRITE",
                    var_addresses[var_name[0] - 'A'],
                    &current_address);

        } else if (strcmp(operator_str, "GOTO") == 0) {
            int target_line;
            sscanf(params, "%d", &target_line);
            int target_addr = get_label_address(target_line);
            generate_code(out, "JUMP", target_addr, &current_address);

        } else if (strcmp(operator_str, "END") == 0) {
            generate_code(out, "HALT", 0, &current_address);

        } else if (strcmp(operator_str, "LET") == 0) {
            char dest_var_name_str[2];
            char* expr_start;

            char* eq_pos = strchr(params, '=');
            dest_var_name_str[0] = params[0];
            dest_var_name_str[1] = '\0';
            expr_start = eq_pos + 1;
            while (isspace(*expr_start))
                expr_start++;

            int rpn_count;
            char** rpn_tokens = infix_to_rpn(expr_start, &rpn_count, false);

            if (rpn_count == 1) {
                int operand_addr;
                char* single_token = rpn_tokens[0];
                if (strlen(single_token) == 1 && isupper(single_token[0])) {
                    operand_addr = var_addresses[single_token[0] - 'A'];
                } else {
                    operand_addr = const_addresses[find_constant_index(
                            atoi(single_token))];
                }
                generate_code(out, "LOAD", operand_addr, &current_address);
            } else {
                generate_rpn_code(
                        out,
                        (const char**)rpn_tokens,
                        rpn_count,
                        &current_address);
            }

            int dest_addr = var_addresses[dest_var_name_str[0] - 'A'];
            generate_code(out, "STORE", dest_addr, &current_address);

            for (int j = 0; j < rpn_count; ++j)
                free(rpn_tokens[j]);
            free(rpn_tokens);

        } else if (strcmp(operator_str, "IF") == 0) {
            char if_params_copy[MAX_LINE];
            strcpy(if_params_copy, params);

            char* tokens[MAX_TOKENS];
            int num_tokens = 0;
            char* param_token = strtok(if_params_copy, " \t");
            while (param_token && num_tokens < MAX_TOKENS) {
                tokens[num_tokens++] = param_token;
                param_token = strtok(NULL, " \t");
            }

            char* op_str = tokens[1];
            int target_line = atoi(tokens[4]);
            int target_addr = get_label_address(target_line);

            int op1_addr;
            if (strlen(tokens[0]) == 1 && isupper(tokens[0][0])) {
                op1_addr = var_addresses[tokens[0][0] - 'A'];
            } else {
                op1_addr
                        = const_addresses[find_constant_index(atoi(tokens[0]))];
            }

            int op2_addr;
            if (strlen(tokens[2]) == 1 && isupper(tokens[2][0])) {
                op2_addr = var_addresses[tokens[2][0] - 'A'];
            } else {
                op2_addr
                        = const_addresses[find_constant_index(atoi(tokens[2]))];
            }

            generate_code(out, "LOAD", op1_addr, &current_address);
            generate_code(out, "SUB", op2_addr, &current_address);

            if (strcmp(op_str, "<") == 0) {
                generate_code(out, "JNEG", target_addr, &current_address);
            } else if (strcmp(op_str, "=") == 0) {
                generate_code(out, "JZ", target_addr, &current_address);
            } else if (strcmp(op_str, ">") == 0) {
                int skip_address = current_address + 3;

                generate_code(out, "JNEG", skip_address, &current_address);
                generate_code(out, "JZ", skip_address, &current_address);
                generate_code(out, "JUMP", target_addr, &current_address);
            } else {
                fprintf(stderr,
                        "Ошибка: Внутренняя - Неизвестный оператор сравнения "
                        "'%s'.\n",
                        op_str);
                exit(1);
            }
        } else {
            fprintf(stderr,
                    "Ошибка: Внутренняя - Неизвестный оператор '%s'.\n",
                    operator_str);
            exit(1);
        }
    }

    // Секция данных
    for (int i = 0; i < 26; i++) {
        if (used_vars[i]) {
            fprintf(out,
                    "%02d = +0000 ; Переменная %c\n",
                    var_addresses[i],
                    'A' + i);
        }
    }
    for (int i = 0; i < constant_count; i++) {
        fprintf(out,
                "%02d = %+05d ; Константа %d\n",
                const_addresses[i],
                constants[i],
                constants[i]);
    }
    fprintf(out,
            "%02d = +0000 ; Временная переменная для RPN\n",
            temp_storage_address);

    fclose(in);
    fclose(out);

    return 0;
}
