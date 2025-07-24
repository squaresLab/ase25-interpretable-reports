//Falcon-180B DATASET v1.0 Category: Error handling ; Style: future-proof
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>

#define MAX_ERRORS 10

typedef struct {
    jmp_buf env;
    int error_count;
    char *errors[MAX_ERRORS];
} error_context_t;

void error_context_init(error_context_t *ctx) {
    ctx->error_count = 0;
    for (int i = 0; i < MAX_ERRORS; i++) {
        ctx->errors[i] = NULL;
    }
}

void error_context_destroy(error_context_t *ctx) {
    for (int i = 0; i < ctx->error_count; i++) {
        free(ctx->errors[i]);
    }
}

void error_context_add_error(error_context_t *ctx, const char *format,...) {
    if (ctx->error_count >= MAX_ERRORS) {
        return;
    }

    va_list args;
    va_start(args, format);
    ctx->errors[ctx->error_count] = malloc(1024);
    vsnprintf(ctx->errors[ctx->error_count], 1024, format, args);
    ctx->error_count++;
    va_end(args);
}

void error_context_print_errors(error_context_t *ctx) {
    for (int i = 0; i < ctx->error_count; i++) {
        printf("%s\n", ctx->errors[i]);
    }
}

void error_context_clear_errors(error_context_t *ctx) {
    for (int i = 0; i < ctx->error_count; i++) {
        free(ctx->errors[i]);
    }
    ctx->error_count = 0;
}

int main() {
    error_context_t ctx;
    error_context_init(&ctx);

    int result = setjmp(ctx.env);

    if (result == 0) {
        int a = 10;
        int b = 0;
        int c = a / b;
    } else {
        error_context_print_errors(&ctx);
        error_context_clear_errors(&ctx);
        printf("Recovered from error\n");
    }

    return 0;
}